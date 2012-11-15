import unittest

from KMCLocalConfigurationTest import KMCLocalConfigurationTest

from Utilities import UtilitiesTests


def suite():
    suite = unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(KMCLocalConfigurationTest),
         UtilitiesTests.suite()])
    return suite

if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())

