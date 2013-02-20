

# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest

from CheckUtilitiesTest import CheckUtilitiesTest
from CoordinateUtilitiesTest import CoordinateUtilitiesTest
from ConversionUtilitiesTest import ConversionUtilitiesTest
from SaveAndReadUtilitiesTest import SaveAndReadUtilitiesTest
from PrintUtilitiesTest import PrintUtilitiesTest

from Trajectory import TrajectoryTests

def suite():
    suite = unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(CheckUtilitiesTest),
         unittest.TestLoader().loadTestsFromTestCase(CoordinateUtilitiesTest),
         unittest.TestLoader().loadTestsFromTestCase(ConversionUtilitiesTest),
         unittest.TestLoader().loadTestsFromTestCase(SaveAndReadUtilitiesTest),
         unittest.TestLoader().loadTestsFromTestCase(PrintUtilitiesTest),
         TrajectoryTests.suite()])
    return suite


if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())

