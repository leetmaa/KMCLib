""" Module for testing the common checking utilities. """


# Copyright (c)  2012-2014  Mikael Leetmaa
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
from KMCLib.Utilities.CheckUtilities import checkSequenceOf
from KMCLib.Utilities.CheckUtilities import checkSequenceOfPositiveIntegers
from KMCLib.Utilities.CheckUtilities import checkTypes
from KMCLib.Utilities.CheckUtilities import checkCellVectors
from KMCLib.Utilities.CheckUtilities import checkPositiveInteger
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat
from KMCLib.Utilities.CheckUtilities import checkAndNormaliseBucketEntry

# Implement the test.
class CheckUtilitiesTest(unittest.TestCase):
    """ Class for testing the checking utility functions. """

    def testCheckCoordinateList(self):
        """ Test the coordinate list checking function. """
        # Setup some valid coordinates.
        valid_coordinates = [[1.0,2.0,3.4],[3.0,3.0,3.5]]

        # Make sure they pass the check.
        checked_coords = checkCoordinateList(valid_coordinates)
        self.assertAlmostEqual( numpy.linalg.norm(valid_coordinates-checked_coords), 0.0, 10)

        # Again, with numpy.
        valid_coordinates = numpy.array(valid_coordinates)
        checked_coords = checkCoordinateList(valid_coordinates)
        self.assertAlmostEqual( numpy.linalg.norm(valid_coordinates-checked_coords), 0.0, 10)

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

    def testCheckCellVectors(self):
        """ Test the cell vector checking function. """
        # The simplest possible vectors.
        trial_vectors = [[1.0,0.0,0.0],[0.0,1.0,0.0],[0.0,0.0,1.0]]
        numpy_vectors = numpy.array(trial_vectors)

        # This must pass.
        checked_vectors = checkCellVectors(trial_vectors)
        self.assertAlmostEqual(numpy.linalg.norm(checked_vectors - numpy_vectors), 0.0, 10)

        # This should also pass.
        checked_vectors = checkCellVectors(numpy_vectors)
        self.assertAlmostEqual(numpy.linalg.norm(checked_vectors - numpy_vectors), 0.0, 10)

        # This should fail because of wrong format / shape.
        trial_vectors = [[1.0,0.0,0.0],[0.0,1.0,0.0,0.0],[0.0,1.0]]
        self.assertRaises(Error, lambda: checkCellVectors(trial_vectors))

        # As well as this.
        trial_vectors = [1.0,0.0,0.0,0.0,1.0]
        self.assertRaises(Error, lambda: checkCellVectors(trial_vectors))

        # This should also fail, because of wrong shape.
        trial_vectors = numpy.array([[1.0,0.0,0.0],[0.0,1.0,0.0,0.0],[0.0,1.0]])
        self.assertRaises(Error, lambda: checkCellVectors(trial_vectors))

        # This should fail because of wrong type.
        trial_vectors = "ABC"
        self.assertRaises(Error, lambda: checkCellVectors(trial_vectors))

        # These should fail because of linear dependencies.
        trial_vectors = [[1.0,0.0,0.0],[0.0,1.0,2.0],[0.5,0.5,1.000001]]
        self.assertRaises(Error, lambda: checkCellVectors(trial_vectors))

        trial_vectors = [[1.0,0.0,0.0],[0.5,0.5,1.0],[0.0,1.0,2.0]]
        self.assertRaises(Error, lambda: checkCellVectors(trial_vectors))


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
        self.assertAlmostEqual( numpy.linalg.norm(checked_sequence-sequence), 0.0, 10)

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

    def testCheckSequenceOfPositiveIntegers(self):
        """ Test that the positive integer sequence checking works. """
        # This is a valid.
        sequence = [1,2,3,12]
        checked_sequence = checkSequenceOfPositiveIntegers(sequence)
        self.assertEqual(checked_sequence, sequence)

        # This is not.
        sequence = numpy.array([[1,1],[1,4]])
        self.assertRaises(Error, lambda: checkSequenceOfPositiveIntegers(sequence))

        # This is not.
        sequence = [1,2,-4]
        self.assertRaises(Error, lambda: checkSequenceOfPositiveIntegers(sequence))

        # And this is.
        sequence = numpy.array([1,1,1,4])
        checked_sequence = checkSequenceOfPositiveIntegers(sequence)
        self.assertAlmostEqual( numpy.linalg.norm(checked_sequence-sequence), 0.0, 10)

        # But this is not.
        sequence = [1.0,2.0,0.0]
        self.assertRaises(Error, lambda: checkSequenceOfPositiveIntegers(sequence))

    def testCheckSequenceOf(self):
        """ Test that the general sequence checking works. """
        # Make a classes to check.
        class Dummy:
            def __init__(self):
                pass
        class Dummy2:
            def __init__(self):
                pass

        # Setup a valid sequence.
        sequence = [Dummy(),Dummy(), Dummy()]
        checked_sequence = checkSequenceOf(sequence, Dummy, msg="Error error.")

        # Check that it passes the test.
        self.assertEqual(checked_sequence, sequence)

        # Setup an invald sequence.
        sequence = [Dummy(),Dummy2(), Dummy()]

        # Check that it does not pass.
        self.assertRaises(Error,
                          lambda : checkSequenceOf(sequence, Dummy))

        # Setup a sequence containing classes (not instances)
        sequence = [Dummy(), Dummy, Dummy2]
        self.assertRaises(Error,
                          lambda : checkSequenceOf(sequence, Dummy))
        sequence = [Dummy]
        self.assertRaises(Error,
                          lambda : checkSequenceOf(sequence, Dummy))

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

    def testCheckPositiveInteger(self):
        """ Test that the positive integer checking works. """
        # Test pass.
        integer0 = checkPositiveInteger(21, 1234, "integer0")
        self.assertEqual(integer0, 21)

        integer0 = checkPositiveInteger(0, 1234, "integer0")
        self.assertEqual(integer0, 0)

        # Test default.
        integer0 = checkPositiveInteger(None, 1234, "integer0")
        self.assertEqual(integer0, 1234)

        # Test fail negative.
        self.assertRaises( Error,
                           lambda: checkPositiveInteger(-1, 12, "fail") )

        # Test fail wrong type.
        self.assertRaises( Error,
                           lambda: checkPositiveInteger(1.1, 12, "fail") )

        # Test fail wrong type.
        self.assertRaises( Error,
                           lambda: checkPositiveInteger("1", 12, "fail") )

    def testCheckPositiveFloat(self):
        """ Test that the positive float checking works. """
        # Test pass.
        float0 = checkPositiveFloat(21.0, 1.234, "float0")
        self.assertEqual(float0, 21.0)

        float0 = checkPositiveFloat(0.0, 1.234, "float0")
        self.assertEqual(float0, 0.0)

        # Test default.
        float0 = checkPositiveFloat(None, 1.234, "float0")
        self.assertEqual(float0, 1.234)

        # Test fail negative.
        self.assertRaises( Error,
                           lambda: checkPositiveFloat(-1.0, 1.2, "fail") )

        # Test fail wrong type.
        self.assertRaises( Error,
                           lambda: checkPositiveFloat(1, 1.2, "fail") )

        # Test fail wrong type.
        self.assertRaises( Error,
                           lambda: checkPositiveFloat("1.1", 1.2, "fail") )

    def testCheckAndNormaliseBucketEntry(self):
        """ Check normalization of types information in the bucket format. """
        # One A.
        t = checkAndNormaliseBucketEntry("A")
        self.assertEqual(t, [(1, "A")])

        t = checkAndNormaliseBucketEntry(["A"])
        self.assertEqual(t, [(1, "A")])

        t = checkAndNormaliseBucketEntry([(1, "A")])
        self.assertEqual(t, [(1, "A")])

        # Two A.
        t = checkAndNormaliseBucketEntry(["A", "A"])
        self.assertEqual(t, [(2, "A")])

        t = checkAndNormaliseBucketEntry((2, "A"))
        self.assertEqual(t, [(2, "A")])

        t = checkAndNormaliseBucketEntry([(2, "A")])
        self.assertEqual(t, [(2, "A")])

        t = checkAndNormaliseBucketEntry(["A", (1, "A")])
        self.assertEqual(t, [(2, "A")])

        t = checkAndNormaliseBucketEntry([(1, "A"), (1, "A")])
        self.assertEqual(t, [(2, "A")])

        # Three A.
        t = checkAndNormaliseBucketEntry(["A", "A", "A"])
        self.assertEqual(t, [(3, "A")])

        t = checkAndNormaliseBucketEntry((3, "A"))
        self.assertEqual(t, [(3, "A")])

        t = checkAndNormaliseBucketEntry([(3, "A")])
        self.assertEqual(t, [(3, "A")])

        t = checkAndNormaliseBucketEntry(["A", (2, "A")])
        self.assertEqual(t, [(3, "A")])

        t = checkAndNormaliseBucketEntry(["A", (1, "A"), "A"])
        self.assertEqual(t, [(3, "A")])

        t = checkAndNormaliseBucketEntry([(2, "A"), (1, "A")])
        self.assertEqual(t, [(3, "A")])

        # Three A, four B.
        t = checkAndNormaliseBucketEntry(["B", "B", "A", "A", "A", "B", "B"])
        self.assertEqual(t, [(4, "B"), (3, "A")])

        t = checkAndNormaliseBucketEntry(["A", "B", "B", "A", "A", "B", "B"])
        self.assertEqual(t, [(3, "A"), (4, "B")])

        t = checkAndNormaliseBucketEntry([(2, "B"), (3, "A"), "B", (1, "B")])
        self.assertEqual(t, [(4, "B"), (3, "A")])

        # Three A, four B, five C.
        t = checkAndNormaliseBucketEntry(["B", "B", (2, "C"), "A", "A", "A", "B", "B", (2, "C"), "C"])
        self.assertEqual(t, [(4, "B"), (5, "C"), (3, "A")])


if __name__ == '__main__':
    unittest.main()

