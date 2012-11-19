"""" Module for testing KMCInteractions """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest


from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.Exceptions.Error import Error

# Import the module to test.
from KMCLib.KMCInteractions import KMCInteractions

# Implementing the tests.
class KMCInteractionsTest(unittest.TestCase):
    """ Class for testing the KMCInteractions class """

    def testConstruction(self):
        """ Test that the KMCInteractions class can be constructed. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(cartesian_coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(cartesian_coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(cartesian_coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(cartesian_coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions = [interaction_0, interaction_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(interactions=interactions)

        # Check the raw interactions stored on the object.
        stored_interactions = kmc_interactions._KMCInteractions__raw_interactions

        # Checks that the address is the same.
        self.assertTrue(stored_interactions == interactions)

    def testConstructionFailNoList(self):
        """ Test that the construction fails if the interactions list is not a list. """
        self.assertRaises(Error, lambda: KMCInteractions(interactions=KMCLocalConfiguration))

    def testConstructionFailNoTuple(self):
        """ Test that the construction fails if the elements in the interactions list are not lists or tuples of the correct length. """
        # NEEDS IMPLEMENTATION
        pass

    def testConstructionFailWrongType(self):
        """ Test that the construction fails if the configurations are of wrong type. """
        # NEEDS IMPLEMENTATION
        pass

    def testConstructionFailWrongRateType(self):
        """ Test that the construction fails if the rate is of wrong type. """
        # NEEDS IMPLEMENTATION
        pass

    def testConstructionFailNotSameCoords(self):
        """ Test that the construction fails unless the coordinates are equal. """
        # NEEDS IMPLEMENTATION
        pass

    def testConstructionFailSameTypes(self):
        """ Test that the construction fails if the types are identical. """
        # NEEDS IMPLEMENTATION
        pass


if __name__ == '__main__':
    unittest.main()

