

# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest

from TrajectoryTest import TrajectoryTest
from LatticeTrajectoryTest import LatticeTrajectoryTest
from XYZTrajectoryTest import XYZTrajectoryTest

def suite():
    suite = unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(TrajectoryTest),
         unittest.TestLoader().loadTestsFromTestCase(LatticeTrajectoryTest),
         unittest.TestLoader().loadTestsFromTestCase(XYZTrajectoryTest)])
    return suite


if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())

