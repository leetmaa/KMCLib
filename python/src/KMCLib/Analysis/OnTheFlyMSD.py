""" Module for the on-the-fly MSD plugin """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import numpy
import sys

from KMCLib.PluginInterfaces.KMCAnalysisPlugin import KMCAnalysisPlugin
from KMCLib.Utilities.CheckUtilities import checkPositiveInteger
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat
from KMCLib.Utilities.ConversionUtilities import stdVectorCoordinateToNumpy2DArray
from KMCLib.Utilities.ConversionUtilities import numpy2DArrayToStdVectorCoordinate
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend

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

        :param history_steps: The number of steps per atom to store in the
                              history buffer.
        :type history_steps: int

        :param n_bins: The nuber of bins in the histogram.
        :type n_bins: int

        :param t_max: The starting value of the last bin.
        :type t_max: float

        :param track_type: The atom type to track during the simulation.
        :type track_type: str
        """
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

        # Set the step counter to zero.
        self.__n_steps = 0

    def setup(self, step, time, configuration):
        """
        Recieves the setup call from the before the MC loop.

        :param step: The step number of the simulation.
        :type step: int

        :param time: The time of the simulation.
        :type time: float

        :param configuration: The configuration of the simulation.
        :type configuration: KMCConfiguration
        """
        # Make sure the track type is one of the possible types.
        if not self.__track_type in configuration.possibleTypes():
            raise Error("The track type of the MSD calculator is not one of the valid types of the configuration.")

        # Get the cell vectors out.
        abc_to_xyz = numpy.array(configuration.lattice().unitCell().cellVectors()).transpose()
        abc_to_xyz_cpp = numpy2DArrayToStdVectorCoordinate(abc_to_xyz)

        # Setup the backend.
        self.__backend = Backend.OnTheFlyMSD(configuration._backend(),
                                             self.__history_steps,
                                             self.__n_bins,
                                             self.__t_max,
                                             time,
                                             self.__track_type,
                                             abc_to_xyz_cpp)

    def registerStep(self, step, time, configuration):
        """
        Recieves the step call from the MC loop.

        :param step: The step number of the simulation.
        :type step: int

        :param time: The time of the simulation.
        :type time: float

        :param configuration: The configuration of the simulation.
        :type configuration: KMCConfiguration
        """
        self.__backend.registerStep(time, configuration._backend())
        self.__n_steps += 1

    def finalize(self):
        """
        Recieves the finalize call after the MC loop.
        """
        # Get the results from the backend.
        self.__getBackendResults()

        # Generate the numerical n_eff curve from the history bin counters.
        self.__n_eff = numpy.zeros(len(self.__bin_counters))

        # Decompose the total in contributions from different convolutions.
        for b in range(len(self.__history_bin_counters)):

            # Loop over each bin.
            for i in range(len(self.__n_eff)):

                if self.__history_bin_counters[b][i] != 0 and self.__bin_counters[i] != 0:
                    fraction = float(self.__history_bin_counters[b][i]) / float(self.__bin_counters[i])
                    self.__n_eff[i] += fraction * (b+1.0)

        # Remove zeros from the bin counters.
        bin_counters_copy = numpy.array(self.__bin_counters)
        where = bin_counters_copy == 0
        bin_counters_copy[where] = 1

        # Produce the final results by dividing with the bin counters
        # and calculate the standard deviation in each point.
        self.__results    = numpy.zeros((3,self.__n_bins))
        self.__results[0] = self.__raw_histogram[:,0] / bin_counters_copy
        self.__results[1] = self.__raw_histogram[:,1] / bin_counters_copy
        self.__results[2] = self.__raw_histogram[:,2] / bin_counters_copy
        self.__time_steps = (numpy.arange(self.__n_bins)+1)*self.__binsize - self.__binsize / 2.0

        # Calculate the standard deviation (of the mean, i.e. the value) in each point.
        self.__std_dev     = numpy.zeros((3,self.__n_bins))

        # Calculate the standar deviation.
        for i in range(len(self.__results[0])):
            n_eff = self.__n_eff[i]

            if (self.__raw_histogram[i,0] != 0):
                self.__std_dev[0][i] = self.__results[0][i] * numpy.sqrt( ( 2 * n_eff * n_eff + 1 ) / ( 3 * n_eff * self.__bin_counters[i] ) )
                self.__std_dev[1][i] = self.__results[1][i] * numpy.sqrt( ( 2 * n_eff * n_eff + 1 ) / ( 3 * n_eff * self.__bin_counters[i] ) )
                self.__std_dev[2][i] = self.__results[2][i] * numpy.sqrt( ( 2 * n_eff * n_eff + 1 ) / ( 3 * n_eff * self.__bin_counters[i] ) )

    def __getBackendResults(self):
        """
        Private helper function to get the backend results.
        """
        # Setup the histogram data in python.
        self.__raw_histogram        = stdVectorCoordinateToNumpy2DArray(self.__backend.histogramBuffer())
        self.__raw_histogram_sqr    = stdVectorCoordinateToNumpy2DArray(self.__backend.histogramBufferSqr())
        self.__bin_counters         = self.__backend.histogramBinCounts()
        self.__history_bin_counters = self.__backend.historyStepsHistogramBinCounts()

    def results(self):
        """
        Query function for the result.

        :returns: The results as a 3*N numpy array with the MSD data in the
                  x, y and z directions (expressed in fractional coordinates).
        """
        return self.__results

    def timeSteps(self):
        """
        Query function for the time steps.

        :returns: The bin center time values of the MSD(t) histogram.
        """
        return self.__time_steps

    def stdDev(self):
        """
        Query function for the standard deviation.

        :returns: The standard deviation of the results, as a 3*N numpy array.
        """
        return self.__std_dev

    def binCounters(self):
        """
        Query function for the bin counters.

        :returns: The number of values that was averaged over in each bin.
        """
        return self.__bin_counters

    def safeCutoff(self):
        """
        Query function for the safe cutoff as determined by the
        start of the last contributing convolution.

        :returns: The bin index for the cutoff.
        """
        # Return the cutoff based on where the last convolution
        # is larger than zero.
        for i,c in enumerate(self.__history_bin_counters[-1]):
            if c > 0:
                return i

        # If all values in the last convolution are zero
        # all values are safe to use.
        return i+1

    def printResults(self, stream=sys.stdout):
        """
        Print the results to a stream.

        :param stream: The stream to print to. Defaults to 'sys.stdout'.
        """
        # Bunch the results together and cutoff.
        cutoff_bin = self.safeCutoff()
        all_results = zip(self.__time_steps,
                          self.__results[0],
                          self.__results[1],
                          self.__results[2],
                          self.__std_dev[0],
                          self.__std_dev[1],
                          self.__std_dev[2])[:cutoff_bin]

        stream.write("%10s %10s %10s %10s %10s %10s %10s\n"%("TIME", "MSD_a", "DSD_b", "MSD_c", "STD_a", "STD_b", "STD_c"))
        for t, x, y, z, sx, sy, sz in all_results:
            stream.write("%10.5f %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f\n"%(t, x, y, z, sx, sy, sz))
