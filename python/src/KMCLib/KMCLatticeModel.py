""" Module for the KMCLatticeModel """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.Backend import Backend

from KMCLib.KMCConfiguration import KMCConfiguration
from KMCLib.KMCInteractions import KMCInteractions
from KMCLib.KMCControlParameters import KMCControlParameters
from KMCLib.Exceptions.Error import Error

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

            # Construct a timer.
            self.__cpp_timer = Backend.SimulationTimer()

            # Construct the backend iobject.
            self.__backend = Backend.LatticeModel(cpp_config,
                                                  self.__cpp_timer,
                                                  cpp_lattice_map,
                                                  cpp_interactions)
        # Return.
        return self.__backend

    def run(self,
            control_parameters=None,
            trajectory_filename=None):
        """
        Run the KMC lattice model simulation with specified parameters.
        """
        # Check the input.
        if not isinstance(control_parameters, KMCControlParameters):
            raise Error("The 'control_parameters' input to the KMCLatticeModel run funtion\nmust be an instance of type KMCControlParameters.")

        # Check the trajectory filename.
        # NEEDS IMPLEMENTATION

        # Construct the C++ lattice model.
        print " KMCLib: setting up the backend C++ object."
        cpp_model = self._backend()

        # Setup a trajector object.
        # NEEDS IMPLEMENTATION
        # trajectory = KMCTrajectory(filename=trajectory_filename)

        # Get the needed parameters.
        n_steps = control_parameters.numberOfSteps()

        print " KMCLib: Runing for %i steps, starting from time: %f\n"%(n_steps,self.__cpp_timer.simulationTime())

        # For profiling.
        for step in range(n_steps):
            # Take a step.
            cpp_model.singleStep()

            if ((step+1)%10 == 0):
                print " KMCLib: %i steps executed. time: %f "%(step+1, self.__cpp_timer.simulationTime())

            # Perform IO using the trajectory object.

    def _script(self, variable_name="model"):
        """
        Generate a script representation of an instance.

        :param variable_name: A name to use as variable name for
                              the KMCLatticeModel in the generated script.
        :type variable_name: str

        :returns: A script that can generate this kmc lattice model.
        """
        # Get the configuration and interactions scripts.
        configuration_script = self.__configuration._script(variable_name="configuration")
        interactions_script  = self.__interactions._script(variable_name="interactions")

        # Setup the lattice model string.
        lattice_model_string = variable_name + """ = KMCLatticeModel(
    configuration=configuration,
    interactions=interactions)
"""

        # And a comment string.
        comment_string = """
# -----------------------------------------------------------------------------
# Lattice model

"""
        # Return the script.
        return configuration_script + interactions_script + \
            comment_string + lattice_model_string

