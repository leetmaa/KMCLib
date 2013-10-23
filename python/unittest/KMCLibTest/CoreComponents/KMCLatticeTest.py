""" Module for testing the KMCLattice class. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

from KMCLib.CoreComponents.KMCUnitCell import KMCUnitCell
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend

# Import from the module we test.
from KMCLib.CoreComponents.KMCLattice import KMCLattice


# Implement the test.
class KMCLatticeTest(unittest.TestCase):
    """ Class for testing the KMCLattice class. """

    def testConstruction(self):
        """ Test the construction of the lattice """
        # Setup a valid unitcell.
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        # Setup the other input.
        repetitions = (10,12,45)
        periodic = (True,True,False)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)

        # Check that the unitcell object stored on the class
        # is the same one as constructed here (checked by reference)
        self.assertTrue( unit_cell == lattice._KMCLattice__unit_cell)

    def testQueryLatticeSites(self):
        """ Test that the returned lattice site data is coorect. """
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        # Setup the other input.
        repetitions = (2,1,1)
        periodic = (True,True,False)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)

        # These are all the sites in the lattice, given in
        # the fractional coordinates of the original cell.
        sites = lattice.sites()
        ref_sites = numpy.array([[ 0. ,  0. ,  0. ],
                                 [ 0.5,  0.5,  0. ],
                                 [ 1. ,  0. ,  0. ],
                                 [ 1.5,  0.5,  0. ]])

        # Check against a hardcoded reference.
        self.assertAlmostEqual(numpy.linalg.norm(sites-ref_sites), 0.0, 10)

        # Test with another repetition.
        repetitions = (2,3,4)
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)
        sites = lattice.sites()
        ref_sites = numpy.array([[ 0. ,  0. ,  0. ],
                                 [ 0.5,  0.5,  0. ],
                                 [ 0. ,  0. ,  1. ],
                                 [ 0.5,  0.5,  1. ],
                                 [ 0. ,  0. ,  2. ],
                                 [ 0.5,  0.5,  2. ],
                                 [ 0. ,  0. ,  3. ],
                                 [ 0.5,  0.5,  3. ],
                                 [ 0. ,  1. ,  0. ],
                                 [ 0.5,  1.5,  0. ],
                                 [ 0. ,  1. ,  1. ],
                                 [ 0.5,  1.5,  1. ],
                                 [ 0. ,  1. ,  2. ],
                                 [ 0.5,  1.5,  2. ],
                                 [ 0. ,  1. ,  3. ],
                                 [ 0.5,  1.5,  3. ],
                                 [ 0. ,  2. ,  0. ],
                                 [ 0.5,  2.5,  0. ],
                                 [ 0. ,  2. ,  1. ],
                                 [ 0.5,  2.5,  1. ],
                                 [ 0. ,  2. ,  2. ],
                                 [ 0.5,  2.5,  2. ],
                                 [ 0. ,  2. ,  3. ],
                                 [ 0.5,  2.5,  3. ],
                                 [ 1. ,  0. ,  0. ],
                                 [ 1.5,  0.5,  0. ],
                                 [ 1. ,  0. ,  1. ],
                                 [ 1.5,  0.5,  1. ],
                                 [ 1. ,  0. ,  2. ],
                                 [ 1.5,  0.5,  2. ],
                                 [ 1. ,  0. ,  3. ],
                                 [ 1.5,  0.5,  3. ],
                                 [ 1. ,  1. ,  0. ],
                                 [ 1.5,  1.5,  0. ],
                                 [ 1. ,  1. ,  1. ],
                                 [ 1.5,  1.5,  1. ],
                                 [ 1. ,  1. ,  2. ],
                                 [ 1.5,  1.5,  2. ],
                                 [ 1. ,  1. ,  3. ],
                                 [ 1.5,  1.5,  3. ],
                                 [ 1. ,  2. ,  0. ],
                                 [ 1.5,  2.5,  0. ],
                                 [ 1. ,  2. ,  1. ],
                                 [ 1.5,  2.5,  1. ],
                                 [ 1. ,  2. ,  2. ],
                                 [ 1.5,  2.5,  2. ],
                                 [ 1. ,  2. ,  3. ],
                                 [ 1.5,  2.5,  3. ]])

        # Check against a hardcoded reference.
        self.assertAlmostEqual(numpy.linalg.norm(sites-ref_sites), 0.0, 10)

    def testQueryRepetitionsAndPeriodic(self):
        """ Test that the returned repetitions data is coorect. """
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        # Setup the repetitions.
        repetitions = (2,1,1)
        periodic = (True,True,False)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)

        # Check that the repetitions are the same.
        self.assertEqual(lattice.repetitions()[0], repetitions[0])
        self.assertEqual(lattice.repetitions()[1], repetitions[1])
        self.assertEqual(lattice.repetitions()[2], repetitions[2])

        # Check that the periodic is the same.
        self.assertTrue(  lattice.periodic()[0] )
        self.assertTrue(  lattice.periodic()[1] )
        self.assertFalse( lattice.periodic()[2] )

        # Setup again with different periodicity and check.
        periodic = (True,False,True)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)
        self.assertTrue(  lattice.periodic()[0] )
        self.assertFalse( lattice.periodic()[1] )
        self.assertTrue(  lattice.periodic()[2] )

        # And again.
        periodic = (False,True,True)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)
        self.assertFalse( lattice.periodic()[0] )
        self.assertTrue(  lattice.periodic()[1] )
        self.assertTrue(  lattice.periodic()[2] )

    def testQueryBasis(self):
        """ Test that the returned basis data is coorect. """
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        # Setup the repetitions.
        repetitions = (2,1,1)
        periodic = (True,True,False)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)

        # Check that the basis is the one from the unitcell.
        self.assertAlmostEqual(numpy.linalg.norm(lattice.basis() - lattice._KMCLattice__unit_cell.basis()), 0.0, 10)

    def testQueryUnitCell(self):
        """ Test that the unitcell query function returns correctly. """
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        # Setup the repetitions.
        nI = 2
        nJ = 12
        nK = 3
        nB = 2
        repetitions = (nI,nJ,nK)
        periodic = (True,True,False)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)

        # Query.
        ret_unit_cell = lattice.unitCell()

        # Test by reference.
        self.assertTrue( unit_cell == ret_unit_cell )

    def testGlobalIndex(self):
        """ Test that the global index function returns correct values. """
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        # Setup the repetitions.
        nI = 2
        nJ = 12
        nK = 3
        nB = 2
        repetitions = (nI,nJ,nK)
        periodic = (True,True,False)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)

        # Loop through all indices and check that the globalIndex function computes them correctly.
        increment = 0
        for i in range(nI):
            for j in range(nJ):
                for k in range(nK):
                    for b in range(nB):
                        index = lattice._globalIndex(i,j,k,b)
                        self.assertEqual(index, increment)
                        increment += 1

    def testConstructionFailUnitcell(self):
        """ Test that construction of the lattice fails if the unitcell parameter has the wrong type."""
        # Setup a wrong unitcell.
        unit_cell = "KMCUnitCell"

        # Setup the other input.
        repetitions = (10,12,45)
        periodic = (True,True,False)

        # Construct the KMCLattice object.
        self.assertRaises(Error, lambda: KMCLattice(unit_cell=unit_cell,
                                                    repetitions=repetitions,
                                                    periodic=periodic))

    def testConstructionFailRepetitions(self):
        """ Test that construction of the lattice fails if the periodic parameter is incorrect."""
        # Setup a valid unitcell.
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        periodic = (False,True,True)

        # Fail because of not being a sequence.
        repetitions = 1
        self.assertRaises(Error, lambda: KMCLattice(unit_cell=unit_cell,
                                                    repetitions=repetitions,
                                                    periodic=periodic))

        # Fail because of wrong length.
        repetitions = (1,2,3,4,5)
        self.assertRaises(Error, lambda: KMCLattice(unit_cell=unit_cell,
                                                    repetitions=repetitions,
                                                    periodic=periodic))

        # Fail because of wrong type.
        repetitions = (1,3.0,1)
        self.assertRaises(Error, lambda: KMCLattice(unit_cell=unit_cell,
                                                    repetitions=repetitions,
                                                    periodic=periodic))

    def testConstructionFailPeriodic(self):
        """ Test that construction of the lattice fails if the periodic parameter is incorrect."""
        # Setup a valid unitcell.
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        repetitions = (10,12,45)

        # Fail because of not being a sequence.
        periodic = True
        self.assertRaises(Error, lambda: KMCLattice(unit_cell=unit_cell,
                                                    repetitions=repetitions,
                                                    periodic=periodic))

        # Fail because of wrong length.
        periodic = (True,True,True,True)
        self.assertRaises(Error, lambda: KMCLattice(unit_cell=unit_cell,
                                                    repetitions=repetitions,
                                                    periodic=periodic))

        # Fail because of wrong type.
        periodic = (True,True,1)
        self.assertRaises(Error, lambda: KMCLattice(unit_cell=unit_cell,
                                                    repetitions=repetitions,
                                                    periodic=periodic))


    def testLatticeMap(self):
        """ Check that we can get a valid lattice map out. """
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        # Setup the repetitions.
        nI = 2
        nJ = 12
        nK = 3
        nB = 2
        repetitions = (nI,nJ,nK)
        periodic = (True,True,False)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)

        # Get the lattice map.
        cpp_lattice_map = lattice._map()

        # Check the type.
        self.assertTrue(isinstance(cpp_lattice_map, Backend.LatticeMap))

        # Get it again and check that we get the same instance.
        cpp_lattice_map2 = lattice._map()

        # Check the instance.
        self.assertTrue(cpp_lattice_map == cpp_lattice_map2)

    def testScript(self):
        """ Check that we can generate a valid script. """
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = KMCUnitCell(cell_vectors=cell_vectors,
                                basis_points=basis_points)

        # Setup the repetitions.
        nI = 2
        nJ = 12
        nK = 3
        nB = 2
        repetitions = (nI,nJ,nK)
        periodic = (True,True,False)

        # Construct the KMCLattice object.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=repetitions,
                             periodic=periodic)

        # Get the script.
        script =  lattice._script()

        ref_script = """
# -----------------------------------------------------------------------------
# Unit cell

cell_vectors = [[   2.300000e+00,   0.000000e+00,   0.000000e+00],
                [   2.400000e+00,   3.000000e+00,   0.000000e+00],
                [   0.000000e+00,   0.000000e+00,   1.180000e+01]]

basis_points = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
                [   5.000000e-01,   5.000000e-01,   0.000000e+00]]

unit_cell = KMCUnitCell(
    cell_vectors=cell_vectors,
    basis_points=basis_points)

# -----------------------------------------------------------------------------
# Lattice

lattice = KMCLattice(
    unit_cell=unit_cell,
    repetitions=(2,12,3),
    periodic=(True, True, False))
"""
        self.assertEqual(script, ref_script)

        # Get the script.
        script =  lattice._script(variable_name="ANOTHER_NAME")

        ref_script = """
# -----------------------------------------------------------------------------
# Unit cell

cell_vectors = [[   2.300000e+00,   0.000000e+00,   0.000000e+00],
                [   2.400000e+00,   3.000000e+00,   0.000000e+00],
                [   0.000000e+00,   0.000000e+00,   1.180000e+01]]

basis_points = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
                [   5.000000e-01,   5.000000e-01,   0.000000e+00]]

unit_cell = KMCUnitCell(
    cell_vectors=cell_vectors,
    basis_points=basis_points)

# -----------------------------------------------------------------------------
# Lattice

ANOTHER_NAME = KMCLattice(
    unit_cell=unit_cell,
    repetitions=(2,12,3),
    periodic=(True, True, False))
"""
        self.assertEqual(script, ref_script)


if __name__ == '__main__':
    unittest.main()

