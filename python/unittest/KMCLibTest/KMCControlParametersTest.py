""" Module for testing the KMCControlParameters class. """


# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest

# Import from the module we test.
from KMCLib.KMCControlParameters import KMCControlParameters


# Implement the test.
class KMCControlParametersTest(unittest.TestCase):
    """ Class for testing the KMCControlParameters class. """

    def testConstructionAndQuery(self):
        """ Test the construction of the control parametes object """
        # Default construction.
        control_params = KMCControlParameters()

        # Check the default values.
        self.assertEqual(control_params.numberOfSteps(), 0)
        self.assertEqual(control_params.dumpInterval(), 1)

        # Non-default construction.
        control_params = KMCControlParameters(number_of_steps=2000000,
                                              dump_interval=1000)

        # Check the values.
        self.assertEqual(control_params.numberOfSteps(), 2000000)
        self.assertEqual(control_params.dumpInterval(), 1000)


if __name__ == '__main__':
    unittest.main()

