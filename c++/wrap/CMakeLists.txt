# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

# Make sure the swig and python packages are loaded.
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
#find_package(PythonLibs)

# Add the inclusion paths.
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set the flags.
set(CMAKE_SWIG_FLAGS "")

# Set the properties for the interface file.
set_source_files_properties(backend.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(backend.i PROPERTIES SWIG_FLAGS "")

# Set the flag to ignore error from SWIG auto generated code.
add_definitions( -Wno-maybe-uninitialized)
add_definitions( -Wno-uninitialized )

# Add the target.
swig_add_module(Backend python backend.i)
swig_link_libraries(Backend src)
