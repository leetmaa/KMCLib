# Copyright (c)  2012-2015  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
  set( CPPUNIT ${KMCLib_SOURCE_DIR}/externals/lib/cppunit/lib/libcppunit.dylib )
  add_definitions( -Wno-unused-value )

else()
  # Use the custom search path for the cppunit test library if given.
  message( STATUS "Custom search path for -lcppunit: ${CPPUNIT_DIR}")
  find_library( CPPUNIT NAMES cppunit PATHS
                ${CPPUNIT_DIR}
                ${KMCLib_SOURCE_DIR}/externals/lib/cppunit/lib )
endif()

# Use the custom search path for the cppunit test library if given.
message( STATUS "Custom search path for -lcppunit: ${CPPUNIT_DIR}")
find_library( CPPUNIT NAMES cppunit PATHS
              ${CPPUNIT_DIR}
              ${KMCLib_SOURCE_DIR}/externals/lib/cppunit/lib )
message( STATUS "Found cppunit:  ${CPPUNIT}")

# Find all test files.
file( GLOB CppSources test_* )

# Needed for clang.
add_definitions( -Wno-unused-parameter )
add_definitions( -Wno-unused-value )

# Compiler flag needed for using CPPUNIT.
add_definitions( -Wno-deprecated )

# Includsion from the source.
include_directories( ${KMCLib_SOURCE_DIR}/src )

# Compile the the unittest source.
add_library( unittest EXCLUDE_FROM_ALL ${CppSources} )

# Build and link the test runner.
add_executable( test.x EXCLUDE_FROM_ALL testRunner )

# Define the libraries to link the test.x executable against.
target_link_libraries( test.x ${CPPUNIT} unittest src custom )
