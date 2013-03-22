# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.8)
project(KMCLib)

# Set to on to use verbose output default.
set( CMAKE_VERBOSE_MAKEFILE off )


# -----------------------------------------------------------------------------
# SET THE COMPILER
# -----------------------------------------------------------------------------

# Force using the "mpicxx" compiler wrapper.
include(CMakeForceCompiler)
cmake_force_cxx_compiler(mpicxx "System CXX compiler with MPI wrapping")

# Check for supported compiler and set the flags.
if (CMAKE_COMPILER_IS_GNUCXX)
  set( CMAKE_CXX_FLAGS "-std=c++0x -O3 -fPIC -Wall -Wextra -Werror -pedantic")
  set( CMAKE_CXX_FLAGS_DEBUG "-g -pg" )
  set( CMAKE_CXX_FLAGS_RELEASE  "" )
  message( STATUS "Using g++ via mpicxx" )

  # Allow for unused parameters.
  add_definitions( -Wno-unused-parameter )

  # Workaround for known mpich c++ bug.
  add_definitions( -DMPICH_IGNORE_CXX_SEEK )
  add_definitions( -DMPICH_SKIP_MPICXX )

else()
  message( FATAL_ERROR "Invalid CXX compiler. Make sure you have g++ installed" )
endif ()

# Includsion from the source.
include_directories( ${KMCLib_SOURCE_DIR}/src )


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

