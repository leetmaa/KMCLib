""" Module for the on-the-fly MSD plugin """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import numpy
import sys

from KMCLib.KMCAnalysisPlugin import KMCAnalysisPlugin
from KMCLib.Utilities.CheckUtilities import checkPositiveInteger
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat
from KMCLib.Utilities.ConversionUtilities import stdVectorCoordinateToNumpy2DArray
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend

class FastOnTheFlyMSD(KMCAnalysisPlugin):
    """
    Class for performing on-the-fly MSD analysis.
    """

    def __init__(self,
                 history_steps=None,
                 n_bins=None,
                 t_max=None,
                 track_type=None):
        """
        Constructor for the OnTheFlyMSD.
        """
        # FIXME: Make sure the default values are reasonable.

        # Check and set the history steps input.
        self.__history_steps = checkPositiveInteger(history_steps, 5, "history_step")

        # Check and set the number of bins.
        self.__n_bins = checkPositiveInteger(n_bins, 100, "n_bins")

        # Check and set the time maximum.
        self.__t_max   = checkPositiveFloat(t_max, 100.0, "t_max")

        # Check that the track type.
        if not isinstance(track_type, str):
            raise Error("The 'track_type' parameter must be a string.")
        self.__track_type = track_type

        # Calculate and store the binsize.
        self.__binsize = self.__t_max / self.__n_bins

    def setup(self, step, time, configuration):
        """
        Recieves the setup call from the before the MC loop.
        """
        # Make sure the track type is one of the possible types.
        if not self.__track_type in configuration.possibleTypes():
            raise Error("The track type of the MSD calculator is not one of the valid types of the configuration.")

        # Setup the backend.
        self.__backend = Backend.OnTheFlyMSD(configuration._backend(),
                                             self.__history_steps,
                                             self.__n_bins,
                                             self.__t_max,
                                             time,
                                             self.__track_type)

    def registerStep(self, step, time, configuration):
        """
        Recieves the step call from the MC loop.
        """
        self.__backend.registerStep(time, configuration._backend())

    def finalize(self):
        """
        Recieves the finalize call after the MC loop.
        """
        # Setup the histogram data in python.
        raw_histogram       = stdVectorCoordinateToNumpy2DArray(self.__backend.histogramBuffer())
        raw_histogram_sqr   = stdVectorCoordinateToNumpy2DArray(self.__backend.histogramBufferSqr())
        self.__bin_counters = self.__backend.histogramBinCounts()

        # Remove zeros from the bin counters.
        bin_counters_copy = numpy.array(self.__bin_counters)
        where = bin_counters_copy == 0
        bin_counters_copy[where] = 1

        # Produce the final results by dividing with the bin counters
        # and calculate the standard deviation in each point.
        self.__results    = numpy.zeros((3,self.__n_bins))
        self.__results[0] = raw_histogram[:,0] / bin_counters_copy
        self.__results[1] = raw_histogram[:,1] / bin_counters_copy
        self.__results[2] = raw_histogram[:,2] / bin_counters_copy
        self.__time_steps = (numpy.arange(self.__n_bins)+1)*self.__binsize - self.__binsize / 2.0

        # Calculate the standard deviation (of the mean, i.e. the value) in each point.
        sqr_raw_histogram  = numpy.square(raw_histogram)
        self.__std_dev     = numpy.zeros((3,self.__n_bins))
        sqr_raw_histogram_div = numpy.array(sqr_raw_histogram)
        sqr_raw_histogram_div[:,0] = sqr_raw_histogram[:,0] / bin_counters_copy

        # Calculate the prefactors.
        prefactor2 = (1.0 / numpy.sqrt(bin_counters_copy))
        where = bin_counters_copy == 1
        bin_counters_copy[where] = 2
        prefactor1 = (1.0 / (bin_counters_copy - 1))

        # Get the standard deviation.
        self.__std_dev[0] = prefactor2 * numpy.sqrt(prefactor1 * (raw_histogram_sqr[:,0] - sqr_raw_histogram_div[:,0] ))
        self.__std_dev[1] = prefactor2 * numpy.sqrt(prefactor1 * (raw_histogram_sqr[:,1] - sqr_raw_histogram_div[:,1] ))
        self.__std_dev[2] = prefactor2 * numpy.sqrt(prefactor1 * (raw_histogram_sqr[:,2] - sqr_raw_histogram_div[:,2] ))

    def result(self, stream=sys.stdout):
        """
        Print results.
        """
        all_results = zip(self.__time_steps, self.__results[0], self.__results[1], self.__results[2], self.__bin_counters, self.__std_dev[0], self.__std_dev[1], self.__std_dev[2])
        for t, x, y, z, c, sx, sy, sz in all_results:
            stream.write("%10.5f %10.5f %10.5f %10.5f %10i %10.5f %10.5f %10.5f\n"%(t, x, y, z, c, sx, sy, sz))

    def _calculateBin(self, dt):
        """
        Calculate the bin for a given delta t.
        """
        return (int)(dt / self.__binsize)

    def _storePartialHistograms(self):
        """
        Store the partial histogram to avoid numerical overflow.
        """
        # NEEDS IMPLEMENTATION
        raise Error("NOT IMPLEMENTED")

