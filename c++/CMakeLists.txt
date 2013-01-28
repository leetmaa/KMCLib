# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.8)
project(KMCLib)

# To use verbose output
set( CMAKE_VERBOSE_MAKEFILE off )

# Setup general compiler options.
add_definitions(-std=c++0x -O3 -fPIC -Wall -Wextra -Werror -pedantic)

# Compile the source.
add_subdirectory(src)

# Compile the tests.
add_subdirectory(unittest)

# Make the wrapping.
add_subdirectory(wrap)

# Install.
install( FILES ${CMAKE_BINARY_DIR}/wrap/Backend.py ${CMAKE_BINARY_DIR}/wrap/_Backend.so
         DESTINATION ${CMAKE_BINARY_DIR}/../../python/src/KMCLib/Backend/ )

