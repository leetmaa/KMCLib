"""" Module for testing KMCConfiguration """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy


from KMCLib.Exceptions.Error import Error
from KMCLib.KMCUnitCell import KMCUnitCell
from KMCLib.KMCLattice import KMCLattice


# Import the module to test.
from KMCLib.KMCConfiguration import KMCConfiguration

# Implementing the tests.
class KMCConfigurationTest(unittest.TestCase):
    """ Class for testing the KMCConfiguration class """

    def testConstruction(self):
        """ Test that the KMCConfiguration class can be constructed. """

        # Setup a valid KMCUnitCell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.8,0.0,0.0],
                                                          [0.0,3.2,0.0],
                                                          [0.0,0.5,3.0]]),
                                basis_points=[[0.0,0.0,0.0],
                                              [0.5,0.5,0.5],
                                              [0.25,0.25,0.75]])

        # Setup the lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(4,4,1),
                             periodic=(True,True,False))

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=['a','a','a','a','b','b',
                                         'a','a','a','b','b','b',
                                         'b','b','a','a','b','a',
                                         'b','b','b','a','b','a',
                                         'b','a','a','a','b','b',
                                         'b','b','b','b','b','b',
                                         'a','a','a','a','b','b',
                                         'b','b','a','b','b','a'],
                                  possible_types=['a','c','b'])
        # Test something.
        # NEEDS IMPLEMENTATION



if __name__ == '__main__':
    unittest.main()



