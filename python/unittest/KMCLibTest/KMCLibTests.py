

# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest


from KMCLocalConfigurationTest import KMCLocalConfigurationTest
from KMCInteractionsTest import KMCInteractionsTest
from KMCConfigurationTest import KMCConfigurationTest
from KMCUnitCellTest import KMCUnitCellTest
from KMCLatticeTest import KMCLatticeTest
from KMCLatticeModelTest import KMCLatticeModelTest

from Utilities import UtilitiesTests
from Exceptions import ExceptionsTests

def suite():
    suite = unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(KMCLocalConfigurationTest),
         unittest.TestLoader().loadTestsFromTestCase(KMCInteractionsTest),
         unittest.TestLoader().loadTestsFromTestCase(KMCConfigurationTest),
         unittest.TestLoader().loadTestsFromTestCase(KMCUnitCellTest),
         unittest.TestLoader().loadTestsFromTestCase(KMCLatticeTest),
         unittest.TestLoader().loadTestsFromTestCase(KMCLatticeModelTest),
         UtilitiesTests.suite(),
         ExceptionsTests.suite()])
    return suite

if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())

