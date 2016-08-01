""" Module for the XYZTrajectory object """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import sys
import time


from KMCLib.Backend.Backend import MPICommons
from KMCLib.Utilities.Trajectory.Trajectory import Trajectory


class XYZTrajectory(Trajectory):
    """
    Class for handling xyz IO to a trajectory file.
    """

    def __init__(self,
                 trajectory_filename,
                 configuration,
                 max_buffer_size=None,
                 max_buffer_time=None):
        """
        Constructor for the XYZTrajectory.

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
        self.__atom_id_types = []
        self.__atom_id_coordinates = []
        self.__time = []
        self.__step = []

        # Write the header.
        self.__writeHeader(configuration)

    def __writeHeader(self, configuration):
        """
        Write the header to the file.

        :param configuration: The configuration of the system.
        """
        # Make sure only master writes.
        if MPICommons.isMaster():

            # Open the file and write the meta information.
            with open(self._trajectory_filename, 'w') as trajectory:

                # Version.
                trajectory.write("KMCLib XYZ FORMAT VERSION 2013.10.15\n\n")

                # Cellvectors.
                cell_vectors = configuration.lattice().unitCell().cellVectors()
                trajectory.write("CELL VECTORS\n")
                trajectory.write("a: %15.10e %15.10e %15.10e\n"%(cell_vectors[0][0], cell_vectors[0][1], cell_vectors[0][2]))
                trajectory.write("b: %15.10e %15.10e %15.10e\n"%(cell_vectors[1][0], cell_vectors[1][1], cell_vectors[1][2]))
                trajectory.write("c: %15.10e %15.10e %15.10e\n\n"%(cell_vectors[2][0], cell_vectors[2][1], cell_vectors[2][2]))

                # Repetitions.
                repetitions = configuration.lattice().repetitions()
                trajectory.write("REPETITIONS %i %i %i\n\n"%(repetitions[0], repetitions[1], repetitions[2]))

                # Periodicity.
                periodicity = configuration.lattice().periodic()
                trajectory.write("PERIODICITY %s %s %s\n\n"%(str(periodicity[0]), str(periodicity[1]), str(periodicity[2])))

        # While the other processes wait.
        MPICommons.barrier()

    def _storeData(self, simulation_time, step, configuration):
        """
        Append the coordinate, types and time information to the
        internal buffers.

        :param simulation_time: The current time of the simulation.
        :type simulation_time: float

        :param step: The step number in the simulation.
        :type step: int

        :param configuration: The configuration of the simulation.
        """
        # Store to the local buffers.
        self.__time.append(simulation_time)
        self.__step.append(step)
        self.__atom_id_coordinates.append(configuration.atomIDCoordinates())
        self.__atom_id_types.append(configuration.atomIDTypes())

    def _bufferSize(self):
        """
        Calculate and return the buffer size.
        """
        size =  sys.getsizeof(self.__atom_id_coordinates)
        size +=  sys.getsizeof(self.__atom_id_coordinates[0])*len(self.__atom_id_coordinates)
        size += sys.getsizeof(self.__atom_id_types)
        size += sys.getsizeof(self.__atom_id_types[0])*len(self.__atom_id_types)
        size += sys.getsizeof(self.__time)
        size += sys.getsizeof(self.__time[0])*len(self.__time)
        size += sys.getsizeof(self.__step)
        size += sys.getsizeof(self.__step[0])*len(self.__step)

        return size

    def flush(self):
        """ Write all buffers to file. """
        if not len(self.__step) < 1:

            # Make sure only master writes.
            if MPICommons.isMaster():

                # Write data to file.
                with open(self._trajectory_filename, 'a') as trajectory:
                    for i in range(len(self.__step)):

                        step = self.__step[i]
                        time = self.__time[i]
                        n_atoms = len(self.__atom_id_types[i])

                        trajectory.write("STEP %i\n"%(step))
                        trajectory.write("          %i\n"%(n_atoms))
                        trajectory.write("    TIME %15.10e\n"%(time))

                        for j in range(n_atoms):
                            t = self.__atom_id_types[i][j]
                            c = self.__atom_id_coordinates[i][j]
                            trajectory.write(" %16s   %15.10e %15.10e %15.10e  %i\n"%(t, c[0], c[1], c[2], j))

            # While the other processes wait.
            MPICommons.barrier()

            # Reset the buffers.
            self.__atom_id_types = []
            self.__atom_id_coordinates = []
            self.__time = []
            self.__step = []
