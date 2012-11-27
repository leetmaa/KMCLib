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


class KMCInteractions:
    """
    Class for holding information about all possible interactions used in a
    Lattice KMC simulation.
    """

    def __init__(self,
                 interactions=None):
        """
        Constructor for the KMCInteractions.

        :param interactions: The interactions, given as a list of lists or tuples with
                             two local configurations and a rate constant.
        :type interactions: [(KMCLocalConfiguration, KMCLocalConfiguration, float), ...]
        """
        # Check the interactions inpuy.
        self.__raw_interactions = self.__checkInteractionsInput(interactions)

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

    def _backend(self):
        """ Query for the interactions backend object. """
        if self.__backend is None:
            # NEEDS IMPLEMENTATION
            self.__backend = Backend.Interactions()

        # Return the stored backend.
        return self.__backend
