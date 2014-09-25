""" Module for testing the time step distribution analysis. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import numpy
import StringIO

# Import from the module we test.
from KMCLib.Analysis.TimeStepDistribution import TimeStepDistribution
from KMCLib.Backend.Backend import MPICommons


# Implement the test.
class TimeStepDistributionTest(unittest.TestCase):
    """ Class for testing the time step distribution analysis. """

    def testConstruction(self):
        """ Test the time step distribution analysis construction. """
        # Check default member data.
        tsd = TimeStepDistribution()
        self.assertAlmostEqual(tsd._TimeStepDistribution__binsize, 1.0, 12)
        self.assertEqual(len(tsd._TimeStepDistribution__histogram), 10)
        self.assertEqual(numpy.linalg.norm(tsd._TimeStepDistribution__histogram - numpy.zeros(10, dtype=int)), 0)

        # Check construction with a given binsize.
        tsd = TimeStepDistribution(binsize=2.3)
        self.assertAlmostEqual(tsd._TimeStepDistribution__binsize, 2.3, 12)
        self.assertEqual(len(tsd._TimeStepDistribution__histogram), 10)
        self.assertEqual(numpy.linalg.norm(tsd._TimeStepDistribution__histogram - numpy.zeros(10, dtype=int)), 0)

    def testSetup(self):
        """ Test the setup interface function. """
        # Construct.
        tsd = TimeStepDistribution()

        # Set the time.
        time = numpy.random.rand()

        # Call setup.
        tsd.setup("step", time, "configuration")

        # Check that the time was saved.
        self.assertAlmostEqual(tsd._TimeStepDistribution__last_time, time, 12)

    def testRegisterStep(self):
        """ Test the register step interface function. """
        # Construct.
        binsize = 1.23
        tsd = TimeStepDistribution(binsize=binsize)

        # Store the histogram size for reference.
        h_size = len(tsd._TimeStepDistribution__histogram)

        # Set the first time and call setup.
        t0 = numpy.random.rand()
        tsd.setup("step", t0, "configuration")

        # Call the step function with a new time.
        t1 = t0 + 5*numpy.random.rand()
        tsd.registerStep("step", t1, "configuration")

        # Calculate the bin.
        b = int((t1 - t0) / binsize)

        # Check that this bin was incremented.
        self.assertEqual(tsd._TimeStepDistribution__histogram[b], 1)
        for i in range(10):
            if i != b:
                self.assertEqual(tsd._TimeStepDistribution__histogram[i], 0)

        # Give a new value that requires the histogram to be extended.
        t2 = 10
        tsd.registerStep("step", t2, "configuration")
        t3 = 23
        tsd.registerStep("step", t3, "configuration")

        # Check that the length has incresed to 11.
        self.assertEqual(len(tsd._TimeStepDistribution__histogram), 11)
        self.assertEqual(tsd._TimeStepDistribution__histogram[10], 1)

        # Increment again. This should end up with a length of 22.
        t4 = 48
        tsd.registerStep("step", t4, "configuration")
        self.assertEqual(len(tsd._TimeStepDistribution__histogram), 22)

    def testFinalizeAndQuery(self):
        """ Test the finalization and query functions. """
        tsd = TimeStepDistribution()

        # Set the member data.
        histogram = numpy.array(numpy.random.rand(12)*34, dtype=int)
        normalized_histogram = histogram / float(numpy.sum(histogram))
        binsize = 2.13
        tsd._TimeStepDistribution__binsize   = binsize
        tsd._TimeStepDistribution__histogram = histogram

        # Call the finalize function.
        tsd.finalize()

        # We now have the normalized histogram and timesteps on the class.
        ret_histogram      = tsd.histogram()
        ret_norm_histogram = tsd.normalizedHistogram()
        ret_time_steps     = tsd.timeSteps()

        # Check the histograms.
        self.assertAlmostEqual(numpy.linalg.norm(ret_histogram - histogram), 0.0, 12)
        self.assertAlmostEqual(numpy.linalg.norm(ret_norm_histogram - normalized_histogram), 0.0, 12)

        # Check the time steps.
        time_steps = (numpy.arange(12)+1)*binsize - binsize/2.0
        self.assertAlmostEqual(numpy.linalg.norm(ret_time_steps - time_steps), 0.0, 12)

    def testPrintResults(self):
        """ Test the time step distribution print result function. """
        tsd = TimeStepDistribution()

        # Set the member data.

        histogram = numpy.array([12,41,55,
                                 23,43,12,
                                 11,19,98,
                                 97,95,93])
        normalized_histogram = histogram / float(numpy.sum(histogram))
        binsize = 2.13
        tsd._TimeStepDistribution__binsize   = binsize
        tsd._TimeStepDistribution__histogram = histogram

        # Call the finalize function.
        tsd.finalize()

        # Print the results to a stream.
        stream = StringIO.StringIO()
        tsd.printResults(stream)

        ref_value = """   1.06500         12    0.020033388981636
   3.19500         41    0.068447412353923
   5.32500         55    0.091819699499165
   7.45500         23    0.038397328881469
   9.58500         43    0.071786310517529
  11.71500         12    0.020033388981636
  13.84500         11    0.018363939899833
  15.97500         19    0.031719532554257
  18.10500         98    0.163606010016695
  20.23500         97    0.161936560934891
  22.36500         95    0.158597662771285
  24.49500         93    0.155258764607679
"""
        # Check the values.
        if MPICommons.isMaster():
            self.assertEqual(stream.getvalue(), ref_value)
        else:
            self.assertEqual(stream.getvalue(), "")


if __name__ == '__main__':
    unittest.main()
