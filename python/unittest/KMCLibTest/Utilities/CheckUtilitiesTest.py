""" Module for testing the common checking utilities. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

from KMCLib.Exceptions.Error import Error

# Import from the module we test.
from KMCLib.Utilities.CheckUtilities import checkCoordinateList
from KMCLib.Utilities.CheckUtilities import checkIndexWithinBounds


# Implement the test.
class CheckUtilitiesTest(unittest.TestCase):
    """ Class for testing the checking utility functions. """

    def testCheckCoordinateList(self):
        """ Test the coordinate list checking function. """
        # Setup some valid coordinates.
        valid_coordinates = [[1.0,2.0,3.4],[3.0,3.0,3.5]]

        # Make sure they pass the check.
        checked_coords = checkCoordinateList(valid_coordinates)
        self.assertAlmostEqual(valid_coordinates, checked_coords, 10)

        # Again, with numpy.
        valid_coordinates = numpy.array(valid_coordinates)
        checked_coords = checkCoordinateList(valid_coordinates)
        self.assertAlmostEqual((valid_coordinates-checked_coords).sum(), 0.0, 10)

        # Test some things that fails.

        # Wrong type.
        invalid_coordinates = [[1.0,1.0,1.0],[1.0,1.0,1]]
        self.assertRaises(Error, lambda: checkCoordinateList(invalid_coordinates))

        # Wrong type.
        invalid_coordinates = "[[1.0,1.0,1.0],[1.0,1.0,1]]"
        self.assertRaises(Error, lambda: checkCoordinateList(invalid_coordinates))

        # Wrong size.
        invalid_coordinates = [[1.0,2.0,3.4],[3.0,3.0],[3.0,3.0,3.5]]
        self.assertRaises(Error, lambda: checkCoordinateList(invalid_coordinates))

        invalid_coordinates = [[1.0,2.0,3.4],[3.0,3.0,1.2],[3.0,3.0,3.5,32.3]]
        self.assertRaises(Error, lambda: checkCoordinateList(invalid_coordinates))


    def testCheckIndexWithinBounds(self):
        """ Test the index within bounds checking function. """
        # NEEDS IMPLEMENTATION
        pass


    def testCheckSequence(self):
        """ Test that the sequence checking works. """
        # NEEDS IMPLEMENTATION
        pass



if __name__ == '__main__':
    unittest.main()

