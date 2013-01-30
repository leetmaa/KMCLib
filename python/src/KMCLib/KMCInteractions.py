""" Module for the KMCInteractions """


# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy
import inspect

from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.Utilities.CheckUtilities import checkSequence
from KMCLib.Utilities.CheckUtilities import checkPositiveInteger
from KMCLib.KMCRateCalculatorPlugin import KMCRateCalculatorPlugin
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend


class KMCInteractions(object):
    """
    Class for holding information about all possible interactions used in a
    Lattice KMC simulation.
    """

    def __init__(self,
                 interactions_list=None,
                 implicit_wildcards=None,
                 rate_calculator=None):
        """
        Constructor for the KMCInteractions.

        :param interactions_list: The interactions, given as a list of lists or tuples with
                                  two local configurations, a rate constant and a list of integers
                                  refering to the basis sites the interaction should be applicable to.
                                  The list of basis sites is optional. If not given it will be treated
                                  as applicable to all basis sites. To use implicit wildcards for an
                                  interaction the list of basis sites must be of length one for that
                                  interaction.
        :type interactions_list:  [(KMCLocalConfiguration, KMCLocalConfiguration, float, [int, ...]), ...]

        :param implicit_wildcards: A flag indicating if implicit wildcards should be used in
                                   the matching of processes with the configuration. The default
                                   is True, i.e. to use implicit wildcards.
        :type implicit_wildcards:  bool

        :param rate_calculator:    An instance of a class inheriting from the
                                   KMCRateCalculatorPlugin interface. If not given
                                   the rates specified for each process will be used unmodified.
        """
        # Check the interactions input.
        self.__raw_interactions = self.__checkInteractionsInput(interactions_list)

        # Check the implicit wildcard flag.
        if implicit_wildcards is None:
            implicit_wildcards = True
        if not isinstance(implicit_wildcards, bool):
            raise Error("The 'implicit_wildcard' flag to the KMCInteractions constructor must be given as either True or False")
        self.__implicit_wildcards = implicit_wildcards

        # Check the rate calculator.
        if rate_calculator is not None:

            # Check if this is a class.
            if not inspect.isclass(rate_calculator):
                msg = """
The 'rate_calculator' input to the KMCInteractions constructor must
be a class (not instantiated) inheriting from the KMCRateCalculatorPlugin. """
                raise Error(msg)

            # Save the class name for use in scripting.
            self.__rate_calculator_str = str(rate_calculator).replace("'>","").split('.')[-1]
            # Instantiate.
            rate_calculator = rate_calculator()
            if not isinstance(rate_calculator, KMCRateCalculatorPlugin):
                msg = """
The 'rate_calculator' input to the KMCInteractions constructor must
be a class inheriting from the KMCRateCalculatorPlugin. """
                raise Error(msg)
            elif rate_calculator.__class__ == KMCRateCalculatorPlugin().__class__:
                msg = """
The 'rate_calculator' input to the KMCInteractions constructor must
be inheriting from the KMCRateCalculatorPlugin class. It may not be
the KMCRateCalculatorPlugin class itself. """
                raise Error(msg)
        # All tests passed. Save the instantiated rate calculator on the class.
        self.__rate_calculator = rate_calculator

        # Set the backend to be generated at first query.
        self.__backend = None

    def __checkInteractionsInput(self, interactions):
        """ """
        """
        Private helper function to check the interactions input parameter.
        """
        msg="The 'interactions' input must be a list of lists or tuples."
        interactions = checkSequence(interactions, msg)

        # Check each interaction.
        for i,interaction in enumerate(interactions):

            # Check that it is a sequence.
            interaction = checkSequence(interaction)

            # Of correct length.
            if len(interaction) != 3 and len(interaction) != 4:
                msg = "Each interaction in the list of interactions must be given as list or tuple of length == 3 or 4.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

            # Check the types of the first two elements.
            c0 = interaction[0]
            c1 = interaction[1]
            if not (isinstance(c0,KMCLocalConfiguration) and isinstance(c1,KMCLocalConfiguration)):
                msg = "The first two elements in each interaction list or tuple must be instances of KMCLocalConfiguration.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

            # Check the type of the third element.
            rate = interaction[2]
            if not isinstance(rate, float):
                msg = "All rate constants must be given as floating point numbers.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

            # Check the fourth element if given.
            if len(interaction) == 4:
                sites = interaction[3]
                sites = checkSequence(sites, msg="The fourth element in the interaction list must be a list of integers.")

                if len(sites) == 0:
                    msg = "The list of available sites for the interaction may not be empty."
                    raise Error(msg)

                for s in sites:
                    checkPositiveInteger(s,
                                         default_parameter=None,
                                         parameter_name="entry in the interaction basis list")

            # Now, check that the coordinates are the same.
            if numpy.linalg.norm(c0.coordinates()-c1.coordinates()) > 1.0e-8:
                msg = "The list of coordinates must match for the two configurations in each interaction.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

            # Check that the types are not the same - this ensures that the interaction does some thing.
            if c0.types() == c1.types():
                msg = "The list of types may not match for the two configurations in each interaction.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

            # Check the positions of the wildcards. If these are not the same
            # the move is invalid.
            wildcards_0 = [i for i,t in enumerate(c0.types()) if t == "*"]
            wildcards_1 = [i for i,t in enumerate(c1.types()) if t == "*"]

            if wildcards_0 != wildcards_1:
                msg = "Explicit wildcards must be in the same positions in both local configurations corresponding to a process.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

        # Done with initial checking of input.
        return interactions

    def implicitWildcards(self):
        """
        Query for the implicit wildcard flag.

        :returns: The implicit wildcard flag stored.
        """
        return self.__implicit_wildcards

    def _backend(self, possible_types, n_basis):
        """
        Query for the interactions backend object.

        :param possible_types: A dict with the global mapping of type strings
                               to integers.

        :param n_basis: The size of the configuration basis is.
        :type n_basis: int

        :returns: The interactions object in C++
        """
        if self.__backend is None:

            # For each interaction.
            cpp_processes = Backend.StdVectorProcess()
            for interaction in self.__raw_interactions:

                # Get the corresponding C++ objects.
                cpp_config1   = interaction[0]._backend(possible_types)
                cpp_config2   = interaction[1]._backend(possible_types)
                barrier       = interaction[2]

                basis_list = range(n_basis)
                if len(interaction) == 4:
                    # Make sure this basis list does not contain positions
                    # that are not in the
                    basis_list = []
                    for b in interaction[3]:
                        if b < n_basis:
                            basis_list.append(b)
                # And construct the C++ vector.
                cpp_basis = Backend.StdVectorInt(basis_list)

                # Construct and store the C++ process.
                cpp_processes.push_back(Backend.Process(cpp_config1,
                                                        cpp_config2,
                                                        barrier,
                                                        cpp_basis))

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

        interaction_strings = []
        configuration_script = ""
        interactions_string = "interactions_list = ["
        for i,interaction in enumerate(self.__raw_interactions):

            conf1_name = "conf1_%i"%(i)
            conf1_script = interaction[0]._script(conf1_name)
            configuration_script += conf1_script

            conf2_name = "conf2_%i"%(i)
            conf2_script = interaction[1]._script(conf2_name)
            configuration_script += conf2_script

            if i == 0:
                indent = ""
            else:
                indent = " "*21

            interactions_string += indent + "(%s, %s, %15.6e"%(conf1_name, conf2_name, interaction[2])

            # Get the basis sites information.
            if len(interaction) == 4:
                basis_string = ",  ["
                for j,b in enumerate(interaction[3]):
                    if j == (len(interaction[3])-1):
                        basis_string += "%i]"%(b)
                    else:
                        basis_string += "%i,"%(b)

                interactions_string += basis_string

            # Close the parenthesis.
            interactions_string += ")"

            # If this is the last string, close.
            if (i == len(self.__raw_interactions)-1):
                interactions_string += "]\n"
            else:
                interactions_string += ",\n"

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
            "    interactions_list=interactions_list,\n" + \
            "    implicit_wildcards=%s)\n"%(implicit)

        # Return the script.
        return configuration_script + comment_string + interactions_string + "\n" + \
            kmc_interactions_string


