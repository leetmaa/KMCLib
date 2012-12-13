from KMCLib import *

# -----------------------------------------------------------------------------
# Unit cell

cell_vectors = [[   2.800000e+00,   0.000000e+00,   0.000000e+00],
                [   0.000000e+00,   3.200000e+00,   0.000000e+00],
                [   0.000000e+00,   5.000000e-01,   3.000000e+00]]

basis_points = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
                [   5.000000e-01,   5.000000e-01,   5.000000e-01],
                [   2.500000e-01,   2.500000e-01,   7.500000e-01]]

unit_cell = KMCUnitCell(
    cell_vectors=cell_vectors,
    basis_points=basis_points)

# -----------------------------------------------------------------------------
# Lattice

lattice = KMCLattice(
    unit_cell=unit_cell,
    repetitions=(4,4,1),
    periodic=(True, True, False))

# -----------------------------------------------------------------------------
# Configuration

types = ['A','A','A','A','B','B','A','A','A','B','B','B','B',
         'B','A','A','B','A','B','B','B','A','B','A','B','A',
         'A','A','B','B','B','B','B','B','B','B','A','A','A',
         'A','B','B','B','B','A','B','B','A']

possible_types = ['A','C','B']

configuration = KMCConfiguration(
    lattice=lattice,
    types=types,
    possible_types=possible_types)

# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['A', 'B']

conf1_0 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['B', 'A']

conf2_0 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['A', 'C']

conf1_1 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['C', 'A']

conf2_1 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Interactions

interactions_list = [(conf1_0, conf2_0,    3.500000e+00),
                     (conf1_1, conf2_1,    1.500000e+00)]

interactions = KMCInteractions(interactions_list=interactions_list)

# -----------------------------------------------------------------------------
# Lattice model

model = KMCLatticeModel(
    configuration=configuration,
    interactions=interactions)

