# Copyright (C) 2012,2013,2014,2015,2016 The ESPResSo project
# Copyright (C) 2006,2007,2008,2009,2010,2011 Olaf Lenz, Axel Arnold
# Copyright (C) 2008,2009,2010 
#   Max-Planck-Institute for Polymer Research, Theory Group
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

dnl Process this file with autoconf to produce a configure script.
dnl Determine the version of ESPResSo at autoconf time
m4_define([ESPRESSO_VERSION],[esyscmd([/bin/sh config/genversion.sh -rd])])

# Initialize autoconf
AC_INIT([ESPResSo],[ESPRESSO_VERSION],[espressomd-users@nongnu.org])

AC_PREREQ([2.59])

AC_CONFIG_SRCDIR([src/features.def])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(config)
AC_PREFIX_DEFAULT($HOME/Espresso)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# Initialize automake
# tar-ustar is needed for filenames longer than 99 chars
AM_INIT_AUTOMAKE([1.11 gnu tar-ustar subdir-objects dist-xz])
AM_MAINTAINER_MODE

# silent rules, backwards compatiblity
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Are we in an unpacked dist or in the git repo?
AC_MSG_CHECKING([whether we are using development sources])
AS_IF([test -d $srcdir/.git], [devel_src=yes], [devel_src=no])
AM_CONDITIONAL(DEVEL_SRC, [test ."$devel_src" = .yes])
AC_MSG_RESULT($devel_src)

cat <<EOF
****************************************************************
*      Checking compiler characteristics and MPI support       *
****************************************************************
EOF

dnl overwrite _AC_PROC_CC_G to avoid check for -g and -O2 flags
m4_define([_AC_PROG_CXX_G],[])

AM_CONDITIONAL(DARWIN, [test $(uname) = "Darwin"])

#############
#### MPI ####
#############
# we use C++ as main code language
AC_LANG([C++])
AX_PROG_CXX_MPI(,,[
AC_MSG_FAILURE([Couldn't compile with MPI.])
])

# determine ESPRESSO_MPIEXEC
# mpiexec executable
AC_ARG_VAR([MPIEXEC], [MPI command mpiexec])
AS_IF([test x"$MPIEXEC" = x], [
AC_PATH_PROG([MPIEXEC], [mpiexec], [no])
])

##################################
#### COMPILER CHARACTERISTICS ####
##################################
# try to find best __FUNCTION__ variant
AX_CXX_VAR_PRETTYFUNC()

# check for c++11 support
AX_CXX_COMPILE_STDCXX_11(noext,mandatory)

# set the optimization flags
# never overwrite users CXXFLAGS
if test "${CXXFLAGS+set}" != set; then
  # test for -O3
  AC_MSG_CHECKING([whether the compiler accepts -O3])
  saved_CXXFLAGS=$CXXFLAGS
  CXXFLAGS="-O3 $CXXFLAGS"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],[
    AC_MSG_RESULT(yes);try_add_flag_res=yes
  ],[
    AC_MSG_RESULT(no); CXXFLAGS=$saved_CXXFLAGS; try_add_flag_res=no
  ])

  ##################################
  # test for -Wall
  AC_MSG_CHECKING([whether the compiler accepts -Wall])
  saved_CXXFLAGS=$CXXFLAGS
  CXXFLAGS="-Wall $CXXFLAGS"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[])],[
    AC_MSG_RESULT(yes);try_add_flag_res=yes
  ],[
    AC_MSG_RESULT(no); CXXFLAGS=$saved_CXXFLAGS; try_add_flag_res=no
  ])
fi

##################################
# miscellaneous
AC_PROG_CXX_C_O

cat <<EOF
****************************************************************
*                  Checking for programs                       *
****************************************************************
EOF

LT_INIT([disable-static])
AC_PROG_AWK
AC_PROG_EGREP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PATH_PROG(DOXYGEN,doxygen,no)
AM_CONDITIONAL(HAVE_DOXYGEN,[test .$DOXYGEN != .no])
AC_PATH_PROG(DOT,dot,no)
if test "$DOT" != no; then HAVE_DOT=YES; else HAVE_DOT=NO; fi
AC_SUBST(HAVE_DOT)
AC_SUBST(DOT_PATH, `AS_DIRNAME(["$DOT"])`)
AC_PATH_PROG(PDFLATEX,pdflatex,no)
AC_PATH_PROG(MAKEINDEX,makeindex,no)
AC_PATH_PROG(BIBTEX,bibtex,no)
AM_CONDITIONAL(HAVE_LATEX,
	[test .$PDFLATEX != .no && 
	test .$MAKEINDEX != .no && 
	test .$BIBTEX != .no])


cat <<EOF
****************************************************************
*               Checking for standard headers                  *
****************************************************************
EOF

##################################
# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h inttypes.h limits.h netdb.h \
 netinet/in.h stddef.h stdlib.h string.h strings.h sys/file.h sys/socket.h \
 sys/time.h unistd.h])


cat <<EOF
****************************************************************
*               Checking for standard functions                *
****************************************************************
EOF
##################################
# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIZE_T
AC_FUNC_STRTOD
AC_CHECK_FUNCS([atexit floor getcwd gethostbyname memset pow rint select socket sqrt])

cat <<EOF
****************************************************************
*         Checking for required tools and libraries            *
****************************************************************
EOF

LIBS="$LIBS -lm -ldl"

##################################
# check for FFTW
# with_fftw=no    don't use FFTW
# with_fftw=yes   try to find a working FFTW, bail out if none is found
# not given	  try to find a working FFTW, continue if not found
AC_MSG_CHECKING([whether to use FFTW])
AC_ARG_WITH([fftw],
	AS_HELP_STRING([--with-fftw],
                       [specify whether or not to use FFTW3 [guess]]),
	, with_fftw=guess)
AC_MSG_RESULT($with_fftw)

AS_IF([test x$with_fftw = xno], [fftw_found=no],
  [ ES_ADDPATH_CHECK_LIB(fftw3, fftw_plan_many_dft, [fftw_found=yes], [fftw_found=no],)
    AS_IF([test x$fftw_found = xyes],
      [ES_ADDPATH_CHECK_HEADER([fftw3.h],, [fftw_found=no],)])
  ])

AS_IF([test x$fftw_found = xno],[
  AS_IF([test x$with_fftw = xyes],[
    AC_MSG_FAILURE([FFTW requested, but FFTW was not found!])
    ],[ AS_IF([test x$with_fftw = xguess], [
          AC_MSG_WARN([FFTW3 not found. Some features will not be available!])
         ])
    ])
  ])
AS_IF([test x$fftw_found = xyes],[
  AC_DEFINE(FFTW,[],[Whether FFTW is available])])

##################################
# check for CUDA
AC_MSG_CHECKING([whether to use CUDA])
AC_ARG_WITH([cuda],
	AS_HELP_STRING([--with-cuda@<:@=DIR@:>@], [specify
	where CUDA is installed.  The cuda compiler can also be
	specified by setting the NVCC environment variable. The CUDA
	library and header can be manually specified by using
	CPPFLAGS, LDFLAGS and LIBS. [guess]]), ,
	with_cuda=guess) 
AC_MSG_RESULT($with_cuda)

AS_IF([test x$with_cuda = xguess || test x$with_cuda = xyes],[
   cuda_path=/usr/local/cuda
],[test x$with_cuda != xno],[
   cuda_path=$with_cuda
   with_cuda=yes
])

cuda_ok=no

AS_IF([test x$with_cuda != xno],[
    cuda_ok=yes

    # save current libs in case CUDA is not enabled at the end
    save_LIBS=$LIBS
    save_LDFLAGS=$LDFLAGS
    save_CPPFLAGS=$CPPFLAGS
    save_CXXFLAGS=$CXXFLAGS

    # NVCC
    AC_PROG_CUDA

    AS_IF([test x$NVCC = xno],[
        AS_IF([test x$with_cuda = xyes],[
            AC_MSG_FAILURE([CUDA compiler nvcc was not found, specify location using the NVCC variable])
        ],[
            AC_MSG_WARN([could not find the CUDA compiler (nvcc). Some features will not be available!])
        ])
    ])

    # since we link with mpic++, we need to find the cuda libraries manually
    ES_ADDPATH_CHECK_LIB(cudart, cudaGetDevice, [], [
        cuda_ok=no
        
        AS_IF([test x$with_cuda = xyes],[
            AC_MSG_FAILURE([could not find cuda runtime library (cudart), specify location using LDFLAGS])
        ],[
            AC_MSG_WARN([could not find cuda runtime library (cudart). Some features will not be available!])
        ])
    ], [$cuda_path/lib $cuda_path/lib64])

    ES_ADDPATH_CHECK_LIB(cufft, cufftPlan3d, [], [
        cuda_ok=no
        
        AS_IF([test x$with_cuda = xyes],[
            AC_MSG_FAILURE([could not find cuda FFT library (cufft), specify location using LDFLAGS])
        ],[
            AC_MSG_WARN([could not find cuda FFT library (cufft). Some features will not be available!])
        ])
    ], [$cuda_path/lib $cuda_path/lib64])

    ES_ADDPATH_CHECK_LIB(cublas, cublasDgemm_v2, [], [
        cuda_ok=no
        
        AS_IF([test x$with_cuda = xyes],[
            AC_MSG_FAILURE([could not find cuda BLAS library (cublas), specify location using LDFLAGS])
        ],[
            AC_MSG_WARN([could not find cuda BLAS library (cublas). Some features will not be available!])
        ])
    ], [$cuda_path/lib $cuda_path/lib64])

    ES_ADDPATH_CHECK_HEADER([cublas_v2.h],, [
    	cuda_ok=no
        
        AS_IF([test x$with_cuda = xyes],[
            AC_MSG_FAILURE([could not find cuda BLAS header files (cublas_v2.h), specify location using CPPFLAGS])
        ],[
            AC_MSG_WARN([could not find cuda BLAS header files (cublas_v2.h). Some features will not be available!])
        ])
    ], [$cuda_path/include])


    ES_ADDPATH_CHECK_LIB(curand, curandCreateGenerator, [], [
        cuda_ok=no
        
        AS_IF([test x$with_cuda = xyes],[
            AC_MSG_FAILURE([could not find cuda RAND library (curand), specify location using LDFLAGS])
        ],[
            AC_MSG_WARN([could not find cuda RAND library (curand). Some features will not be available!])
        ])
    ], [$cuda_path/lib $cuda_path/lib64])

    # NVCC compile check
    AC_MSG_CHECKING([whether CUDA compiles])

    # Enable C++11 
    NVCCFLAGS="$NVCCFLAGS -O3 -std c++11"    

    # if no other compute capability is defined by the user, we default to 2.0
    case "$NVCCFLAGS" in
       *-arch=*) ;;
       *) NVCCFLAGS="$NVCCFLAGS --ptxas-options=-v -gencode arch=compute_20,code=compute_20"
    esac

    # use nvcc
    
    # autoconf currently doesn't allow to define new languages like cuda, this is a workaround
    save_noncuda_CXX=$CXX
    save_noncuda_CXXFLAGS=$CXXFLAGS
    save_noncuda_LDFLAGS=$LDFLAGS
    save_noncuda_LIBS=$LIBS

    CXX=$NVCC
    CXXFLAGS="$NVCCFLAGS -x cu"
    # libtool linking options likely break NVCC linking, sort out all but simple -L options
    AS_IF([test -n "$LDFLAGS"],[ LDFLAGS="-Xlinker `echo $LDFLAGS | sed -e ['s|-[^L][^ 	]*||g' -e 's|[ 	][ 	]*|,|g']`" ])
    AS_IF([test -n "$LIBS"   ],[ LIBS="-Xlinker `echo $LIBS | sed ['s|[ 	][ 	]*|,|g']`"])
    
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <cuda.h>],[cudaGetDevice(0);])], [
        AC_MSG_RESULT(yes)
    ],[
        cuda_ok=no
        AC_MSG_RESULT(no)
        AS_IF([test x$with_cuda = xyes],[
            AC_MSG_FAILURE([cannot compile CUDA code. Look at config.log for more details.])
        ],[
            AC_MSG_WARN([cannot compile CUDA code. Some features will not be available!])
        ])
    ])

    # NVCC compile check
    AC_MSG_CHECKING([whether CUDA runs])
    
    AC_RUN_IFELSE([AC_LANG_PROGRAM([#include <cuda.h>],[
    int no; 
    cudaGetDeviceCount(&no);
    if (no == 0) return 1;
    ])], [
        AC_MSG_RESULT(yes)
    ],[
        AC_MSG_RESULT(no)
        AS_IF([test x$with_cuda = xyes],[
            cuda_running=no
            AC_MSG_WARN([cannot run CUDA code. Look at config.log for more details. Still compiling since explicitely requested.])
        ],[
            cuda_ok=no
            AC_MSG_WARN([cannot run CUDA code. Some features will not be available!])
        ])
    ],[
        AC_MSG_RESULT([unknown (cross-compiling)])
    ])

    CXX=$save_noncuda_CXX
    CXXFLAGS=$save_noncuda_CXXFLAGS
    LDFLAGS=$save_noncuda_LDFLAGS
    LIBS=$save_noncuda_LIBS
    
    AS_IF([test x$cuda_ok == xyes], [       
        AC_DEFINE(CUDA,[],[Whether CUDA is available])
    ],[
        # reset standard compiler options
        LIBS=$save_LIBS
        LDFLAGS=$save_LDFLAGS
	CPPFLAGS=$save_CPPFLAGS
    ])
])

AM_CONDITIONAL(CUDA, [test x$cuda_ok == xyes])

## Check for Boost
##################

#AX_BOOST_BASE([1.48],, [AC_MSG_WARN([boost was not found on you system, parts of the test system will not be available.])])
#AX_BOOST_UNIT_TEST_FRAMEWORK

BOOST_REQUIRE
BOOST_MPI
BOOST_SERIALIZATION
BOOST_TEST

LIBS="$LIBS $BOOST_MPI_LIBS"

AM_CONDITIONAL([UNIT_TESTS], [test x$ax_cv_boost_unit_test_framework == xyes])

## Checking for MAGMA
##################################
AS_IF([test x$with_magma = xguess || test x$with_magma = xyes || test x$with_magma = x],[
   magma_path=/usr/local/magma
],[test x$with_magma != xno],[
   magma_path=$with_magma
   with_magma=yes
])

AC_MSG_CHECKING([whether to use MAGMA])
AC_ARG_WITH([magma],
	AS_HELP_STRING([--with-magma],
                       [specify whether or not to use MAGMA [guess]]),
	, with_magma=guess)
AC_MSG_RESULT($with_magma)

AS_IF([test x$with_magma = xno], [magma_found=no],
  [ AC_SEARCH_LIBS(magma_init, [magma], [magma_found=yes], [magma_found=no])
    AS_IF([test x$magma_found = xyes],
      [ AC_CHECK_HEADER([magma.h],, [magma_found=no],[[#define HAVE_CUBLAS]])
        AS_IF([test x$magma_found = xyes],
          [ AC_SEARCH_LIBS([lsame_], [acml acml_mp],, [magma_found=no]
	                                              AC_MSG_WARN([MAGMA needs LAPACK. Currently only for the acml libary is searched - others have still to be added to configure.ac]) )])
      ])
  ])


AS_IF([test x$magma_found = xno],[
  AS_IF([test x$with_magma = xyes],[
    AC_MSG_FAILURE([MAGMA requested, but MAGMA was not found!])
    ],[ AS_IF([test x$with_magma = xguess], [
          AC_MSG_WARN([MAGMA not found. Some features will not be available!])
         ])
    ])
  ])
AS_IF([test x$magma_found = xyes],[
  AC_DEFINE(MAGMA,[],[Whether MAGMA is available])])
# end of check for MAGMA


## Checking for ARPACK
##################################
AS_IF([test x$with_arpack = xguess || test x$with_arpack = xyes || test x$with_arpack = x],[
   arpack_path=/usr/local/arpack
],[test x$with_arpack != xno],[
   arpack_path=$with_arpack
   with_arpack=yes
])

AC_MSG_CHECKING([whether to use ARPACK])
AC_ARG_WITH([arpack],
	AS_HELP_STRING([--with-arpack],
                       [specify whether or not to use ARPACK [guess]]),
	, with_arpack=guess)
AC_MSG_RESULT($with_arpack)


ES_ADDPATH_CHECK_LIB(arpack, dnaupd_, [arpack_found=yes], [
	arpack_found=no
        
        AS_IF([test x$with_arpack = xyes],[
            AC_MSG_FAILURE([could not find ARPACK library, specify location using LDFLAGS])
        ],[
            AC_MSG_WARN([could not find ARPACK library. Some features will not be available!])
        ])
    ], [$arpack_path/.libs])



AS_IF([test x$arpack_found = xno],[
  AS_IF([test x$with_arpack = xyes],[
    AC_MSG_FAILURE([ARPACK requested, but ARPACK was not found!])
    ],[ AS_IF([test x$with_arpack = xguess], [
          AC_MSG_WARN([ARPACK not found. Some features will not be available!])
         ])
    ])
  ])
AS_IF([test x$arpack_found = xyes],[
  AC_DEFINE(ARPACK,[],[Whether ARPACK is available])])
# end of check for ARPACK



cat <<EOF
****************************************************************
*                  Checking for Python                         *
****************************************************************
EOF

# check whether the python interpreter is available
# This is needed for automatic generation of myconfig etc.
AM_PATH_PYTHON([2.5],,no)
AM_CONDITIONAL(HAVE_PYTHON, [test .$PYTHON != .no])
if test .$PYTHON != .no; then
   # Check whether python -B works
   AC_MSG_CHECKING([whether python -B works])
   if $PYTHON -B -c pass > /dev/null 2>&1; then
     PYTHON="$PYTHON -B"
     AC_MSG_RESULT(yes)
   else
     AC_MSG_RESULT(no)
   fi
fi

AC_MSG_CHECKING([whether the python interface is wanted])
AC_ARG_WITH([python-interface], AS_HELP_STRING([--with-python-interface], 
                      [specify whether or not to use python [guess]]),
                      [], [with_python_interface=guess])
AC_MSG_RESULT($with_python_interface)
dnl with_python_interface=yes  try to find a working PYTHON devel version, 
dnl                            bail out if none is found
dnl with_python_interface=guess  try to find a working PYTHON devel version, 
dnl with_python_interface=no   do not use python interface

AS_IF([test .$with_python_interface != .no], [
	AX_PYTHON_DEVEL([>= '2.5'])

        AC_ARG_VAR(CYTHON,[Specify the cython tool to use])
        AC_ARG_VAR(CYFLAGS,[Specify cython flags])
   	AC_PATH_PROG(CYTHON,cython,no)
   	AS_IF([test .$CYTHON = .no],
                    [build_python_interface=no],
                    [build_python_interface=yes])

   	AX_PYTHON_MODULE(numpy, required)
   	AC_MSG_CHECKING([for numpy include path])
   	NUMPY_INCLUDE="`$PYTHON -c 'import numpy; import sys; sys.stdout.write(numpy.get_include())'`" #`
   	PYTHON_CPPFLAGS="$PYTHON_CPPFLAGS -I$NUMPY_INCLUDE"
   	AC_MSG_RESULT([$NUMPY_INCLUDE])

	AS_IF([test .$with_python_interface = .yes && test .$build_python_interface = .no],
		[AC_MSG_FAILURE([Cannot build python interface!])])
   	AC_DEFINE(PYTHON_DEV,[],[Whether Python is available]) 
])
AM_CONDITIONAL(PYTHON_INTERFACE, [test .$build_python_interface = .yes])
# export libtool objdir, so that we can link the .so files to the dir itself
AC_SUBST(objdir)

cat <<EOF
****************************************************************
*                  Checking for Tcl/Tk                         *
****************************************************************
EOF

AC_MSG_CHECKING([whether to build the Tcl interface])
AC_ARG_WITH(tcl,AS_HELP_STRING([--with-tcl=VERSION],
            [specify the tcl library to use (e.g. tcl8.4)]),
            [], [with_tcl=yes])
AC_MSG_RESULT($with_tcl)

dnl with_tcl=yes  try to find a working TCL version, bail out if none is found
dnl with_tcl=no   bail out
dnl otherwise     use the specified version

if test .$with_tcl = .no; then
   use_tcl=none
elif test .$with_tcl = .yes; then
	for version in $TCL_VERSION tcl8.6 tcl8.5 tcl8.4 tcl8.3 tcl8.2 tcl; do
		ES_ADDPATH_CHECK_LIB($version, Tcl_Init, [use_tcl=$version])
		if test .$use_tcl != .; then break; fi
	done
        AC_DEFINE(TCL,[],[Whether Tcl is available])
else
	ES_ADDPATH_CHECK_LIB($with_tcl, Tcl_Init, [use_tcl=$with_tcl])
fi

# check the results
if test .$use_tcl = . ; then
   # Tcl was not found
   case $target_os in
	*darwin*) AC_MSG_NOTICE(
[If you have Tcl installed, make sure that either libtcl<tclversion>.dylib is
 in one of the library paths set by LDFLAGS, or that you set the appropriate
 apple gcc -framework option in CPPFLAGS and LDFLAGS.]) ;;
	*) AC_MSG_FAILURE([
********************************************************************************
* Could not link against the (static) Tcl library (libtcl*.a).                 *
* Please add the library path to LDFLAGS (e.g. configure LDFLAGS=-L/usr/lib)!  *
********************************************************************************
]) ;;
   esac
fi

case $target_os in
  *darwin*) extrapaths=/Library/Frameworks/Tcl.framework/Headers ;;
  *linux*)  # path used by *buntu
	  extrapaths=/usr/include/$version ;;
esac

if test .$use_tcl != .none; then
   ES_ADDPATH_CHECK_HEADER(tcl.h, [], 
			[AC_MSG_FAILURE([
********************************************************************************
* Could not find the Tcl header files (tcl.h).                                 *
* Please add the include path to CPPFLAGS                                      *
* (e.g. configure CPPFLAGS=-I/usr/include)!                                    *
********************************************************************************
])], $extrapaths)
fi

if test .$use_tcl = .; then
   use_tcl=none
fi

AM_CONDITIONAL(TCL, [test .$use_tcl != .none])

##################################
# check for tk

AC_MSG_CHECKING([whether to use Tk])
AC_ARG_WITH(tk,
	AS_HELP_STRING([--with-tk=VERSION],[whether to use Tk, and which version to use]),
	[], [with_tk=no])
AC_MSG_RESULT($with_tcl)

dnl with_tk=no   don't use Tk
dnl with_tk=yes  try to find a working Tk version, bail out if none is found
dnl otherwise    use the specified version
if test .$with_tk != .no; then
	# test for X11
	AC_PATH_XTRA
	saved_CPPFLAGS=$CPPFLAGS
	saved_LDFLAGS=$LDFLAGS
	saved_LIBS=$LIBS
	CPPFLAGS="$CPPFLAGS $X_CXXFLAGS"
	LDFLAGS="$LDFLAGS $X_LIBS"
	LIBS="$LIBS $X_PRE_LIBS -lX11 $X_EXTRA_LIBS"
	AC_LINK_IFELSE([AC_LANG_CALL([],[XOpenDisplay])],[x11_works=yes],[x11_works=no])
	if test $x11_works = no; then
		AC_MSG_WARN([could not link against X11, hoping Tk works without])
		CPPFLAGS=$saved_CPPFLAGS
		LDFLAGS=$saved_LDFLAGS
		LIBS=$saved_LIBS
	fi
	# now test whether Tk can be found
	if test .$with_tk = .yes; then
		for version in $TK_VERSION tk8.6 tk8.5 tk8.4 tk8.3 tk8.2 tk; do
			ES_ADDPATH_CHECK_LIB($version, Tk_Init, [use_tk=$version], [])
			if test .$use_tk != .; then break; fi
		done
	else
		ES_ADDPATH_CHECK_LIB($with_tk, Tk_Init, [use_tk=$with_tk], [])
	fi
	if test .$use_tk = .; then
		case $target_os in
		*darwin*) AC_MSG_ERROR(
[If you have Tk installed, make sure that either libtcl<tclversion>.dylib is
 in one of the library paths set by LDFLAGS, or that you set the appropriate
 apple gcc -framework option in CPPFLAGS and LDFLAGS.]) ;;
		*) AC_MSG_FAILURE([Tk library $with_tk not found]) ;;
		esac
	fi
	if test .$use_tk = .tk; then
		if test .$use_tcl != .tcl; then
			AC_MSG_WARN([You are using a generic Tk version, but a defined Tcl version. This may cause problems.
Try --with-tcl=tcl to also use a generic Tcl version, which may fit better.])
		fi
	fi
	case $target_os in
	*darwin*) extrapaths=/Library/Frameworks/Tk.framework/Headers ;;
	*linux*)  # path used by *buntu
		  extrapaths="/usr/include/$version /usr/include/$use_tcl" ;;
	(*) ;;
	esac
	ES_ADDPATH_CHECK_HEADER(tk.h, [], 
		[AC_MSG_ERROR([Tk headers not found. Please add the include path to CPPFLAGS (e.g. configure CPPFLAGS=-I/usr/include/tcl8.4).])
		]
		,$extrapaths)
	AC_DEFINE_UNQUOTED(TK,$use_tk,[Whether to use Tk])
else
	use_tk=none
fi
# check for h5md library
cat <<EOF
****************************************************************
*                          h5md                                *
****************************************************************
EOF
AC_MSG_CHECKING([whether to build with h5md support])
AC_ARG_WITH(h5md, AS_HELP_STRING([--with-h5md],[wether to build with h5mdsupport]), , [with_h5md=no])
AC_MSG_RESULT($with_h5md)
if test .$with_h5md = .yes; then
  ES_ADDPATH_CHECK_LIB(hdf5,H5open,[hdf5_found=yes],[hdf5_found=no])

  ES_ADDPATH_CHECK_HEADER(hdf5.h, [], 
		[AC_MSG_ERROR([hdf5 headers not found. Please add the include path to
                       CPPFLAGS (e.g. configure CPPFLAGS=-I/usr/include/hdf5).])
		]
		,$extrapaths)
AS_IF([test x$hdf5_found = xyes],[
  AC_DEFINE(H5MD,[],[wether to build with h5mdsupport])]
  )
AS_IF([test x$hdf5_found = xno],[
  AC_MSG_FAILURE([H5MD requested, but hdf5 library was not found!])]
  )
fi
cat <<EOF
****************************************************************
*                   Other settings                             *
****************************************************************
EOF

##################################
# check for efence
AC_MSG_CHECKING([whether to use efence])
AC_ARG_WITH(efence,
  AS_HELP_STRING([--with-efence],[use ElectricFence memory debugging for the debug binary]),
    ,with_efence=no)
if test .$with_efence = .yes; then
  ES_ADDPATH_CHECK_LIB(efence,malloc,,AC_MSG_FAILURE([could not link against the efence library]),)

  CPPFLAGS="$CPPFLAGS -DEFENCE"
  LDFLAGS="$LDFLAGS -lefence"
fi
AC_MSG_RESULT($with_efence)

##################################
# Handling the myconfig-header
AC_ARG_VAR([MYCONFIG], 
           [Which myconfig file to use. @<:@myconfig.hpp@:>@])
AC_MSG_CHECKING([whether the variable MYCONFIG is set])
AS_IF([test "x$MYCONFIG" != "x"], [
  AC_MSG_RESULT(yes)
  AC_MSG_CHECKING([whether file $MYCONFIG exists])
  if ! test -f $MYCONFIG; then
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([MYCONFIG file $MYCONFIG does not exist!])
  fi
],[
  AC_MSG_RESULT(no)
  MYCONFIG="myconfig.hpp"
])
AM_CONDITIONAL(MYCONFIG, [test x$MYCONFIG != xmyconfig.hpp])

##################################
# Number of CPUs
AS_IF([test "X$CPU_COUNT" = "X"], [AX_COUNT_CPUS])
AC_ARG_VAR(CPU_COUNT,[number of tasks to run in test runs])
AC_SUBST(CPU_COUNT)

AC_ARG_VAR(LIBS,[use this to specify additional libraries to link against, e.g. -lmx])

# where to install the init scripts and tools
buildscriptsdir='${abs_top_builddir}/scripts'
AC_SUBST(buildscriptsdir)
scriptsdir='${pkgdatadir}/scripts'
AC_SUBST(scriptsdir)
toolsdir='${pkgdatadir}/tools'
AC_SUBST(toolsdir)

cat <<EOF
****************************************************************
*                   Creating output files                      *
****************************************************************
EOF
# Creating the output
AC_CONFIG_HEADERS([src/acconfig.hpp])
AC_CONFIG_FILES([
	Makefile
	config/Makefile
	config/myconfig-sample-header.hpp
	src/Makefile
	src/core/Makefile
	src/tcl/Makefile
	src/python/espressomd/Makefile
	src/core/unit_tests/Makefile
	tools/Makefile
	scripts/Makefile 
	testsuite/Makefile
	testsuite/tcl/Makefile
	testsuite/python/Makefile
	doc/Makefile
	doc/logo/Makefile
	doc/ug/Makefile
	doc/dg/Makefile
	doc/doxygen/Makefile
	doc/tutorials/Makefile
	doc/tutorials/00-building_espresso/Makefile
	doc/tutorials/01-lennard_jones/Makefile 
	doc/tutorials/02-charged_system/Makefile 
	doc/tutorials/03-object_in_fluid/Makefile 
        doc/tutorials/04-lattice_boltzmann/Makefile 
	doc/tutorials/05-raspberry_electrophoresis/Makefile 
	doc/tutorials/06-active_matter/Makefile 
	doc/tutorials/07-electrokinetics/Makefile 
	doc/tutorials/08-visualization/Makefile 
	doc/latexit.sh
	])
AC_CONFIG_FILES([testsuite/tcl/runtest.sh],
	[chmod 755 testsuite/tcl/runtest.sh])
AC_CONFIG_FILES([testsuite/python/runtest.sh],
	[chmod 755 testsuite/python/runtest.sh])
AC_CONFIG_FILES([tools/es_mpiexec],
	[chmod 755 tools/es_mpiexec])
AC_CONFIG_FILES([src/python/pypresso],
	[chmod 755 src/python/pypresso])

AC_SUBST([CONFIG_STATUS_DEPENDENCIES], ['$(top_srcdir)/src/features.def $(top_srcdir)/config/gen_sampleconfig.py'])
AC_CONFIG_COMMANDS([myconfig-sample.hpp],
        [if test x"$PYTHON" != xno; then \
            cat config/myconfig-sample-header.hpp > myconfig-sample.hpp; \
            $PYTHON $srcdir/config/gen_sampleconfig.py $srcdir/src/features.def >> myconfig-sample.hpp;\
        else echo "Python is not available, not (re)creating sample config."; \
        fi
        ], [PYTHON="$PYTHON"])


AC_OUTPUT

cat <<EOF
****************************************************************
*                   Configuration summary                      *
****************************************************************
platform detected       = $target
package version         = $PACKAGE_VERSION
development sources     = $devel_src

Compiler
--------
compiler                = $CXX
linker                  = $LD
c++ compiler flags      = $CXXFLAGS
c++ preprocessor flags  = $CPPFLAGS
linker flags            = $LDFLAGS
libraries               = $LIBS
using debug flags       = $enable_debug
using profiling flags   = $enable_profiling

cython complier         = $CYTHON
cython preproc flags    = $CYFLAGS


Libraries
---------
FFTW                    = $fftw_found
efence                  = $with_efence
h5md			= $with_h5md
boost_test		= $ax_cv_boost_unit_test_framework

EOF

if test x$cuda_ok = xyes; then
cat <<EOF
Cuda
----
CUDA compiler           = $NVCC
CUDA flags              = $NVCCFLAGS
EOF
fi

cat  <<EOF

Tcl/Tk
------
Tcl version             = $use_tcl
Tk version              = $use_tk

Python
------
python                  = $PYTHON
build python interface  = $build_python_interface
EOF

if test .$build_python_interface = .yes; then
  cat <<EOF
preprocessor flags      = $PYTHON_CPPFLAGS
linker flags            = $PYTHON_LDFLAGS
extra flags             = $PYTHON_EXTRA_LIBS
extra linker flags      = $PYTHON_EXTRA_LDFLAGS
EOF
fi

cat <<EOF

Other settings
--------------
config                  = $MYCONFIG
installation prefix     = $prefix
number of test tasks    = $CPU_COUNT
mympiexec               = $MYMPIEXEC
mpiexec                 = $MPIEXEC
pdflatex                = $PDFLATEX
makeindex               = $MAKEINDEX
bibtex                  = $BIBTEX
doxygen                 = $DOXYGEN
dot                     = $DOT

EOF

if test x$cuda_ok = xno; then
   echo "CUDA disabled or not supported."
fi
if test x$cuda_running = xno; then
   echo "CUDA enabled, but cannot run CUDA code out of the box. Make sure to set the dynamic library loader path correctly."
fi

if test .$pdflatex = .no; then
   echo "To be able to rebuild the user's guide, add pdflatex to your PATH."
fi
if test .$makeindex = .no; then
   echo "To be able to rebuild the user's guide, add makeindex to your PATH."
fi
if test .$bibtex = .no; then
   echo "To be able to rebuild the user's guide, add bibtex to your PATH."
fi
if test .$doxygen = .no; then
   echo "To be able to build the developer's guide, add doxygen to your PATH."
fi
if test .$with_python_interface = .no; then
   echo "Not building Python interface."
fi
if test .$use_tcl = .no; then
   echo "Not building Tcl interface."
fi

cat << EOF
****************************************************************
* DEPRECATION WARNING: The autoconf/autotools build system     *
* will be replaced by cmake in the future.                     *
* New features are only available via cmake.                   *
****************************************************************
EOF
