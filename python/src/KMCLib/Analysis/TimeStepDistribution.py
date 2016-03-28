""" Module for the time step distribution analysis plugin """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import numpy
import sys

from KMCLib.PluginInterfaces.KMCAnalysisPlugin import KMCAnalysisPlugin
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat
from KMCLib.Backend.Backend import MPICommons

class TimeStepDistribution(KMCAnalysisPlugin):
    """
    Class for collecting the time step distribution during a simulation.
    """

    def __init__(self,
                 binsize=None):
        """
        Constructor for the time step distribution analysis object.

        :param binsize: The size of the bins in the histogram.
        :type binsize: float
        """
        # Check the input parameters.
        self.__binsize = checkPositiveFloat(binsize, 1.0, "binsize")

        # Setup an initial histogram of 10 entries.
        self.__histogram = numpy.zeros(10, dtype=int)

    def setup(self, step, time, configuration):
        """
        Recieves the setup call from before the MC loop.
        """
        self.__last_time = time

    def registerStep(self, step, time, configuration):
        """
        Recieves the step call from the MC loop.
        """
        # Calculate the delta t since last time.
        dt = time - self.__last_time
        self.__last_time = time

        # Calculate the bin.
        b = int(dt / self.__binsize)

        # Make sure we have place for the bin.
        while len(self.__histogram) <= b:
            new_len = len(self.__histogram) + len(self.__histogram)/10
            self.__histogram.resize(new_len)

        # Increment the value at this bin.
        self.__histogram[b] += 1

    def finalize(self):
        """
        Recieves the finalize call after the MC loop.
        """
        n_bins = len(self.__histogram)
        self.__time_steps = (numpy.arange(n_bins)+1)*self.__binsize - self.__binsize / 2.0

        n_samples = float(numpy.sum(self.__histogram))
        self.__normalized_histogram = self.__histogram / n_samples

    def histogram(self):
        """
        Query function for the histogram.

        :returns: The raw histogram as a numpy array.
        """
        return self.__histogram

    def normalizedHistogram(self):
        """
        Query function for the histogram normalized to 1.

        :returns: The normalized historgram.
        """
        return self.__normalized_histogram

    def timeSteps(self):
        """
        Query function for the time steps (x-values).

        :returns: The time steps as a numpy array.
        """
        return self.__time_steps

    def printResults(self, stream=sys.stdout):
        """
        Print the results to the stream.

        :param stream: The stream to print to.
        """
        # Only master writes.
        if MPICommons.isMaster():

            all_results = zip(self.__time_steps, self.__histogram, self.__normalized_histogram)
            for t, v, n in all_results:
                stream.write("%10.5f %10i %20.15f\n"%(t, v, n))
