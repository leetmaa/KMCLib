""" Module for testing the common type conversion utility functions. """


# Copyright (c)  2012-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

from KMCLib.Backend import Backend

# Import from the module we test.
from KMCLib.Utilities.ConversionUtilities import stringListToStdVectorString
from KMCLib.Utilities.ConversionUtilities import stdVectorStringToStringList
from KMCLib.Utilities.ConversionUtilities import stringListToStdVectorStdVectorString
from KMCLib.Utilities.ConversionUtilities import bucketListToStdVectorStdVectorString
from KMCLib.Utilities.ConversionUtilities import numpy2DArrayToStdVectorStdVectorDouble
from KMCLib.Utilities.ConversionUtilities import numpy2DArrayToStdVectorCoordinate
from KMCLib.Utilities.ConversionUtilities import stdVectorCoordinateToNumpy2DArray
from KMCLib.Utilities.ConversionUtilities import toShortBucketsFormat
from KMCLib.Utilities.ConversionUtilities import stdVectorTypeBucketToPython



# Implement the test.
class ConversionUtilitiesTest(unittest.TestCase):
    """ Class for testing the type conversion utility functions. """

    def testStringListToStdVectorString(self):
        """ Test the conversion of a string list to a std::vector<std::string>. """
        # Take a list of strings.
        string_list = ["AAA","BB","AbC","def"]

        # Convert.
        cpp_repr = stringListToStdVectorString(string_list)

        # Check the type.
        self.assertTrue(isinstance(cpp_repr, Backend.StdVectorString))

        # Check the values.
        list_from_cpp = list(cpp_repr)
        self.assertEqual(string_list, list_from_cpp)

    def testStdVectorStringToStringList(self):
        """ Test the conversion of a std::vector<std::string> to a string list. """
        # Take a list of strings.
        cpp_list = Backend.StdVectorString()
        ref_list = ["AAA","BB","AbC","def"]
        for s in ref_list:
            cpp_list.push_back(s)

        # Convert.
        py_repr = stdVectorStringToStringList(cpp_list)

        # Check the type.
        self.assertTrue(isinstance(py_repr, list))

        # Check the result.
        self.assertEqual(py_repr, ref_list)

    def testStringListToStdVectorStdVectorString(self):
        """ Test the conversion of a string list to a std::vector<std::string>. """
        # Take a list of strings.
        string_list = ["AAA","BB","AbC","def"]

        # Convert.
        cpp_repr = stringListToStdVectorStdVectorString(string_list)

        # Check the type.
        self.assertTrue(isinstance(cpp_repr, Backend.StdVectorStdVectorString))

        # Check the values.
        list_from_cpp = [e[0] for e in cpp_repr]
        self.assertEqual(string_list, list_from_cpp)

    def testBucketListToStdVectorStdVectorString(self):
        """ Test that a bucket list can be converted to C++ """
        bucket_list = [[(1, "J")], [(4, "G"), (1, "DD")], [(2, "P")]]
        cpp_list = bucketListToStdVectorStdVectorString(bucket_list)

        # Check the size:
        self.assertEqual(cpp_list.size(), 3)
        self.assertEqual(len(cpp_list[0]), 1)
        self.assertEqual(len(cpp_list[1]), 5)
        self.assertEqual(len(cpp_list[2]), 2)

        # Check the content.
        self.assertEqual(cpp_list[0][0], "J")
        self.assertEqual(cpp_list[1][0], "G")
        self.assertEqual(cpp_list[1][1], "G")
        self.assertEqual(cpp_list[1][2], "G")
        self.assertEqual(cpp_list[1][3], "G")
        self.assertEqual(cpp_list[1][4], "DD")
        self.assertEqual(cpp_list[2][0], "P")
        self.assertEqual(cpp_list[2][1], "P")

    def testNumpy2DArrayToStdVectorStdVectorDouble(self):
        """ Test the conversion of a 2D numpy array to a std::vector representation. """
        nI = 3;
        nJ = 5;
        array = numpy.random.rand(nI,nJ)

        # Convert to c++
        cpp_array = numpy2DArrayToStdVectorStdVectorDouble(array)

        # Check the size.
        self.assertEqual(cpp_array.size(), nI)
        self.assertEqual(len(cpp_array[0]), nJ)

        # Convert back and check values.
        converted = []
        for row in cpp_array:
            converted.append(numpy.array(row))
        converted = numpy.array(converted)

        # Check.
        self.assertAlmostEqual(numpy.linalg.norm(array-converted), 0.0, 10)

    def testStdVectorCoordinateToNumpy2DArray(self):
        """ Test the conversion from a std::vector<Coordinate> to a numpy array. """
        # Get a reference.
        ref_array = numpy.random.rand(6).reshape((2,3))

        # Setup the data to convert.
        cpp_vector = Backend.StdVectorCoordinate()
        cpp_vector.push_back(Backend.Coordinate(ref_array[0,0], ref_array[0,1], ref_array[0,2]))
        cpp_vector.push_back(Backend.Coordinate(ref_array[1,0], ref_array[1,1], ref_array[1,2]))

        # Convert.
        py_array = stdVectorCoordinateToNumpy2DArray(cpp_vector)

        # Check.
        diff = numpy.linalg.norm(ref_array - py_array)
        self.assertAlmostEqual( diff, 0.0, 10 )

    def testNumpy2DArrayToStdVectorCoordinate(self):
        """ Test the conversion of a Nx3 2D numpy array to a std::vector<Coordinate> representation. """
        nI = 12;
        nJ = 3;
        array = numpy.random.rand(nI,nJ)

        # Convert to c++
        cpp_vector = numpy2DArrayToStdVectorCoordinate(array)

        # Check the size.
        self.assertEqual(cpp_vector.size(), nI)

        # Check the type.
        self.assertTrue( isinstance(cpp_vector, Backend.StdVectorCoordinate) )

        # Convert back and check values.
        converted = stdVectorCoordinateToNumpy2DArray(cpp_vector)
        self.assertAlmostEqual(numpy.linalg.norm(array-converted), 0.0, 10)

    def testToShortBucketsFormat(self):
        """ Test the buckets format conversion routine. """
        # Simple case, only strings.
        types = ["A", "B", "C"]
        converted = toShortBucketsFormat(types)
        ref_converted = [[(1, "A")], [(1, "B")], [(1, "C")]]

        self.assertEqual(converted, ref_converted)

        # Strings and tuples.
        types = ["A", "B", (3, "C")]
        converted = toShortBucketsFormat(types)
        ref_converted = [[(1, "A")], [(1, "B")], [(3, "C")]]

        self.assertEqual(converted, ref_converted)

        # Strings, lists and tuples.
        types = ["A", ["B", "B"], (3, "C")]
        converted = toShortBucketsFormat(types)
        ref_converted = [[(1, "A")], [(2, "B")], [(3, "C")]]

        # Strings, lists and tuples.
        types = ["A", ["B", "B", "C", "B", "A", "A"], (3, "C")]
        converted = toShortBucketsFormat(types)
        ref_converted = [[(1, "A")], [(3, "B"), (1, "C"), (2, "A")], [(3, "C")]]

        # Strings, lists and tuples.
        types = ["A",
                 ["B", "B", "C", "B", "A", (2, "A")],
                 ["B", "B", "C", "B", "A", "A"], (3, "C")]

        converted = toShortBucketsFormat(types)
        ref_converted = [[(1, "A")], [(3, "B"), (1, "C"), (3, "A")],
                          [(3, "B"), (1, "C"), (2, "A")], [(3, "C")]]

        self.assertEqual(converted, ref_converted)

    def testStdVectorBucketTypeToPython(self):
        """ Test the buckets format conversion routine from C++ to Python. """
        empty_py_vector = stdVectorTypeBucketToPython(Backend.StdVectorTypeBucket(), Backend.StdVectorString())

        self.assertEqual(empty_py_vector, [])

        # Add a vector with content.
        cpp_map = Backend.StdVectorString(3)
        cpp_map[0] = "*"
        cpp_map[1] = "A"
        cpp_map[2] = "B"

        cpp_vector = Backend.StdVectorTypeBucket(4, Backend.TypeBucket(3))

        # [(1,"A")]
        cpp_vector[0][0] = 0
        cpp_vector[0][1] = 1
        cpp_vector[0][2] = 0

        # []
        cpp_vector[1][0] = 0
        cpp_vector[1][1] = 0
        cpp_vector[1][2] = 0

        # [(3,"A"), (1, "B")]
        cpp_vector[2][0] = 0
        cpp_vector[2][1] = 3
        cpp_vector[2][2] = 1

        # [(4,"A"), (5, "B")]
        cpp_vector[3][0] = 1
        cpp_vector[3][1] = 4
        cpp_vector[3][2] = 5

        # Translate to Python.
        py_vector = stdVectorTypeBucketToPython(cpp_vector,
                                                cpp_map)

        ref_py_vector = [[(1,"A")],
                         [],
                         [(3,"A"), (1, "B")],
                         [(4,"A"), (5, "B")]]

        # Check.
        self.assertEqual(py_vector, ref_py_vector)


if __name__ == '__main__':
    unittest.main()

