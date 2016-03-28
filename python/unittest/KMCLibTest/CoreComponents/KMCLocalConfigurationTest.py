"""" Module for testing KMCLocalConfiguration """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend

# Import the module to test.
from KMCLib.CoreComponents.KMCLocalConfiguration import KMCLocalConfiguration

# Implementing the tests.
class KMCLocalConfigurationTest(unittest.TestCase):
    """ Class for testing the KMCLocalConfiguration class """

    def testConstruction(self):
        """ Test that the KMCLocalConfiguration class can be constructed. """
        # Construct with coordinate list.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config = KMCLocalConfiguration(coordinates=coords,
                                             types=types,
                                             center=1)

        # Test.
        self.assertTrue(isinstance(local_config, KMCLocalConfiguration))

        # Construct with numpy coordinates.
        coords = numpy.array([[1.0,2.0,3.4],[1.1,1.2,1.3]])
        local_config = KMCLocalConfiguration(coordinates=coords,
                                             types=types,
                                             center=0)

        # Test.
        self.assertTrue(isinstance(local_config, KMCLocalConfiguration))

    def testMemberDataQuery(self):
        """ Check that the correct member data is stored and returned. """
        # Setup the configuration.
        coords = [[1.0,2.0,3.0],[1.0,1.0,3.0],[3.0,8.0,9.0]]
        types = ["C","B","A"]
        local_config = KMCLocalConfiguration(coordinates=coords,
                                             types=types,
                                             center=1)

        # Define the reference data.
        ref_coords    = numpy.array([[0.0,1.0,0.0],[0.0,0.0,0.0],[2.0,7.0,6.0]])
        ref_types     = [[(1, "C")],[(1, "B")],[(1, "A")]]

        # Check the coordinates.
        self.assertAlmostEqual(numpy.linalg.norm(local_config._KMCLocalConfiguration__coordinates - ref_coords), 0.0, 10)
        #
        self.assertAlmostEqual(numpy.linalg.norm(local_config.coordinates() - ref_coords), 0.0, 10)

        # Check the types.
        self.assertEqual(local_config._KMCLocalConfiguration__types, ref_types)
        #
        self.assertEqual(local_config.types(), ref_types)

    def testConstructionFails(self):
        """ Make sure the construction fails in the correct way with wrong arguments. """
        # Data do use.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        center = -1
        # No arguments.
        self.assertRaises(Error, lambda: KMCLocalConfiguration())
        # Center out of bounds.
        self.assertRaises(Error, lambda: KMCLocalConfiguration(coords,
                                                               types,
                                                               center))
        # Missing types information.
        self.assertRaises(Error, lambda: KMCLocalConfiguration(coordinates=coords,
                                                               center=center))
        # Missing coordinate information.
        self.assertRaises(Error, lambda: KMCLocalConfiguration(types=types,
                                                               center=center))
        # Wrong type of types.
        self.assertRaises(Error, lambda: KMCLocalConfiguration(coords,
                                                               coords,
                                                               center))
        # Wrong type of coordinates.
        self.assertRaises(Error, lambda: KMCLocalConfiguration(types,
                                                               types,
                                                               center))
        # Wrong type of center.
        self.assertRaises(Error, lambda: KMCLocalConfiguration(coords,
                                                               types,
                                                               center="A"))

    def testScript(self):
        """ Test the scripting functionality. """
        # Setup the configuration.
        coords = [[1.0,2.0,3.0]]
        types = ["C"]
        local_config = KMCLocalConfiguration(coordinates=coords,
                                             types=types,
                                             center=0)

        ref_script = """
# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00]]

types = [[(1, 'C')]]

local_configuration = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)
"""
        # Check the script.
        self.assertEqual(local_config._script(), ref_script)

        # Get another script.
        coords = [[1.0,2.0,3.0],[1.0,1.0,3.0],[3.0,8.0,9.0]]
        types = ["C","B","A"]
        local_config = KMCLocalConfiguration(coordinates=coords,
                                             types=types,
                                             center=0)
        ref_script2 = """
# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   0.000000e+00,  -1.000000e+00,   0.000000e+00],
               [   2.000000e+00,   6.000000e+00,   6.000000e+00]]

types = [[(1, 'C')], [(1, 'B')], [(1, 'A')]]

local_config2 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)
"""
        # Check.
        self.assertEqual(local_config._script(variable_name="local_config2"), ref_script2)

    def testBackend(self):
        """ Test that we can get a valid backend object out. """
        # Construct with coordinate list.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config = KMCLocalConfiguration(coordinates=coords,
                                             types=types,
                                             center=1)

        # Setup a possible types dict.
        possible_types = {
            "A" : 0,
            "B" : 1,
            "C" : 2,
            }

        # Get the backend.
        backend = local_config._backend(possible_types)

        # Check its type.
        self.assertTrue( isinstance(backend, Backend.Configuration) )

        # Get the coordinates back out and check.
        cpp_coords = backend.coordinates()
        py_coords  = local_config.coordinates()

        self.assertAlmostEqual( cpp_coords[0][0], py_coords[0][0], 10 )
        self.assertAlmostEqual( cpp_coords[0][1], py_coords[0][1], 10 )
        self.assertAlmostEqual( cpp_coords[0][2], py_coords[0][2], 10 )

        # Get the types back out.
        cpp_types = backend.elements()
        ref_cpp_types = (("A",),("B",))
        self.assertEqual(cpp_types, ref_cpp_types)

        py_types  = local_config.types()
        ref_py_types = [[(1, "A")],[(1, "B")]]
        self.assertEqual(py_types, ref_py_types)



if __name__ == '__main__':
    unittest.main()



