""" Module for testing the common coordinate utility functions. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

# Import from the module we test.
from KMCLib.Utilities.CoordinateUtilities import centerCoordinates
from KMCLib.Utilities.CoordinateUtilities import sortCoordinatesDistance
from KMCLib.Utilities.CoordinateUtilities import sortCoordinates


# Implement the test.
class CoordinateUtilitiesTest(unittest.TestCase):
    """ Class for testing the coordinate utility functions. """

    def testSortCoordinatesDistance(self):
        """ Test the coordinate sorting wrt distances function. """
        # Lets, use these coordinates.
        coords = numpy.array([[2.0,1.0,0.0],[1.0,2.0,0.0],[0.0,0.0,0.0],[-8.9,12.0,4.0]])
        types  = ["B","A","C","A"]
        center = 2

        # Sort - wrt distance,type,x,y,z
        (sorted_coords, sorted_distances, sorted_types, co_sorted_types) = sortCoordinatesDistance(coords, center, types)

        # Setup references.
        ref_coords = numpy.array([[0.0,0.0,0.0],[1.0,2.0,0.0],[2.0,1.0,0.0],[-8.9,12.0,4.0]])
        ref_types = ["C","A","B","A"]
        ref_distances=numpy.array([numpy.linalg.norm(c) for c in ref_coords])

        # Check.
        self.assertAlmostEqual(numpy.linalg.norm(sorted_distances-ref_distances), 0.0, 10)
        self.assertAlmostEqual(numpy.linalg.norm(sorted_coords-ref_coords), 0.0, 10)
        self.assertEqual(sorted_types, ref_types)

    def testSortCoordinates(self):
        """ Test the coordinate sorting function. """
        # Lets, use these coordinates.
        coords = numpy.array([[2.0,1.0,0.0],[1.0,2.0,0.0],[0.0,0.0,0.0],[-8.9,12.0,4.0]])
        types1  = ["B","A","C","A"]
        types2  = ["D","G","C","V"]

        # Sort - wrt x,y,z,type
        (sorted_coords, sorted_types1, sorted_types2) = sortCoordinates(coords, types1, types2)

        # Setup references.
        ref_coords = numpy.array([[ -8.9,  12.,    4. ],
                                  [  0.,    0.,    0. ],
                                  [  1.,    2.,    0. ],
                                  [  2.,    1.,    0. ]])
        ref_types1 = ["A","C","A","B"]
        ref_types2 = ['V', 'C', 'G', 'D']

        # Check.
        self.assertAlmostEqual(numpy.linalg.norm(sorted_coords-ref_coords), 0.0, 10)
        self.assertEqual(sorted_types1, ref_types1)
        self.assertEqual(sorted_types2, ref_types2)

    def testCenterCoordinates(self):
        """ Test the coordinate centering function. """
        # Setup some coordinates.
        coords = numpy.array([[1.0,2.0,3.0],[2.0,3.0,-1.1],[-8.9,12.0,4.0]])

        # Center around an index.
        centered_coords = centerCoordinates(coords, 1)

        # This should get us these coordinates.
        ref_coords = numpy.array([[-1.0,-1.0,4.1],[0.0,0.0,0.0],[-10.9,9.0,5.1]])

        # Check.
        self.assertAlmostEqual( numpy.linalg.norm(centered_coords-ref_coords), 0.0, 10)


if __name__ == '__main__':
    unittest.main()

