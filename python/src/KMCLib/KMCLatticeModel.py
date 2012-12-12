""" Module for the KMCLatticeModel """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.Backend import Backend

from KMCLib.KMCConfiguration import KMCConfiguration
from KMCLib.KMCInteractions import KMCInteractions

class KMCLatticeModel(object):
    """
    Class for representing a lattice KMC model.
    """

    def __init__(self,
                 configuration=None,
                 interactions=None):
        """
        The KMCLatticeModel class is the central object in the KMCLib framework
        for running a KMC simulation. Once a configuration with a lattice is
        defined and a set of interactions are setup, the KMCLatticeModel object
        unites this information, checks that the given interactions match the
        configurations, and provides means for running a KMC Lattice simulation.

        :param configuration: The KMCConfiguration to run the simulation for.

        :param interactions: The KMCInteractions that specify possible local
                             states and barriers to use in the simulation.

        """
        # Check the configuration.
        if not isinstance(configuration, KMCConfiguration):
            raise Error("The 'configuration' parameter to the KMCLatticeModel must be an instance of type KMCConfiguration.")

        # Store.
        self.__configuration = configuration

        # Check the interactions.
        if not isinstance(interactions, KMCInteractions):
            raise Error("The 'interactions' parameter to the KMCLatticeModel must be an instance of type KMCInteractions.")

        # Store.
        self.__interactions = interactions

        # Set the backend to be generated at first query.
        self.__backend = None

    def _backend(self):
        """
        Function for generating the C++ backend reperesentation of this object.

        :returns: The C++ LatticeModel based on the parameters given to this class on construction.
        """
        if self.__backend is None:
            # Setup the C++ objects we need.
            cpp_config       = self.__configuration._backend()
            cpp_lattice_map  = self.__configuration._latticeMap()
            cpp_interactions = self.__interactions._backend(self.__configuration.possibleTypes())
            # Construct the backend iobject.
            self.__backend = Backend.LatticeModel(cpp_config, cpp_lattice_map, cpp_interactions)
        # Return.
        return self.__backend

    def run(control_parameters=None,
            trajectory_filename=None):
        """
        Run the KMC lattice model simulation with specified parameters.
        """
        pass
        # Check the input.
        #if not isinstance(control_parameters, KMCControlParameters):
        #    raise Error("The 'control_parameters' input to the KMCLatticeModel run funtion\nmust be an instance of type KMCControlParameters.")

        # Check the trajectory filename.
        # NEEDS IMPLEMENTATION

        # Construct the C++ lattice model.
        #cpp_model = self._backend()

        # Setup a trajector object.
        # NEEDS IMPLEMENTATION
        # trajectory = KMCTrajectory(filename=trajectory_filename)

        # Get the needed parameters.
        # NEEDS IMPLEMENTATION
        #temperature = control_parameters.temperature()
        #n_steps     = control_parameters.steps()

        # Run the loop.
        # for step in n_steps:
        # NEEDS IMPLEMENTATION

            # Take a step.
            # cpp_model.singleStep(temperature)

            # Perform IO using the trajectory object.

        # Sync the configurations.

