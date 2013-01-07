""" Module for the Trajectory object """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import sys
import time


class Trajectory(object):
    """
    Class for handling IO to a trajectory file.
    """

    def __init__(self,
                 trajectory_filename,
                 sites):
        """
        Constructor for the Trajectory.

        :param trajectory_filename: The file name to write trajectory information to.

        :param sites: The lattice sites of the configuration.
        """
        # Note that this object is not in the interface, so input is allready checked.

        # Open the file and write the meta information.
        # NEEDS IMPLEMENTATION

        # Write the sites.
        # NEEDS IMPLEMENTATION

        # Init the member data.
        self.__types_buffer = []
        self.__simulation_time_buffer = []
        self.__step_buffer = []

        # The constants defining the buffering behaviour.
        self.__max_buffer_size = 1024*1024*10    #   <- Max buffer size in bytes (10 MB)
        self.__max_buffer_time = 30*60           #   <- Max time in seconds (30 min)

    def append(self, simulation_time, step, types):
        """
        Append the types and time information to the trajectory.
        The trajectory if flushed to file if the flush time limit has passed.

        :param simulation_time: The current time of the simulation.
        :type simulation_time: float

        :param step: The step number in the simulation.
        :type step: int

        :param types: The types given as a list of strings.
        """
        # Store to the local buffers.
        self.__types_buffer.append(types)
        self.__simulation_time_buffer.append(simulation_time)
        self.__step_buffer.append(step)

        # Check the size of the types buffer.
        types_size = sys.getsizeof(self.__types_buffer)

        # Write if the buffer is to large of if enough time has passed.
        if (types_size > self.__max_buffer_size):
            self.__writeToFile(simulation_time, step, types)

        elif ((time.time() - self.__time_last_dump) > max_buffer_time):
            self.__writeToFile(simulation_time, step, types)

    def __writeToFile(self, simulation_time, step, types):
        """
        Append the types and time information to the trajectory.
        The trajectory if flushed to file if the flush time limit has passed.

        :param simulation_time: The current time of the simulation.
        :type simulation_time: float

        :param step: The step number in the simulation.
        :type step: int

        :param types: The types given as a list of strings.
        """
        # Save to file.
        # NEEDS IMPLEMENTATION

        # Update the time.
        self.__time_last_dump = time.time()


