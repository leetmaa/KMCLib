# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

# Make sure the swig and python packages are loaded.
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

# To force a Mac OSX with macports Python, use with -DMACPORT=TRUE
if (MACPORT)
   set(CMAKE_LIBRARY_PATH "/opt/local/Library/Frameworks")
   message( STATUS "Looking for libraries in ${CMAKE_LIBRARY_PATH}" )
endif()

# Find the python library.
find_library( PYTHON_LIB python )
message( STATUS "Using python library ${PYTHON_LIB}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories( ${KMCLib_SOURCE_DIR}/src )

# Set the flags.
set(CMAKE_SWIG_FLAGS "")

# Set the properties for the interface file.
set_source_files_properties(backend.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(backend.i PROPERTIES SWIG_FLAGS "")

# Set the flag to ignore error from SWIG auto generated code.
add_definitions( -Wno-uninitialized )

# For old g++.
add_definitions( -Wno-missing-field-initializers )
add_definitions( -Wno-write-strings )

# For clang.
#add_definitions( -Wno-self-assign )
#add_definitions( -Wno-dynamic-class-memaccess )

# Add the target.
swig_add_module( Backend python backend.i )
swig_link_libraries( Backend ${PYTHON_LIB} src )
