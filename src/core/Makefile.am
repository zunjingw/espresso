# Copyright (C) 2012,2013,2014,2015,2016 The ESPResSo project
# Copyright (C) 2007,2008,2009,2010,2011 Olaf Lenz, Axel Arnold
#
# This file is part of ESPResSo.
#
# ESPResSo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ESPResSo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

EXTRA_DIST =
CLEANFILES =
if UNIT_TESTS
SUBDIRS = unit_tests
endif

AM_DEFAULT_SOURCE_EXT = .cpp

lib_LTLIBRARIES = libEspresso.la
# List the sources of the Espresso binary here
# config-features.cpp must be at the beginning so that it is compiled first
libEspresso_la_SOURCES = \
	config-features.cpp \
	cells.cpp cells.hpp \
	collision.cpp collision.hpp \
	communication.cpp communication.hpp \
	comfixed.cpp comfixed.hpp \
	comforce.hpp comforce.cpp \
	config.hpp \
	constraint.cpp constraint.hpp \
	cuda_interface.cpp cuda_interface.hpp\
	cuda_init.hpp cuda_init.cpp\
	debug.cpp debug.hpp \
	domain_decomposition.cpp domain_decomposition.hpp \
	electrokinetics_pdb_parse.cpp electrokinetics_pdb_parse.hpp \
	energy.cpp energy_inline.hpp energy.hpp \
	external_potential.hpp external_potential.cpp \
	errorhandling.cpp errorhandling.hpp \
	fft.cpp fft.hpp \
	fft-common.cpp fft-common.hpp \
	fft-dipolar.cpp fft-dipolar.hpp \
	forcecap.cpp forcecap.hpp \
	forces.cpp forces_inline.hpp forces.hpp \
	galilei.cpp galilei.hpp \
	ghosts.cpp ghosts.hpp \
	global.cpp global.hpp \
	grid.cpp grid.hpp \
	halo.cpp halo.hpp \
	iccp3m.cpp iccp3m.hpp \
	imd.cpp imd.hpp \
	initialize.cpp initialize.hpp \
	integrate.cpp integrate.hpp \
	interaction_data.cpp interaction_data.hpp \
	lattice.cpp lattice_inline.hpp lattice.hpp \
	layered.cpp layered.hpp \
	lb.cpp lb.hpp \
	lb-boundaries.cpp lb-boundaries.hpp \
	lb-d3q18.hpp lb-d3q19.hpp \
	lbgpu.cpp lbgpu.hpp\
	lees_edwards.cpp lees_edwards.hpp \
	lees_edwards_domain_decomposition.cpp lees_edwards_domain_decomposition.hpp \
	lees_edwards_comms_manager.cpp lees_edwards_comms_manager.hpp \
	metadynamics.cpp metadynamics.hpp \
	minimize_energy.cpp minimize_energy.hpp \
	modes.cpp modes.hpp \
	molforces.cpp molforces.hpp \
	mol_cut.cpp mol_cut.hpp \
	nemd.cpp nemd.hpp \
	npt.cpp npt.hpp \
	nsquare.cpp nsquare.hpp \
	particle_data.cpp particle_data.hpp \
	polymer.cpp polymer.hpp \
	polynom.cpp polynom.hpp \
	pressure.cpp pressure.hpp \
	random.cpp random.hpp \
	rattle.cpp rattle.hpp \
	reaction.cpp reaction.hpp \
	readpdb.cpp readpdb.hpp \
	Ringbuffer.cpp Ringbuffer.hpp \
	rotate_system.cpp rotate_system.hpp \
	rotation.cpp rotation.hpp \
	RuntimeError.cpp RuntimeError.hpp \
	RuntimeErrorCollector.cpp RuntimeErrorCollector.hpp \
	RuntimeErrorStream.cpp RuntimeErrorStream.hpp \
	scafacos.cpp scafacos.hpp \
	specfunc.cpp specfunc.hpp \
	statistics.cpp statistics.hpp \
	statistics_chain.cpp statistics_chain.hpp \
	statistics_cluster.cpp statistics_cluster.hpp \
	statistics_correlation.cpp statistics_correlation.hpp \
	statistics_fluid.cpp statistics_fluid.hpp \
	statistics_molecule.cpp statistics_molecule.hpp \
	statistics_observable.cpp statistics_observable.hpp \
	statistics_wallstuff.cpp statistics_wallstuff.hpp \
	thermostat.cpp thermostat.hpp \
	topology.cpp topology.hpp \
	tuning.cpp tuning.hpp \
	utils.cpp utils.hpp \
	uwerr.cpp uwerr.hpp \
	verlet.cpp verlet.hpp \
	virtual_sites.cpp virtual_sites.hpp \
	virtual_sites_com.cpp virtual_sites_com.hpp \
	virtual_sites_relative.cpp virtual_sites_relative.hpp \
	vmdsock.cpp vmdsock.hpp \
	ghmc.cpp ghmc.hpp \
	Vector.cpp Vector.hpp \
	SystemInterface.cpp SystemInterface.hpp \
	integrate_sd.hpp integrate_sd.cpp \
	EspressoSystemInterface.hpp EspressoSystemInterface.cpp \
	PdbParser.cpp PdbParser.hpp \
	utils/statistics/RunningAverage.hpp \
	mpiio.cpp mpiio.hpp

# nonbonded potentials and forces
libEspresso_la_SOURCES += \
	bmhtf-nacl.cpp bmhtf-nacl.hpp \
	buckingham.cpp buckingham.hpp \
	cos2.cpp cos2.hpp \
	dpd.cpp dpd.hpp \
	gaussian.cpp gaussian.hpp \
	gb.cpp gb.hpp \
	hat.cpp hat.hpp \
	hertzian.cpp hertzian.hpp \
	lj.cpp lj.hpp \
	ljangle.cpp ljangle.hpp \
	ljcos.cpp ljcos.hpp \
	ljcos2.cpp ljcos2.hpp \
	ljgen.cpp ljgen.hpp \
	morse.cpp morse.hpp \
	soft_sphere.cpp soft_sphere.hpp \
	object-in-fluid/affinity.cpp object-in-fluid/affinity.hpp \
	object-in-fluid/membrane_collision.cpp object-in-fluid/membrane_collision.hpp \
	steppot.cpp steppot.hpp \
	tab.cpp tab.hpp \
	tunable_slip.cpp tunable_slip.hpp

# bonded potentials and forces
libEspresso_la_SOURCES += \
	angle.cpp angle.hpp \
	angle_harmonic.cpp angle_harmonic.hpp \
	angle_cosine.cpp angle_cosine.hpp \
	angle_cossquare.cpp angle_cossquare.hpp \
	angledist.cpp angledist.hpp \
	dihedral.cpp dihedral.hpp \
	endangledist.cpp endangledist.hpp \
	fene.cpp fene.hpp \
	harmonic_dumbbell.cpp harmonic_dumbbell.hpp \
	harmonic.cpp harmonic.hpp \
	quartic.cpp quartic.hpp \
	overlap.cpp overlap.hpp \
	umbrella.cpp umbrella.hpp \
	bonded_coulomb.cpp bonded_coulomb.hpp \
	subt_lj.cpp subt_lj.hpp \
	object-in-fluid/oif_global_forces.cpp object-in-fluid/oif_global_forces.hpp \
	object-in-fluid/oif_local_forces.cpp object-in-fluid/oif_local_forces.hpp \
	object-in-fluid/out_direction.cpp object-in-fluid/out_direction.hpp \
	hydrogen_bond.cpp hydrogen_bond.hpp \
	twist_stack.cpp twist_stack.hpp

# Immersed Boundary
libEspresso_la_SOURCES += \
	immersed_boundary/ibm_main.cpp                immersed_boundary/ibm_main.hpp \
	immersed_boundary/ibm_triel.cpp               immersed_boundary/ibm_triel.hpp \
	immersed_boundary/ibm_volume_conservation.cpp immersed_boundary/ibm_volume_conservation.hpp \
	immersed_boundary/ibm_tribend.cpp             immersed_boundary/ibm_tribend.hpp \
	immersed_boundary/ibm_tribend_helpers.hpp \
	immersed_boundary/ibm_cuda_interface.cpp      immersed_boundary/ibm_cuda_interface.hpp


# Coulomb methods
libEspresso_la_SOURCES += \
	debye_hueckel.cpp debye_hueckel.hpp \
	elc.cpp elc.hpp \
	magnetic_non_p3m_methods.cpp magnetic_non_p3m_methods.hpp \
	mdlc_correction.cpp  mdlc_correction.hpp \
	maggs.cpp maggs.hpp \
	mmm1d.cpp mmm1d.hpp \
	mmm2d.cpp mmm2d.hpp \
	mmm-common.cpp mmm-common.hpp \
	p3m.cpp p3m.hpp \
	p3m-common.cpp p3m-common.hpp \
	p3m-dipolar.cpp p3m-dipolar.hpp \
	reaction_field.cpp reaction_field.hpp

# Generic actors
libEspresso_la_SOURCES += \
	actor/Actor.hpp \
	actor/ActorList.cpp actor/ActorList.hpp \
	actor/HarmonicWell.cpp actor/HarmonicWell.hpp \
	actor/HarmonicOrientationWell.cpp actor/HarmonicOrientationWell.hpp \
	actor/Mmm1dgpuForce.cpp actor/Mmm1dgpuForce.hpp \
	actor/EwaldGPU.cpp actor/EwaldGPU.hpp actor/EwaldGPU_ShortRange.hpp \
	actor/DipolarDirectSum.cpp actor/DipolarDirectSum.hpp \
	actor/mmm-common_cuda.hpp actor/specfunc_cuda.hpp

##################################################
# CUDA rules
##################################################
if CUDA

SUFFIXES=.cu

CUDA_HEADER_FILES = \
	cuda_utils.hpp \
	cuda_interface.hpp \
	p3m_gpu.hpp \
	cuda_init.hpp \
	actor/mmm-common_cuda.hpp actor/specfunc_cuda.hpp \
	electrokinetics.hpp \
	integrate_sd_cuda_debug.hpp \
	integrate_sd_cuda_kernel.hpp \
	integrate_sd_cuda.hpp \
	integrate_sd_cuda_device.hpp \
	integrate_sd_matrix.hpp \
	fd-electrostatics.hpp \
	actor/DipolarDirectSum_cuda.hpp \
	p3m_gpu_error.hpp

CUDA_SOURCE_FILES = \
	cuda_init_cuda.cu \
	cuda_common_cuda.cu \
	electrokinetics_cuda.cu \
	fd-electrostatics_cuda.cu \
	lbgpu_cuda.cu \
	p3m_gpu_cuda.cu \
	EspressoSystemInterface_cuda.cu \
	integrate_sd_cuda.cu \
	integrate_sd_cuda_kernel.cu \
	integrate_sd_cuda_debug.cu \
	integrate_sd_matrix.cu \
	integrate_sd_cuda_device.cu \
	actor/Mmm1dgpuForce_cuda.cu \
	actor/EwaldGPU_cuda.cu \
	actor/HarmonicWell_cuda.cu \
	immersed_boundary/ibm_cuda.cu \
	actor/DipolarDirectSum_cuda.cu \
	actor/HarmonicOrientationWell_cuda.cu \
	p3m_gpu_error_cuda.cu

# include the dependency tracking files
include ./$(DEPDIR)/cuda_init_cuda.Plo
include ./$(DEPDIR)/cuda_common_cuda.Plo
include ./$(DEPDIR)/electrokinetics_cuda.Plo
include ./$(DEPDIR)/libgpu_cuda.Plo
include ./$(DEPDIR)/p3m_gpu_cuda.Plo
include ./$(DEPDIR)/EspressoSystemInterface_cuda.Plo
include ./actor/$(DEPDIR)/HarmonicWell_cuda.Plo
include ./actor/$(DEPDIR)/HarmonicOrientationWell_cuda.Plo
include ./actor/$(DEPDIR)/Mmm1dgpuForce.Plo
include ./actor/$(DEPDIR)/Mmm1dgpuForce_cuda.Plo
include ./actor/$(DEPDIR)/EwaldGPU.Plo
include ./actor/$(DEPDIR)/EwaldGPU_cuda.Plo

LOFILES = $(CUDA_SOURCE_FILES:.cu=.lo)

cuda_verbose = $(cuda_verbose_@AM_V@)
cuda_verbose_ = $(cuda_verbose_@AM_DEFAULT_V@)
cuda_verbose_0 = @echo "  NVCC     $@";

# nvcc does not allow for option MF for dependency generation,
# therefore make a separate run to generate dependencies
# putting them into DEPDIR ensures configure creates empties dummies
$(LOFILES): %.lo: %.cu
	$(cuda_verbose)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.lo$$||'`;\
	$(NVCC) $(NVCCFLAGS) $(DEFAULT_INCLUDES) $(CPPFLAGS) $(INCLUDES) $(DEFS) \
	  -M -MT $@ -o $$depbase.Plo $<; \
	$(LIBTOOL) $(AM_V_lt) --tag=CUDA $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
		--mode=compile $(NVCC) $(DEFS) \
		$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
		$(AM_NVCCFLAGS) $(NVCCFLAGS) -c -o $@ $<

libEspresso_la_LIBADD = $(LOFILES)
EXTRA_DIST += $(CUDA_SOURCE_FILES) $(CUDA_HEADER_FILES)
CLEANFILES += $(LOFILES)

endif


#################################################################
# Handling of myconfig.hpp
#################################################################
noinst_HEADERS = myconfig-default.hpp
CLEANFILES += myconfig-final.hpp
BUILT_SOURCES = myconfig-final.hpp

if MYCONFIG
myconfig-final.hpp: FORCE
	@if test -e $(top_builddir)/@MYCONFIG@; then \
	  echo "  GEN      $@ <= @MYCONFIG@"; \
	  if test -e $@; then \
	    cmp -s $(top_builddir)/@MYCONFIG@ $@ || cp $(top_builddir)/@MYCONFIG@ $@; \
	  else \
	     cp $(top_builddir)/@MYCONFIG@ $@; \
	  fi; \
	else \
	  echo "ERROR: Couldn't find @MYCONFIG@."; \
	  exit 1; \
	fi
else
myconfig-final.hpp: FORCE
	@config_files="\
	  $(top_builddir)/myconfig.hpp \
	  $(top_srcdir)/myconfig.hpp \
	  $(srcdir)/myconfig-default.hpp"; \
	for file in $$config_files; do \
	  if test -e $$file; then \
	    echo "  GEN      $@ <= $$file"; \
	    myconfig_found=1; \
	    if test -e $@; then \
	      cmp -s $$file $@ || cp $$file $@; \
	    else \
	       cp $$file $@; \
	    fi; \
	    break; \
	  fi; \
	done; \
	if test ! $$myconfig_found; then \
	  echo "ERROR: Couldn't find $(myconfig) anywhere"; \
	  exit 1; \
	fi
endif

#################################################################
# Handling of the features
#################################################################
EXTRA_DIST += gen_featureconfig.py config-features.hpp config-features.cpp

if HAVE_PYTHON

BUILT_SOURCES += config-features.hpp config-features.cpp
CLEANFILES += config-features.hpp config-features.cpp
config-features.hpp config-features.cpp: $(top_srcdir)/src/features.def $(srcdir)/gen_featureconfig.py
	$(AM_V_GEN)$(PYTHON) $(srcdir)/gen_featureconfig.py \
	  $(top_srcdir)/src/features.def config-features.hpp config-features.cpp \
	  > /dev/null

else

config-features.hpp config-features.cpp: features.def
	@echo "Python was not found in your PATH."
	@echo "If you change the set of features, you need Python!"
	@echo "Install it and rerun configure."

endif

#################################################################
# Handling of the version
#################################################################
libEspresso_la_SOURCES += config-version.cpp
CLEANFILES += config-version.cpp
config-version.cpp: FORCE
	$(AM_V_GEN)VERSION=`cd $(top_srcdir); sh config/genversion.sh -c`; \
	if ! test -e "config-version.cpp" || \
		test "$$VERSION" != "`cat config-version.cpp`"; then \
		echo "$$VERSION" > config-version.cpp; \
	fi

#################################################################
# Handling of the MPI fake implementation
#################################################################

.PHONY: FORCE
FORCE:
