# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

from KMCLib import *
# -----------------------------------------------------------------------------
# Interactions


# Oxygen - Vacancy jump.
elements_before = ['V', 'O']
elements_after  = ['O', 'V']

# Only on Oxygen sites.
basis_sites     = [0]

# Flow up.
rate_constant_up     = 0.9
rate_constant_down   = 1.1

# All other rates.
rate_constant        = 1.0

# Fill the list of processes.
processes = []

# Right.
coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e+00,   0.000000e+00,   0.000000e+00]]
processes.append( KMCProcess(coordinates=coordinates,
                             elements_before=elements_before,
                             elements_after=elements_after,
                             basis_sites=basis_sites,
                             rate_constant=rate_constant) )

# Left.
coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [  -1.000000e+00,   0.000000e+00,   0.000000e+00]]
processes.append( KMCProcess(coordinates=coordinates,
                             elements_before=elements_before,
                             elements_after=elements_after,
                             basis_sites=basis_sites,
                             rate_constant=rate_constant) )

# Forward.
coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   0.000000e+00,   1.000000e+00,   0.000000e+00]]
processes.append( KMCProcess(coordinates=coordinates,
                             elements_before=elements_before,
                             elements_after=elements_after,
                             basis_sites=basis_sites,
                             rate_constant=rate_constant) )

# Back.
coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   0.000000e+00,  -1.000000e+00,   0.000000e+00]]
processes.append( KMCProcess(coordinates=coordinates,
                             elements_before=elements_before,
                             elements_after=elements_after,
                             basis_sites=basis_sites,
                             rate_constant=rate_constant) )

# Up.
coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   0.000000e+00,   0.000000e+00,   1.000000e+00]]
processes.append( KMCProcess(coordinates=coordinates,
                             elements_before=elements_before,
                             elements_after=elements_after,
                             basis_sites=basis_sites,
                             rate_constant=rate_constant_up) )

# Down.
coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   0.000000e+00,   0.000000e+00,  -1.000000e+00]]
processes.append( KMCProcess(coordinates=coordinates,
                             elements_before=elements_before,
                             elements_after=elements_after,
                             basis_sites=basis_sites,
                             rate_constant=rate_constant_down) )

# The final interactions object.
interactions = KMCInteractions(
    processes=processes,
    implicit_wildcards=True)
