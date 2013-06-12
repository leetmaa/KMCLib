""" Module for the on-the-fly MSD plugin """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import numpy

from KMCLib.KMCAnalysisPlugin import KMCAnalysisPlugin
from KMCLib.Utilities.CheckUtilities import checkPositiveInteger
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat

class OnTheFlyMSD(KMCAnalysisPlugin):
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

        # Setup the histogram.
        self.__raw_histogram = numpy.zeros((self.__n_bins))
        self.__bin_counters  = numpy.zeros((self.__n_bins), dtype=int)
        self.__binsize = self.__t_max / self.__n_bins

        print self.__raw_histogram
        print self.__bin_counters

    def setup(self, step, time, configuration):
        """
        Recieves the setup call from the before the MC loop.
        """
        # Make sure the track type is one of the possible types.
        if not self.__track_type in configuration.possibleTypes():
            raise Error("The track type of the MSD calculator is not one of the valid types of the configuration.")

        # Setup the internal buffer data structures.
        self.__buffer = numpy.zeros((len(configuration.types()), self.__history_steps, 4))

    def registerStep(self, step, time, configuration):
        """
        Recieves the step call from the MC loop.
        """
        # Check if any of our tracking type has moved.

        # Make place for the latest coordinate and time.

        # Add the latest coordinate and time.

        # Calculate the MSD values and MDS values squared.

        # Bin the values and increment the bin counters.

        # Make sure we do not get a numerical overvlow here.

        # NEEDS IMPLEMENTATION
        pass

    def finalize(self):
        """
        Recieves the finalize call after the MC loop.
        """
        # Produce the final results by dividing with the bin counters
        # and calculate the standard deviation in each point.

        # NEEDS IMPLEMENTATION
        pass

    def result(self):
        """
        Access function for the MSD results.
        """
        # FIXME: Dummy return
        return numpy.array([[1.0,0.0],[2.0,0.0],[3.0,0.0],[4.0,0.0]])
