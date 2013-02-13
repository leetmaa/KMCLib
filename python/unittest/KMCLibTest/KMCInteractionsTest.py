"""" Module for testing KMCInteractions """


# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.KMCRateCalculatorPlugin import KMCRateCalculatorPlugin
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend

# Import the module to test.
from KMCLib.KMCInteractions import KMCInteractions

# Import the test helpers.
from TestUtilities.Plugins.CustomRateCalculator.CustomRateCalculator import CustomRateCalculator

# Implementing the tests.
class KMCInteractionsTest(unittest.TestCase):
    """ Class for testing the KMCInteractions class """

    def testConstruction(self):
        """ Test that the KMCInteractions class can be constructed. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        types = ["A","*","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","*","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1, [0,1,3])

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(interactions_list=interactions_list)

        # Check the default implicit wildcard.
        self.assertTrue( kmc_interactions.implicitWildcards() )

        # Construct again with non default value.
        kmc_interactions = KMCInteractions(interactions_list=interactions_list,
                                           implicit_wildcards=False)

        # Check the wildcard again.
        self.assertFalse( kmc_interactions.implicitWildcards() )

        # Check the raw interactions stored on the object.
        stored_interactions = kmc_interactions._KMCInteractions__raw_interactions

        # Checks that the address is the same.
        self.assertTrue(stored_interactions == interactions_list)

    def testConstructionWithCustomRates(self):
        """ Test construction with custom rates. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        types = ["A","*","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","*","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1, [0,1,3])

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Test that it fails with the wrong rate calculator.
        interactions = KMCInteractions(interactions_list=interactions_list,
                                       implicit_wildcards=True)

        # Test that it fails if the rate calculator is of wrong type.
        self.assertRaises( Error, lambda : interactions.setRateCalculator(rate_calculator="CustomRateCalculator") )

        # Test that it fails if the rate calculator is of wrong class.
        self.assertRaises( Error, lambda : interactions.setRateCalculator(rate_calculator=Error) )

        # Test that it fails if the rate calculator is instantiated.
        self.assertRaises( Error, lambda : interactions.setRateCalculator(rate_calculator=CustomRateCalculator()) )

        # Test that it fails if the rate calculator is the base class.
        self.assertRaises( Error, lambda : interactions.setRateCalculator(rate_calculator=KMCRateCalculatorPlugin) )

        # Construct the interactions object with a custom rate calculator class.
        kmc_interactions = KMCInteractions(interactions_list=interactions_list,
                                           implicit_wildcards=True)
        kmc_interactions.setRateCalculator(rate_calculator=CustomRateCalculator)

        # Test the stored name.
        self.assertEqual(kmc_interactions._KMCInteractions__rate_calculator_str, "CustomRateCalculator")

        # Test the stored rate calculator.
        self.assertTrue(isinstance(kmc_interactions._KMCInteractions__rate_calculator, CustomRateCalculator) )

    def testConstructionFailNoList(self):
        """ Test that the construction fails if the interactions list is not a list. """
        self.assertRaises(Error, lambda: KMCInteractions(interactions_list=KMCLocalConfiguration))

    def testConstructionFailNoTuple(self):
        """ Test that the construction fails if the elements in the interactions list are not lists or tuples of the correct length. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1, rate_0_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Fails because interaction_1 is of wrong length.
        self.assertRaises( Error, lambda: KMCInteractions(interactions_list=interactions_list) )

        interaction_0 = local_config_0
        interaction_1 = local_config_1
        interactions_list = [interaction_0, interaction_1]

        # Fails because the interactions are not lists or tuples.
        self.assertRaises( Error, lambda: KMCInteractions(interactions_list=interactions_list) )

    def testConstructionFailWildcardMove(self):
        """ Test for failure if wildcards change place in the move. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["*","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","*"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Fails because the wildcard moves from position 0 to 1 in the move.
        self.assertRaises( Error, lambda: KMCInteractions(interactions_list=interactions_list) )

    def testConstructionFailWrongType(self):
        """ Test that the construction fails if the configurations are of wrong type. """
        interaction_0 = (Error("dummy"),1.0, 123.0)
        interactions_list = [interaction_0, interaction_0]

        # Check that it fails.
        self.assertRaises( Error, lambda: KMCInteractions(interactions_list=interactions_list) )

    def testConstructionFailWrongRateType(self):
        """ Test that the construction fails if the rate is of wrong type. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        # Integer rate to fail.
        rate_0_1 = 1
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Construct the interactions object - fails because of the integer rate.
        self.assertRaises( Error, lambda: KMCInteractions(interactions_list=interactions_list) )

    def testConstructionFailNotSameCoords(self):
        """ Test that the construction fails unless the coordinates are equal. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction - not the same coords, to fail.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.2]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.3
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Fails because of different coordinates.
        self.assertRaises( Error, lambda: KMCInteractions(interactions_list=interactions_list) )

    def testConstructionFailSameTypes(self):
        """ Test that the construction fails if the types are identical. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["A","C"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Fails because of same types.
        self.assertRaises( Error, lambda: KMCInteractions(interactions_list=interactions_list) )

    def testConstructionFailWrongSitesList(self):
        """ Test that the construction fails if there is a problem with the sites list. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1, [0,1])

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1, [])  # <- with empty list.

        interactions_list = [interaction_0, interaction_1]

        # Fails because of the empty sites list in interaction_1
        self.assertRaises( Error, lambda: KMCInteractions(interactions_list=interactions_list) )

    def testConstructionFailsWrongWildcard(self):
        """ Test that we fail to construct with other than bool input for the implicit wildcard flag """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Construct and fail.
        self.assertRaises(Error, lambda: KMCInteractions(interactions_list=interactions_list,
                                                         implicit_wildcards=0) )

        self.assertRaises(Error, lambda: KMCInteractions(interactions_list=interactions_list,
                                                         implicit_wildcards="True") )

        self.assertRaises(Error, lambda: KMCInteractions(interactions_list=interactions_list,
                                                         implicit_wildcards=[False]) )

    def testBackend(self):
        """
        Test that the generated backend object is what we expect.
        """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(interactions_list=interactions_list)

        # Setup a dict with the possible types.
        possible_types = {
            "A" : 13,
            "D" : 2,
            "B" : 3,
            "J" : 4,
            "C" : 5,
            }

        # Get the backend.
        cpp_interactions = kmc_interactions._backend(possible_types, 2)

        # Check the type.
        self.assertTrue( isinstance(cpp_interactions, Backend.Interactions) )

        # Get the processes out.
        cpp_processes = cpp_interactions.processes()

        # Check the length of the processes.
        self.assertEqual(cpp_processes.size(), 2)

        # Get the elements out of the second process.
        match_type  = cpp_processes[1].minimalMatchList()[0].match_type
        update_type = cpp_processes[1].minimalMatchList()[0].update_type

        # Match type should be "A" -> 15 and update type "C" -> 5
        self.assertEqual( match_type, 13)
        self.assertEqual( update_type, 5)

        # Get the elements out of the second process.
        match_type  = cpp_processes[1].minimalMatchList()[1].match_type
        update_type = cpp_processes[1].minimalMatchList()[1].update_type

        # Match type should be "C" -> 5 and update type "A" -> 13
        self.assertEqual( match_type,   5)
        self.assertEqual( update_type, 13)

    def testBackendWithCustomRates(self):
        """ Test that we can construct the backend object with custom rates. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1)

        interactions_list = [interaction_0, interaction_1]

        # Set the custom rates class to use.
        custom_rates_class = CustomRateCalculator

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(interactions_list=interactions_list,
                                           implicit_wildcards=False)
        kmc_interactions.setRateCalculator(rate_calculator=custom_rates_class)

        # Set the rate function on the custom rates calculator for testing.
        ref_rnd = numpy.random.uniform(0.0,1.0)
        def testRateFunction(coords, types_before, types_after, rate_const):
            return ref_rnd
        kmc_interactions._KMCInteractions__rate_calculator.rate = testRateFunction

        # Setup a dict with the possible types.
        possible_types = {
            "A" : 13,
            "D" : 2,
            "B" : 3,
            "J" : 4,
            "C" : 5,
            }

        # Construct the backend.
        cpp_interactions = kmc_interactions._backend(possible_types, 2)

        # Get the rate calculator reference out of the C++ object and
        # check that a call from C++ calls the Python extension.
        cpp_coords = Backend.StdVectorDouble()
        cpp_types1 = Backend.StdVectorString()
        cpp_types2 = Backend.StdVectorString()
        rate_constant = 543.2211

        self.assertAlmostEqual( cpp_interactions.rateCalculator().backendRateCallback(cpp_coords,
                                                                                      cpp_coords.size()/3,
                                                                                      cpp_types1,
                                                                                      cpp_types2,
                                                                                      rate_constant), ref_rnd, 12 )
        self.assertAlmostEqual( kmc_interactions._KMCInteractions__rate_calculator.backendRateCallback(cpp_coords,
                                                                                                       cpp_coords.size()/3,
                                                                                                       cpp_types1,
                                                                                                       cpp_types2,
                                                                                                       rate_constant), ref_rnd, 12 )

        # Construct a C++ RateCalculator object directly and check that this object
        # returns the rate given to it.
        cpp_rate_calculator = Backend.RateCalculator()
        self.assertAlmostEqual( cpp_rate_calculator.backendRateCallback(cpp_coords,
                                                                        cpp_coords.size()/3,
                                                                        cpp_types1,
                                                                        cpp_types2,
                                                                        rate_constant), rate_constant, 12 )


    def testBackendNoFailWrongBasisMatch(self):
        """ Test for no failure when constructing backend with wrong n_basis """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1, [0,4])

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1, [0,1])

        interactions_list = [interaction_0, interaction_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(interactions_list=interactions_list)

        # Setup a dict with the possible types.
        possible_types = {
            "A" : 13,
            "D" : 2,
            "B" : 3,
            "J" : 4,
            "C" : 5,
            }

        # Get the backend - The [0,4] sites list for interaction_0 is simply ignored.
        cpp_interactions = kmc_interactions._backend(possible_types, 2)

        self.assertEqual( len(cpp_interactions.processes()[0].basisSites()), 1 )
        self.assertEqual( cpp_interactions.processes()[0].basisSites()[0], 0 )

        self.assertEqual( len(cpp_interactions.processes()[1].basisSites()), 2 )
        self.assertEqual( cpp_interactions.processes()[1].basisSites()[0], 0 )
        self.assertEqual( cpp_interactions.processes()[1].basisSites()[1], 1 )

    def testScript(self):
        """ Test that the KMCInteractions can generate its own script. """
        # A first interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","B"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["B","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 3.5
        interaction_0 = (local_config_0, local_config_1, rate_0_1)

        # A second interaction.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types = ["A","C"]
        local_config_0 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        types = ["C","A"]
        local_config_1 = KMCLocalConfiguration(coordinates=coords,
                                               types=types,
                                               center=0)
        rate_0_1 = 1.5
        interaction_1 = (local_config_0, local_config_1, rate_0_1, [0,1,2,3,8])

        interactions_list = [interaction_0, interaction_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(interactions_list=interactions_list)

        script = kmc_interactions._script()

        ref_script = """
# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['A', 'B']

conf1_0 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['B', 'A']

conf2_0 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['A', 'C']

conf1_1 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['C', 'A']

conf2_1 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Interactions

interactions_list = [(conf1_0, conf2_0,    3.500000e+00),
                     (conf1_1, conf2_1,    1.500000e+00,  [0,1,2,3,8])]

interactions = KMCInteractions(
    interactions_list=interactions_list,
    implicit_wildcards=True)
"""
        self.assertEqual(script, ref_script)

        # Get a script with another name and another number of interactions.

        interactions_list = [interaction_0]
        kmc_interactions = KMCInteractions(interactions_list=interactions_list,
                                           implicit_wildcards=False)

        script = kmc_interactions._script(variable_name="my_kmc_interactions")

        ref_script = """
# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['A', 'B']

conf1_0 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Local configuration

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

types = ['B', 'A']

conf2_0 = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)

# -----------------------------------------------------------------------------
# Interactions

interactions_list = [(conf1_0, conf2_0,    3.500000e+00)]

my_kmc_interactions = KMCInteractions(
    interactions_list=interactions_list,
    implicit_wildcards=False)
"""
        self.assertEqual(script, ref_script)


if __name__ == '__main__':
    unittest.main()

