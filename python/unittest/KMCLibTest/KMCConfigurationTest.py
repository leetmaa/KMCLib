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
from KMCLib.Backend import Backend

# Import the module to test.
from KMCLib.KMCConfiguration import KMCConfiguration

# Implementing the tests.
class KMCConfigurationTest(unittest.TestCase):
    """ Class for testing the KMCConfiguration class """

    # FIXME
    def notestConstructionShortFormat(self):
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

        types = ['a','a','a','a','b','b',
                 'a','a','a','b','b','b',
                 'b','b','a','a','b','a',
                 'b','b','b','a','b','a',
                 'b','a','a','a','b','b',
                 'b','b','b','b','b','b',
                 'a','a','a','a','b','b',
                 'b','b','a','b','b','a']

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=['a','c','b'])

        # Get the types information out.
        ret_types = config.types()

        # Check that they are what we inserted.
        self.assertEqual(types, ret_types)

        # Check that the possible types are what we expect.
        self.assertEqual(['a','c','b'], config._KMCConfiguration__possible_types)
        # Check that the number of lattice sites corresponds
        # to the lattice.
        self.assertEqual(config._KMCConfiguration__n_lattice_sites, len(lattice.sites()))

        # Construct without possible types and check that the list is set correctly
        # from the given types.
        config = KMCConfiguration(lattice=lattice,
                                  types=types)
        self.assertEqual(['a','b'], config._KMCConfiguration__possible_types)


    # FIXME
    def notestConstructionLongFormat(self):
        """ Test that the KMCConfiguration class can be constructed with the long types format. """

        # Setup a valid KMCUnitCell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.8,0.0,0.0],
                                                          [0.0,3.2,0.0],
                                                          [0.0,0.5,3.0]]),
                                basis_points=[[0.0,0.0,0.0],
                                              [0.5,0.5,0.5],
                                              [0.25,0.25,0.75]])

        # Setup the lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(4,3,2),
                             periodic=(True,True,False))

        types = [(0,0,0,0,'g'),(3,2,1,2,'h')]
        default_type = 'a'

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  default_type=default_type)

        # Get the types information out.
        ret_types = config.types()

        ref_types = ['g', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                     'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                     'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                     'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                     'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                     'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                     'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                     'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                     'a', 'a', 'a', 'a', 'a', 'a', 'a', 'h']

        # Check that they are what we inserted.
        self.assertEqual(ref_types, ret_types)

        # Check that the possible types are what we expect.
        self.assertEqual(set(['a','g','h']), set(config._KMCConfiguration__possible_types))

        # Check that the number of lattice sites corresponds
        # to the lattice.
        self.assertEqual(config._KMCConfiguration__n_lattice_sites, len(lattice.sites()))

        # Construct again, now with a list of possible types.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  default_type=default_type,
                                  possible_types=['aa','a','h','g'])

        # Check that the possible types are what we expect.
        self.assertEqual(set(['aa','a','g','h']), set(config._KMCConfiguration__possible_types))


    def testConstructionFailShort(self):
        """ Test every thing that should fail when using the short format. """

        # Setup a valid KMCUnitCell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.8,0.0,0.0],
                                                          [0.0,3.2,0.0],
                                                          [0.0,0.5,3.0]]),
                                basis_points=[[0.0,0.0,0.0],
                                              [0.5,0.5,0.5],
                                              [0.25,0.25,0.75]])

        # Setup the lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(4,3,2),
                             periodic=(True,True,False))

        types = ['g', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'h']

        default_type = 'a'


        # This fails because the lattice is of wrong type.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=unit_cell,
                                                           types=types))


        # This fails because there is a default type given.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types,
                                                           default_type=default_type))

        # This fails becaulse of wrong length of the types.
        types_1 = ['g', 'a', 'a']
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1))

        types_1 = ['g', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                   'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                   'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                   'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                   'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                   'a', 'a', 'a', 'a', 'a', 3, 'a', 'a',
                   'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                   'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                   'a', 'a', 'a', 'a', 'a', 'a', 'a', 1]

        # This fails because of the wrong type of the types information.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1))

        types_1 = "ABCDE"
        # And this also.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1))

        # Use an incompatible possible types list.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types,
                                                           possible_types=['a']))

        # Use an incompatible possible types list.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types,
                                                           possible_types="ABCD"))


    def testConstructionFailLongFormat(self):
        """ Test everything that can go wrong when constructing with the long fomat. """

        # Setup a valid KMCUnitCell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.8,0.0,0.0],
                                                          [0.0,3.2,0.0],
                                                          [0.0,0.5,3.0]]),
                                basis_points=[[0.0,0.0,0.0],
                                              [0.5,0.5,0.5],
                                              [0.25,0.25,0.75]])

        # Setup the lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(4,3,2),
                             periodic=(True,True,False))

        types = [(0,0,0,0,'g'),(3,2,1,2,'h')]
        default_type = 'a'

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  default_type=default_type)


        # This fails because the lattice is of wrong type.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=unit_cell,
                                                           types=types,
                                                           default_type=default_type))

        # This fails because no default type is given.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types))

        # This fails because no default type is not a string.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types,
                                                           default_type=['a']))

        # This fails because the possible types are not set correctly.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types,
                                                           default_type=default_type,
                                                           possible_types="agh"))

        # This fails because there are types missing in the possible types.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types,
                                                           default_type=default_type,
                                                           possible_types=['a','h']))

        types_1 = [123,345]
        # This fails because of wrong type in the types list.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(0,0,0,0,'a'),(0,0,0,'a')]
        # This fails because of wrong type in the types list.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(0,0,0,0,'a'),(0,0,0,1,'b',3)]
        # This fails because of wrong type in the types list.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [[0,0,0,0,'a'],[0,0,0,1,'b']]
        # This fails because of wrong type in the types list.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(0,0,0,0,'a'),(-1,0,0,0,'b')]
        # This fails because of a negative first index.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(100,0,0,0,'a'),(0,0,0,0,'b')]
        # This fails because of a too large firs index.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(0,0,0,0,'a'),(0,-1,0,0,'b')]
        # This fails because of a negative second index.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(0,100,0,0,'a'),(0,0,0,1,'b')]
        # This fails because of a too large second index.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(0,0,0,0,'a'),(0,0,-1,0,'b')]
        # This fails because of a negative third index.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(0,0,100,0,'a'),(0,0,0,0,'b')]
        # This fails because of a too large third index.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(0,0,0,0,'a'),(0,0,0,-1,'b')]
        # This fails because of a negative fourth index.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))

        types_1 = [(0,0,0,100,'a'),(0,0,0,1,'b')]
        # This fails because of a too large fourth index.
        self.assertRaises(Error, lambda : KMCConfiguration(lattice=lattice,
                                                           types=types_1,
                                                           default_type=default_type))


    def testLatticeMap(self):
        """ Make sure the lattice map we get correspond to the lattice we give. """
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

        types = ['a','a','a','a','b','b',
                 'a','a','a','b','b','b',
                 'b','b','a','a','b','a',
                 'b','b','b','a','b','a',
                 'b','a','a','a','b','b',
                 'b','b','b','b','b','b',
                 'a','a','a','a','b','b',
                 'b','b','a','b','b','a']

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=['a','c','b'])


        # Get the lattice map.
        cpp_lattice_map = config._latticeMap()

        # Get the map from the lattice.
        cpp_lattice_map_ref = lattice._map()

        # Check that these two are references to the same underlying object.
        self.assertTrue(cpp_lattice_map == cpp_lattice_map_ref)

    # FIXME
    def notestBackend(self):
        """ Make sure the C++ backend is what we expect. """
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

        types = ['a','a','a','a','b','b',
                 'a','a','a','b','b','b',
                 'b','b','a','a','b','a',
                 'b','b','b','a','b','a',
                 'b','a','a','a','b','b',
                 'b','b','b','b','b','b',
                 'a','a','a','a','b','b',
                 'b','b','a','b','b','a']

        # Setup the configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=['a','c','b'])

        # Make sure that the backend stored on the config is None.
        self.assertTrue(config._KMCConfiguration__backend is None)

        # Query for the backend.
        cpp_backend = config._backend()

        # Check that the backend on the class is returned.
        self.assertTrue(config._KMCConfiguration__backend == cpp_backend)

        # Check the type of the cpp backend.
        self.assertTrue(isinstance(cpp_backend, Backend.Configuration))


if __name__ == '__main__':
    unittest.main()



