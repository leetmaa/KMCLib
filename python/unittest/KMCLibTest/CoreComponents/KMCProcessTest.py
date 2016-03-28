"""" Module for testing KMCProcess class """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

from KMCLib.Exceptions.Error import Error
from KMCLib.CoreComponents.KMCLocalConfiguration import KMCLocalConfiguration

# Import the module to test.
from KMCLib.CoreComponents.KMCProcess import KMCProcess

# Implementing the tests.
class KMCProcessTest(unittest.TestCase):
    """ Class for testing the KMCProcess class """

    def testConstruction(self):
        """ Test that the KMCProcess class can be constructed. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        basis_sites = [0]
        move_vectors = [(0, [1.0,2.0,3.0]),
                        (1, [-1.0,-2.0,-3.0])]

        # Construct.
        p = KMCProcess(coordinates=coordinates,
                       elements_before=elements_before,
                       elements_after=elements_after,
                       move_vectors=move_vectors,
                       basis_sites=basis_sites,
                       rate_constant=1.0)

        # Check that we got a valid instance.
        self.assertTrue( isinstance(p, KMCProcess) )

    def testConstructionSorting(self):
        """ Test that the coordinates elements and move vectors gets sorted """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],
                       [3.0, 3.0, 3.0],
                       [1.0, 1.0, 1.0],
                       [1.1, 1.1, 1.1],
                       [1.5, 1.5, 1.5],
                       [2.5, 2.5, 2.5]]

        elements_before = ["A", "B", "C", "P", "D", "E"]
        elements_after =  ["B", "C", "D", "P", "A", "E"]
        basis_sites = [0]
        rate_constant = 1.0

        move_vectors = [(0, [ 1.5, 1.5, 1.5]),
                        (1, [-3.0,-3.0,-3.0]),
                        (2, [ 2.0, 2.0, 2.0]),
                        (4, [-0.5,-0.5,-0.5])]

        # Construct.
        p = KMCProcess(coordinates=coordinates,
                       elements_before=elements_before,
                       elements_after=elements_after,
                       move_vectors=move_vectors,
                       basis_sites=basis_sites,
                       rate_constant=1.0)

        # Get the configurations out.
        config_before = p.localConfigurations()[0]
        config_after  = p.localConfigurations()[1]

        # Check that the coordinates, elements and move vectors
        # have been sorted.

        ref_coords = numpy.array([[ 0.,  0.,  0. ],
                                  [ 1.,  1.,  1. ],
                                  [ 1.1, 1.1, 1.1],
                                  [ 1.5, 1.5, 1.5],
                                  [ 2.5, 2.5, 2.5],
                                  [ 3.,  3.,  3.]])

        # Before move.
        coords = config_before.coordinates()
        norm = numpy.linalg.norm(ref_coords - coords)
        self.assertAlmostEqual( norm, 0.0, 10 )

        # After move.
        coords = config_after.coordinates()
        norm = numpy.linalg.norm(ref_coords - coords)
        self.assertAlmostEqual( norm, 0.0, 10 )

        # Check the elements before move.
        ref_types = [[(1, "A")], [(1, "C")], [(1, "P")],
                     [(1, "D")], [(1, "E")], [(1, "B")]]
        self.assertEqual( ref_types, config_before.types() )

        # Check the elements after move.
        ref_types = [[(1, "B")], [(1, "D")], [(1, "P")],
                     [(1, "A")], [(1, "E")], [(1, "C")]]
        self.assertEqual( ref_types, config_after.types() )

        ref_vectors = [(0, [ 1.5, 1.5, 1.5]),
                       (1, [ 2.0, 2.0, 2.0]),
                       (3, [-0.5,-0.5,-0.5]),
                       (5, [-3.0,-3.0,-3.0])]

        ret_vectors = p.moveVectors()

        for ref, ret in zip(ref_vectors, ret_vectors):
            # Check the 'from' index.
            self.assertEqual(ref[0], ret[0])

            # Check the vector.
            norm = numpy.linalg.norm(numpy.array(ref[1]) - numpy.array(ret[1]))
            self.assertAlmostEqual( norm, 0.0, 10 )


    def testEqualOperator(self):
        """ Test the equal operator. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        move_vectors = [(0, [1.0,2.0,3.0]),
                        (1, [-1.0,-2.0,-3.0])]
        basis_sites = [0]

        # Construct.
        p1 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=move_vectors,
                        basis_sites=basis_sites,
                        rate_constant=1.0)
        # Construct.
        p2 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=move_vectors,
                        basis_sites=basis_sites,
                        rate_constant=1.0)
        # Check.
        eq = (p1 == p2)
        self.assertTrue( eq )

        # Set the input again, different coordinates.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0001]]
        move_vectors = [(0, [1.0,2.0,3.0001]),
                        (1, [-1.0,-2.0,-3.0001])]
        # Construct.
        p3 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=move_vectors,
                        basis_sites=basis_sites,
                        rate_constant=1.0)
        # Check.
        eq = (p1 == p3)
        self.assertFalse( eq )

        # Set the input again, different length.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.],[1.0,2.0,3.0]]
        elements_before = ["A", "B", "C"]
        elements_after = ["B", "A", "A"]
        basis_sites = [0]
        move_vectors = None

        # Construct.
        p4 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=move_vectors,
                        basis_sites=basis_sites,
                        rate_constant=1.0)
        # Check.
        eq = (p1 == p4)
        self.assertFalse( eq )

        # Set the input again, different basis sites.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        basis_sites = [1]

        # Construct.
        p5 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=move_vectors,
                        basis_sites=basis_sites,
                        rate_constant=1.0)
        # Check.
        eq = (p1 == p5)
        self.assertFalse( eq )

        # Set the input again, different length of the basis sites.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        basis_sites = [0, 2]

        # Construct.
        p6 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=move_vectors,
                        basis_sites=basis_sites,
                        rate_constant=1.0)
        # Check.
        eq = (p1 == p6)
        self.assertFalse( eq )

        # Set the input again, different elements before.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.]]
        elements_before = ["A", "D"]
        elements_after = ["B", "A"]
        basis_sites = [0]

        # Construct.
        p7 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=move_vectors,
                        basis_sites=basis_sites,
                        rate_constant=1.0)
        # Check.
        eq = (p1 == p7)
        self.assertFalse( eq )

        # Set the input again, different elements after.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.]]
        elements_before = ["A", "B"]
        elements_after = ["D", "A"]
        basis_sites = [0]

        # Construct.
        p7 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=move_vectors,
                        basis_sites=basis_sites,
                        rate_constant=1.0)
        # Check.
        eq = (p1 == p7)
        self.assertFalse( eq )

    def testEqualOperatorMoveVectors(self):
        """ Test the equal operator w.r.t. the move vectors. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        move_vectors = [(0, [1.0,2.0,3.0]),
                        (1, [-1.0,-2.0,-3.0])]
        basis_sites = [0]

        # Construct.
        p1 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=move_vectors,
                        basis_sites=basis_sites,
                        rate_constant=1.0)

        # Construct.
        p2 = KMCProcess(coordinates=coordinates,
                        elements_before=elements_before,
                        elements_after=elements_after,
                        move_vectors=None,
                        basis_sites=basis_sites,
                        rate_constant=1.0)

        # The same move vectors should be reconstructed, so there are equal.
        eq = (p1 == p2)
        self.assertTrue( eq )

        # If se explicitly set the move vectors to empty on p2 they are not equal.
        p2._move_vectors = []
        eq = (p1 == p2)
        self.assertFalse( eq )

        # And the other way around.
        eq = (p2 == p1)
        self.assertFalse( eq )

        # Now, make the other one empty also.
        p1._move_vectors = []
        eq = (p1 == p2)
        self.assertTrue( eq )

        # Set explicitly to different vectors.
        p1._move_vectors = move_vectors = [(0, [1.0,2.0,3.0]),
                                           (1, [-1.0,-2.0,-3.0])]

        p2._move_vectors = move_vectors = [(1, [1.0,2.0,3.0]),
                                           (0, [-1.0,-2.0,-3.0])]
        eq = (p1 == p2)
        self.assertFalse( eq )

        # And the vector.
        p2._move_vectors = move_vectors = [(0, [1.0,2.0,3.0]),
                                           (1, [-1.0,-2.1,-3.0])]
        eq = (p1 == p2)
        self.assertFalse( eq )

        # The length.
        p2._move_vectors = move_vectors = [(0, [1.0,2.0,3.0]),
                                           (1, [-1.0,-2.1,-3.0]),
                                           (2, [ 1.0,-2.1,-3.0])]
        eq = (p1 == p2)
        self.assertFalse( eq )

    def testEqualOperatorUnsorted(self):
        """ Test the equal operator for unsorted coordinates. """
        # Set the first input.
        coordinates_1     = [[0.0, 0.0, 0.0],
                             [1.0, 3.0, 1.0],
                             [1.0, 2.0, 3.0]]
        elements_before_1 = ["A", "C", "B"]
        elements_after_1  = ["B", "C", "A"]
        move_vectors_1    = [(0, [1.0,2.0,3.0]),
                             (2, [-1.0,-2.0,-3.0])]
        basis_sites_1     = [0]

        # Set the second input.
        coordinates_2     = [[0.0, 0.0, 0.0],
                             [1.0, 2.0, 3.0],
                             [1.0, 3.0, 1.0],]
        elements_before_2 = ["A", "B", "C"]
        elements_after_2  = ["B", "A", "C"]
        move_vectors_2    = [(0, [1.0,2.0,3.0]),
                             (1, [-1.0,-2.0,-3.0])]
        basis_sites_2     = [0]

        # Construct.
        p1 = KMCProcess(coordinates=coordinates_1,
                        elements_before=elements_before_1,
                        elements_after=elements_after_1,
                        move_vectors=move_vectors_1,
                        basis_sites=basis_sites_1,
                        rate_constant=1.0)

        # Construct.
        p2 = KMCProcess(coordinates=coordinates_2,
                        elements_before=elements_before_2,
                        elements_after=elements_after_2,
                        move_vectors=move_vectors_2,
                        basis_sites=basis_sites_2,
                        rate_constant=1.0)

        # These should be equal despite differing order.
        eq = (p1 == p2)
        self.assertTrue( eq )

    def testConstructionFailCoordinates(self):
        """ Test that construction fails with the wrong coordinates format. """
        # Set the input.
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        move_vectors = [(0, [1.0,2.0,3.0]),
                        (1, [-1.0,-2.0,-3.0])]
        basis_sites = [0]

        # Fail on wrong dimension.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[[0.0, 0.0],[1.0, 2.0, 3.0]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )
        # Fail on wrong dimension.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[[0.0, 0.0, 1.3],[2.0, 3.0]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )
        # Fail on wrong dimension.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[[0.0, 0.0, 1.3, 3.4, 2.0, 3.0]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        # Fail on wrong type.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[[0, 0, 1], [3, 2, 3]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        # Fail on wrong type.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates="[[0, 0, 1], [3, 2, 3]]",
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        # Fail on wrong type.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[["0.1", "0.4", "1.3"],
                                                            ["3.1", "2.1", "3"]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

    def testConstructionFailElements(self):
        """ Test that construction fails with the wrong elements format. """

        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        move_vectors = [(0, [1.0,2.0,3.0]),
                        (1, [-1.0,-2.0,-3.0])]
        basis_sites = [0]

        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before="A,B",
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after="B,A",
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

    def testConstructionFailNumberOfSites(self):
        """ Test that construction fails mismatch in the number of sites. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0], [1.2,3.4,5.6]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A", "B"]
        move_vectors = [(0, [1.0,2.0,3.0]),
                        (1, [-1.0,-2.0,-3.0])]
        basis_sites = [0]

        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               move_vectors=move_vectors,
                                               rate_constant=1.0) )

        elements_before = ["A", "B", "B"]
        elements_after = ["B", "A"]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               move_vectors=move_vectors,
                                               rate_constant=1.0) )


        coordinates = [[0.0, 0.0, 0.0], [1.2,3.4,5.6]]
        elements_before = ["A", "B", "D"]
        elements_after = ["B", "A", "B"]
        basis_sites = [0]

        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               move_vectors=move_vectors,
                                               rate_constant=1.0) )

    def testConstructionFailBasisSites(self):
        """ Test that construction fails with the wrong basis sites input. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        move_vectors = [(0, [1.0,2.0,3.0]),
                        (1, [-1.0,-2.0,-3.0])]

        basis_sites = ["First"]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        basis_sites = 0
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        basis_sites = [1.0, 2.0]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               move_vectors=move_vectors,
                                               rate_constant=1.0) )

    def testConstructionFailRate(self):
        """ Test that construction fails with a wrong rate. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        move_vectors = [(0, [1.0,2.0,3.0]),
                        (1, [-1.0,-2.0,-3.0])]
        basis_sites = [0]

        # Negative rate.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=-1.0) )
        # Integer rate.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=345) )
        # Wrong type.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=[3.459]) )


    def testConstructionFailMoveVectors(self):
        """ Test that the wrong move vector input gives and error. """
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        basis_sites = [0]

        # The move vectors do not match configuration.
        move_vectors = [(1, [1.0,2.0,3.0]),
                        (0, [-1.0,-2.0,-3.0])]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        move_vectors = [(0, [1.1,2.0,3.0]),
                        (1, [-1.0,-2.0,-3.0])]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        move_vectors = [(0, [1.0,2.0,3.0])]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        # Wrong format.
        move_vectors = [(0, [0.0])]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        move_vectors = [(0, [1.0,2.0,3.0]),
                        (1.2, [-1.0,-2.0,-3.0])]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        move_vectors = "[(0, [1.0,2.0,3.0]),(1, [-1.0,-2.0,-3.0])]"
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               move_vectors=move_vectors,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

    def testReconstructMoveVectors(self):
        """ Test that the helper function to reconstruct move vectors works as intended. """
        # Get an empty process.
        process = KMCProcess.__new__(KMCProcess)

        # Give it coordinate values.
        coordinates = [[ 0.0, 0.0, 0.0],
                       [ 1.0, 2.0, 3.0],
                       [-1.0, 1.0, 5.0],
                       [ 8.0, 4.0, 7.0]]
        process._coordinates = coordinates

        # Give it element values.
        process._elements_before = ["A","B","D","F"]
        process._elements_after  = ["A","F","D","B"]

        move_vectors = process._KMCProcess__reconstructMoveVectors()

        self.assertTrue( move_vectors is not None )
        self.assertEqual( len(move_vectors), 2 )

        self.assertEqual( move_vectors[0][0], 1 )
        self.assertEqual( move_vectors[1][0], 3 )

        ref_v1 = numpy.array([ 7.,  2.,  4.])
        norm = numpy.linalg.norm(move_vectors[0][1] - ref_v1)
        self.assertAlmostEqual( norm, 0.0, 10 )

        ref_v2 = numpy.array([-7., -2., -4.])
        norm = numpy.linalg.norm(move_vectors[1][1] - ref_v2)
        self.assertAlmostEqual( norm, 0.0, 10 )

        # Give it  new element values.
        process._elements_before = ["A","B","G","F"]
        process._elements_after  = ["A","F","D","B"]

        move_vectors = process._KMCProcess__reconstructMoveVectors()

        self.assertTrue( move_vectors is None )

    def testScript(self):
        """ Test that the process can generate its own valid script. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        move_vectors = [(0, [1.0, 2.0, 3.0]),
                        (1, [-1.0, -2.0, -3.0])]
        basis_sites = [0, 1, 4]

        # Construct with given move vectors.
        p = KMCProcess(coordinates=coordinates,
                       elements_before=elements_before,
                       elements_after=elements_after,
                       move_vectors=move_vectors,
                       basis_sites=basis_sites,
                       rate_constant=1.0)

        # Get the script.
        script = p._script()

        ref_script = """coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e+00,   2.000000e+00,   3.000000e+00]]

elements_before = ['A','B']
elements_after  = ['B','A']
move_vectors    = [(  0,[   1.000000e+00,   2.000000e+00,   3.000000e+00]),
                   (  1,[  -1.000000e+00,  -2.000000e+00,  -3.000000e+00])]
basis_sites     = [0,1,4]
rate_constant   =    1.000000e+00

process = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    move_vectors=move_vectors,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

"""
        # Check.
        self.assertEqual(ref_script, script)

        # Construct with default move vectors.
        p = KMCProcess(coordinates=coordinates,
                       elements_before=elements_before,
                       elements_after=elements_after,
                       basis_sites=basis_sites,
                       rate_constant=1.0)

        # Get the script.
        script = p._script()

        # The generated script is still the same.
        self.assertEqual(ref_script, script)

        # Construct with complicated move and no move vectors.
        coordinates = [[0.0, 0.0, 0.0],[1.0,0.0,0.0], [0.0,1.0,0.0]]
        elements_before = ["A", "B", "C"]
        elements_after = ["C", "A", "B"]
        basis_sites = [0, 1, 4]

        p = KMCProcess(coordinates=coordinates,
                       elements_before=elements_before,
                       elements_after=elements_after,
                       basis_sites=basis_sites,
                       rate_constant=1.0)

        # Get the script.
        script = p._script()

        ref_script = """coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e+00,   0.000000e+00,   0.000000e+00],
               [   0.000000e+00,   1.000000e+00,   0.000000e+00]]

elements_before = ['A','B','C']
elements_after  = ['C','A','B']
move_vectors    = None
basis_sites     = [0,1,4]
rate_constant   =    1.000000e+00

process = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    move_vectors=move_vectors,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

"""
        self.assertEqual(script, ref_script)

    def testLocalConfigurations(self):
        """ Test that the local configurations are correctly set up """
        coordinates = numpy.array([[1.0,2.0,3.0], [2.3,5.5,3.2]])
        elements1 = ["First", "Second"]
        elements2 = ["Second", "First"]

        # Construct the process.
        process = KMCProcess(coordinates,
                             elements1,
                             elements2,
                             basis_sites=[9],
                             rate_constant=1.0)

        # Get the local configurations out.
        c1 = process.localConfigurations()[0]
        c2 = process.localConfigurations()[1]

        # Construct the two reference local configurations.
        ref_c1 = KMCLocalConfiguration(coordinates, elements1, center=0)
        ref_c2 = KMCLocalConfiguration(coordinates, elements2, center=0)

        # Check coordinates.
        self.assertAlmostEqual( numpy.linalg.norm(c1.coordinates() - ref_c1.coordinates()), 0.0, 12 )
        self.assertAlmostEqual( numpy.linalg.norm(c2.coordinates() - ref_c2.coordinates()), 0.0, 12 )

        # Check types.
        self.assertAlmostEqual( c1.types(), ref_c1.types() )
        self.assertAlmostEqual( c2.types(), ref_c2.types() )

    def testElementsBefore(self):
        """ Test the elements before query function. """
        process = KMCProcess.__new__(KMCProcess)
        ref = ["123"]
        process.__elements_before = ref
        # Check by reference.
        self.assertTrue( process.__elements_before == ref )

    def testElementsAfter(self):
        """ Test the elements after query function. """
        process = KMCProcess.__new__(KMCProcess)
        ref = ["123"]
        process.__elements_after = ref
        # Check by reference.
        self.assertTrue( process.__elements_after == ref )

    def testConstructionFailWildcardMove(self):
        """ Test for failure if wildcards change place in the move. """
        # This should fail.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["*","B"]
        types1 = ["B","*"]
        sites  = [0]
        rate_0_1 = 3.5
        self.assertRaises( Error,
                           lambda : KMCProcess(coords, types0, types1, sites, rate_0_1) )

        # This should fail.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["*","*"]
        types1 = ["B","*"]
        sites  = [0]
        rate_0_1 = 3.5
        self.assertRaises( Error,
                           lambda : KMCProcess(coords, types0, types1, sites, rate_0_1) )

    def testConstructionFailNoMove(self):
        """ Test for failure if no move takes place. """
        # This should fail.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","B", "*"]
        types1 = ["A","B", "*"]
        sites  = [0]
        rate_0_1 = 3.5
        self.assertRaises( Error,
                           lambda : KMCProcess(coords, types0, types1, sites, rate_0_1) )


if __name__ == '__main__':
    unittest.main()

