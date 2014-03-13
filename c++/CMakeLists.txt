# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.8)
project(KMCLib)

# -----------------------------------------------------------------------------
# CHECK IF USING MPI OR NOT
# -----------------------------------------------------------------------------

# Determine if this is a parallel build or not.
if(NOT MPI)
   set(MPI False
       CACHE STRING "Choose to force MPI via mpicxx or not : True or False"
       FORCE)
endif()

# If it is, set use mpicxx.
if(MPI)
  message( STATUS "== This is a parallel MPI build ==" )
  # Force using the "mpicxx" compiler wrapper.
  include(CMakeForceCompiler)
  cmake_force_cxx_compiler(${MPI} "System CXX compiler with MPI wrapping")

  # User verbosit as default when compiling in parallel.
  set( CMAKE_VERBOSE_MAKEFILE on )

  # Generate the mpiflag.h file.
  file(WRITE ${KMCLib_SOURCE_DIR}/src/mpiflag.h "#define RUNMPI true\n")

else()

  message( STATUS "== This is a serial build ==" )

  # Shut verbosity off as default in serial build mode.
  set( CMAKE_VERBOSE_MAKEFILE on )

  # Generate the mpiflag.h file.
  file(WRITE ${KMCLib_SOURCE_DIR}/src/mpiflag.h "#define RUNMPI false\n")

endif()

# -----------------------------------------------------------------------------
# SET THE COMPILER
# -----------------------------------------------------------------------------


# Check for supported compiler and set the flags.
if (CMAKE_COMPILER_IS_GNUCXX)

  # Do not use -Bsymbolic-functions with g++ on Mac OS X
  if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC -Wall -Wextra -Werror -pedantic -std=c++0x")
  else()
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bsymbolic -O3 -fPIC -Wall -Wextra -Werror -pedantic -std=c++0x")
  endif()

  set( CMAKE_CXX_FLAGS_DEBUG "-g -pg" )
  set( CMAKE_CXX_FLAGS_RELEASE  "" )
  set( CMAKE_CXX_FLAGS_RELWITHDEBINFO  "" )

  # Allow for unused parameters.
  add_definitions( -Wno-unused-parameter )

  if(MPI)
    message( STATUS "Using g++ via ${MPI}" )
    # Workaround for known mpich c++ bug.
    add_definitions( -DMPICH_IGNORE_CXX_SEEK )
    add_definitions( -DMPICH_SKIP_MPICXX )
    add_definitions( -Wno-long-long )
  endif()

# Only g++ supported.
else()
  message( FATAL_ERROR "Invalid CXX compiler. Make sure you have g++ installed" )
endif()

# Includsion from the source.
include_directories( ${KMCLib_SOURCE_DIR}/src )
include_directories( ${KMCLib_SOURCE_DIR}/externals/include )


# -----------------------------------------------------------------------------
# ADD THE SRC TARGET
# -----------------------------------------------------------------------------

add_subdirectory(src)

# -----------------------------------------------------------------------------
# ADD THE TESTS TARGET
# -----------------------------------------------------------------------------

add_subdirectory(unittest)

# -----------------------------------------------------------------------------
# ADD THE WRAPPINGS
# -----------------------------------------------------------------------------

add_subdirectory(wrap)

# -----------------------------------------------------------------------------
# ADD THE INSTALL TARGET
# -----------------------------------------------------------------------------

install( FILES ${CMAKE_BINARY_DIR}/wrap/Backend.py ${CMAKE_BINARY_DIR}/wrap/_Backend.so
         DESTINATION ${CMAKE_BINARY_DIR}/../../python/src/KMCLib/Backend/ )
