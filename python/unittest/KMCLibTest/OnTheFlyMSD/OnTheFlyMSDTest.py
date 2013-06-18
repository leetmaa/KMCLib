""" Module for testing the on-the-fly MSD analysis. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest

# Import from the module we test.
from KMCLib.OnTheFlyMSD.OnTheFlyMSD import OnTheFlyMSD

# Implement the test.
class OnTheFlyMSDTest(unittest.TestCase):
    """ Class for testing the on-the-fly MSD analysis. """

    def testConstruction(self):
        """ Test the MSD analysis object can be constructed. """
        msd = OnTheFlyMSD(history_steps=13,
                          n_bins=512,
                          t_max = 1024.0,
                          track_type="A")

    def testCalculateBin(self):
        """ Test that the bins are correctly determined. """
        msd = OnTheFlyMSD(history_steps=13,
                          n_bins=512,
                          t_max = 1024.0,
                          track_type="A")
        self.assertEqual( msd._calculateBin(0.0), 0 )
        self.assertEqual( msd._calculateBin(1.0), 0 )
        self.assertEqual( msd._calculateBin(1.4), 0 )
        self.assertEqual( msd._calculateBin(1.9), 0 )
        self.assertEqual( msd._calculateBin(2.0), 1 )
        self.assertEqual( msd._calculateBin(3.4), 1 )
        self.assertEqual( msd._calculateBin(30.4), 15 )
        self.assertEqual( msd._calculateBin(34.0), 17 )
        self.assertEqual( msd._calculateBin(1026.1), 513 )


if __name__ == '__main__':
    unittest.main()
