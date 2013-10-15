""" Module for testing the XYZTrajectory object. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import os

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


if __name__ == '__main__':
    unittest.main()
