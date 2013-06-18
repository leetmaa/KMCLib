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
from KMCLib.Exceptions.Error import Error

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
        self.__raw_histogram     = numpy.zeros((self.__n_bins, 3))
        self.__raw_histogram_sqr = numpy.zeros((self.__n_bins, 3))
        self.__bin_counters      = numpy.zeros((self.__n_bins), dtype=int)

        # Calculate and store the binsize.
        self.__binsize = self.__t_max / self.__n_bins

    def setup(self, step, time, configuration):
        """
        Recieves the setup call from the before the MC loop.
        """
        # Make sure the track type is one of the possible types.
        if not self.__track_type in configuration.possibleTypes():
            raise Error("The track type of the MSD calculator is not one of the valid types of the configuration.")

        # Setup the internal buffer data structures.
        self.__buffer = numpy.zeros((len(configuration.types()), self.__history_steps, 4))
        self.__step_counters = numpy.ones(len(configuration.types()), dtype=int)

        # Populate the internal buffer data with the t=0 values.
        atom_id_coordinates = configuration.atomIDCoordinates()
        atom_id_types       = configuration.atomIDTypes()
        atom_ids = range(len(configuration.atomIDCoordinates()))

        for atom_id in atom_ids:
            if atom_id_types[atom_id] == self.__track_type:
                # Get the coordinate.
                coord = numpy.array(list(atom_id_coordinates[atom_id].data()))

                # Store the transformed initial coordinate.
                self.__buffer[atom_id][0][0:3] = coord

        # Done.

    def registerStep(self, step, time, configuration):
        """
        Recieves the step call from the MC loop.
        """
        # Check if any of our tracking type has moved.
        atom_ids            = configuration.movedAtomIDs()
        atom_id_types       = configuration.atomIDTypes()
        atom_id_coordinates = configuration.atomIDCoordinates()

        # Track the atom ids.
        for atom_id in atom_ids:
            if atom_id_types[atom_id] == self.__track_type:

                # Make place for the latest coordinate and time.
                for s in range(self.__history_steps - 1):
                    step = self.__history_steps - s - 2
                    self.__buffer[atom_id][step+1][:] = self.__buffer[atom_id][step][:]

                # Add the latest coordinate and time.
                coord = numpy.array(list(atom_id_coordinates[atom_id].data()))

                # NEEDS IMPLEMENTATION
                # Transform the coordinate to Angstrom.
                # transf_coord =

                self.__buffer[atom_id][0][0:3] = coord
                self.__buffer[atom_id][0][3]   = time

                # Calculate the squared displacement values
                # and squared displacement values squared.
                steps = min(self.__history_steps, self.__step_counters[atom_id] + 1)
                for s in range(steps - 1):
                    step = s + 1
                    diff = numpy.square(self.__buffer[atom_id][0][0:3] - self.__buffer[atom_id][step][0:3])
                    dt   = self.__buffer[atom_id][0][3] - self.__buffer[atom_id][step][3]

                    # Bin the values and increment the bin counters.
                    bin_idx = self._calculateBin(dt)
                    if bin_idx < self.__n_bins:
                        self.__raw_histogram[bin_idx]     += diff
                        self.__raw_histogram_sqr[bin_idx] += numpy.square(diff)
                        self.__bin_counters[bin_idx]      += 1

                # Increment the step counter.
                self.__step_counters[atom_id] += 1

        # FIXME : Implement this properly.
        # Check the bounds of the largest value.
        if self.__raw_histogram_sqr.max() >= sys.float_info.max - 10000:

            # Time to handle the numerical overflow case.
            self._storePartialHistograms()

        # Done.

    def finalize(self):
        """
        Recieves the finalize call after the MC loop.
        """
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
        sqr_raw_histogram  = numpy.square(self.__raw_histogram)
        self.__std_dev     = numpy.zeros((3,self.__n_bins))
        sqr_raw_histogram_div = numpy.array(sqr_raw_histogram)
        sqr_raw_histogram_div[:,0] = sqr_raw_histogram[:,0] / bin_counters_copy

        # Calculate the prefactors.
        prefactor2 = (1.0 / numpy.sqrt(bin_counters_copy))
        where = bin_counters_copy == 1
        bin_counters_copy[where] = 2
        prefactor1 = (1.0 / (bin_counters_copy - 1))

        # Get the standard deviation.
        self.__std_dev[0] = prefactor2 * numpy.sqrt(prefactor1 * (self.__raw_histogram_sqr[:,0] - sqr_raw_histogram_div[:,0] ))
        self.__std_dev[1] = prefactor2 * numpy.sqrt(prefactor1 * (self.__raw_histogram_sqr[:,1] - sqr_raw_histogram_div[:,1] ))
        self.__std_dev[2] = prefactor2 * numpy.sqrt(prefactor1 * (self.__raw_histogram_sqr[:,2] - sqr_raw_histogram_div[:,2] ))

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

