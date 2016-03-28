# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

file( GLOB CppSources *.cpp )

add_library( custom ${CppSources} )

add_definitions( -Wno-unused-private-field )
