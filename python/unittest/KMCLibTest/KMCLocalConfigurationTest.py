"""" Module for testing KMCLocalConfiguration """

import unittest

# Import the module to test.
from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration

# Implementing the tests.
class KMCLocalConfigurationTest(unittest.TestCase):
    """ Class for testing the KMCLocalConfiguration class """

    def setup(self):
        pass

    def tearDown(self):
        pass

    def testConstruction(self):
        """ Test that the KMCLocalConfiguration class can be constructed. """
        local_config = KMCLocalConfiguration()

        # NEEDS IMPLEMENTATION

    def testDummy(self):
        """ Test the dummy function. """
        local_config = KMCLocalConfiguration()
        self.assertAlmostEqual(local_config.dummy(), 13.7, 10)


if __name__ == '__main__':
    unittest.main()



