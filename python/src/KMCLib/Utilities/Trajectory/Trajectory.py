""" Module for the Trajectory base class """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import time


class Trajectory(object):
    """
    Trajectory base class for defining common trajectory functionality.
    """

    def __init__(self,
                 trajectory_filename,
                 max_buffer_size=None,
                 max_buffer_time=None):
        """
        Constructor for the Trajectory.

        :param trajectory_filename: The file name to write trajectory information to.
        :type trajectory_filename: str

        :param max_buffer_size: The max size of the the buffer in memory
                                before writing to file.
        :type max_buffer_size: int

        :param max_buffer_time: The max time limit between dumps to file.
        :type max_buffer_time: float
        """
        # Save the file name. Note that this is not a user object. Data is checked.
        self._trajectory_filename = trajectory_filename

        # Set the defaults.
        if max_buffer_size is None:
            # Max buffer size in bytes (10 MB)
            self.__max_buffer_size = 1024*1024*10
        else:
            self.__max_buffer_size = max_buffer_size

        if max_buffer_time is None:
            # Max time in seconds (30 min)
            self.__max_buffer_time = 30*60.0
        else:
            self.__max_buffer_time = max_buffer_time

        # Set the time counter to zero.
        self.__time_last_dump = 0.0

    def append(self, simulation_time, step, configuration):
        """
        Append to the trajectory. The trajectory if flushed to file if the
        flush time limit or buffer memory size limit has passed.

        :param simulation_time: The current time of the simulation.
        :type simulation_time: float

        :param step: The step number in the simulation.
        :type step: int

        :param configuration: The configuration of the simulation.
        """
        # Store the data.
        self._storeData(simulation_time, step, configuration)

        # Write if the buffer is to large of if enough time has passed.
        buffer_size = self._bufferSize()
        if (buffer_size > self.__max_buffer_size) or \
                ((time.time() - self.__time_last_dump) > self.__max_buffer_time):
            self.flush()

            # Update the time.
            self.__time_last_dump = time.time()

