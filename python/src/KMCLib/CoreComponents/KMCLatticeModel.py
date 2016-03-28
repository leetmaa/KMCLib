""" Module for the KMCLatticeModel """


# Copyright (c)  2012-2015  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.Backend import Backend

from KMCLib.CoreComponents.KMCConfiguration import KMCConfiguration
from KMCLib.CoreComponents.KMCInteractions import KMCInteractions
from KMCLib.CoreComponents.KMCControlParameters import KMCControlParameters
from KMCLib.PluginInterfaces.KMCAnalysisPlugin import KMCAnalysisPlugin
from KMCLib.PluginInterfaces.KMCBreakerPlugin import KMCBreakerPlugin
from KMCLib.Exceptions.Error import Error
from KMCLib.Utilities.Trajectory.LatticeTrajectory import LatticeTrajectory
from KMCLib.Utilities.Trajectory.XYZTrajectory import XYZTrajectory
from KMCLib.Utilities.PrintUtilities import prettyPrint
from KMCLib.Utilities.CheckUtilities import checkSequenceOf
from KMCLib.Backend import Backend

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

        # Set the verbosity level of output to minimal.
        self.__verbosity_level = 0

    def _backend(self):
        """
        Function for generating the C++ backend reperesentation of this object.

        :returns: The C++ LatticeModel based on the parameters given to this class on construction.
        """
        if self.__backend is None:
            # Setup the C++ objects we need.
            cpp_config       = self.__configuration._backend()
            cpp_lattice_map  = self.__configuration._latticeMap()
            cpp_interactions = self.__interactions._backend(self.__configuration.possibleTypes(),
                                                            cpp_lattice_map.nBasis(),
                                                            self.__configuration)
            # Construct a timer.
            self.__cpp_timer = Backend.SimulationTimer()

            # Construct the backend object.
            self.__backend = Backend.LatticeModel(cpp_config,
                                                  self.__cpp_timer,
                                                  cpp_lattice_map,
                                                  cpp_interactions)
        # Return.
        return self.__backend

    def run(self,
            control_parameters=None,
            trajectory_filename=None,
            trajectory_type=None,
            analysis=None,
            breakers=None):
        """
        Run the KMC lattice model simulation with specified parameters.

        :param control_paramters:   An instance of KMCControlParameters specifying
                                    number of steps to run etc.

        :param trajectory_filename: The filename of the trajectory. If not given
                                    no trajectory will be saved.

        :param trajectory_type:     The type of trajectory to use. Either 'lattice' or 'xyz'.
                                    The 'lattice' format shows the types at the latice points.
                                    The 'xyz' format gives type and coordinate for each particle.
                                    The default type is 'lattice'.
        :param analysis:            A list of instantiated analysis objects that should be used for on-the-fly analysis.

        :param breakers:            A list of instantiated breaker objects to break the Monte Carlo loop with a custom criterion.
        """
        # Check the input.
        if not isinstance(control_parameters, KMCControlParameters):
            msg ="""
The 'control_parameters' input to the KMCLatticeModel run funtion
must be an instance of type KMCControlParameters."""
            raise Error(msg)

        # Check the trajectory filename.
        use_trajectory = True
        if trajectory_filename is None:
            use_trajectory = False
            msg =""" KMCLib: WARNING: No trajectory filename given -> no trajectory will be saved."""
            prettyPrint(msg)

        elif not isinstance(trajectory_filename, str):
            msg = """
The 'trajectory_filename' input to the KMCLattice model run function
must be given as string."""
            raise Error(msg)

        # Check the analysis type.
        if trajectory_type is None:
            trajectory_type = 'lattice'

        if not isinstance(trajectory_type, str):
            raise Error("The 'trajectory_type' input must given as a string.")

        # Check the analysis.
        if analysis is None:
            analysis = []
        else:
            msg = "Each element in the 'analyis' list must be an instance of KMCAnalysisPlugin."
            analysis = checkSequenceOf(analysis, KMCAnalysisPlugin, msg)

        # Check the breakers.
        if breakers is None:
            breakers = []
        else:
            msg = "Each element in the 'breakers' list must be an instance of KMCBreakerPlugin."
            breakers = checkSequenceOf(breakers, KMCBreakerPlugin, msg)

        # Set and seed the backend random number generator.
        if not Backend.setRngType(control_parameters.rngType()):
            raise Error("DEVICE random number generator is not supported by your system, or the std::random_device in the standard C++ library you use is implemented using a pseudo random number generator (entropy=0).")

        Backend.seedRandom(control_parameters.timeSeed(),
                           control_parameters.seed())

        # Construct the C++ lattice model.
        prettyPrint(" KMCLib: setting up the backend C++ object.")

        cpp_model = self._backend()

        # Print the initial matching information if above the verbosity threshold.
        if self.__verbosity_level > 9:
            self.__printMatchInfo(cpp_model)

        # Check that we have at least one available process to  run the KMC simulation.
        if cpp_model.interactions().totalAvailableSites() == 0:
            raise Error("No available processes. None of the processes defined as input match any position in the configuration. Change the initial configuration or processes to run KMC.")

        # Setup a trajectory object.
        last_time = self.__cpp_timer.simulationTime()
        if use_trajectory:
            if trajectory_type == 'lattice':
                trajectory = LatticeTrajectory(trajectory_filename=trajectory_filename,
                                               configuration=self.__configuration)
            elif trajectory_type == 'xyz':
                trajectory = XYZTrajectory(trajectory_filename=trajectory_filename,
                                           configuration=self.__configuration)
            else:
                raise Error("The 'trajectory_type' input must be either 'lattice' or 'xyz'.")

            # Add the first step.
            trajectory.append(simulation_time  = self.__cpp_timer.simulationTime(),
                              step             = 0,
                              configuration    = self.__configuration)

        # Setup the analysis objects.
        for ap in analysis:
            step = 0
            ap.setup(step,
                     self.__cpp_timer.simulationTime(),
                     self.__configuration);

        # Get the needed parameters.
        n_steps   = control_parameters.numberOfSteps()
        n_dump    = control_parameters.dumpInterval()
        n_analyse = control_parameters.analysisInterval()
        dump_time = control_parameters.dumpTimeInterval()

        prettyPrint(" KMCLib: Runing for %i steps, starting from time: %f\n"%(n_steps, self.__cpp_timer.simulationTime()))

        # Run the KMC simulation.
        try:
            # Loop over the steps.
            step = 0
            time_step = 0
            while(step < n_steps):
                step += 1

                # Check if it is possible to take a step.
                nP = cpp_model.interactions().totalAvailableSites()
                if nP == 0:
                    raise Error("No more available processes.")

                # Take a step.
                time_before = self.__cpp_timer.simulationTime()
                cpp_model.propagateTime()
                time_after  = self.__cpp_timer.simulationTime()

                # Check if it is time to dump the previous step to the equidistant trajectory.
                if ((dump_time is not None) and ((time_after-last_time) >= dump_time)):
                    time_step += 1
                    sample_time = time_step * dump_time
                    prettyPrint(" KMCLib: %14i steps executed. time: %20.10e"%(step-1, sample_time))
                    last_time = sample_time

                    # Perform IO using the trajectory object.
                    if use_trajectory:
                        trajectory.append(simulation_time  = time_before,
                                          step             = step-1,
                                          configuration    = self.__configuration)

                # Update the model.
                cpp_model.singleStep()

                # Get the current simulation time.
                now = self.__cpp_timer.simulationTime()

                # Check if it is time to write a trajectory dump.
                if ((dump_time is None) and ((step)%n_dump == 0)):
                    last_time = now
                    prettyPrint(" KMCLib: %i steps executed. time: %20.10e "%(step, now))

                    # Perform IO using the trajectory object.
                    if use_trajectory:
                        trajectory.append(simulation_time  = now,
                                          step             = step,
                                          configuration    = self.__configuration)

                if ((step)%n_analyse == 0):
                    # Run all other python analysis.
                    for ap in analysis:
                        ap.registerStep(step,
                                        now,
                                        self.__configuration)

                # Check all custom break criteria.
                break_the_loop = False
                for b in breakers:

                    # If it is time to evaluate this breaker.
                    if ((step)%b.interval() == 0):
                        break_the_loop = b.evaluate(step,
                                                    now,
                                                    self.__configuration)
                        # Break the inner loop.
                        if break_the_loop:
                            break

                # Break the main Monte-Carlo loop.
                if break_the_loop:
                    break

        finally:

            # Flush the trajectory buffers when done.
            if use_trajectory:
                trajectory.flush()

            # Perform the analysis post processing.
            for ap in analysis:
                ap.finalize();

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

    def __printMatchInfo(self, cpp_model):
        """ """
        """
        Private routine for printing the initial matching information
        given a C++ lattice model object.

        :param cpp_model: The C++ lattice model to print the matching for.
        """
        cpp_processes = cpp_model.interactions().processes()

        prettyPrint("")
        prettyPrint(" Matching Information: ")
        for i,p in enumerate(cpp_processes):
            print i,p.sites()

