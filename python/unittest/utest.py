import unittest

# Import the tets.
from KMCLibTest import KMCLibTests

# Setup the suites.
def suite():
    suite = unittest.TestSuite(
        [KMCLibTests.suite()])
    return suite

# Run the tests.
if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())

