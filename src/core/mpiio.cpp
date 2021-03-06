/*
  Copyright (C) 2010,2011,2012,2013,2014,2015,2016 The ESPResSo project
  Copyright (C) 2002,2003,2004,2005,2006,2007,2008,2009,2010
    Max-Planck-Institute for Polymer Research, Theory Group

  This file is part of ESPResSo.

  ESPResSo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ESPResSo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file mpiio.cpp
 *
 * Concerning the file layots.
 * - Scalar arrays are written like this:
 *   rank0 --- rank1 --- rank2 ...
 *   where each rank dumps its scalars in the ordering of the particles.
 * - Vector arrays are written in the rank ordering like scalar arrays.
 *   The ordering of the vector data is: v[0] v[1] v[2], so the data
 *   looks like this:
 *   v1[0] v1[1] v1[2] v2[0] v2[1] v2[2] v3[0] ...
 *
 * To be able to determine the rank bondaries (a multiple of
 * nlocalparts), the file 1.pref is written, which dumps the Exscan
 * results of nlocalparts, i.e. the prefixes in scalar arrays:
 * - 1.prefs looks like this:
 *   0 nlocalpats_rank0 nlocalparts_rank0+nlocalparts_rank1 ...
 *
 * Bonds are dumped as two arrays, namely 1.bond which stores the
 * bonding partners of the particles and 1.boff which stores the
 * iteration indices for each particle.
 * - 1.boff is a scalar array of size (nlocalpart + 1) per rank.
 * - The last element (at index nlocalpart) of 1.boff's subpart
 *   [rank * (nlocalpart + 1) : (rank + 1) * (nlocalpart + 1)]
 *   determines the number of bonds for processor "rank".
 * - In this subarray one can find the bonding partners of particle
 *   id[i]. The iteration indices for local part of 1.bonds are:
 *   subarray[i] : subarray[i+1]
 * - Take a look at the bond input code. It's easy to understand.
 */

#include <string>
#include <vector>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "config.hpp"
#include "initialize.hpp"
#include "particle_data.hpp"
#include "interaction_data.hpp"
#include "integrate.hpp"
#include "cells.hpp"
#include "utils.hpp"
#include "mpiio.hpp"
#include "../tcl/mpiio_tcl.hpp"

#include <mpi.h>

/** Dumps arr of size len starting from prefix pref of type T using
 * MPI_T as MPI datatype. Beware, that T and MPI_T have to match!
 *
 * \param fn The file name to dump to. Must not exist already
 * \param arr The array to dump
 * \param len The number of elements to dump
 * \param pref The prefix for this process
 * \param MPI_T The MPI_Datatype corresponding to the template parameter T.
 */
template <typename T>
static void mpiio_dump_array(std::string fn, T* arr, size_t len,
                             size_t pref, MPI_Datatype MPI_T)
{
  MPI_File f;
  int ret;

  ret = MPI_File_open(MPI_COMM_WORLD, const_cast<char *>(fn.c_str()),
                      // MPI_MODE_EXCL: Prohibit overwriting
                      MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_EXCL,
                      MPI_INFO_NULL, &f);
  if (ret) {
    char buf[MPI_MAX_ERROR_STRING];
    int len;
    MPI_Error_string(ret, buf, &len);
    buf[len] = '\0';
    fprintf(stderr, "MPI-IO Error: Could not open file \"%s\": %s\n",
            fn.c_str(), buf);
    errexit();
  }
  ret = MPI_File_set_view(f, pref * sizeof(T), MPI_T, MPI_T,
                          const_cast<char *>("native"), MPI_INFO_NULL);
  ret |= MPI_File_write_all(f, arr, len, MPI_T, MPI_STATUS_IGNORE);
  MPI_File_close(&f);
  if (ret) {
    fprintf(stderr, "MPI-IO Error: Could not write file \"%s\".\n",
            fn.c_str());
    errexit();
  }
}

/** Dumps some generic infos like the dumped fields and info to process
 *  the bond information offline (without ESPResSo). To be called by the
 *  master node only.
 *
 * \param fn The filename to write to
 * \param fields The dumped fields
 */
static void dump_info(std::string fn, unsigned fields)
{
  static std::vector<int> npartners;
  int success;
  FILE *f = fopen(fn.c_str(), "wb");
  if (!f) {
    fprintf(stderr, "MPI-IO Error: Could not open %s for writing.\n",
            fn.c_str());
    errexit();
  }
  success = (fwrite(&fields, sizeof(fields), 1, f) == 1);
  // Pack the necessary information of bonded_ia_params:
  // The number of partners. This is needed to interpret the bond IntList.
  if (n_bonded_ia > npartners.size())
    npartners.resize(n_bonded_ia);

  for (int i = 0; i < n_bonded_ia; ++i) {
    if (bonded_ia_params[i].type == BONDED_IA_NONE)
      npartners[i] = 0;
    else
      npartners[i] = bonded_ia_params[i].num;
  }
  success = success && (fwrite(&n_bonded_ia, sizeof(int), 1, f) == 1);
  success = success && (fwrite(npartners.data(), sizeof(int),
                               n_bonded_ia, f) == n_bonded_ia);
  fclose(f);
  if (!success) {
    fprintf(stderr, "MPI-IO Error: Failed to write %s.\n", fn.c_str());
    errexit();
  }
}


void mpi_mpiio_common_write(const char *filename, unsigned fields)
{
  std::string fnam(filename);
  int nlocalpart = cells_get_n_particles(), pref = 0, bpref = 0;
  int rank, ret;
  // Keep static buffers in order not having to allocate them on every
  // function call
  static std::vector<double>pos, vel;
  static std::vector<int>id, type, boff, bond;
  Cell *cell;

  // Nlocalpart prefixes
  // Prefixes based for arrays: 3 * pref for vel, pos.
  MPI_Exscan(&nlocalpart, &pref, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  // Realloc static buffers if necessary
  if (nlocalpart > id.size())
    id.resize(nlocalpart);
  if (fields & MPIIO_OUT_POS && 3 * nlocalpart > pos.size())
    pos.resize(3 * nlocalpart);
  if (fields & MPIIO_OUT_VEL && 3 * nlocalpart > vel.size())
    vel.resize(3 * nlocalpart);
  if (fields & MPIIO_OUT_TYP && nlocalpart > type.size())
    type.resize(nlocalpart);
  if (fields & MPIIO_OUT_BND && nlocalpart + 1 > boff.size())
    boff.resize(nlocalpart + 1);

  // Pack the necessary information
  // Esp. rescale the velocities.
  int i1 = 0, i3 = 0;
  for (int c = 0; c < local_cells.n; ++c) {
    cell = local_cells.cell[c];
    for (int j = 0; j < cell->n; ++j) {
      id[i1] = cell->part[j].p.identity;
      if (fields & MPIIO_OUT_POS) {
        pos[i3] = cell->part[j].r.p[0];
        pos[i3 + 1] = cell->part[j].r.p[1];
        pos[i3 + 2] = cell->part[j].r.p[2];
      }
      if (fields & MPIIO_OUT_VEL) {
        vel[i3] = cell->part[j].m.v[0] / time_step;
        vel[i3 + 1] = cell->part[j].m.v[1] / time_step;
        vel[i3 + 2] = cell->part[j].m.v[2] / time_step;
      }
      if (fields & MPIIO_OUT_TYP) {
        type[i1] = cell->part[j].p.type;
      }
      if (fields & MPIIO_OUT_BND) {
        boff[i1 + 1] = cell->part[j].bl.n;
      }
      i1++;
      i3 += 3;
    }
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
    dump_info(fnam + ".head", fields);
  mpiio_dump_array<int>(fnam + ".pref", &pref, 1, rank, MPI_INT);
  mpiio_dump_array<int>(fnam + ".id", id.data(), nlocalpart, pref,
                        MPI_INT);
  if (fields & MPIIO_OUT_POS)
    mpiio_dump_array<double>(fnam + ".pos", pos.data(), 3 * nlocalpart,
                             3 * pref, MPI_DOUBLE);
  if (fields & MPIIO_OUT_VEL)
    mpiio_dump_array<double>(fnam + ".vel", vel.data(), 3 * nlocalpart,
                             3 * pref, MPI_DOUBLE);
  if (fields & MPIIO_OUT_TYP)
    mpiio_dump_array<int>(fnam + ".type", type.data(), nlocalpart, pref,
                          MPI_INT);

  if (fields & MPIIO_OUT_BND) {
    // Convert the bond counts to bond prefixes
    boff[0] = 0;
    for (int i = 1; i <= nlocalpart; ++i)
      boff[i] += boff[i - 1];
    int numbonds = boff[nlocalpart];

    // Realloc bond buffer if necessary
    if (numbonds > bond.size())
      bond.resize(numbonds);

    // Pack the bond information
    int i = 0;
    for (int c = 0; c < local_cells.n; ++c) {
      cell = local_cells.cell[c];
      for (int j = 0; j < cell->n; ++j) {
        for (int k = 0; k < cell->part[j].bl.n; ++k)
          bond[i++] = cell->part[j].bl.e[k];
      }
    }

    // Determine the prefixes in the bond file
    MPI_Exscan(&numbonds, &bpref, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    mpiio_dump_array<int>(fnam + ".boff", boff.data(), nlocalpart + 1,
                          pref + rank, MPI_INT);
    mpiio_dump_array<int>(fnam + ".bond", bond.data(), numbonds, bpref,
                          MPI_INT);
  }
}


/** Get the number of elements in a file by its file size and
 *  elem_sz. I.e. query the file size using stat(2) and divide it by
 *  elem_sz.
 *
 * \param fn The filename
 * \param elem_sz Sizeof a single element
 * \return The number of elements stored binary in the file
 */
static int get_num_elem(std::string fn, size_t elem_sz)
{
  // Could also be done via MPI_File_open, MPI_File_get_size,
  // MPI_File_cose.
  struct stat st;
  errno = 0;
  if (stat(fn.c_str(), &st) != 0) {
    fprintf(stderr, "MPI-IO Input Error: Could not get file size of %s: %s\n",
            fn.c_str(), strerror(errno));
    errexit();
  }
  return st.st_size / elem_sz;
}

/** Reads a previously dumped array of size len starting from prefix
 *  pref of type T using MPI_T as MPI datatype. Beware, that T and MPI_T
 *  have to match!
 */
template <typename T>
static void mpiio_read_array(std::string fn, T* arr, size_t len,
                            size_t pref, MPI_Datatype MPI_T)
{
  MPI_File f;
  int ret;

  ret = MPI_File_open(MPI_COMM_WORLD, const_cast<char *>(fn.c_str()),
                      MPI_MODE_RDONLY, MPI_INFO_NULL, &f);

  if (ret) {
    char buf[MPI_MAX_ERROR_STRING];
    int len;
    MPI_Error_string(ret, buf, &len);
    buf[len] = '\0';
    fprintf(stderr, "MPI-IO Error: Could not open file \"%s\": %s\n",
            fn.c_str(), buf);
    errexit();
  }
  ret = MPI_File_set_view(f, pref * sizeof(T), MPI_T, MPI_T,
                          const_cast<char *>("native"), MPI_INFO_NULL);
  
  ret |= MPI_File_read_all(f, arr, len, MPI_T, MPI_STATUS_IGNORE);
  MPI_File_close(&f);
  if (ret) {
    fprintf(stderr, "MPI-IO Error: Could not read file \"%s\".\n",
            fn.c_str());
    errexit();
  }
}

/** Read the header file and store the information in the pointer
 *  "field". To be called by all processes.
 *
 * \param fn Filename of the head file
 * \param rank The rank of the current process in MPI_COMM_WORLD
 * \param file Pointer to store the fields to
 */
static void read_head(std::string fn, int rank, unsigned *fields)
{
  FILE *f;
  if (rank == 0) {
    if (!(f = fopen(fn.c_str(), "rb"))) {
      fprintf(stderr, "MPI-IO: Could not open %s.head.\n", fn.c_str());
      errexit();
    }
    if (fread((void *) fields, sizeof(unsigned), 1, f) != 1) {
      fprintf(stderr, "MPI-IO: Read on %s.head failed.\n", fn.c_str());
      errexit();
    }
    MPI_Bcast(fields, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
  } else {
    MPI_Bcast(fields, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
  }
}

/** Reads the pref file and fills pref and nlocalpart with their
 *  corresponding values. Needs to be called by all processes.
 *
 * \param fn The file name of the prefs file
 * \param rank The rank of the current process in MPI_COMM_WORLD
 * \param size The size of MPI_COMM_WORLD
 * \param nglobalpart The global amount of particles
 * \param prefs Pointer to store the prefix to
 * \param nlocalpart Pointer to store the amount of local particles to
 */
static void read_prefs(std::string fn, int rank, int size,
                       int nglobalpart, int *pref, int *nlocalpart)
{
  mpiio_read_array<int>(fn, pref, 1, rank, MPI_INT);
  if (rank > 0)
    MPI_Send(pref, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
  if (rank < size - 1)
    MPI_Recv(nlocalpart, 1, MPI_INT, rank + 1, MPI_ANY_TAG,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  else
    *nlocalpart = nglobalpart;
  *nlocalpart -= *pref;
}

void mpi_mpiio_common_read(const char *filename, unsigned fields)
{
  std::string fnam(filename);
  int size, rank;
  int nproc, nglobalpart, pref, nlocalpart, nlocalbond, bpref;
  unsigned avail_fields;

  local_remove_all_particles();

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  nproc = get_num_elem(fnam + ".pref", sizeof(int));
  nglobalpart = get_num_elem(fnam + ".id", sizeof(int));

  if (rank == 0 && nproc != size) {
    fprintf(stderr,
            "MPI-IO Error: Trying to read a file with a different COMM size than at point of writing.\n");
    errexit();
  }

  // 1.head on master node:
  // Read head to determine fields at time of writing.
  // Compare this var to the current fields.
  read_head(fnam + ".head", rank, &avail_fields);
  if (rank == 0 && (fields & avail_fields) != fields) {
    fprintf(stderr, "MPI-IO Error: Requesting to read fields which were not dumped.\n");
    errexit();
  }

  // 1.pref on all nodes:
  // Read own prefix (1 int at prefix rank).
  // Communicate own prefix to rank-1
  // Determine nlocalpart (prefix of rank+1 - own prefix) on every node.
  read_prefs(fnam + ".pref", rank, size, nglobalpart, &pref, &nlocalpart);

  // Prepare ESPResSo data structures
  local_particles = (Particle **) Utils::realloc(local_particles,
                                                 sizeof(Particle *) * nglobalpart);
  for (int i = 0; i < nglobalpart; ++i)
    local_particles[i] = NULL;
  n_part = nglobalpart;
  max_seen_particle = nglobalpart;

  // 1.id on all nodes:
  // Read nlocalpart ints at defined prefix.
  std::vector<int> id(nlocalpart);
  mpiio_read_array<int>(fnam + ".id", id.data(), nlocalpart, pref,
                        MPI_INT);

  if (fields & MPIIO_OUT_POS) {
    // 1.pos on all nodes:
    // Read nlocalpart * 3 doubles at defined prefix * 3
    std::vector<double> pos(3 * nlocalpart);
    mpiio_read_array<double>(fnam + ".pos", pos.data(), 3 * nlocalpart,
                             3 * pref, MPI_DOUBLE);

    for (int i = 0; i < nlocalpart; ++i) {
      local_place_particle(id[i], &pos[3 * i], 1);
    }
  }

  if (fields & MPIIO_OUT_TYP) {
    // 1.type on all nodes:
    // Read nlocalpart ints at defined prefix.
    std::vector<int> type(nlocalpart);
    mpiio_read_array<int>(fnam + ".type", type.data(), nlocalpart, pref,
                          MPI_INT);

    for (int i = 0; i < nlocalpart; ++i)
      local_particles[id[i]]->p.type = type[i];
  }

  if (fields & MPIIO_OUT_VEL) {
    // 1.vel on all nodes:
    // Read nlocalpart * 3 doubles at defined prefix * 3
    std::vector<double> vel(3 * nlocalpart);
    mpiio_read_array<double>(fnam + ".vel", vel.data(), 3 * nlocalpart,
                             3 * pref, MPI_DOUBLE);

    for (int i = 0; i < nlocalpart; ++i)
      for (int k = 0; k < 3; ++k)
        local_particles[id[i]]->m.v[k] = vel[3*i+k] * time_step;
  }

  if (fields & MPIIO_OUT_BND) {
    // 1.boff
    // nlocalpart + 1 ints per process
    std::vector<int> boff(nlocalpart + 1);
    mpiio_read_array<int>(fnam + ".boff", boff.data(), nlocalpart + 1,
                          pref + rank, MPI_INT);

    nlocalbond = boff[nlocalpart];
    // Determine the bond prefixes
    bpref = 0;
    MPI_Exscan(&nlocalbond, &bpref, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // 1.bond
    // nlocalbonds ints per process
    std::vector<int> bond(nlocalbond);
    mpiio_read_array<int>(fnam + ".bond", bond.data(), nlocalbond, bpref,
                          MPI_INT);

    for (int i = 0; i < nlocalpart; ++i) {
      int blen = boff[i+1] - boff[i];
      IntList *il = &local_particles[id[i]]->bl;
      realloc_intlist(il, blen);
      memcpy(il->e, &bond[boff[i]], blen * sizeof(int));
      il->n = blen;
    }
  }

  if (rank == 0)
    build_particle_node();
  rebuild_verletlist = 1;
  on_particle_change();
}

