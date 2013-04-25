""" Module for testing the KMCLatticeModel class. """


# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy
import os

from KMCLib.KMCInteractions import KMCInteractions
from KMCLib.KMCProcess import KMCProcess
from KMCLib.KMCConfiguration import KMCConfiguration
from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.KMCControlParameters import KMCControlParameters
from KMCLib.KMCUnitCell import KMCUnitCell
from KMCLib.KMCLattice  import KMCLattice

# Import from the module we test.
from KMCLib.KMCLatticeModel import KMCLatticeModel

# Test helpers.
from TestUtilities.Plugins.CustomRateCalculator.CustomRateCalculator import CustomRateCalculator
from KMCLib.Backend.Backend import MPICommons

# Implement the test.
class KMCLatticeModelTest(unittest.TestCase):
    """ Class for testing the KMCLatticeModel class. """

    def setUp(self):
        """ The setUp method for test fixtures. """
        self.__files_to_remove = []

    def tearDown(self):
        """ The tearDown method for test fixtures. """
        for f in self.__files_to_remove:
            if MPICommons.isMaster():
                os.remove(f)
            MPICommons.barrier()

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

        # A first process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","B"]
        types1 = ["B","A"]
        sites  = [0,1,2]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords,
                               types0,
                               types1,
                               basis_sites=sites,
                               rate_constant=rate_0_1)

        # A second process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","C"]
        types1 = ["C","A"]
        sites  = [0,1,2]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords,
                               types0,
                               types1,
                               basis_sites=sites,
                               rate_constant=rate_0_1)

        # Construct the interactions object.
        processes = [process_0, process_1]
        interactions = KMCInteractions(processes=processes)

        # Construct the model.
        model = KMCLatticeModel(config, interactions)

        # Check that it has the attribute _backend which is None
        self.assertTrue(hasattr(model,"_KMCLatticeModel__backend"))
        self.assertTrue(model._KMCLatticeModel__backend is None)

        # Check that it has the correct interactions stored.
        self.assertTrue(model._KMCLatticeModel__interactions == interactions)

        # Check that it has the correct configuration stored.
        self.assertTrue(model._KMCLatticeModel__configuration == config)


    def testRunImplicitWildcards(self):
        """ Test that ta valid model can run for a few steps. """
        # Setup a unitcell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[1.0,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0]])
        # And a lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(10,10,1),
                             periodic=(True,True,False))

        # Set the stating configuration types.
        types = ['B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
                 'B', 'B', 'B', 'B', 'B', 'A', 'B', 'B', 'B', 'B',
                 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
                 'B', 'A', 'B', 'B', 'B', 'B', 'B', 'A', 'B', 'B',
                 'B', 'B', 'B', 'B', 'B', 'B', 'A', 'B', 'B', 'B',
                 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
                 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
                 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
                 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'A', 'B', 'B',
                 'A', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'A']

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=["A","B"])

        # Generate the interactions with a distance so large that we get a
        # layer of implicite wildcards in the C++ matchlists.
        sites = [0]
        coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
                       [  -1.000000e+00,   0.000000e+00,   0.000000e+00],
                       [   1.000000e+00,   0.000000e+00,   0.000000e+00],
                       [   0.000000e+00,  -1.000000e+00,   0.000000e+00],
                       [   0.000000e+00,   1.000000e+00,   0.000000e+00],
                       [   2.000000e+00,   2.000000e+00,   0.000000e+00]]

        types0 = ['A', 'B', 'B', 'B', 'B', 'A']
        types1 = ['B', 'A', 'B', 'B', 'B', 'A']
        process_0 = KMCProcess(coordinates, types0, types1, None, sites, 1.0)

        types0 = ['A', 'B', 'B', 'B', 'B', 'B']
        types1 = ['B', 'B', 'A', 'B', 'B', 'B']
        process_1 = KMCProcess(coordinates, types0, types1, None, sites, 1.0)

        types0 = ['A', 'B', 'B', 'B', 'B', 'B']
        types1 = ['B', 'B', 'B', 'A', 'B', 'B']
        process_2 = KMCProcess(coordinates, types0, types1, None, sites, 1.0)

        types0 = ['A', 'B', 'B', 'B', 'B', 'B']
        types1 = ['B', 'B', 'B', 'B', 'A', 'B']
        process_3 = KMCProcess(coordinates, types0, types1, None, sites, 1.0)

        # Processes.
        processes = [process_0,
                     process_1,
                     process_2,
                     process_3]

        # No implicit wildcards.
        interactions = KMCInteractions(processes=processes,
                                       implicit_wildcards=False)

        # Create the model.
        model = KMCLatticeModel(config, interactions)

        # Get the match types out.
        match_types = [ l.match_type for l in model._backend().interactions().processes()[0].minimalMatchList() ]

        # This does not have wildcards added.
        ref_match_types = [1, 2, 2, 2, 2, 1]
        self.assertEqual( match_types, ref_match_types )

        # Create with implicit wildcards - this is default behavior.
        interactions = KMCInteractions(processes=processes)

        # Create the model.
        model = KMCLatticeModel(config, interactions)

        # Check the process matchlists again.
        match_types = [ l.match_type for l in model._backend().interactions().processes()[0].minimalMatchList() ]

        ref_match_types = [1, 2, 2, 2, 2,
                           0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0,
                           0, 0, 0, 0, 1]

        # This one has the wildcards (zeroes) added.
        self.assertEqual( match_types, ref_match_types )

        # Setup the run paramters.
        control_parameters = KMCControlParameters(number_of_steps=10,
                                                  dump_interval=1)
        model.run(control_parameters)

    def testRun2(self):
        """ Test the run of an A-B flip model. """
        # Cell.
        cell_vectors = [[   1.000000e+00,   0.000000e+00,   0.000000e+00],
                        [   0.000000e+00,   1.000000e+00,   0.000000e+00],
                        [   0.000000e+00,   0.000000e+00,   1.000000e+00]]

        basis_points = [[   0.000000e+00,   0.000000e+00,   0.000000e+00]]

        unit_cell = KMCUnitCell(
            cell_vectors=cell_vectors,
            basis_points=basis_points)

        # Lattice.
        lattice = KMCLattice(
            unit_cell=unit_cell,
            repetitions=(10,10,1),
            periodic=(True, True, False))

        # Configuration.
        types = ['B']*100
        possible_types = ['A','B']
        configuration = KMCConfiguration(
            lattice=lattice,
            types=types,
            possible_types=possible_types)

        # Interactions.
        coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00]]
        process_0 = KMCProcess(coordinates,
                               ['A'],
                               ['B'],
                               basis_sites=[0],
                               rate_constant=4.0)
        process_1 = KMCProcess(coordinates,
                               ['B'],
                               ['A'],
                               basis_sites=[0],
                               rate_constant=1.0)

        processes = [process_0, process_1]
        interactions = KMCInteractions(processes)

        # Setup the model.
        ab_flip_model = KMCLatticeModel(configuration, interactions)

        # Run the model with a trajectory file.
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "ab_flip_traj.py")
        self.__files_to_remove.append(trajectory_filename)

        # The control parameters.
        control_parameters = KMCControlParameters(number_of_steps=1000,
                                                  dump_interval=500)

        # Run the model for 1000 steps.
        ab_flip_model.run(control_parameters,
                          trajectory_filename=trajectory_filename)

        # Read the first last frames from the trajectory file and check that
        # the fraction of A is close to 20% in the last, and 0 in the first.
        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        # Count the first frame.
        elem = local_dict["types"][0]
        nA = len([ ee for ee in elem if ee == "A" ])
        nB = len([ ee for ee in elem if ee == "B" ])
        self.assertEqual(nA, 0)
        self.assertEqual(nB, 100)

        # Count the last frame.
        elem = local_dict["types"][-1]
        nA = len([ ee for ee in elem if ee == "A" ])
        nB = len([ ee for ee in elem if ee == "B" ])

        # Note that the average should be 20.0% over a long run.
        # It is pure luck that it is exact at this particular
        # step with the presently used random number seed.
        fraction =  nA * 100.0 / (nA + nB)
        target = 20.0
        self.assertAlmostEqual(fraction, target, 3)

    def testCustomRatesRun(self):
        """ Test the run of an A-B flip model with custom rates. """
        # Cell.
        cell_vectors = [[   1.000000e+00,   0.000000e+00,   0.000000e+00],
                        [   0.000000e+00,   1.000000e+00,   0.000000e+00],
                        [   0.000000e+00,   0.000000e+00,   1.000000e+00]]

        basis_points = [[   0.000000e+00,   0.000000e+00,   0.000000e+00]]

        unit_cell = KMCUnitCell(
            cell_vectors=cell_vectors,
            basis_points=basis_points)

        # Lattice.
        lattice = KMCLattice(
            unit_cell=unit_cell,
            repetitions=(10,10,1),
            periodic=(True, True, False))

        # Configuration.
        types = ['B']*100
        possible_types = ['A','B']
        configuration = KMCConfiguration(
            lattice=lattice,
            types=types,
            possible_types=possible_types)

        # Interactions.
        coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00]]
        process_0 = KMCProcess(coordinates, ['A'], ['B'], None, [0], 4.0)
        process_1 = KMCProcess(coordinates, ['B'], ['A'], None, [0], 1.0)
        processes = [process_0, process_1]
        interactions = KMCInteractions(processes,
                                       implicit_wildcards=True)

        # Custom rates.
        rate_calculator = CustomRateCalculator
        interactions.setRateCalculator(rate_calculator)

        # Setup the model.
        ab_flip_model = KMCLatticeModel(configuration, interactions)

        # Run the model with a trajectory file.
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "ab_flip_traj.py")
        self.__files_to_remove.append(trajectory_filename)

        # The control parameters.
        control_parameters = KMCControlParameters(number_of_steps=1000,
                                                  dump_interval=500)

        # Run the model for 1000 steps.
        ab_flip_model.run(control_parameters,
                          trajectory_filename=trajectory_filename)

        # Read the first last frames from the trajectory file and check that
        # the fraction of A is close to 75% in the last, and 0 in the first.
        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        # Count the first frame.
        elem = local_dict["types"][0]
        nA = len([ ee for ee in elem if ee == "A" ])
        nB = len([ ee for ee in elem if ee == "B" ])
        self.assertEqual(nA, 0)
        self.assertEqual(nB, 100)

        # Count the last frame.
        elem = local_dict["types"][-1]
        nA = len([ ee for ee in elem if ee == "A" ])
        nB = len([ ee for ee in elem if ee == "B" ])

        # Note that the average over a long simulation should be
        # 75% A using the modified rate function. In this particular
        # step the A population is 74%.
        value = 1.0 * nA / (nA + nB)
        self.assertAlmostEqual(0.74, value, 2)

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


        # A first process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","B"]
        types1 = ["B","A"]
        sites  = [0,1,2,3,4]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords,
                               types0,
                               types1,
                               basis_sites=sites,
                               rate_constant=rate_0_1)

        # A second process.
        types0 = ["A","C"]
        types1 = ["C","A"]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords,
                               types0,
                               types1,
                               basis_sites=sites,
                               rate_constant=rate_0_1)

        # Construct the interactions object.
        processes = [process_0, process_1]
        interactions = KMCInteractions(processes=processes)

        # Construct the model.
        model = KMCLatticeModel(config, interactions)

        # Get the c++ backend out.
        cpp_model = model._backend()

        # Check that this backend object is stored on the class.
        self.assertTrue(model._KMCLatticeModel__backend == cpp_model)

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

        # A first process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","B"]
        types1 = ["B","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords,
                               types0,
                               types1,
                               basis_sites=[0],
                               rate_constant=rate_0_1)

        # A second process.
        types0 = ["A","C"]
        types1 = ["C","A"]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords,
                               types0,
                               types1,
                               basis_sites=[0],
                               rate_constant=rate_0_1)

        # Construct the interactions object.
        processes = [process_0, process_1]
        interactions = KMCInteractions(processes)

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
# Interactions

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

elements_before = ['A','B']
elements_after  = ['B','A']
move_vectors    = [(  0,[   1.000000e-01,  -8.000000e-01,  -2.100000e+00]),
                   (  1,[  -1.000000e-01,   8.000000e-01,   2.100000e+00])]
basis_sites     = [0]
rate_constant   =    3.500000e+00

process_0 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    move_vectors=move_vectors,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

elements_before = ['A','C']
elements_after  = ['C','A']
move_vectors    = [(  0,[   1.000000e-01,  -8.000000e-01,  -2.100000e+00]),
                   (  1,[  -1.000000e-01,   8.000000e-01,   2.100000e+00])]
basis_sites     = [0]
rate_constant   =    1.500000e+00

process_1 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    move_vectors=move_vectors,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

processes = [process_0,
             process_1]

interactions = KMCInteractions(
    processes=processes,
    implicit_wildcards=True)

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
# Interactions

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

elements_before = ['A','B']
elements_after  = ['B','A']
move_vectors    = [(  0,[   1.000000e-01,  -8.000000e-01,  -2.100000e+00]),
                   (  1,[  -1.000000e-01,   8.000000e-01,   2.100000e+00])]
basis_sites     = [0]
rate_constant   =    3.500000e+00

process_0 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    move_vectors=move_vectors,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

elements_before = ['A','C']
elements_after  = ['C','A']
move_vectors    = [(  0,[   1.000000e-01,  -8.000000e-01,  -2.100000e+00]),
                   (  1,[  -1.000000e-01,   8.000000e-01,   2.100000e+00])]
basis_sites     = [0]
rate_constant   =    1.500000e+00

process_1 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    move_vectors=move_vectors,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

processes = [process_0,
             process_1]

interactions = KMCInteractions(
    processes=processes,
    implicit_wildcards=True)

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

