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
                 sites,
                 max_buffer_size=None,
                 max_buffer_time=None):
        """
        Constructor for the Trajectory.

        :param trajectory_filename: The file name to write trajectory information to.
        :type trajectory_filename: str

        :param sites: The lattice sites of the configuration as an Nx3 list.

        :param max_buffer_size: The max size of the the buffer in memory
                                before writing to file.
        :type max_buffer_size: int

        :param max_buffer_time: The max time limit between dumps to file.
        :type max_buffer_time: float
        """
        # Set the defaults.
        if max_buffer_size is None:
            self.__max_buffer_size = 1024*1024*10    #   <- Max buffer size in bytes (10 MB)
        else:
            self.__max_buffer_size = max_buffer_size

        if max_buffer_time is None:
            self.__max_buffer_time = 30*60.0           #   <- Max time in seconds (30 min)
        else:
            self.__max_buffer_time = max_buffer_time

        # Note that this object is not in the interface, so input is allready checked.
        self.__trajectory_filename = trajectory_filename

        # Open the file and write the meta information.
        with open(self.__trajectory_filename, 'w') as trajectory:
            trajectory.write( "# KMCLib Trajectory\n" )
            trajectory.write( "version=\"2013.1.0\"\n" )
            trajectory.write( "creation_time=\"%s\"\n"%(time.ctime()) )

            # Write the sites.
            sites_str = "sites=["
            indent    = " "*7
            for i,site in enumerate(sites):
                sites_str += "[%15.6f,%15.6f,%15.6f]"%(site[0],site[1],site[2])

                # Handle the last site differently.
                if i == len(sites)-1:
                    sites_str += "]\n"
                else:
                    sites_str += ",\n" + indent

            trajectory.write( sites_str )

            # Write the empty lists.
            trajectory.write("times=[]\n")
            trajectory.write("steps=[]\n")
            trajectory.write("types=[]\n")

        # Init the member data.
        self.__types_buffer = []
        self.__simulation_time_buffer = []
        self.__step_buffer = []

        # Set the time counter to zero.
        self.__time_last_dump = 0.0

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
        if (types_size > self.__max_buffer_size) or \
                ((time.time() - self.__time_last_dump) > self.__max_buffer_time):
            self.flush()

    def flush(self):
        """ Write all buffers to file. """
        # Wite - but only if there is something to write.
        if len(self.__simulation_time_buffer) > 0 and \
                len(self.__step_buffer) > 0 and \
                len(self.__types_buffer) > 0:

            self.__writeToFile(self.__simulation_time_buffer,
                               self.__step_buffer,
                               self.__types_buffer)
            # Reset.
            self.__simulation_time_buffer = []
            self.__step_buffer            = []
            self.__types_buffer           = []

    def __writeToFile(self, simulation_time_buffer, step_buffer, types_buffer):
        """
        Append the types and time information to the trajectory.
        The trajectory if flushed to file if the flush time limit has passed.

        :param simulation_time_buffer: A list of simulation times to be written to disk.
        :param step_buffer:            A list of step numbers to be written.
        :param types_buffer:           The types buffer given as a list of lists of strings.
        """
        # Save to file.
        with open(self.__trajectory_filename, 'a') as trajectory:
            for (sim_time, step, types) in zip(simulation_time_buffer, step_buffer, types_buffer):
                trajectory.write( "times.append(%f)\n"%sim_time )
                trajectory.write( "steps.append(%i)\n"%step )

                # Write the types.
                types_str = "types.append(["
                indent = " "*14
                row_length = len(types_str)
                for t in types[:-1]:
                    row_length += len(t) + 2
                    types_str += "\"" + t + "\"" + ","

                    # Berak the row if above 70 positions.
                    if row_length >= 70:
                        types_str += "\n" + indent
                        row_length = len(indent)
                # Add the last type.
                types_str += "\"" + types[-1] + "\"" + "])\n"

                # Write it to file.
                trajectory.write(types_str)

        # Update the time.
        self.__time_last_dump = time.time()


