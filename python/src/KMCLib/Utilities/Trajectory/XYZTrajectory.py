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
        # NEEDS IMPLEMENTATION

        # Write the header.
        self.__writeHeader()

    def __writeHeader(self):
        """
        Write the header to the file.

        :param sites: The sites in the system.
        """
        # Make sure only master writes.
        if MPICommons.isMaster():

            # Open the file and write the meta information.
            with open(self._trajectory_filename, 'w') as trajectory:
                pass

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
        pass

    def _bufferSize(self):
        """
        Calculate and return the buffer size.
        """
        # FIXME
        return 13.7

    def flush(self):
        """ Write all buffers to file. """
        pass



