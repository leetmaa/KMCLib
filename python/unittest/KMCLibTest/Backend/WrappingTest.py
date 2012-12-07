""" Module for specific wrapping testing from python. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest

# Import from the module we test.
from KMCLib.Backend import Backend

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

if __name__ == '__main__':
    unittest.main()

