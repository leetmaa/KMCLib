""" Module for the KMCInteractions """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy

from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.Utilities.CheckUtilities import checkSequence
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend


class KMCInteractions(object):
    """
    Class for holding information about all possible interactions used in a
    Lattice KMC simulation.
    """

    def __init__(self,
                 interactions_list=None):
        """
        Constructor for the KMCInteractions.

        :param interactions_list: The interactions, given as a list of lists or tuples with
                                  two local configurations and a rate constant.
        :type interactions_list: [(KMCLocalConfiguration, KMCLocalConfiguration, float), ...]
        """
        # Check the interactions inpuy.
        self.__raw_interactions = self.__checkInteractionsInput(interactions_list)

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
            if len(interaction) != 3:
                msg = "Each interaction in the list of interactions must be given as list or tuple of length == 3.\nThe failing interaction is number %i."%(i)
                raise Error()

            # Check the types of the first two elements.
            c0 = interaction[0]
            c1 = interaction[1]
            if not (isinstance(c0,KMCLocalConfiguration) and isinstance(c1,KMCLocalConfiguration)):
                msg = "The first two elements in each interaction list or tuple must be instances of KMCLocalConfiguration.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

            # Check the type of the last element.
            rate = interaction[2]
            if not isinstance(rate, float):
                msg = "All rate constants must be given as floating point numbers.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

            # Now, check that the coordinates are the same.
            if numpy.linalg.norm(c0.coordinates()-c1.coordinates()) > 1.0e-8:
                msg = "The list of coordinates must match for the two configurations in each interaction.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

            # Check that the types are not the same - this ensures that the interaction does some thing.
            if c0.types() == c1.types():
                msg = "The list of types may not match for the two configurations in each interaction.\nThe failing interaction is number %i."%(i)
                raise Error(msg)

        # Done with initial checking of input.
        return interactions

    def _backend(self, possible_types):
        """
        Query for the interactions backend object.

        :param possible_types: A dict with the global mapping of type strings
                               to integers.

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

                # Construct and store the C++ process.
                cpp_processes.push_back(Backend.Process(cpp_config1,
                                                        cpp_config2,
                                                        barrier))

            # Construct the C++ interactions object.
            self.__backend = Backend.Interactions(cpp_processes)

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

            interactions_string += indent + "(%s, %s, %15.6e)"%(conf1_name, conf2_name, interaction[2])

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
        kmc_interactions_string = variable_name + " = KMCInteractions(interactions_list=interactions_list)\n"

        # Return the script.
        return configuration_script + comment_string + interactions_string + "\n" + \
            kmc_interactions_string


