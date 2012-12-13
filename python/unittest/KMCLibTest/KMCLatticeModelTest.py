""" Module for testing the KMCLatticeModel class. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

from KMCLib.KMCInteractions import KMCInteractions
from KMCLib.KMCConfiguration import KMCConfiguration
from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.KMCUnitCell import KMCUnitCell
from KMCLib.KMCLattice  import KMCLattice

# Import from the module we test.
from KMCLib.KMCLatticeModel import KMCLatticeModel

# Implement the test.
class KMCLatticeModelTest(unittest.TestCase):
    """ Class for testing the KMCLatticeModel class. """

    def testConstruction(self):
        """ Test the construction of the lattice model """
        # Setup a unitcell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.8,0.0,0.0],
                                                          [0.0,3.2,0.0],
                                                          [0.0,0.5,3.0]]),
                                basis_points=[[0.0,0.0,0.0],
                                              [0.5,0.5,0.5],
                                              [0.25,0.25,0.75]])

        # Setup the lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(4,4,1),
                             periodic=(True,True,False))

        types = ['A','A','A','A','B','B',
                 'A','A','A','B','B','B',
                 'B','B','A','A','B','A',
                 'B','B','B','A','B','A',
                 'B','A','A','A','B','B',
                 'B','B','B','B','B','B',
                 'A','A','A','A','B','B',
                 'B','B','A','B','B','A']

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=['A','C','B'])


        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        # Construct the interactions object.
        interactions_list = [interaction_0, interaction_1]
        interactions = KMCInteractions(interactions_list=interactions_list)

        # Construct the model.
        model = KMCLatticeModel(config, interactions)

        # Check that it has the attribute _backend which is None
        self.assertTrue(hasattr(model,"_KMCLatticeModel__backend"))
        self.assertTrue(model._KMCLatticeModel__backend is None)

        # Check that it has the correct interactions stored.
        self.assertTrue(model._KMCLatticeModel__interactions == interactions)

        # Check that it has the correct configuration stored.
        self.assertTrue(model._KMCLatticeModel__configuration == config)

    def testBackend(self):
        """ Test that the backend object is correctly constructed. """
        # Setup a unitcell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.8,0.0,0.0],
                                                          [0.0,3.2,0.0],
                                                          [0.0,0.5,3.0]]),
                                basis_points=[[0.0,0.0,0.0],
                                              [0.5,0.5,0.5],
                                              [0.25,0.25,0.75]])

        # Setup the lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(4,4,1),
                             periodic=(True,True,False))

        types = ['A','A','A','A','B','B',
                 'A','A','A','B','B','B',
                 'B','B','A','A','B','A',
                 'B','B','B','A','B','A',
                 'B','A','A','A','B','B',
                 'B','B','B','B','B','B',
                 'A','A','A','A','B','B',
                 'B','B','A','B','B','A']

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=['A','C','B'])


        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        # Construct the interactions object.
        interactions_list = [interaction_0, interaction_1]
        interactions = KMCInteractions(interactions_list=interactions_list)

        # Construct the model.
        model = KMCLatticeModel(config, interactions)

        # Get the c++ backend out.
        cpp_model = model._backend()

        # Check that this backend object is stored on the class.
        self.assertTrue(model._KMCLatticeModel__backend == cpp_model)

        # Check that we can call the singleStep function.
        cpp_model.singleStep()

    def testScript(self):
        """ Test that a script can be created. """
        # Setup a unitcell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.8,0.0,0.0],
                                                          [0.0,3.2,0.0],
                                                          [0.0,0.5,3.0]]),
                                basis_points=[[0.0,0.0,0.0],
                                              [0.5,0.5,0.5],
                                              [0.25,0.25,0.75]])

        # Setup the lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(4,4,1),
                             periodic=(True,True,False))

        types = ['A','A','A','A','B','B',
                 'A','A','A','B','B','B',
                 'B','B','A','A','B','A',
                 'B','B','B','A','B','A',
                 'B','A','A','A','B','B',
                 'B','B','B','B','B','B',
                 'A','A','A','A','B','B',
                 'B','B','A','B','B','A']

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=['A','C','B'])


        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        # Construct the interactions object.
        interactions_list = [interaction_0, interaction_1]
        interactions = KMCInteractions(interactions_list=interactions_list)

        # Construct the model.
        model = KMCLatticeModel(config, interactions)

        # Get the script.
        script = model._script()

        ref_script = """
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
"""
        self.assertEqual(script, ref_script)

        # Get the script again, with a different variable name.
        script = model._script(variable_name="my_model")

        ref_script = """
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

my_model = KMCLatticeModel(
    configuration=configuration,
    interactions=interactions)
"""
        # Check.
        self.assertEqual(script, ref_script)


if __name__ == '__main__':
    unittest.main()

