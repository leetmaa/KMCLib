

# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


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

