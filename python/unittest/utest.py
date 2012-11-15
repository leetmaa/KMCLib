

# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


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

