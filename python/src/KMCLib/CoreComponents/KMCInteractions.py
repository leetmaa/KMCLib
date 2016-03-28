""" Module for the KMCInteractions """


# Copyright (c)  2012-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy
import inspect

from KMCLib.CoreComponents.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.CoreComponents.KMCBaseProcess import KMCBaseProcess
from KMCLib.Utilities.CheckUtilities import checkSequence
from KMCLib.Utilities.CheckUtilities import checkPositiveInteger
from KMCLib.Utilities.CheckUtilities import checkSequenceOf
from KMCLib.Utilities.ConversionUtilities import stdVectorStringToStringList
from KMCLib.PluginInterfaces.KMCRateCalculatorPlugin import KMCRateCalculatorPlugin
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend
from KMCLib.Backend import Custom


class KMCInteractions(object):
    """
    Class for holding information about all possible interactions used in a
    Lattice KMC simulation.
    """

    def __init__(self,
                 processes=None,
                 implicit_wildcards=None):
        """
        Constructor for the KMCInteractions.

        :param processes: A list of possible processes in the simulation.

        :param implicit_wildcards: A flag indicating if implicit wildcards should be used in
                                   the matching of processes with the configuration. The default
                                   is True, i.e. to use implicit wildcards.
        :type implicit_wildcards:  bool
        """
        # Check the processes input.
        processes = checkSequenceOf(processes, KMCBaseProcess, msg="The 'processes' input must be a list of KMCProcess or KMCBucketProcess instances.")

        # Store the processes.
        self.__processes = processes

        # Check the implicit wildcard flag.
        if implicit_wildcards is None:
            implicit_wildcards = True
        if not isinstance(implicit_wildcards, bool):
            raise Error("The 'implicit_wildcard' flag to the KMCInteractions constructor must be given as either True or False")
        self.__implicit_wildcards = implicit_wildcards

        # Set the backend to be generated at first query.
        self.__backend = None

        # Set the rate calculator.
        self.__rate_calculator = None
        self.__rate_calculator_class = None
        self.__builtin_custom = False

    def rateCalculator(self):
        """
        Query for the rate calculator.
        :returns: The stored rate calculator.
        """
        return self.__rate_calculator

    # FIXME: NEEDS MORE TESTING
    def setRateCalculator(self,
                          rate_calculator=None):
        """
        Set the rate calculator of the class. The rate calculator must be
        set before the backend is generated to take effect.

        :param rate_calculator:    A class inheriting from the
                                   KMCRateCalculatorPlugin interface. If not given
                                   the rates specified for each process will be used unmodified.
        """

        # If the rate calculator given is a string we should use one of the
        # builtin custom calculators.
        if isinstance(rate_calculator, str):

            # Get the list of custom calculators.
            calculator_candidates = []
            forbidden = ['RateCalculator',
                         'SimpleDummyBaseClass',
                         'SwigPyIterator']
            for obj in dir(Custom):
                attr = getattr(Custom, obj)

                # Check that the attribute is a class and that its name
                # is not on the list of known forbidden names.

                if (str(attr).replace("<","").split(" ")[0] == 'class') and not obj in forbidden:
                    calculator_candidates.append(obj)

            # Check that the name matches one of the candidates.
            if not rate_calculator in calculator_candidates:
                format_str = "\n    %s"*len(calculator_candidates)
                msg = """
The 'rate_calculator' given to the KMCInteractions object must
be a class (not instantiated) inheriting from the KMCRateCalculatorPlugin, or
a string refering to the name of one of the builtin custom calculators.
The list of candidate calculators are:  """ + format_str%tuple(calculator_candidates)
                raise Error(msg)

            # Save the class name for use in scripting.
            self.__rate_calculator_str = "'" + rate_calculator + "'"

            # Get the class.
            rate_calculator = getattr(Custom, rate_calculator)
            self.__builtin_custom = True

        # Check the rate calculator.
        if rate_calculator is not None:

            # Check if this is a class.
            if not inspect.isclass(rate_calculator):
                msg = """
The 'rate_calculator' given to the KMCInteractions object must
be a class (not instantiated) inheriting from the KMCRateCalculatorPlugin. """
                raise Error(msg)

            # Save the class name for use in scripting.
            self.__rate_calculator_str = str(rate_calculator).replace("'>","").split('.')[-1]

        # Store the class for later instantiation.
        self.__rate_calculator_class = rate_calculator

    def implicitWildcards(self):
        """
        Query for the implicit wildcard flag.

        :returns: The implicit wildcard flag stored.
        """
        return self.__implicit_wildcards

    def _backend(self, possible_types, n_basis, configuration):
        """
        Query for the interactions backend object.

        :param possible_types: A dict with the global mapping of type strings
                               to integers.

        :param n_basis: The size of the configuration basis is.
        :type n_basis: int

        :param configuration: The configuration of the systm, to be passed
                              on to any attached custom rate calculator.

        :returns: The interactions object in C++
        """
        if self.__backend is None:

            # Check the possible_types against the types in the processes.
            for process_number, process in enumerate(self.__processes):
                all_elements = process.allPresentTypes()
                if (not all([(e in possible_types) for e in all_elements])):
                    raise Error("Process %i contains elements not present in the list of possible types of the configuration."%(process_number))

            # Setup the correct type of backend process objects
            # depending on the presence of a rate calculator.

            if self.__rate_calculator_class is not None:

                # Instantiate the rate calculator.
                if self.__builtin_custom == False:
                    rate_calculator = self.__rate_calculator_class(configuration)
                else:
                    rate_calculator = self.__rate_calculator_class(configuration._backend())

                if self.__builtin_custom == False:
                    if not isinstance(rate_calculator, KMCRateCalculatorPlugin):
                        msg = """
The 'rate_calculator' given to the KMCInteractions class must
inherit from the KMCRateCalculatorPlugin. """
                        raise Error(msg)
                    elif rate_calculator.__class__ == KMCRateCalculatorPlugin(configuration).__class__:
                        msg = """
The 'rate_calculator' given to the KMCInteractions class must
inherit from the KMCRateCalculatorPlugin class. It may not be
the KMCRateCalculatorPlugin class itself. """
                        raise Error(msg)
                # Tests passed. Save the instantiated rate calculator on the class.
                self.__rate_calculator = rate_calculator

                # Generate the process vector.
                cpp_processes = Backend.StdVectorCustomRateProcess()
            else:
                # Generate the process vector.
                cpp_processes = Backend.StdVectorProcess()

            # For each interaction.
            for process_number, process in enumerate(self.__processes):

                # Get the corresponding C++ objects.
                cpp_config1 = process.localConfigurations()[0]._backend(possible_types)

                if len(process.localConfigurations()) == 2:
                    cpp_config2 = process.localConfigurations()[1]._backend(possible_types)
                else:
                    # Take a copy of the first configuration and set the update from the
                    # process.
                    cpp_config2 = cpp_config1
                    cpp_config2.setUpdateInfo(process._update())

                rate_constant   = process.rateConstant()

                basis_list = range(n_basis)
                if process.basisSites() is not None:
                    # Make sure this basis list does not contain positions
                    # that are not in the configuration.
                    basis_list = []
                    for b in process.basisSites():
                        if b < n_basis:
                            basis_list.append(b)

                # And construct the C++ entry.
                cpp_basis = Backend.StdVectorInt(basis_list)

                # Setup the move vectors representation in C++.
                move_origins = [int(v[0]) for v in process.moveVectors()]
                cpp_move_origins = Backend.StdVectorInt(move_origins)
                cpp_move_vectors = Backend.StdVectorCoordinate()
                for v in process.moveVectors():
                    cpp_move_vectors.push_back(Backend.Coordinate(v[1][0], v[1][1], v[1][2]))

                # Construct and store the C++ process.
                if self.__rate_calculator is not None:
                    # Set the cutoff correctly.
                    cutoff = self.__rate_calculator.cutoff()
                    if cutoff is None:
                        cutoff = 1.0

                    # Get the cache_rate flag.
                    cache_rate = (self.__rate_calculator.cacheRates() and \
                                      not process_number in self.__rate_calculator.excludeFromCaching())

                    cpp_processes.push_back(Backend.CustomRateProcess(cpp_config1,
                                                                      cpp_config2,
                                                                      rate_constant,
                                                                      cpp_basis,
                                                                      cutoff,
                                                                      cpp_move_origins,
                                                                      cpp_move_vectors,
                                                                      process_number,
                                                                      cache_rate))
                else:
                    cpp_processes.push_back(Backend.Process(cpp_config1,
                                                            cpp_config2,
                                                            rate_constant,
                                                            cpp_basis,
                                                            cpp_move_origins,
                                                            cpp_move_vectors,
                                                            process_number))

            # Construct the C++ interactions object.
            if self.__rate_calculator is not None:
                self.__backend = Backend.Interactions(cpp_processes,
                                                      self.__implicit_wildcards,
                                                      self.__rate_calculator)
            else:
                self.__backend = Backend.Interactions(cpp_processes,
                                                      self.__implicit_wildcards)

        # Return the stored backend.
        return self.__backend

    def _script(self, variable_name="interactions"):
        """
        Generate a script representation of an instance.

        :param variable_name: A name to use as variable name for
                              the KMCInteractions in the generated script.
        :type variable_name: str

        :returns: A script that can generate this interactions object.
        """

        # Loop through the list of interactions and for each one set up the
        # script for the pair of local configurations that goes together in a tuple.
        processes_script = ""
        processes_string = "processes = ["

        for i,process in enumerate(self.__processes):

            var_name = "process_%i"%(i)
            processes_script += process._script(var_name)

            if i == 0:
                indent = ""
            else:
                indent = " "*13

            processes_string += indent + var_name

            # If this is the last process, close.
            if i == len(self.__processes) - 1:
                processes_string += "]\n"
            else:
                processes_string += ",\n"

        # Add a comment line.
        comment_string = """
# -----------------------------------------------------------------------------
# Interactions

"""
        if self.__implicit_wildcards:
            implicit = "True"
        else:
            implicit = "False"

        kmc_interactions_string = variable_name + " = KMCInteractions(\n" + \
            "    processes=processes,\n" + \
            "    implicit_wildcards=%s)\n"%(implicit)

        # Return the script.
        return comment_string + processes_script + processes_string + "\n" + \
            kmc_interactions_string



