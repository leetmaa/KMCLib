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
from KMCLib.Utilities.CheckUtilities import checkSequence
from KMCLib.Utilities.CheckUtilities import checkTypes

# Implement the test.
class CheckUtilitiesTest(unittest.TestCase):
    """ Class for testing the checking utility functions. """

    def testCheckCoordinateList(self):
        """ Test the coordinate list checking function. """
        # Setup some valid coordinates.
        valid_coordinates = [[1.0,2.0,3.4],[3.0,3.0,3.5]]

        # Make sure they pass the check.
        checked_coords = checkCoordinateList(valid_coordinates)
        self.assertAlmostEqual(((valid_coordinates-checked_coords)**2).sum(), 0.0, 10)

        # Again, with numpy.
        valid_coordinates = numpy.array(valid_coordinates)
        checked_coords = checkCoordinateList(valid_coordinates)
        self.assertAlmostEqual(((valid_coordinates-checked_coords)**2).sum(), 0.0, 10)

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
        # Shoud pass.
        index = 0
        list = [0,12,'a',21.2]
        checked_index = checkIndexWithinBounds(index, list)
        self.assertEqual(checked_index, index)

        # This should also pass.
        index = 4
        list = "ABCDE"
        checked_index = checkIndexWithinBounds(index, list)
        self.assertEqual(checked_index, index)

        # And this.
        index = 1
        list = numpy.array([[12.0,1.3],[1.,4.3]])
        checked_index = checkIndexWithinBounds(index, list)
        self.assertEqual(checked_index, index)

        # Should fail - index not within bounds.
        index = -1
        self.assertRaises(Error, lambda: checkIndexWithinBounds(index, list))
        index = 2
        self.assertRaises(Error, lambda: checkIndexWithinBounds(index, list))

        # Catch and check the error.
        msg = "Custom Error msg."
        try:
            checkIndexWithinBounds(index, list, msg)
        except Error as e:
            error_msg = str(e)
            self.assertEqual(error_msg, msg)


    def testCheckSequence(self):
        """ Test that the sequence checking works. """
        # This is a sequence.
        sequence = [1,2,3,'a',12.3, "ABC"]
        checked_sequence = checkSequence(sequence)
        self.assertEqual(checked_sequence, sequence)

        # This also.
        sequence = numpy.array([[12.0,1.3],[1.,4.3]])
        checked_sequence = checkSequence(sequence)
        self.assertAlmostEqual(((checked_sequence-sequence)**2).sum(), 0.0, 10)

        # And these.
        sequence = "A"
        checked_sequence = checkSequence(sequence)
        self.assertEqual(checked_sequence, sequence)

        sequence = []
        checked_sequence = checkSequence(sequence)
        self.assertEqual(checked_sequence, sequence)

        # But this is not.
        sequence = 1
        self.assertRaises(Error, lambda: checkSequence(sequence))

    def testCheckTypes(self):
        """ Test that the types checking works. """
        # This is a valid types list.
        types = ['A','a', """A""", "ABC"]
        size = 4
        checked_types = checkTypes(types, size)
        self.assertEqual(checked_types, types)

        # Wrong size.
        size = 3
        self.assertRaises(Error, lambda: checkTypes(types,size))

        # Mixed types.
        types = ['A','a', """A""", 2]
        self.assertRaises(Error, lambda: checkTypes(types,size))

        # Not a list.
        types = "ABCDEfgH"
        self.assertRaises(Error, lambda: checkTypes(types,8))


if __name__ == '__main__':
    unittest.main()

