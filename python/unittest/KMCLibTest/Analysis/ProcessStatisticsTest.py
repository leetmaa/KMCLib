""" Module for testing the process statistics analysis. """


# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import StringIO
import numpy

from KMCLib.Exceptions.Error import Error
from KMCLib.Backend.Backend import MPICommons

# Import from the module we test.
from KMCLib.Analysis.ProcessStatistics import ProcessStatistics


# Implement the test.
class ProcessStatisticsTest(unittest.TestCase):
    """ Class for tersting the process statistics analysis. """

    def testConstruction(self):
        """ Test that we can construct a valid ProcessStatistics object. """
        ps0 = ProcessStatistics(processes=[1,2,5],
                                time_interval=2.3,
                                spatially_resolved=True)

        # Check values on the class.
        self.assertEqual(ps0._ProcessStatistics__processes, [1,2,5])
        self.assertEqual(ps0._ProcessStatistics__current_count, 0)
        self.assertAlmostEqual(ps0._ProcessStatistics__time_interval, 2.3, 10)
        self.assertTrue(ps0._ProcessStatistics__spatially_resolved)
        self.assertTrue(ps0._ProcessStatistics__spatial_data is None)

        # Construct with default time interval and spatial resolution.
        ps0 = ProcessStatistics(processes=[1,2,5])
        self.assertAlmostEqual(ps0._ProcessStatistics__time_interval, 1.0, 10)
        self.assertFalse(ps0._ProcessStatistics__spatially_resolved)
        self.assertTrue(ps0._ProcessStatistics__spatial_data is None)

    def testConstructionFail(self):
        """ Test that construction fails with wrong input. """
        # Fail because of missing processes input.
        self.assertRaises( Error,
                           lambda : ProcessStatistics(time_interval=2.3) )

        # Wrong type.
        self.assertRaises( Error,
                           lambda : ProcessStatistics(processes=["1","2"],
                                                      time_interval=2.3) )
        self.assertRaises( Error,
                           lambda : ProcessStatistics(processes=0,
                                                      time_interval=2.3) )
        self.assertRaises( Error,
                           lambda : ProcessStatistics(processes=[1],
                                                      time_interval=1) )
        self.assertRaises( Error,
                           lambda : ProcessStatistics(processes=[1],
                                                      time_interval=1.0,
                                                      spatially_resolved="YES") )

        # Wrong value.
        self.assertRaises( Error,
                           lambda : ProcessStatistics(processes=[-1],
                                                      time_interval=2.3) )
        self.assertRaises( Error,
                           lambda : ProcessStatistics(processes=[1],
                                                      time_interval=-1.0) )

    def testSetup(self):
        """ Test that the setup function stores the time properly. """
        ps = ProcessStatistics(processes=[0],
                               time_interval=2.3)
        # Check initial value.
        self.assertAlmostEqual(ps._ProcessStatistics__last_time, 0.0, 10)

        # Call setup.
        ps.setup("dummy_step", "dummy_time", "dummy_config")
        self.assertEqual(ps._ProcessStatistics__last_time, "dummy_time")

        class DummyConfig:
            def sites(self):
                return [1,2,3,4,5]

        ps = ProcessStatistics(processes=[0],
                               time_interval=2.3,
                               spatially_resolved=True)

        ps.setup("dummy_step", "dummy_time", DummyConfig())
        self.assertEqual(ps._ProcessStatistics__last_time, "dummy_time")
        self.assertEqual(len(ps._ProcessStatistics__spatial_data), 5)
        self.assertAlmostEqual(numpy.linalg.norm(ps._ProcessStatistics__spatial_data), 0.0, 10)

    def testRegisterStepAndFinalize(self):
        """ Test the step registration. """
        ps = ProcessStatistics(processes=[0],
                               time_interval=0.3,
                               spatially_resolved=True)

        class DummyConfig:
            def latestEventProcess(self):
                return 0
            def latestEventSite(self):
                return 1
            def sites(self):
                return [1,2,3]

        # Set the initial time.
        config = DummyConfig()
        ps.setup("dummy_step", 0.0, config)

        # Set the current count.
        ps._ProcessStatistics__current_count = 156

        # Check initial value.
        self.assertEqual(ps._ProcessStatistics__data, [])
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[0], 0.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[1], 0.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[2], 0.0, 10)

        # Register the step.
        time = 1.0
        ps.registerStep("dummy_step", time, config)

        # Check the result.
        self.assertEqual(ps._ProcessStatistics__data, [156])
        self.assertEqual(ps._ProcessStatistics__current_count, 1)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[0], 0.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[1], 1.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[2], 0.0, 10)

        # Add again.
        time += 0.2
        ps.registerStep("dummy_step", time, config)
        self.assertEqual(ps._ProcessStatistics__data, [156])
        self.assertEqual(ps._ProcessStatistics__current_count, 2)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[0], 0.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[1], 2.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[2], 0.0, 10)

        # Add again.
        time += 0.5
        ps.registerStep("dummy_step", time, config)
        self.assertEqual(ps._ProcessStatistics__data, [156, 2])
        self.assertEqual(ps._ProcessStatistics__current_count, 1)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[0], 0.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[1], 3.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[2], 0.0, 10)

        # Now with another dummy config.
        class DummyConfig1:
            def latestEventProcess(self):
                return 1
            def latestEventSite(self):
                return 1
        config = DummyConfig1()

        time += 0.5
        ps.registerStep("dummy_step", time, config)
        self.assertEqual(ps._ProcessStatistics__data, [156, 2, 1])
        self.assertEqual(ps._ProcessStatistics__current_count, 0)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[0], 0.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[1], 3.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[2], 0.0, 10)

        # Finalize.
        ps.finalize()

        # Check that the spatially resolved data gets divided by the total time.
        total_t = 0.5 + 0.5 + 0.2 + 1.0
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[0], 0.0, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[1], 3.0/total_t, 10)
        self.assertAlmostEqual(ps._ProcessStatistics__spatial_data[2], 0.0, 10)

    def testPrintResults(self):
        """ Test that we can print the results """
        # Setup.
        ps = ProcessStatistics(processes=[0],
                               time_interval=0.3)

        # Add data on the class.
        ps._ProcessStatistics__data = [0, 12, 245, 1435]

        stream = StringIO.StringIO()
        ps.printResults(stream)

        # Reference checked by hand.
        ref_value = \
"""       time (t)       count (n)      (dn/dt)           (n/t)
        0.30000              12        40.00000        40.00000
        0.60000             257       816.66667       428.33333
        0.90000            1692      4783.33333      1880.00000
"""
        if MPICommons.isMaster():
            self.assertEqual(stream.getvalue(), ref_value)
        else:
            self.assertEqual(stream.getvalue(), "")

    def testSpatialData(self):
        """ Test that the query for the spatial data works. """
        ps = ProcessStatistics(processes=[0],
                               time_interval=0.3,
                               spatially_resolved=True)
        ref_data = "REFERENCE"
        ps._ProcessStatistics__spatial_data = ref_data

        # Check by reference.
        self.assertTrue( ps.spatialData() == ref_data )


if __name__ == '__main__':
    unittest.main()
