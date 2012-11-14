import unittest

from KMCLocalConfigurationTest import KMCLocalConfigurationTest

def suite():
    suite = unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(KMCLocalConfigurationTest)])
    return suite

if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())

