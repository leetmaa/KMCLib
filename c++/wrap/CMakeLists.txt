# Copyright (c)  2012-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

# -----------------------------------------------------------------------------
# LOAD SWIG
# -----------------------------------------------------------------------------

# Make sure the swig package is loaded.
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories( ${KMCLib_SOURCE_DIR}/src )

# Set the flags.
set(CMAKE_SWIG_FLAGS "")

# Set the properties for the interface file.
set_source_files_properties(backend.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(backend.i PROPERTIES SWIG_FLAGS "")


# -----------------------------------------------------------------------------
# SET COMPILER FLAGS TO IGNORE KNOWN SWIG GENERATED ERRORS
# -----------------------------------------------------------------------------
add_definitions( -Wno-uninitialized )

# For older g++.
add_definitions( -Wno-missing-field-initializers )
add_definitions( -Wno-write-strings )

# For clang - FIXME
#add_definitions( -Wno-self-assign )
#add_definitions( -Wno-dynamic-class-memaccess )
add_definitions( -fno-strict-aliasing )

# Add the target.
swig_add_module( Backend python backend.i )


# -----------------------------------------------------------------------------
# LINK
# -----------------------------------------------------------------------------

message( STATUS "Creating makefiles for system: ${CMAKE_SYSTEM}")
# For Mac OS X
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  # To force a Mac OSX with macports Python, use with -DMACPORT=TRUE
  if (MACPORT)
    set(CMAKE_LIBRARY_PATH "/opt/local/Library/Frameworks")
    message( STATUS "Looking for libraries in ${CMAKE_LIBRARY_PATH}" )
  endif()

  find_library( PYTHON_LIB python )
  message( STATUS "Using python library ${PYTHON_LIB}")
  swig_link_libraries( Backend ${PYTHON_LIB} src )

# For Linux (Ubuntu 12.04 LTS)
else()#${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  swig_link_libraries( Backend src )
endif()

