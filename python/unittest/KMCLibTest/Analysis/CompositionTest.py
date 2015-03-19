""" Module for testing the composition analysis. """


# Copyright (c)  2015  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import StringIO
import numpy

from KMCLib.Backend.Backend import MPICommons

# Import from the module we test.
from KMCLib.Analysis.Composition import Composition
from KMCLib.Exceptions.Error import Error

# Implement the test.
class CompositionTest(unittest.TestCase):
    """ Class for tersting the composition analysis. """

    def testConstruction(self):
        """ Test that we can construct a valid Composition analysis object. """
        # Default construct.
        c = Composition()

        # Check values on the class.
        self.assertAlmostEqual(c._Composition__time_interval, 1.0, 10)
        self.assertAlmostEqual(c._Composition__last_time, 0.0, 10)
        self.assertEqual(c._Composition__data,          [])
        self.assertEqual(c._Composition__current_count,  None)
        self.assertEqual(c._Composition__current_steps,  0)
        self.assertTrue(c._Composition__empty_count is None)

        # Construct with given time interval.
        c = Composition(1.2)

        # Check.
        self.assertAlmostEqual(c._Composition__time_interval, 1.2, 10)

    def testConstructionFail(self):
        """ Test that construction fails with wrong input. """
        # Fail because of wrong type.
        self.assertRaises( Error,
                           lambda : Composition(time_interval="123") )

    def testSetup(self):
        """ Test that the setup function gets the initial value correctly. """
        cc = Composition(time_interval=2.3)

        # Check initial value.
        self.assertAlmostEqual(cc._Composition__last_time, 0.0, 10)

        # Get a proxy config.
        class DummyConfig:
            def _backendTypeNames(self):
                return ("a","b","c","d","e")
            def particlesPerType(self):
                return (1,2,3,4,5)

        config = DummyConfig()

        # Call setup.
        cc.setup("dummy_step", "dummy_time", config)
        self.assertEqual(cc._Composition__last_time, "dummy_time")

        ref = numpy.array([0,0,0,0,0])
        diff = numpy.linalg.norm(cc._Composition__empty_count - ref)
        self.assertAlmostEqual(diff, 0.0, 10)

        ref = numpy.array([1,2,3,4,5])
        diff = numpy.linalg.norm(cc._Composition__current_count - ref)
        self.assertAlmostEqual(diff, 0.0, 10)

    def testRegisterStepAndFinalize(self):
        """ Test the step registration. """
        cc = Composition(time_interval=2.0)

        # Get a proxy config.
        class DummyConfig:
            def _backendTypeNames(self):
                return ("a","b","c","d","e")
            def particlesPerType(self):
                return (1,2,3,4,5)

        class DummyConfig2:
            def _backendTypeNames(self):
                return ("a","b","c","d","e")
            def particlesPerType(self):
                return (5,4,3,2,1)

        config = DummyConfig()
        config2 = DummyConfig2()

        # Call setup.
        cc.setup("dummy_step", 2.0, config)
        self.assertEqual(cc._Composition__data, [])

        ref_steps = 1
        self.assertEqual(cc._Composition__current_steps, ref_steps)

        ref_count = numpy.array([1,2,3,4,5])
        diff = numpy.linalg.norm(cc._Composition__current_count - ref_count)
        self.assertAlmostEqual(diff, 0.0, 10)


        # Register a step that triggers append to data.
        cc.registerStep(4, 5.0, config2)

        ref_data = numpy.array([[1.0, 2.0, 3.0, 4.0, 5.0]])
        diff = numpy.linalg.norm(cc._Composition__data - ref_data)
        self.assertAlmostEqual(diff, 0.0, 10)

        ref_steps = 1
        self.assertEqual(cc._Composition__current_steps, ref_steps)

        ref_count = numpy.array([5,4,3,2,1])
        diff = numpy.linalg.norm(cc._Composition__current_count - ref_count)
        self.assertAlmostEqual(diff, 0.0, 10)

        # Call again, this should trigger a new append.
        cc.registerStep(5, 10.0, config)

        ref_data = numpy.array([[1.0, 2.0, 3.0, 4.0, 5.0], [5.0, 4.0, 3.0, 2.0, 1.0]])
        diff = numpy.linalg.norm(cc._Composition__data - ref_data)
        self.assertAlmostEqual(diff, 0.0, 10)

        ref_steps = 1
        self.assertEqual(cc._Composition__current_steps, ref_steps)

        ref_count = numpy.array([1,2,3,4,5])
        diff = numpy.linalg.norm(cc._Composition__current_count - ref_count)
        self.assertAlmostEqual(diff, 0.0, 10)

        # Run again with a small time difference so that the average becomes 3.

        # This adds to the current count.
        cc.registerStep(6, 12.0, config2)

        ref_data = numpy.array([[1.0, 2.0, 3.0, 4.0, 5.0], [5.0, 4.0, 3.0, 2.0, 1.0]])
        diff = numpy.linalg.norm(cc._Composition__data - ref_data)
        self.assertAlmostEqual(diff, 0.0, 10)

        ref_steps = 2
        self.assertEqual(cc._Composition__current_steps, ref_steps)

        ref_count = numpy.array([6,6,6,6,6])
        diff = numpy.linalg.norm(cc._Composition__current_count - ref_count)
        self.assertAlmostEqual(diff, 0.0, 10)

        # This triggers the data append.
        cc.registerStep(7, 20.0, config2)

        ref_data = numpy.array([[1.0, 2.0, 3.0, 4.0, 5.0],
                                [5.0, 4.0, 3.0, 2.0, 1.0],
                                [3.0, 3.0, 3.0, 3.0, 3.0]])

        diff = numpy.linalg.norm(cc._Composition__data - ref_data)
        self.assertAlmostEqual(diff, 0.0, 10)

        ref_steps = 1
        self.assertEqual(cc._Composition__current_steps, ref_steps)

        ref_count = numpy.array([5,4,3,2,1])
        diff = numpy.linalg.norm(cc._Composition__current_count - ref_count)
        self.assertAlmostEqual(diff, 0.0, 10)

    def testPrintResults(self):
        """ Test that we can print the results """
        cc = Composition(time_interval=2.0)

        # Get a proxy config.
        class DummyConfig:
            def _backendTypeNames(self):
                return ("a","b","c","d","e")
            def particlesPerType(self):
                return (1,2,3,4,5)

        class DummyConfig2:
            def _backendTypeNames(self):
                return ("a","b","c","d","e")
            def particlesPerType(self):
                return (5,4,3,2,1)

        config = DummyConfig()
        config2 = DummyConfig2()

        # Call setup.
        cc.setup("dummy_step", 2.0, config)
        cc.registerStep(4, 5.0, config2)
        cc.registerStep(5, 10.0, config)
        cc.registerStep(6, 12.0, config2)
        cc.registerStep(7, 20.0, config2)

        stream = StringIO.StringIO()
        cc.printResults(stream)

        ref_str = \
"""            time               b               c               d               e
       1.000e+00       2.000e+00       3.000e+00       4.000e+00       5.000e+00
       3.000e+00       4.000e+00       3.000e+00       2.000e+00       1.000e+00
       5.000e+00       3.000e+00       3.000e+00       3.000e+00       3.000e+00
"""
        # Reference checked by hand.
        if MPICommons.isMaster():
            self.assertEqual(stream.getvalue(), ref_str)
        else:
            self.assertEqual(stream.getvalue(), "")

    def testQuery(self):
        """ Test that the query for the data works. """
        cc = Composition(time_interval=2.0)

        # Get a proxy config.
        class DummyConfig:
            def _backendTypeNames(self):
                return ("a","b","c","d","e")
            def particlesPerType(self):
                return (1,2,3,4,5)

        class DummyConfig2:
            def _backendTypeNames(self):
                return ("a","b","c","d","e")
            def particlesPerType(self):
                return (5,4,3,2,1)

        config = DummyConfig()
        config2 = DummyConfig2()

        # Fill with values.
        cc.setup("dummy_step", 2.0, config)
        cc.registerStep(4, 5.0, config2)
        cc.registerStep(5, 10.0, config)
        cc.registerStep(6, 12.0, config2)
        cc.registerStep(7, 20.0, config2)

        # Check.
        ref_data = numpy.array([[ 1.,  2.,  3.,  4.,  5.],
                                [ 5.,  4.,  3.,  2.,  1.],
                                [ 3.,  3.,  3.,  3.,  3.]])
        diff = numpy.linalg.norm(cc.data() - ref_data)
        self.assertAlmostEqual(diff, 0.0, 10)

        ref_time = numpy.array([ 1.,  3.,  5.])
        diff = numpy.linalg.norm(cc.times() - ref_time)
        self.assertAlmostEqual(diff, 0.0, 10)


if __name__ == '__main__':
    unittest.main()
