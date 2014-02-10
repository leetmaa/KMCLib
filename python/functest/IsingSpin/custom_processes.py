# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib import *

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00]]

# Flip 'U' to 'D'.
process_0 = KMCProcess(coordinates=coordinates,
                       elements_before=['U'],
                       elements_after=['D'],
                       basis_sites=[0],
                       rate_constant=1.0)

# Flip 'D' to 'U'.
process_1 = KMCProcess(coordinates=coordinates,
                       elements_before=['D'],
                       elements_after=['U'],
                       basis_sites=[0],
                       rate_constant=1.0)

# Create the interactions object.
interactions = KMCInteractions(processes=[process_0, process_1],
                               implicit_wildcards=True)
