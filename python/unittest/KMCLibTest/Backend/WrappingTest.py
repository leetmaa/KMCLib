""" Module for specific wrapping testing from python. """


# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest

# Import from the module we test.
from KMCLib.Backend import Backend
from KMCLib.Backend.Backend import SimpleDummyBaseClass

# Implement the test.
class WrappingTest(unittest.TestCase):
    """ Class for testing the wrappings. """

    def testCoordinate(self):
        """ Test the coordinate wrapping. """
        # Test the indexing function access.
        coord = Backend.Coordinate(1.0,2.0,3.0)
        self.assertAlmostEqual(float(coord[0]), 1.0, 10)
        self.assertAlmostEqual(coord[1], 2.0, 10)
        self.assertAlmostEqual(coord[2], 3.0, 10)

        # Test indexing function modification.
        coord[0] = 12.33;
        self.assertAlmostEqual(coord[0], 12.33, 10)
        coord[1] = 1.33;
        self.assertAlmostEqual(coord[1], 1.33, 10)
        coord[2] = 34.33;
        self.assertAlmostEqual(coord[2], 34.33, 10)

    def testCppPythonInheritanceDirectors(self):
        """ Test that we can extend a C++ object in Python and use the extensions from C++ """
        # This returns "I am a C++ object".
        cpp_class = SimpleDummyBaseClass()
        self.assertEqual(cpp_class.whoAmI(), "I am a C++ object")

        # This is a dummy prototype.
        class DerrivedClass(SimpleDummyBaseClass):
            def __init__(self):
                # Call the base class constructor.
                SimpleDummyBaseClass.__init__(self)
            def whoAmI(self):
                return "I am extended in Python"

        # Now we get another answer from calling whoAmI()
        py_class = DerrivedClass()
        self.assertEqual(py_class.whoAmI(), "I am extended in Python")

        # Calling from C++ gives the same result.
        self.assertEqual(Backend.callWhoAmI(cpp_class), "I am a C++ object")

        self.assertEqual(Backend.callWhoAmI(py_class), "I am extended in Python")

if __name__ == '__main__':
    unittest.main()

