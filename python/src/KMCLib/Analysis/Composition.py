""" Module for the composition analysis plugin """


# Copyright (c)  2015  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import sys
import numpy

from KMCLib.PluginInterfaces.KMCAnalysisPlugin import KMCAnalysisPlugin

from KMCLib.Utilities.CheckUtilities import checkPositiveFloat
from KMCLib.Backend.Backend import MPICommons

class Composition(KMCAnalysisPlugin):
    """ Class for collecting composition statistics during a run. """


    def __init__(self,
                 time_interval=None):
        """
        Constructor for the composition analysis.

        :param time_interval: Time interval for binning results. Defaults to
                              1.0 if not specified.
        :type time_interval: float
        """
        # Check and set the input.
        self.__time_interval = checkPositiveFloat(time_interval, 1.0, 'time_interval')

        # Done checking.
        self.__current_steps = 0
        self.__last_time = 0.0
        self.__data = []
        self.__current_count = None
        self.__empty_count = None

    def setup(self, step, time, configuration):
        """
        Recieves the setup call.
        """
        # Set the initial values.
        self.__last_time     = time
        self.__empty_count   = numpy.array([0]*len(configuration._backendTypeNames()))
        self.__current_count = numpy.array(configuration.particlesPerType())
        self.__current_steps = 1
        self.__type_names    = configuration._backendTypeNames()

    def registerStep(self, step, time, configuration):
        """
        Recieves the register step call from the main loop.
        """
        # Check if the time_intervall has passed since last block finalization.
        if (time - self.__last_time) > self.__time_interval:
            # Finalize the previous block before adding the new value.
            step_data = self.__current_count / (self.__current_steps * 1.0)
            self.__data.append( step_data )
            self.__current_count = numpy.array(self.__empty_count)
            self.__current_steps = 0
            self.__last_time = time

        # Add the new values.
        self.__current_count += numpy.array(configuration.particlesPerType())
        self.__current_steps += 1

    def finalize(self):
        """
        Recieves the finalize call after the MC loop.
        """
        pass

    def printResults(self, stream=sys.stdout):
        """
        Print the results to the stream.

        :param stream: The stream to print to.
        """
        # Only master writes.
        if MPICommons.isMaster():

            # The format string for the values.
            format_str = " %15.3e"*(len(self.__empty_count))
            format_str += "\n"

            # The format string for the counts.
            format_str_1 = " %15s"*(len(self.__empty_count))
            format_str_1 += "\n"

            types_str = ["time"]
            for t in self.__type_names[1:]:
                types_str.append(t)

            stream.write(format_str_1%tuple(types_str))

            for i,d in enumerate(self.__data):
                # Calculate the time.
                t  = i * self.__time_interval + (self.__time_interval / 2.0)

                # Append the values.
                tt = [t]
                for dd in d[1:]:
                    tt.append(dd)

                # Print.
                stream.write(format_str%tuple(tt))

    def data(self):
        """
        Query for the raw data.
        :returns: The data store on the class.
        """
        return self.__data

    def times(self):
        """
        Query for the times. The times are reported
        at the middle of the collecction interval.
        :returns: The times corresponding to the data.
        """
        # Calculate and return the time.
        return numpy.array([i * self.__time_interval + (self.__time_interval / 2.0) for i,d in enumerate(self.__data)])
