"""" Module for testing KMCLocalConfiguration """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest

# Import the module to test.
from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration

# Implementing the tests.
class KMCLocalConfigurationTest(unittest.TestCase):
    """ Class for testing the KMCLocalConfiguration class """

    def testConstruction(self):
        """ Test that the KMCLocalConfiguration class can be constructed. """
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config = KMCLocalConfiguration(cartesian_coordinates=coords, types=types)
        # NEEDS IMPLEMENTATION


if __name__ == '__main__':
    unittest.main()



