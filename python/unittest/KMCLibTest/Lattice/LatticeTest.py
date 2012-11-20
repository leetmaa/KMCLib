""" Module for testing the Lattice class. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest


from KMCLib.Lattice.UnitCell import UnitCell
from KMCLib.Exceptions.Error import Error

# Import from the module we test.
from KMCLib.Lattice.Lattice import Lattice


# Implement the test.
class LatticeTest(unittest.TestCase):
    """ Class for testing the Lattice class. """

    def testConstruction(self):
        """ Test the construction of the lattice """
        # Setup a valid unitcell.
        cell_vectors = [[2.3, 0.0, 0.0],
                        [2.4, 3.0, 0.0],
                        [0.0, 0.0, 11.8]]

        basis_points = [[0.0, 0.0, 0.0],
                        [0.5, 0.5, 0.0]]

        unit_cell = UnitCell(cell_vectors=cell_vectors,
                             basis_points=basis_points)

        # Setup the other input.
        repetitions = (10,12,45)
        periodic = (True,True,False)

        # Construct the Lattice object.
        lattice = Lattice(unit_cell=unit_cell,
                          repetitions=repetitions,
                          periodic=periodic)

        # Check that the unitcell object stored on the class
        # is the same one as constructed here (checked by reference)
        self.assertTrue( unit_cell == lattice._Lattice__unit_cell)

    def testConstructionFailUnitcell(self):
        """ Test that construction of the lattice fails if the unitcell parameter has the wrong type."""
        # Setup a wrong unitcell.
        unit_cell = "UnitCell"

        # Setup the other input.
        repetitions = (10,12,45)
        periodic = (True,True,False)

        # Construct the Lattice object.
        self.assertRaises(Error, lambda: Lattice(unit_cell=unit_cell,
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

        unit_cell = UnitCell(cell_vectors=cell_vectors,
                             basis_points=basis_points)

        periodic = (False,True,True)

        # Fail because of not being a sequence.
        repetitions = 1
        self.assertRaises(Error, lambda: Lattice(unit_cell=unit_cell,
                                                 repetitions=repetitions,
                                                 periodic=periodic))

        # Fail because of wrong length.
        repetitions = (1,2,3,4,5)
        self.assertRaises(Error, lambda: Lattice(unit_cell=unit_cell,
                                                 repetitions=repetitions,
                                                 periodic=periodic))

        # Fail because of wrong type.
        repetitions = (1,3.0,1)
        self.assertRaises(Error, lambda: Lattice(unit_cell=unit_cell,
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

        unit_cell = UnitCell(cell_vectors=cell_vectors,
                             basis_points=basis_points)

        repetitions = (10,12,45)

        # Fail because of not being a sequence.
        periodic = True
        self.assertRaises(Error, lambda: Lattice(unit_cell=unit_cell,
                                                 repetitions=repetitions,
                                                 periodic=periodic))

        # Fail because of wrong length.
        periodic = (True,True,True,True)
        self.assertRaises(Error, lambda: Lattice(unit_cell=unit_cell,
                                                 repetitions=repetitions,
                                                 periodic=periodic))

        # Fail because of wrong type.
        periodic = (True,True,1)
        self.assertRaises(Error, lambda: Lattice(unit_cell=unit_cell,
                                                 repetitions=repetitions,
                                                 periodic=periodic))


if __name__ == '__main__':
    unittest.main()

