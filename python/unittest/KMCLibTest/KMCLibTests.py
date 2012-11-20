

# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest


from KMCLocalConfigurationTest import KMCLocalConfigurationTest
from KMCInteractionsTest import KMCInteractionsTest
from Utilities import UtilitiesTests
from Exceptions import ExceptionsTests
from Lattice import LatticeTests

def suite():
    suite = unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(KMCLocalConfigurationTest),
         unittest.TestLoader().loadTestsFromTestCase(KMCInteractionsTest),
         UtilitiesTests.suite(),
         ExceptionsTests.suite(),
         LatticeTests.suite()])
    return suite

if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())

