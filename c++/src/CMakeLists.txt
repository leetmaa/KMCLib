# Copyright (c)  2012-2016  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

file( GLOB CppSources *.cpp )

file( GLOB ExternalObj ${KMCLib_SOURCE_DIR}/externals/obj/*.o )

add_library( src ${CppSources} ${ExternalObj} )
