# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#
add_definitions( -Wno-unused-parameter )
file( GLOB CppSources *.cpp )
add_library( src ${CppSources} )
