# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

# Find all test files.
file( GLOB CppSources test_* )

# Compiler flag needed for using CPPUNIT.
add_definitions( -Wno-deprecated )

# Includsion from the source.
include_directories( ${KMCLib_SOURCE_DIR}/src )

# Compile the the unittest source.
add_library( unittest ${CppSources} )

# Build and link the test runner.
add_executable( test.x testRunner )

# Define the libraries to link the test.x executable against.
target_link_libraries( test.x cppunit unittest src )

