""" Module for testing the common type conversion utility functions. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

from KMCLib.Backend import Backend

# Import from the module we test.
from KMCLib.Utilities.ConversionUtilities import stringListToStdVectorString
from KMCLib.Utilities.ConversionUtilities import numpy2DArrayToStdVectorStdVectorDouble


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


if __name__ == '__main__':
    unittest.main()

