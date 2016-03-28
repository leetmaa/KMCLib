""" Module for the process statistics analysis plugin """


# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import sys
import numpy

from KMCLib.PluginInterfaces.KMCAnalysisPlugin import KMCAnalysisPlugin
from KMCLib.Utilities.CheckUtilities import checkSequenceOfPositiveIntegers
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend.Backend import MPICommons

class ProcessStatistics(KMCAnalysisPlugin):
    """ Class for collecting process statistics during a run. """


    def __init__(self,
                 processes=None,
                 time_interval=None,
                 spatially_resolved=None):
        """
        Constructor for the process statistics analysis.

        :param processes: List of process numbers to collect statistics from.
        :param time_interval: Time interval for binning results. Defaults to
                              1.0 if not specified.
        :type time_interval: float
        :param spatially_resolved: True if spatially resolved information about
                                   the total number of events per site should
                                   be collected. Defaults to False.
        :type spatially_resovled: bool
        """
        # Check and set the input.
        msg = "The 'processes' parameter must be given as a list of process numbers."
        self.__processes = checkSequenceOfPositiveIntegers(processes, msg)
        self.__time_interval = checkPositiveFloat(time_interval, 1.0, 'time_interval')
        if (spatially_resolved is None):
            spatially_resolved = False
        elif not isinstance(spatially_resolved, bool):
            raise Error("The 'spatially_resolved' parameter to the ProcessStatistics analysis must be given as a bool.")

        # Done checking.
        self.__spatially_resolved = spatially_resolved
        self.__last_time = 0.0
        self.__data = []
        self.__spatial_data = None
        self.__current_count = 0

    def setup(self, step, time, configuration):
        """
        Recieves the setup call.
        """
        # Set the initial time.
        self.__last_time = time

        # Allocate space for the spatially resolved information.
        if self.__spatially_resolved:
            self.__spatial_data = numpy.zeros((len(configuration.sites())))

    def registerStep(self, step, time, configuration):
        """
        Recieves the register step call from the main loop.
        """
        # Check if the time_intervall has passed since last block finalization.
        if (time - self.__last_time) > self.__time_interval:

            # Finalize the previous block before adding the new value.
            self.__data.append(self.__current_count)
            self.__current_count = 0
            self.__last_time = time

        # Add the new value.
        if configuration.latestEventProcess() in self.__processes:
            self.__current_count += 1
            # Add the spatially resolved data.
            if self.__spatially_resolved:
                self.__spatial_data[configuration.latestEventSite()] += 1.0

    def finalize(self):
        """
        Recieves the finalize call after the MC loop.
        """
        if self.__spatially_resolved:
            # Normalize the spatial data with time.
            self.__spatial_data /= self.__last_time

    def printResults(self, stream=sys.stdout):
        """
        Print the results to the stream.

        :param stream: The stream to print to.
        """
        # Only master writes.
        if MPICommons.isMaster():
            stream.write("%15s %15s %15s %12s\n"%("  time (t)", " count (n)", "(dn/dt)   ", "(n/t)"))
            n_tot  = 0
            for i,n in enumerate(self.__data):
                # Calculate the values to present.
                t  = i * self.__time_interval
                dt = self.__time_interval
                n_tot += n
                dn     = n

                # Only for times != zero.
                if i > 0:
                    stream.write("%15.5f %15i %15.5f %15.5f\n"%(t, n_tot, dn/dt, n_tot/t))

    def spatialData(self):
        """
        Query for the spatial data.
        """
        return self.__spatial_data


