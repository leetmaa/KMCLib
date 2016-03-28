""" Module for testing the KMCControlParameters class. """


# Copyright (c)  2012-2015  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest

from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend

# Import from the module we test.
from KMCLib.CoreComponents.KMCControlParameters import KMCControlParameters


# Implement the test.
class KMCControlParametersTest(unittest.TestCase):
    """ Class for testing the KMCControlParameters class. """

    def testConstructionAndQuery1(self):
        """ Test the construction of the control parametes object """
        # Default construction.
        control_params = KMCControlParameters()

        # Check the default values.
        self.assertEqual(control_params.numberOfSteps(), 0)
        self.assertEqual(control_params.dumpInterval(), 1)
        self.assertEqual(control_params.seed(), 1)
        self.assertTrue(control_params.timeSeed())
        self.assertEqual(control_params.rngType(), Backend.MT)

        # Non-default construction.
        control_params = KMCControlParameters(number_of_steps=2000000,
                                              dump_interval=1000,
                                              analysis_interval=888,
                                              seed=2013,
                                              rng_type='DEVICE')

        # Check the values.
        self.assertEqual(control_params.numberOfSteps(), 2000000)
        self.assertEqual(control_params.dumpInterval(), 1000)
        self.assertEqual(control_params.analysisInterval(), 888)
        self.assertEqual(control_params.seed(), 2013)
        self.assertFalse(control_params.timeSeed())
        self.assertEqual(control_params.rngType(), Backend.DEVICE)

    def testRngTypeInput(self):
        """ Test all valid values of the rng_type parameter. """
        control_params = KMCControlParameters()
        self.assertEqual(control_params.rngType(), Backend.MT)

        control_params = KMCControlParameters(rng_type='MT')
        self.assertEqual(control_params.rngType(), Backend.MT)

        control_params = KMCControlParameters(rng_type='RANLUX24')
        self.assertEqual(control_params.rngType(), Backend.RANLUX24)

        control_params = KMCControlParameters(rng_type='RANLUX48')
        self.assertEqual(control_params.rngType(), Backend.RANLUX48)

        control_params = KMCControlParameters(rng_type='MINSTD')
        self.assertEqual(control_params.rngType(), Backend.MINSTD)

        control_params = KMCControlParameters(rng_type='DEVICE')
        self.assertEqual(control_params.rngType(), Backend.DEVICE)

        # Wrong value.
        self.assertRaises( Error,
                           lambda : KMCControlParameters(rng_type='ABC'))

        # Wrong type.
        self.assertRaises( Error,
                           lambda : KMCControlParameters(rng_type=123))

    def testConstructionAndQuery2(self):
        """ Test the construction of the control parametes object with a dump time interval """
        # Non-default construction.
        control_params = KMCControlParameters(number_of_steps=2000000,
                                              analysis_interval=888,
                                              seed=2013,
                                              dump_time_interval=1.23)

        # Check the values.
        self.assertEqual(control_params.numberOfSteps(), 2000000)
        self.assertEqual(control_params.analysisInterval(), 888)
        self.assertEqual(control_params.seed(), 2013)
        self.assertFalse(control_params.timeSeed())
        self.assertAlmostEqual(control_params.dumpTimeInterval(), 1.23, 10)
        self.assertTrue(control_params.dumpInterval() is None)

        # Check that we cannot construct with both dump interval and
        # dump time interval set.
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=1,
                                                         dump_interval=1,
                                                         analysis_interval=1,
                                                         dump_time_interval=1.23) )

    def testConstructionFail(self):
        """ Make sure we can not give invalid paramtes on construction. """
        # Negative values.
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=-1,
                                                         dump_interval=1,
                                                         analysis_interval=1) )
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=1,
                                                         dump_interval=-1,
                                                         analysis_interval=1) )
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=1,
                                                         dump_interval=1,
                                                         analysis_interval=-1) )
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=1,
                                                         analysis_interval=1,
                                                         seed=-1234) )
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=1,
                                                         analysis_interval=1,
                                                         dump_time_interval=-1234.0) )

        # Wrong type.
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps="1",
                                                         dump_interval=1,
                                                         analysis_interval=1) )
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=1,
                                                         dump_interval="1",
                                                         analysis_interval=1) )
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=1,
                                                         dump_interval=1,
                                                         analysis_interval="1"))
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=1,
                                                         dump_interval=1,
                                                         analysis_interval=1,
                                                         seed="1234") )
        self.assertRaises( Error,
                           lambda : KMCControlParameters(number_of_steps=1,
                                                         analysis_interval=1,
                                                         dump_time_interval="1234.0") )

if __name__ == '__main__':
    unittest.main()

