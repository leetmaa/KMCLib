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
#include_directories( ${KMCLib_SOURCE_DIR}/custom )

# Set the flags.
set(CMAKE_SWIG_FLAGS "")

# Generate the custom.i wrapper file.
message( STATUS "Generating custom.i" )
file( WRITE ${KMCLib_SOURCE_DIR}/wrap/custom.i "// AUTO GENERATED FROM CAMKE - DO NOT EDIT MANUALLY"\n)
file( APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "%include " \"std_vector.i\" \n)
file( APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "%include " \"std_map.i\" \n)
file( APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "%include " \"std_string.i\" \n)
file( APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "%include " \"std_pair.i\" \n)
file( APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "%include " \"ratecalculator.h\" \n)
file( APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "%module Custom"\n)
file( APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "%{"\n )

# Find out which files to include.
file( GLOB CustomIncludes ../custom/*.h )

# Include the files.
foreach( FILE ${CustomIncludes})
  file(APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "#include \""${FILE}\"\n)
endforeach(FILE)
file(APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "%}"\n)
foreach( FILE ${CustomIncludes})
  file(APPEND ${KMCLib_SOURCE_DIR}/wrap/custom.i "%include \""${FILE}\"\n)
endforeach(FILE)

# Set the properties for the interface files.
set_source_files_properties(backend.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(backend.i PROPERTIES SWIG_FLAGS "")

set_source_files_properties(custom.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(custom.i PROPERTIES SWIG_FLAGS "")


# -----------------------------------------------------------------------------
# SET COMPILER FLAGS TO IGNORE KNOWN SWIG GENERATED ERRORS
# -----------------------------------------------------------------------------
add_definitions( -Wno-uninitialized )

# For intel.
if (USING_INTEL)
   message( STATUS "Ignoring Intel SWIG warnings")
  add_definitions( -Wno-unused-but-set-variable )

elseif (CMAKE_COMPILER_IS_GNUCXX)
  message( STATUS "Ignoring g++ SWIG warnings")
  add_definitions( -Wno-unused-but-set-variable )
  # For older g++.
  add_definitions( -Wno-missing-field-initializers )
  add_definitions( -Wno-write-strings )
  add_definitions( -fno-strict-aliasing )
  add_definitions( -Wno-unused-but-set-variable )
else()
  message( STATUS "Ignoring Clang SWIG warnings")
  add_definitions( -Wno-unused-variable )
  add_definitions( -Wno-self-assign )
  add_definitions( -Wno-dynamic-class-memaccess )
endif()


# Add the target.
swig_add_module( Backend python backend.i )
swig_add_module( Custom  python custom.i )


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
  swig_link_libraries( Custom  ${PYTHON_LIB} custom src )

# For Linux (Ubuntu 12.04 LTS)
else()#${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  swig_link_libraries( Backend src )
  swig_link_libraries( Custom  custom src )
endif()
