# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

from KMCLib import *

def rate(diff):
    J   = 1.0
    kbT = 1.0
    from numpy import exp as np_exp
    return min(1.0, np_exp(-diff*J/kbT))

# -----------------------------------------------------------------------------
# Interactions

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e+00,   0.000000e+00,   0.000000e+00],
               [  -1.000000e+00,   0.000000e+00,   0.000000e+00],
               [   0.000000e+00,   1.000000e+00,   0.000000e+00],
               [   0.000000e+00,  -1.000000e+00,   0.000000e+00]]

elements_before = ['U', 'D', 'D', 'D', 'D']
elements_after  = ['D', 'D', 'D', 'D', 'D']
basis_sites     = [0]
diff = -4
rate_constant   = rate(diff)

process_0 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['U', 'U', 'D', 'D', 'D']
elements_after  = ['D', 'U', 'D', 'D', 'D']
diff = -2
rate_constant   = rate(diff)

process_1 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['U', 'D', 'U', 'D', 'D']
elements_after  = ['D', 'D', 'U', 'D', 'D']
diff = -2
rate_constant   = rate(diff)

process_2 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['U', 'D', 'D', 'U', 'D']
elements_after  = ['D', 'D', 'D', 'U', 'D']
diff = -2
rate_constant   = rate(diff)

process_3 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['U', 'D', 'D', 'D', 'U']
elements_after  = ['D', 'D', 'D', 'D', 'U']
diff = -2
rate_constant   = rate(diff)

process_4 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['U', 'U', 'U', 'D', 'D']
elements_after  = ['D', 'U', 'U', 'D', 'D']
diff = 0
rate_constant   = rate(diff)

process_5 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['U', 'U', 'D', 'U', 'D']
elements_after  = ['D', 'U', 'D', 'U', 'D']
diff = 0
rate_constant   = rate(diff)

process_6 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['U', 'U', 'D', 'D', 'U']
elements_after  = ['D', 'U', 'D', 'D', 'U']
diff = 0
rate_constant   = rate(diff)

process_7 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['U', 'D', 'U', 'U', 'D']
elements_after  = ['D', 'D', 'U', 'U', 'D']
diff = 0
rate_constant   = rate(diff)

process_8 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['U', 'D', 'U', 'D', 'U']
elements_after  = ['D', 'D', 'U', 'D', 'U']
diff = 0
rate_constant   = rate(diff)

process_9 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['U', 'D', 'D', 'U', 'U']
elements_after  = ['D', 'D', 'D', 'U', 'U']
diff = 0
rate_constant   = rate(diff)

process_10 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

elements_before = ['U', 'U', 'U', 'U', 'D']
elements_after  = ['D', 'U', 'U', 'U', 'D']
diff = 2
rate_constant   = rate(diff)

process_11 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['U', 'U', 'U', 'D', 'U']
elements_after  = ['D', 'U', 'U', 'D', 'U']
diff = 2
rate_constant   = rate(diff)

process_12 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['U', 'U', 'D', 'U', 'U']
elements_after  = ['D', 'U', 'D', 'U', 'U']
diff = 2
rate_constant   = rate(diff)

process_13 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['U', 'D', 'U', 'U', 'U']
elements_after  = ['D', 'D', 'U', 'U', 'U']
diff = 2
rate_constant   = rate(diff)

process_14 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['U', 'U', 'U', 'U', 'U']
elements_after  = ['D', 'U', 'U', 'U', 'U']
diff = 4
rate_constant   = rate(diff)

process_15 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------


elements_before = ['D', 'D', 'D', 'D', 'D']
elements_after  = ['U', 'D', 'D', 'D', 'D']
basis_sites     = [0]
diff = 4
rate_constant   = rate(diff)

process_16 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['D', 'U', 'D', 'D', 'D']
elements_after  = ['U', 'U', 'D', 'D', 'D']
diff = 2
rate_constant   = rate(diff)

process_17 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['D', 'D', 'U', 'D', 'D']
elements_after  = ['U', 'D', 'U', 'D', 'D']
diff = 2
rate_constant   = rate(diff)

process_18 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['D', 'D', 'D', 'U', 'D']
elements_after  = ['U', 'D', 'D', 'U', 'D']
diff = 2
rate_constant   = rate(diff)

process_19 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['D', 'D', 'D', 'D', 'U']
elements_after  = ['U', 'D', 'D', 'D', 'U']
diff = 2
rate_constant   = rate(diff)

process_20 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['D', 'U', 'U', 'D', 'D']
elements_after  = ['U', 'U', 'U', 'D', 'D']
diff = 0
rate_constant   = rate(diff)

process_21 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['D', 'U', 'D', 'U', 'D']
elements_after  = ['U', 'U', 'D', 'U', 'D']
diff = 0
rate_constant   = rate(diff)

process_22 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['D', 'U', 'D', 'D', 'U']
elements_after  = ['U', 'U', 'D', 'D', 'U']
diff = 0
rate_constant   = rate(diff)

process_23 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['D', 'D', 'U', 'U', 'D']
elements_after  = ['U', 'D', 'U', 'U', 'D']
diff = 0
rate_constant   = rate(diff)

process_24 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['D', 'D', 'U', 'D', 'U']
elements_after  = ['U', 'D', 'U', 'D', 'U']
diff = 0
rate_constant   = rate(diff)

process_25 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['D', 'D', 'D', 'U', 'U']
elements_after  = ['U', 'D', 'D', 'U', 'U']
diff = 0
rate_constant   = rate(diff)

process_26 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

elements_before = ['D', 'U', 'U', 'U', 'D']
elements_after  = ['U', 'U', 'U', 'U', 'D']
diff = -2
rate_constant   = rate(diff)

process_27 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['D', 'U', 'U', 'D', 'U']
elements_after  = ['U', 'U', 'U', 'D', 'U']
diff = -2
rate_constant   = rate(diff)

process_28 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['D', 'U', 'D', 'U', 'U']
elements_after  = ['U', 'U', 'D', 'U', 'U']
diff = -2
rate_constant   = rate(diff)

process_29 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

# -----------------------------------------------------------------------------

elements_before = ['D', 'D', 'U', 'U', 'U']
elements_after  = ['U', 'D', 'U', 'U', 'U']
diff = -2
rate_constant   = rate(diff)

process_30 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)


# -----------------------------------------------------------------------------

elements_before = ['D', 'U', 'U', 'U', 'U']
elements_after  = ['U', 'U', 'U', 'U', 'U']
diff = -4
rate_constant   = rate(diff)

process_31 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

###########################################################

processes = [process_0,
             process_1,
             process_2,
             process_3,
             process_4,
             process_5,
             process_6,
             process_7,
             process_8,
             process_9,
             process_10,
             process_11,
             process_12,
             process_13,
             process_14,
             process_15,
             process_16,
             process_17,
             process_18,
             process_19,
             process_20,
             process_21,
             process_22,
             process_23,
             process_24,
             process_25,
             process_26,
             process_27,
             process_28,
             process_29,
             process_30,
             process_31]

interactions = KMCInteractions(
    processes=processes,
    implicit_wildcards=True)
