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

from KMCLib.CoreComponents.KMCUnitCell import KMCUnitCell
from KMCLib.CoreComponents.KMCLattice import KMCLattice
from KMCLib.CoreComponents.KMCConfiguration import KMCConfiguration
from KMCLib.Backend.Backend import MPICommons

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
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        filename = os.path.join(name, filename)

        if MPICommons.isMaster():
            self.__files_to_remove.append(filename)

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

        if MPICommons.isMaster():
            self.__files_to_remove.append(trajectory_filename)

        # Setup the trajectory object.
        unit_cell = KMCUnitCell(cell_vectors=[[1.0, 0.0, 0.0],
                                              [0.0, 1.0, 0.0],
                                              [0.0, 0.0, 1.0]],
                                basis_points=[[0.0, 0.0, 0.0]])
        lattice = KMCLattice(unit_cell=unit_cell,
                             periodic=(True, True, True),
                             repetitions=(4,2,3))

        config = KMCConfiguration(lattice=lattice,
                                 types=["A","B","C"]*8)

        t = XYZTrajectory(trajectory_filename=trajectory_filename,
                          configuration=config,
                          max_buffer_size=12345,
                          max_buffer_time=123.0)

        if MPICommons.isMaster():

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

REPETITIONS 4 2 3

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

        if MPICommons.isMaster():
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
        """ Test the buffer size function. """
        # Get a file name.
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory.xyz")

        if MPICommons.isMaster():
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

        # Store a bunch of data.
        simulation_time = 1.234
        step = 123
        t._storeData(simulation_time, step, config)
        buffer_size = t._bufferSize()

        # Check the size again.
        ref_size =  sys.getsizeof(t._XYZTrajectory__atom_id_coordinates)
        ref_size += sys.getsizeof(t._XYZTrajectory__atom_id_coordinates[0])*len(t._XYZTrajectory__atom_id_coordinates)
        ref_size += sys.getsizeof(t._XYZTrajectory__atom_id_types)
        ref_size += sys.getsizeof(t._XYZTrajectory__atom_id_types[0])*len(t._XYZTrajectory__atom_id_types)
        ref_size += sys.getsizeof(t._XYZTrajectory__time)
        ref_size += sys.getsizeof(t._XYZTrajectory__time[0])*len(t._XYZTrajectory__time)
        ref_size += sys.getsizeof(t._XYZTrajectory__step)
        ref_size += sys.getsizeof(t._XYZTrajectory__step[0])*len(t._XYZTrajectory__step)

        self.assertEqual(buffer_size, ref_size)

        # Store more data.
        t._storeData(simulation_time, step, config)

        # Check the size again.
        t._storeData(simulation_time, step, config)
        t._storeData(simulation_time, step, config)
        t._storeData(simulation_time, step, config)
        t._storeData(simulation_time, step, config)
        t._storeData(simulation_time, step, config)

        ref_size =  sys.getsizeof(t._XYZTrajectory__atom_id_coordinates)
        ref_size += sys.getsizeof(t._XYZTrajectory__atom_id_coordinates[0])*len(t._XYZTrajectory__atom_id_coordinates)
        ref_size += sys.getsizeof(t._XYZTrajectory__atom_id_types)
        ref_size += sys.getsizeof(t._XYZTrajectory__atom_id_types[0])*len(t._XYZTrajectory__atom_id_types)
        ref_size += sys.getsizeof(t._XYZTrajectory__time)
        ref_size += sys.getsizeof(t._XYZTrajectory__time[0])*len(t._XYZTrajectory__time)
        ref_size += sys.getsizeof(t._XYZTrajectory__step)
        ref_size += sys.getsizeof(t._XYZTrajectory__step[0])*len(t._XYZTrajectory__step)

        buffer_size = t._bufferSize()

        self.assertEqual(buffer_size, ref_size)

    def testFlush(self):
        """ Test the file output. """
        # Get a file name.
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory.xyz")

        if MPICommons.isMaster():
            self.__files_to_remove.append(trajectory_filename)

        # Setup the trajectory object.
        unit_cell = KMCUnitCell(cell_vectors=[[1.0, 0.0, 0.0],
                                              [0.0, 1.0, 0.0],
                                              [0.0, 0.0, 1.0]],
                                basis_points=[[0.0, 0.0, 0.0]])
        lattice = KMCLattice(unit_cell=unit_cell,
                             periodic=(True, False, True),
                             repetitions=(4,4,4))

        config = KMCConfiguration(lattice=lattice,
                                 types=["A","B","C","D"]*16)

        t = XYZTrajectory(trajectory_filename=trajectory_filename,
                          configuration=config,
                          max_buffer_size=12345,
                          max_buffer_time=123.0)

        # Store data.
        simulation_time = 1.234
        step = 123
        t._storeData(simulation_time, step, config)

        # Flush.
        t.flush()

        # Check the file.

        ref_content = """KMCLib XYZ FORMAT VERSION 2013.10.15

CELL VECTORS
a: 1.0000000000e+00 0.0000000000e+00 0.0000000000e+00
b: 0.0000000000e+00 1.0000000000e+00 0.0000000000e+00
c: 0.0000000000e+00 0.0000000000e+00 1.0000000000e+00

REPETITIONS 4 4 4

PERIODICITY True False True

STEP 123
          64
    TIME 1.2340000000e+00
                A   0.0000000000e+00 0.0000000000e+00 0.0000000000e+00  0
                B   0.0000000000e+00 0.0000000000e+00 1.0000000000e+00  1
                C   0.0000000000e+00 0.0000000000e+00 2.0000000000e+00  2
                D   0.0000000000e+00 0.0000000000e+00 3.0000000000e+00  3
                A   0.0000000000e+00 1.0000000000e+00 0.0000000000e+00  4
                B   0.0000000000e+00 1.0000000000e+00 1.0000000000e+00  5
                C   0.0000000000e+00 1.0000000000e+00 2.0000000000e+00  6
                D   0.0000000000e+00 1.0000000000e+00 3.0000000000e+00  7
                A   0.0000000000e+00 2.0000000000e+00 0.0000000000e+00  8
                B   0.0000000000e+00 2.0000000000e+00 1.0000000000e+00  9
                C   0.0000000000e+00 2.0000000000e+00 2.0000000000e+00  10
                D   0.0000000000e+00 2.0000000000e+00 3.0000000000e+00  11
                A   0.0000000000e+00 3.0000000000e+00 0.0000000000e+00  12
                B   0.0000000000e+00 3.0000000000e+00 1.0000000000e+00  13
                C   0.0000000000e+00 3.0000000000e+00 2.0000000000e+00  14
                D   0.0000000000e+00 3.0000000000e+00 3.0000000000e+00  15
                A   1.0000000000e+00 0.0000000000e+00 0.0000000000e+00  16
                B   1.0000000000e+00 0.0000000000e+00 1.0000000000e+00  17
                C   1.0000000000e+00 0.0000000000e+00 2.0000000000e+00  18
                D   1.0000000000e+00 0.0000000000e+00 3.0000000000e+00  19
                A   1.0000000000e+00 1.0000000000e+00 0.0000000000e+00  20
                B   1.0000000000e+00 1.0000000000e+00 1.0000000000e+00  21
                C   1.0000000000e+00 1.0000000000e+00 2.0000000000e+00  22
                D   1.0000000000e+00 1.0000000000e+00 3.0000000000e+00  23
                A   1.0000000000e+00 2.0000000000e+00 0.0000000000e+00  24
                B   1.0000000000e+00 2.0000000000e+00 1.0000000000e+00  25
                C   1.0000000000e+00 2.0000000000e+00 2.0000000000e+00  26
                D   1.0000000000e+00 2.0000000000e+00 3.0000000000e+00  27
                A   1.0000000000e+00 3.0000000000e+00 0.0000000000e+00  28
                B   1.0000000000e+00 3.0000000000e+00 1.0000000000e+00  29
                C   1.0000000000e+00 3.0000000000e+00 2.0000000000e+00  30
                D   1.0000000000e+00 3.0000000000e+00 3.0000000000e+00  31
                A   2.0000000000e+00 0.0000000000e+00 0.0000000000e+00  32
                B   2.0000000000e+00 0.0000000000e+00 1.0000000000e+00  33
                C   2.0000000000e+00 0.0000000000e+00 2.0000000000e+00  34
                D   2.0000000000e+00 0.0000000000e+00 3.0000000000e+00  35
                A   2.0000000000e+00 1.0000000000e+00 0.0000000000e+00  36
                B   2.0000000000e+00 1.0000000000e+00 1.0000000000e+00  37
                C   2.0000000000e+00 1.0000000000e+00 2.0000000000e+00  38
                D   2.0000000000e+00 1.0000000000e+00 3.0000000000e+00  39
                A   2.0000000000e+00 2.0000000000e+00 0.0000000000e+00  40
                B   2.0000000000e+00 2.0000000000e+00 1.0000000000e+00  41
                C   2.0000000000e+00 2.0000000000e+00 2.0000000000e+00  42
                D   2.0000000000e+00 2.0000000000e+00 3.0000000000e+00  43
                A   2.0000000000e+00 3.0000000000e+00 0.0000000000e+00  44
                B   2.0000000000e+00 3.0000000000e+00 1.0000000000e+00  45
                C   2.0000000000e+00 3.0000000000e+00 2.0000000000e+00  46
                D   2.0000000000e+00 3.0000000000e+00 3.0000000000e+00  47
                A   3.0000000000e+00 0.0000000000e+00 0.0000000000e+00  48
                B   3.0000000000e+00 0.0000000000e+00 1.0000000000e+00  49
                C   3.0000000000e+00 0.0000000000e+00 2.0000000000e+00  50
                D   3.0000000000e+00 0.0000000000e+00 3.0000000000e+00  51
                A   3.0000000000e+00 1.0000000000e+00 0.0000000000e+00  52
                B   3.0000000000e+00 1.0000000000e+00 1.0000000000e+00  53
                C   3.0000000000e+00 1.0000000000e+00 2.0000000000e+00  54
                D   3.0000000000e+00 1.0000000000e+00 3.0000000000e+00  55
                A   3.0000000000e+00 2.0000000000e+00 0.0000000000e+00  56
                B   3.0000000000e+00 2.0000000000e+00 1.0000000000e+00  57
                C   3.0000000000e+00 2.0000000000e+00 2.0000000000e+00  58
                D   3.0000000000e+00 2.0000000000e+00 3.0000000000e+00  59
                A   3.0000000000e+00 3.0000000000e+00 0.0000000000e+00  60
                B   3.0000000000e+00 3.0000000000e+00 1.0000000000e+00  61
                C   3.0000000000e+00 3.0000000000e+00 2.0000000000e+00  62
                D   3.0000000000e+00 3.0000000000e+00 3.0000000000e+00  63
"""

        if MPICommons.isMaster():
            with open(trajectory_filename, "r") as f:
                content = f.read()
            self.assertEqual( content, ref_content )

        # Check that the buffers are empty.
        self.assertEqual(t._XYZTrajectory__atom_id_types, [])
        self.assertEqual(t._XYZTrajectory__atom_id_coordinates, [])
        self.assertEqual(t._XYZTrajectory__time, [])
        self.assertEqual(t._XYZTrajectory__step, [])

    def testFlush2(self):
        """ Test the file output. """
        # Get a file name.
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory.xyz")

        if MPICommons.isMaster():
            self.__files_to_remove.append(trajectory_filename)

        # Setup the trajectory object.
        unit_cell = KMCUnitCell(cell_vectors=[[1.0, 0.0, 0.0],
                                              [0.0, 1.0, 0.0],
                                              [0.0, 0.0, 1.0]],
                                basis_points=[[0.0, 0.0, 0.0]])
        lattice = KMCLattice(unit_cell=unit_cell,
                             periodic=(True, False, True),
                             repetitions=(4,4,4))

        config = KMCConfiguration(lattice=lattice,
                                 types=["A","B","C","D"]*16)

        t = XYZTrajectory(trajectory_filename=trajectory_filename,
                          configuration=config,
                          max_buffer_size=12345,
                          max_buffer_time=123.0)

        # Set data directly on the class.
        t._XYZTrajectory__atom_id_types = [("A","B","C","D","E"),
                                          ("This","is","the","next","step")]

        t._XYZTrajectory__atom_id_coordinates = [numpy.zeros((5,3)),
                                                numpy.ones((5,3))*1.234]

        t._XYZTrajectory__step = [0, 12]

        t._XYZTrajectory__time = [12.123,75.43]

        # Flush.
        t.flush()

        ref_content = """KMCLib XYZ FORMAT VERSION 2013.10.15

CELL VECTORS
a: 1.0000000000e+00 0.0000000000e+00 0.0000000000e+00
b: 0.0000000000e+00 1.0000000000e+00 0.0000000000e+00
c: 0.0000000000e+00 0.0000000000e+00 1.0000000000e+00

REPETITIONS 4 4 4

PERIODICITY True False True

STEP 0
          5
    TIME 1.2123000000e+01
                A   0.0000000000e+00 0.0000000000e+00 0.0000000000e+00  0
                B   0.0000000000e+00 0.0000000000e+00 0.0000000000e+00  1
                C   0.0000000000e+00 0.0000000000e+00 0.0000000000e+00  2
                D   0.0000000000e+00 0.0000000000e+00 0.0000000000e+00  3
                E   0.0000000000e+00 0.0000000000e+00 0.0000000000e+00  4
STEP 12
          5
    TIME 7.5430000000e+01
             This   1.2340000000e+00 1.2340000000e+00 1.2340000000e+00  0
               is   1.2340000000e+00 1.2340000000e+00 1.2340000000e+00  1
              the   1.2340000000e+00 1.2340000000e+00 1.2340000000e+00  2
             next   1.2340000000e+00 1.2340000000e+00 1.2340000000e+00  3
             step   1.2340000000e+00 1.2340000000e+00 1.2340000000e+00  4
"""

        if MPICommons.isMaster():
            with open(trajectory_filename, "r") as f:
                content = f.read()
            self.assertEqual( content, ref_content )

        # Check that the buffers are empty.
        self.assertEqual(t._XYZTrajectory__atom_id_types, [])
        self.assertEqual(t._XYZTrajectory__atom_id_coordinates, [])
        self.assertEqual(t._XYZTrajectory__time, [])
        self.assertEqual(t._XYZTrajectory__step, [])


if __name__ == '__main__':
    unittest.main()
