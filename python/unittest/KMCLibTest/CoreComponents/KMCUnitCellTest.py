""" Module for testing the UnitCell class. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy


from KMCLib.Exceptions.Error import Error

# Import from the module we test.
from KMCLib.CoreComponents.KMCUnitCell import KMCUnitCell


# Implement the test.
class KMCUnitCellTest(unittest.TestCase):
    """ Class for testing the KMCUnitCell class. """

    def testConstruction(self):
        """ Test the construction of the unitcell """

        vector_a = [2.3, 0.0, 0.0]
        vector_b = [2.4, 3.0, 0.0]
        vector_c = [0.0, 0.0, 11.8]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.5]]

        cell_vectors = [vector_a,
                        vector_b,
                        vector_c]

        cell = KMCUnitCell(cell_vectors=cell_vectors,
                           basis_points=basis_points)

        # Check the vectors stored on the class.
        ref_vectors = numpy.array(cell_vectors)
        check_vectors = cell._KMCUnitCell__cell_vectors
        self.assertAlmostEqual( numpy.linalg.norm(ref_vectors - check_vectors), 0.0, 10)

        # Check via the query function.
        ret_vectors = cell.cellVectors()
        self.assertAlmostEqual( numpy.linalg.norm(ref_vectors - ret_vectors), 0.0, 10)

        # Check the basis points stored on the class.
        ref_basis = numpy.array(basis_points)
        check_basis = cell._KMCUnitCell__basis_points
        self.assertAlmostEqual( numpy.linalg.norm(ref_basis - check_basis), 0.0, 10)

    def testConstructionFailVectors(self):
        """ Make sure construction fails if there is a problem with the unitcell vectors. """
        vector_a = [2.3, 0.0, 0.0]
        vector_b = [2.4, 3.0, 0.0, 1.3] # <- wrong shape
        vector_c = [0.0, 0.0, 11.8]
        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.5]]
        cell_vectors = [vector_a,
                        vector_b,
                        vector_c]
        self.assertRaises(Error, lambda: KMCUnitCell(cell_vectors=cell_vectors, basis_points=basis_points))

    def testConstructionFailBasis(self):
        """ Make sure construction fails if there is a problem with the basis points. """
        vector_a = [2.3, 0.0, 0.0]
        vector_b = [2.4, 3.0, 0.0]
        vector_c = [0.0, 0.0, 11.8]
        basis_points = [[0.0, 0.0],       # <- wrong shape
                        [0.5, 0.5, 0.5]]
        cell_vectors = [vector_a,
                        vector_b,
                        vector_c]
        self.assertRaises(Error, lambda: KMCUnitCell(cell_vectors=cell_vectors, basis_points=basis_points))


    def testConstructionFailBasis2(self):
        """ Make sure construction fails if there is a problem with the basis points. """
        vector_a = [2.3, 0.0, 0.0]
        vector_b = [2.4, 3.0, 0.0]
        vector_c = [0.0, 0.0, 11.8]
        basis_points = [[0.0, 0.0, 1.1],       # <- too large value
                        [0.5, 0.5, 0.5]]
        cell_vectors = [vector_a,
                        vector_b,
                        vector_c]
        self.assertRaises(Error, lambda: KMCUnitCell(cell_vectors=cell_vectors, basis_points=basis_points))

        vector_a = [2.3, 0.0, 0.0]
        vector_b = [2.4, 3.0, 0.0]
        vector_c = [0.0, 0.0, 11.8]
        basis_points = [[0.0, 0.0, 1.0],       # <- too large value
                        [0.5, 0.5, 0.5]]
        cell_vectors = [vector_a,
                        vector_b,
                        vector_c]
        self.assertRaises(Error, lambda: KMCUnitCell(cell_vectors=cell_vectors, basis_points=basis_points))


        vector_a = [2.3, 0.0, 0.0]
        vector_b = [2.4, 3.0, 0.0]
        vector_c = [0.0, 0.0, 11.8]
        basis_points = [[0.0, 0.0, 0.999999],
                        [0.5, 0.5, -0.01]]       # <- too small value
        cell_vectors = [vector_a,
                        vector_b,
                        vector_c]
        self.assertRaises(Error, lambda: KMCUnitCell(cell_vectors=cell_vectors, basis_points=basis_points))

    def testTransformToCartesian(self):
        """
        Test the transformation from internal to cartesian coordinates.
        """
        # Setup a simple reference system.
        a = numpy.array([[1.0, 2.0, 3.0],
                         [1.1, 0.4, 3.7],
                         [1.2, 2.5, 0.8]])

        r = numpy.array([[1.2, 2.5, 3.8],
                         [3.2, 4.5, 5.8]])

        ref_xyz = numpy.dot(r, a)

        # Cell vectors.
        vector_a = [1.0, 2.0, 3.0]
        vector_b = [1.1, 0.4, 3.7]
        vector_c = [1.2, 2.5, 0.8]

        cell_vectors = [vector_a,
                        vector_b,
                        vector_c]

        cell = KMCUnitCell(cell_vectors=cell_vectors,
                           basis_points=[[0.0, 0.0, 0.0]])

        # Transform r to cartesian coordinates.
        xyz = cell.transformToCartesian(r)
        diff = numpy.linalg.norm(xyz - ref_xyz)
        self.assertAlmostEqual(diff, 0.0, 10)

    def testScript(self):
        """ Test that a valid script can be generated. """
        # Setup a valid cell.
        vector_a = [2.3, 1.0, 0.1]
        vector_b = [2.4, 3.0, 0.0]
        vector_c = [0.4, 0.3, 11.8]
        basis_points = [[0.4, 0.3, 0.2],
                        [0.2, 0.33, 0.11],
                        [0.6, 0.5, 0.1]]
        cell_vectors = [vector_a,
                        vector_b,
                        vector_c]
        cell = KMCUnitCell(cell_vectors=cell_vectors,
                           basis_points=basis_points)

        # Get the script.
        script = cell._script()

        # Check against the known reference script.
        ref_script = """
# -----------------------------------------------------------------------------
# Unit cell

cell_vectors = [[   2.300000e+00,   1.000000e+00,   1.000000e-01],
                [   2.400000e+00,   3.000000e+00,   0.000000e+00],
                [   4.000000e-01,   3.000000e-01,   1.180000e+01]]

basis_points = [[   4.000000e-01,   3.000000e-01,   2.000000e-01],
                [   2.000000e-01,   3.300000e-01,   1.100000e-01],
                [   6.000000e-01,   5.000000e-01,   1.000000e-01]]

unit_cell = KMCUnitCell(
    cell_vectors=cell_vectors,
    basis_points=basis_points)
"""
        self.assertEqual(script, ref_script)

       # Setup another valid cell.
        vector_a = [2.3, 1.0, 0.1]
        vector_b = [2.4, 3.0, 0.0]
        vector_c = [0.4, 0.3, 11.8]
        basis_points = [[0.4, 0.3, 0.2]]
        cell_vectors = [vector_a,
                        vector_b,
                        vector_c]
        cell = KMCUnitCell(cell_vectors=cell_vectors,
                           basis_points=basis_points)
        script = cell._script(variable_name="CELL_NAME_HERE")

        # Check against the known reference.
        ref_script = """
# -----------------------------------------------------------------------------
# Unit cell

cell_vectors = [[   2.300000e+00,   1.000000e+00,   1.000000e-01],
                [   2.400000e+00,   3.000000e+00,   0.000000e+00],
                [   4.000000e-01,   3.000000e-01,   1.180000e+01]]

basis_points = [[   4.000000e-01,   3.000000e-01,   2.000000e-01]]

CELL_NAME_HERE = KMCUnitCell(
    cell_vectors=cell_vectors,
    basis_points=basis_points)
"""
        self.assertEqual(script, ref_script)


if __name__ == '__main__':
    unittest.main()

