""" Module for testing the XYZTrajectory object. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import os
import sys
import numpy

from KMCLib.KMCUnitCell import KMCUnitCell
from KMCLib.KMCLattice import KMCLattice
from KMCLib.KMCConfiguration import KMCConfiguration

# Import from the module we test.
from KMCLib.Utilities.Trajectory.XYZTrajectory import XYZTrajectory


# Implement the test.
class XYZTrajectoryTest(unittest.TestCase):
    """ Class for testing the XYZTrajectory object. """

    def setUp(self):
        """ The setUp method for test fixtures. """
        self.__files_to_remove = []

    def tearDown(self):
        """ The tearDown method for test fixtures. """
        for f in self.__files_to_remove:
            os.remove(f)

    def testConstruction(self):
        """ Test that the XYZTrajectory object can be constructed. """
        filename = "abc123.xyz"
        unit_cell = KMCUnitCell(cell_vectors=[[1.0, 0.0, 0.0],
                                              [0.0, 1.0, 0.0],
                                              [0.0, 0.0, 1.0]],
                                basis_points=[[0.0, 0.0, 0.0]])
        lattice = KMCLattice(unit_cell=unit_cell,
                             periodic=(True, True, True),
                             repetitions=(4,4,4))

        config = KMCConfiguration(lattice=lattice,
                                 types=["A","B","C","D"]*16)

        t = XYZTrajectory(trajectory_filename=filename,
                          configuration=config,
                          max_buffer_size=12345,
                          max_buffer_time=123.0)

        # Check that the internal memory buffers have been initiated.
        self.assertEqual(t._XYZTrajectory__atom_id_types, [])
        self.assertEqual(t._XYZTrajectory__atom_id_coordinates, [])
        self.assertEqual(t._XYZTrajectory__time, [])
        self.assertEqual(t._XYZTrajectory__step, [])

    def testWriteHeader(self):
        """ Test the header output. """
        # Get a file name.
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory.xyz")
        self.__files_to_remove.append(trajectory_filename)

        # Setup the trajectory object.
        unit_cell = KMCUnitCell(cell_vectors=[[1.0, 0.0, 0.0],
                                              [0.0, 1.0, 0.0],
                                              [0.0, 0.0, 1.0]],
                                basis_points=[[0.0, 0.0, 0.0]])
        lattice = KMCLattice(unit_cell=unit_cell,
                             periodic=(True, True, True),
                             repetitions=(4,4,4))

        config = KMCConfiguration(lattice=lattice,
                                 types=["A","B","C","D"]*16)

        t = XYZTrajectory(trajectory_filename=trajectory_filename,
                          configuration=config,
                          max_buffer_size=12345,
                          max_buffer_time=123.0)

        # Check that the header was written.
        self.assertTrue(os.path.exists(trajectory_filename))

        # Check the content of the header.
        with open(trajectory_filename, "r") as f:
            content = f.read()

        ref_content = """KMCLib XYZ FORMAT VERSION 2013.10.15

CELL VECTORS
a: 1.0000000000e+00 0.0000000000e+00 0.0000000000e+00
b: 0.0000000000e+00 1.0000000000e+00 0.0000000000e+00
c: 0.0000000000e+00 0.0000000000e+00 1.0000000000e+00

REPETITIONS 4 4 4

PERIODICITY True True True

"""
        self.assertEqual( content, ref_content )

    def testStoreData(self):
        """ Test the data storage function. """
        # Get a file name.
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory.xyz")
        self.__files_to_remove.append(trajectory_filename)

        # Setup the trajectory object.
        unit_cell = KMCUnitCell(cell_vectors=[[1.0, 0.0, 0.0],
                                              [0.0, 1.0, 0.0],
                                              [0.0, 0.0, 1.0]],
                                basis_points=[[0.0, 0.0, 0.0]])
        lattice = KMCLattice(unit_cell=unit_cell,
                             periodic=(True, True, True),
                             repetitions=(4,4,4))

        config = KMCConfiguration(lattice=lattice,
                                 types=["A","B","C","D"]*16)

        t = XYZTrajectory(trajectory_filename=trajectory_filename,
                          configuration=config,
                          max_buffer_size=12345,
                          max_buffer_time=123.0)

        # Construct data to store.
        simulation_time = 1.234
        step = 123
        configuration = config

        # Store.
        t._storeData(simulation_time, step, configuration)

        # Check that the member data was updated.
        self.assertEqual(len(t._XYZTrajectory__atom_id_types), 1)
        self.assertEqual(len(t._XYZTrajectory__atom_id_coordinates), 1)
        self.assertEqual(len(t._XYZTrajectory__time), 1)
        self.assertEqual(len(t._XYZTrajectory__step), 1)

        # store again.
        simulation_time = 1.999
        step = 4444
        t._storeData(simulation_time, step, configuration)

        # Check that the member data was updated.
        self.assertEqual(len(t._XYZTrajectory__atom_id_types), 2)
        self.assertEqual(len(t._XYZTrajectory__atom_id_coordinates), 2)
        self.assertEqual(len(t._XYZTrajectory__time), 2)
        self.assertEqual(len(t._XYZTrajectory__step), 2)

        # Check the values of the stored data.
        self.assertAlmostEqual(t._XYZTrajectory__time[0], 1.234, 10)
        self.assertAlmostEqual(t._XYZTrajectory__time[1], 1.999, 10)

        self.assertEqual(t._XYZTrajectory__step[0],  123)
        self.assertEqual(t._XYZTrajectory__step[1], 4444)

        diff = numpy.linalg.norm(config.atomIDCoordinates() - t._XYZTrajectory__atom_id_coordinates[0])
        self.assertAlmostEqual(diff, 0.0, 10)

        diff = numpy.linalg.norm(config.atomIDCoordinates() - t._XYZTrajectory__atom_id_coordinates[1])
        self.assertAlmostEqual(diff, 0.0, 10)

        self.assertEqual(t._XYZTrajectory__atom_id_types[0], config.atomIDTypes())
        self.assertEqual(t._XYZTrajectory__atom_id_types[1], config.atomIDTypes())

    def testBufferSize(self):
        """ Test the header output. """
        # Get a file name.
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory.xyz")
        self.__files_to_remove.append(trajectory_filename)

        # Setup the trajectory object.
        unit_cell = KMCUnitCell(cell_vectors=[[1.0, 0.0, 0.0],
                                              [0.0, 1.0, 0.0],
                                              [0.0, 0.0, 1.0]],
                                basis_points=[[0.0, 0.0, 0.0]])
        lattice = KMCLattice(unit_cell=unit_cell,
                             periodic=(True, True, True),
                             repetitions=(4,4,4))

        config = KMCConfiguration(lattice=lattice,
                                 types=["A","B","C","D"]*16)

        t = XYZTrajectory(trajectory_filename=trajectory_filename,
                          configuration=config,
                          max_buffer_size=12345,
                          max_buffer_time=123.0)

        # Check that the empty buffers has the size of four empty lists.
        buffer_size = t._bufferSize()
        self.assertEqual(buffer_size, sys.getsizeof([])*4)

        # Store a bunch of data.
        simulation_time = 1.234
        step = 123
        t._storeData(simulation_time, step, config)
        buffer_size = t._bufferSize()

        # Check the size again.
        ref_size =  sys.getsizeof(t._XYZTrajectory__atom_id_coordinates)
        ref_size += sys.getsizeof(t._XYZTrajectory__atom_id_types)
        ref_size += sys.getsizeof(t._XYZTrajectory__time)
        ref_size += sys.getsizeof(t._XYZTrajectory__step)

        self.assertEqual(buffer_size, ref_size)

        # Store more data.
        simulation_time = 1.234
        step = 123
        t._storeData(simulation_time, step, config)

        # Check the size again.
        t._storeData(simulation_time, step, config)
        t._storeData(simulation_time, step, config)
        t._storeData(simulation_time, step, config)
        t._storeData(simulation_time, step, config)
        t._storeData(simulation_time, step, config)

        ref_size =  sys.getsizeof(t._XYZTrajectory__atom_id_coordinates)
        ref_size += sys.getsizeof(t._XYZTrajectory__atom_id_types)
        ref_size += sys.getsizeof(t._XYZTrajectory__time)
        ref_size += sys.getsizeof(t._XYZTrajectory__step)

        buffer_size = t._bufferSize()

        self.assertEqual(buffer_size, ref_size)


if __name__ == '__main__':
    unittest.main()
