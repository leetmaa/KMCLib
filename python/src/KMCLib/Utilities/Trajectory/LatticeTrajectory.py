""" Module for the LatticeTrajectory object """


# Copyright (c)  2013-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import sys
import time


from KMCLib.Backend.Backend import MPICommons
from KMCLib.Utilities.Trajectory.Trajectory import Trajectory
from KMCLib.Utilities.ConversionUtilities import toShortBucketsFormat

class LatticeTrajectory(Trajectory):
    """
    Class for handling lattice IO to a trajectory file.
    """

    def __init__(self,
                 trajectory_filename,
                 configuration,
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
        # Call the base class constructor.
        Trajectory.__init__(self,
                            trajectory_filename,
                            max_buffer_size,
                            max_buffer_time)

        # Init the member data.
        self.__types_buffer = []
        self.__simulation_time_buffer = []
        self.__step_buffer = []

        # Write the header.
        self.__writeHeader(configuration.sites())

    def __writeHeader(self, sites):
        """
        Write the header to the file.

        :param sites: The sites in the system.
        """
        # Make sure only master writes.
        if MPICommons.isMaster():

            # Open the file and write the meta information.
            with open(self._trajectory_filename, 'w') as trajectory:
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

        # While the other processes wait.
        MPICommons.barrier()

    def _storeData(self, simulation_time, step, configuration):
        """
        Append the types and time information to the internal buffers.

        :param simulation_time: The current time of the simulation.
        :type simulation_time: float

        :param step: The step number in the simulation.
        :type step: int

        :param configuration: The configuration of the simulation.
        """
        # Store to the local buffers.
        self.__types_buffer.append(configuration.types())
        self.__simulation_time_buffer.append(simulation_time)
        self.__step_buffer.append(step)

    def _bufferSize(self):
        """
        Calculate and return the buffer size.
        """
        return sys.getsizeof(self.__types_buffer[0])*len(self.__types_buffer) + sys.getsizeof(self.__types_buffer)

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

        # Make sure only master writes.
        if MPICommons.isMaster():
            with open(self._trajectory_filename, 'a') as trajectory:
                for (sim_time, step, types) in zip(simulation_time_buffer, step_buffer, types_buffer):
                    trajectory.write( "times.append(%18.10e)\n"%sim_time )
                    trajectory.write( "steps.append(%i)\n"%step )

                    # Write the types.
                    types_str = "types.append(["
                    indent = " "*14
                    row_length = len(types_str)

                    if isinstance(types[:-1][0], str):
                        for t in types[:-1]:
                            row_length += len(t) + 2
                            types_str += "\"" + t + "\"" + ","

                            # Berak the row if above 70 positions.
                            if row_length >= 70:
                                types_str += "\n" + indent
                                row_length = len(indent)

                        # Add the last type.
                        types_str += "\"" + types[-1] + "\"" + "])\n"

                    # With bucket types.
                    else:
                        bucket_types = toShortBucketsFormat(types)

                        # For all sites except the last.
                        for bt in bucket_types[:-1]:

                            # Start the types for this site.
                            types_str += "["
                            for t in bt[:-1]:
                                types_str += "(" + str(t[0]) + ",\"" + t[1] + "\"),"
                            if len(bt) > 0:
                                t = bt[-1]
                                types_str += "(" + str(t[0]) + ",\"" + t[1] + "\")"
                                types_str += "],\n" + indent

                            else:
                                types_str += "],\n" + indent

                        # For the last site.
                        bt = bucket_types[-1]
                        types_str += "["
                        for t in bt[:-1]:
                            types_str += "(" + str(t[0]) + ",\"" + t[1] + "\"),"
                        if len(bt) > 0:
                            t = bt[-1]
                            types_str += "(" + str(t[0]) + ",\"" + t[1] + "\")]"
                        else:
                            types_str += "]"

                        types_str += "])\n"

                    # Write it to file.
                    trajectory.write(types_str)

        # While the others wait.
        MPICommons.barrier()


