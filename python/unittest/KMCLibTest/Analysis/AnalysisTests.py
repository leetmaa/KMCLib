

# Copyright (c)  2013 - 2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest

from OnTheFlyMSDTest import OnTheFlyMSDTest
from TimeStepDistributionTest import TimeStepDistributionTest
from ProcessStatisticsTest import ProcessStatisticsTest
from CompositionTest import CompositionTest

def suite():
    suite = unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(OnTheFlyMSDTest),
         unittest.TestLoader().loadTestsFromTestCase(TimeStepDistributionTest),
         unittest.TestLoader().loadTestsFromTestCase(ProcessStatisticsTest),
         unittest.TestLoader().loadTestsFromTestCase(CompositionTest),
         ])
    return suite


if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())

