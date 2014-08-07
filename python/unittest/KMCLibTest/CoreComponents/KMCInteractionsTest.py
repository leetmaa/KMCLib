"""" Module for testing KMCInteractions """


# Copyright (c)  2012-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy
import sys
import os

from KMCLib.CoreComponents.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.CoreComponents.KMCProcess import KMCProcess
from KMCLib.CoreComponents.KMCBucketProcess import KMCBucketProcess
from KMCLib.PluginInterfaces.KMCRateCalculatorPlugin import KMCRateCalculatorPlugin
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend

# Import the module to test.
from KMCLib.CoreComponents.KMCInteractions import KMCInteractions

# Import the test helpers.
sys.path.insert(0, os.path.join(os.path.abspath(os.path.dirname(__file__)), ".."))
from TestUtilities.Plugins.CustomRateCalculator.CustomRateCalculator import CustomRateCalculator

# Implementing the tests.
class KMCInteractionsTest(unittest.TestCase):
    """ Class for testing the KMCInteractions class """

    def testConstruction(self):
        """ Test that the KMCInteractions class can be constructed. """
        # A first process.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        types0 = ["A","*","B"]
        types1 = ["B","*","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)

        # A second process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","C"]
        types1 = ["C","A"]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)

        processes = [process_0, process_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(processes=processes)

        # Check the default implicit wildcard.
        self.assertTrue( kmc_interactions.implicitWildcards() )

        # Construct again with non default value.
        kmc_interactions = KMCInteractions(processes=processes,
                                           implicit_wildcards=False)

        # Check the wildcard again.
        self.assertFalse( kmc_interactions.implicitWildcards() )

        # Check the processes stored on the object.
        stored_processes = kmc_interactions._KMCInteractions__processes

        # Checks that the address is the same.
        self.assertTrue(stored_processes == processes)

    def testConstructionBuckets(self):
        """ Test that the KMCInteractions class can be constructed. """
        # A first process.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        minimum_match = ["A","*","B"]
        update = [[(-1,"A"), (1, "B")],"*",[(1,"A"), (-1, "B")]]
        rate_0_1 = 3.5
        process_0 = KMCBucketProcess(coordinates=coords,
                                     minimum_match=minimum_match,
                                     update=update,
                                     basis_sites=[0,1,3],
                                     rate_constant=rate_0_1)

        # A second process.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        minimum_match = ["A","*","C"]
        update = [[(-1,"A"), (1, "C")],"*",[(1,"A"), (-1, "C")]]
        rate_0_1 = 1.5
        process_1 = KMCBucketProcess(coordinates=coords,
                                     minimum_match=minimum_match,
                                     update=update,
                                     basis_sites=[0,1,3],
                                     rate_constant=rate_0_1)

        processes = [process_0, process_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(processes=processes)

        # Check the default implicit wildcard.
        self.assertTrue( kmc_interactions.implicitWildcards() )

        # Construct again with non default value.
        kmc_interactions = KMCInteractions(processes=processes,
                                           implicit_wildcards=False)

        # Check the wildcard again.
        self.assertFalse( kmc_interactions.implicitWildcards() )

        # Check the processes stored on the object.
        stored_processes = kmc_interactions._KMCInteractions__processes

        # Checks that the address is the same.
        self.assertTrue(stored_processes == processes)

    def testConstructionWithCustomRates(self):
        """ Test construction with custom rates. """
        # A first process.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        types0 = ["A","*","B"]
        types1 = ["B","*","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)

        # A second process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","C"]
        types1 = ["C","A"]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)

        processes = [process_0, process_1]

        # Test that it fails with the wrong rate calculator.
        interactions = KMCInteractions(processes=processes,
                                       implicit_wildcards=True)

        # Test that it fails if the rate calculator is of wrong type.
        self.assertRaises( Error, lambda : interactions.setRateCalculator(rate_calculator="CustomRateCalculator") )

        # Test that it fails if the rate calculator is instantiated.
        self.assertRaises( Error, lambda : interactions.setRateCalculator(rate_calculator=CustomRateCalculator("DummyConfig")) )

        # Construct the interactions object with a custom rate calculator class.
        kmc_interactions = KMCInteractions(processes=processes,
                                           implicit_wildcards=True)
        kmc_interactions.setRateCalculator(rate_calculator=CustomRateCalculator)

        # Test the stored name.
        self.assertEqual(kmc_interactions._KMCInteractions__rate_calculator_str, "CustomRateCalculator")

        # Test the stored rate calculator class.
        self.assertTrue(kmc_interactions._KMCInteractions__rate_calculator_class == CustomRateCalculator)

    def testConstructionFailNoList(self):
        """ Test that the construction fails if the interactions list is not a list. """
        # Setup.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        types0 = ["A","*","B"]
        types1 = ["B","*","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)
        # Construct and fail.
        self.assertRaises(Error, lambda: KMCInteractions(processes=process_0))

    def testConstructionFailWrongList(self):
        """ Test that the construction fails if the interactions list wrong. """
        # Setup.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        types0 = ["A","*","B"]
        types1 = ["B","*","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)
        # Construct and fail.
        self.assertRaises(Error, lambda: KMCInteractions(processes=[process_0, coords]))

    def testConstructionFailsWrongWildcard(self):
        """ Test that we fail to construct with other than bool input for the implicit wildcard flag """
        # A first process.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        types0 = ["A","*","B"]
        types1 = ["B","*","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)

        # A second process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","C"]
        types1 = ["C","A"]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)

        processes = [process_0, process_1]

        # Construct and fail.
        self.assertRaises(Error, lambda: KMCInteractions(processes=processes,
                                                         implicit_wildcards=0) )

        self.assertRaises(Error, lambda: KMCInteractions(processes=processes,
                                                         implicit_wildcards="True") )

        self.assertRaises(Error, lambda: KMCInteractions(processes=processes,
                                                         implicit_wildcards=[False]) )

    def testBackend(self):
        """
        Test that the generated backend object is what we expect.
        """
        # A first process.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        types0 = ["A","D","B"]
        types1 = ["B","F","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)

        # A second process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","C"]
        types1 = ["C","A"]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords, types0, types1, basis_sites=[0,1,3], rate_constant=rate_0_1)

        processes = [process_0, process_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(processes=processes)

        # Setup a dict with the possible types.
        possible_types = {
            "A" : 13,
            "B" : 3,
            "J" : 4,
            "C" : 5,
            }


        # Use a dummy argument for the configuration.
        config = "DummyConfig"

        # Check that setting up the backend fails if we have types in the processes that do
        # not corresponds to a type in the list of possible types.
        self.assertRaises( Error, lambda : kmc_interactions._backend(possible_types, 2, config) )

        possible_types = {
            "A" : 13,
            "B" : 3,
            "D" : 2,
            "J" : 4,
            "C" : 5,
            }

        self.assertRaises( Error, lambda : kmc_interactions._backend(possible_types, 2, config) )

        possible_types = {
            "A" : 13,
            "B" : 3,
            "F" : 2,
            "J" : 4,
            "C" : 5,
            }

        self.assertRaises( Error, lambda : kmc_interactions._backend(possible_types, 2, config) )

        # Both "D" and "F" must be present.
        possible_types["D"] = 123

        # Get the backend.
        cpp_interactions = kmc_interactions._backend(possible_types, 2, config)

        # Check the type.
        self.assertTrue( isinstance(cpp_interactions, Backend.Interactions) )

        # Get the processes out.
        cpp_processes = cpp_interactions.processes()

        # Check the length of the processes.
        self.assertEqual(cpp_processes.size(), 2)

        # Get the elements out of the second process.
        match_types  = cpp_processes[1].processMatchList()[0].match_types
        update_types = cpp_processes[1].processMatchList()[0].update_types

        # Match type should be "A" -> 15 and update type "C" -> 5
        self.assertEqual( match_types[13], 1)
        self.assertEqual( update_types[5], 1)

        # Get the elements out of the second process.
        match_types  = cpp_processes[1].processMatchList()[1].match_types
        update_types = cpp_processes[1].processMatchList()[1].update_types

        # Match type should be "C" -> 5 and update type "A" -> 13
        self.assertEqual( match_types[5],   1)
        self.assertEqual( update_types[13], 1)

    def testBackendBuckets(self):
        """
        Test that the backend behaves as expected with bucket processes.
        """
        # A first process.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        minimum_match = ["A","*","B"]
        update = [[(-1,"A"), (1, "B")],[(0,"*")],[(1,"A"), (-1, "B")]]
        rate_0_1 = 3.5
        process_0 = KMCBucketProcess(coordinates=coords,
                                     minimum_match=minimum_match,
                                     update=update,
                                     basis_sites=[0,1,3],
                                     rate_constant=rate_0_1)

        # A second process.
        coords = [[1.0,2.0,3.4],[12.0,13.0,-1.0],[1.1,1.2,1.3]]
        minimum_match = ["A","*","C"]
        update = [[(-1,"A"), (1, "C")],[(0,"*")],[(1,"A"), (-1, "C")]]
        rate_0_1 = 1.5
        process_1 = KMCBucketProcess(coordinates=coords,
                                     minimum_match=minimum_match,
                                     update=update,
                                     basis_sites=[0,1,3],
                                     rate_constant=rate_0_1)

        processes = [process_0, process_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(processes=processes)

        # Setup a dict with the possible types.
        possible_types = {
            "A" : 0,
            "B" : 1,
            "J" : 2,
            "C" : 3,
            }

        # Use a dummy argument for the configuration.
        config = "DummyConfig"

        # Check that setting up the backend fails if we have types in the processes that do
        # not corresponds to a type in the list of possible types.
        self.assertRaises( Error, lambda : kmc_interactions._backend(possible_types, 2, config) )

        possible_types = {
            "A" : 0,
            "B" : 1,
            "D" : 2,
            "J" : 3,
            "C" : 4,
            }

        self.assertRaises( Error, lambda : kmc_interactions._backend(possible_types, 2, config) )

        possible_types = {
            "A" : 0,
            "B" : 1,
            "F" : 2,
            "J" : 3,
            "C" : 4,
            }

        self.assertRaises( Error, lambda : kmc_interactions._backend(possible_types, 2, config) )

        # Both "D" and "F" must be present, and the widcard "*".
        possible_types["D"] = 5
        possible_types["*"] = 6

        # Get the backend.
        cpp_interactions = kmc_interactions._backend(possible_types, 2, config)

        # Check the type.
        self.assertTrue( isinstance(cpp_interactions, Backend.Interactions) )

        # Get the processes out.
        cpp_processes = cpp_interactions.processes()

        # Check the length of the processes.
        self.assertEqual(cpp_processes.size(), 2)

        # Get the elements out of the second process.
        update_types = cpp_processes[1].processMatchList()[0].update_types

        # This is an "A"
        match_types  = cpp_processes[1].processMatchList()[0].match_types
        self.assertEqual(match_types[0], 1)
        self.assertEqual(match_types[1], 0)
        self.assertEqual(match_types[2], 0)
        self.assertEqual(match_types[3], 0)
        self.assertEqual(match_types[4], 0)
        self.assertEqual(match_types[5], 0)
        self.assertEqual(match_types[6], 0)

        # This is an "C"
        match_types  = cpp_processes[1].processMatchList()[1].match_types
        self.assertEqual(match_types[0], 0)
        self.assertEqual(match_types[1], 0)
        self.assertEqual(match_types[2], 0)
        self.assertEqual(match_types[3], 0)
        self.assertEqual(match_types[4], 1)
        self.assertEqual(match_types[5], 0)
        self.assertEqual(match_types[6], 0)

        # This is a "*"
        match_types  = cpp_processes[1].processMatchList()[2].match_types
        self.assertEqual(match_types[0], 0)
        self.assertEqual(match_types[1], 0)
        self.assertEqual(match_types[2], 0)
        self.assertEqual(match_types[3], 0)
        self.assertEqual(match_types[4], 0)
        self.assertEqual(match_types[5], 0)
        self.assertEqual(match_types[6], 1)

        # Check the update types.
        # NEEDS IMPLEMENTATION

    def testBackendWithCustomRates(self):
        """ Test that we can construct the backend object with custom rates. """
        # A first process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","B"]
        types1 = ["B","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords, types0, types1, basis_sites=[0], rate_constant=rate_0_1)

        # A second process.
        types0 = ["A","C"]
        types1 = ["C","A"]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords, types0, types1, basis_sites=[0], rate_constant=rate_0_1)

        processes = [process_0, process_1]

        # Set the custom rates class to use.
        custom_rates_class = CustomRateCalculator

        # Set the rate function on the custom rates calculator for testing.
        ref_rnd = numpy.random.uniform(0.0,1.0)
        def testRateFunction(obj, coords, types_before, types_after, rate_const, process_number, global_coordinate):
            return ref_rnd

        # Store the original.
        custom_rate_function = custom_rates_class.rate
        try:
            custom_rates_class.rate = testRateFunction

            # Construct the interactions object.
            kmc_interactions = KMCInteractions(processes=processes,
                                               implicit_wildcards=False)

            # Setup a dict with the possible types.
            possible_types = {
                "A" : 13,
                "D" : 2,
                "B" : 3,
                "J" : 4,
                "C" : 5,
                }

            # Use a dummy argument for the configuration.
            config = "DummyConfig"

            # Test that it fails if the rate calculator is of wrong class.
            kmc_interactions.setRateCalculator(rate_calculator=Error)
            self.assertRaises( Error, lambda : kmc_interactions._backend(possible_types, 2, config) )

            # Test that it fails if the rate calculator is the base class.
            kmc_interactions.setRateCalculator(rate_calculator=KMCRateCalculatorPlugin)
            self.assertRaises( Error, lambda : kmc_interactions._backend(possible_types, 2, config) )

            # But this should work.
            kmc_interactions.setRateCalculator(rate_calculator=custom_rates_class)

            # Construct the backend.
            cpp_interactions = kmc_interactions._backend(possible_types, 2, config)

            # Test that the configuration on the custom rate class is the one given.
            self.assertTrue(kmc_interactions._KMCInteractions__rate_calculator.configuration == config) #  <-  Check by reference.

            # Get the rate calculator reference out of the C++ object and
            # check that a call from C++ calls the Python extension.
            cpp_coords = Backend.StdVectorDouble()
            cpp_types1 = Backend.StdVectorString()
            cpp_types2 = Backend.StdVectorString()
            rate_constant = 543.2211
            process_number = 33
            coordinate = (1.2,3.4,5.6)

            self.assertAlmostEqual( cpp_interactions.rateCalculator().backendRateCallback(cpp_coords,
                                                                                          cpp_coords.size()/3,
                                                                                          cpp_types1,
                                                                                          cpp_types2,
                                                                                          rate_constant,
                                                                                          process_number,
                                                                                          coordinate[0],
                                                                                          coordinate[1],
                                                                                          coordinate[2]), ref_rnd, 12 )
            self.assertAlmostEqual( kmc_interactions._KMCInteractions__rate_calculator.backendRateCallback(cpp_coords,
                                                                                                           cpp_coords.size()/3,
                                                                                                           cpp_types1,
                                                                                                           cpp_types2,
                                                                                                           rate_constant,
                                                                                                           process_number,
                                                                                                           coordinate[0],
                                                                                                           coordinate[1],
                                                                                                           coordinate[2]), ref_rnd, 12 )


        finally:
            # Reset the class.
            custom_rates_class.rate = custom_rate_function





        # Construct a C++ RateCalculator object directly and check that this object
        # returns the rate given to it.
        cpp_rate_calculator = Backend.RateCalculator()
        self.assertAlmostEqual( cpp_rate_calculator.backendRateCallback(cpp_coords,
                                                                        cpp_coords.size()/3,
                                                                        cpp_types1,
                                                                        cpp_types2,
                                                                        rate_constant,
                                                                        process_number,
                                                                        coordinate[0],
                                                                        coordinate[1],
                                                                        coordinate[2]), rate_constant, 12 )

    def testBackendNoFailWrongBasisMatch(self):
        """ Test for no failure when constructing backend with wrong n_basis """
        # A first process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","B"]
        types1 = ["B","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords, types0, types1, basis_sites=[0,4], rate_constant=rate_0_1)

        # A second process.
        types0 = ["A","C"]
        types1 = ["C","A"]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords, types0, types1, basis_sites=[0,1], rate_constant=rate_0_1)

        processes = [process_0, process_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(processes=processes)

        # Setup a dict with the possible types.
        possible_types = {
            "A" : 13,
            "D" : 2,
            "B" : 3,
            "J" : 4,
            "C" : 5,
            }

        # Use a dummy argument for the configuration.
        config = "DummyConfig"

        # Get the backend - The [0,4] sites list for process_0 is simply ignored.
        cpp_interactions = kmc_interactions._backend(possible_types, 2, config)

        self.assertEqual( len(cpp_interactions.processes()[0].basisSites()), 1 )
        self.assertEqual( cpp_interactions.processes()[0].basisSites()[0], 0 )

        self.assertEqual( len(cpp_interactions.processes()[1].basisSites()), 2 )
        self.assertEqual( cpp_interactions.processes()[1].basisSites()[0], 0 )
        self.assertEqual( cpp_interactions.processes()[1].basisSites()[1], 1 )

    def testScript(self):
        """ Test that the KMCInteractions can generate its own script. """
        # A first process.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","B"]
        types1 = ["B","A"]
        rate_0_1 = 3.5
        process_0 = KMCProcess(coords, types0, types1, basis_sites=[0], rate_constant=rate_0_1)

        # A second process.
        types0 = ["A","C"]
        types1 = ["C","A"]
        rate_0_1 = 1.5
        process_1 = KMCProcess(coords, types0, types1, basis_sites=[0], rate_constant=rate_0_1)

        processes = [process_0, process_1]

        # Construct the interactions object.
        kmc_interactions = KMCInteractions(processes=processes)

        script = kmc_interactions._script()

        ref_script = """
# -----------------------------------------------------------------------------
# Interactions

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

elements_before = ['A','B']
elements_after  = ['B','A']
move_vectors    = [(  0,[   1.000000e-01,  -8.000000e-01,  -2.100000e+00]),
                   (  1,[  -1.000000e-01,   8.000000e-01,   2.100000e+00])]
basis_sites     = [0]
rate_constant   =    3.500000e+00

process_0 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    move_vectors=move_vectors,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

elements_before = ['A','C']
elements_after  = ['C','A']
move_vectors    = [(  0,[   1.000000e-01,  -8.000000e-01,  -2.100000e+00]),
                   (  1,[  -1.000000e-01,   8.000000e-01,   2.100000e+00])]
basis_sites     = [0]
rate_constant   =    1.500000e+00

process_1 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    move_vectors=move_vectors,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

processes = [process_0,
             process_1]

interactions = KMCInteractions(
    processes=processes,
    implicit_wildcards=True)
"""
        self.assertEqual(script, ref_script)

        # Get a script with another name and another number of processes.

        processes = [process_0]
        kmc_interactions = KMCInteractions(processes=processes,
                                           implicit_wildcards=False)

        script = kmc_interactions._script(variable_name="my_kmc_interactions")

        ref_script = """
# -----------------------------------------------------------------------------
# Interactions

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e-01,  -8.000000e-01,  -2.100000e+00]]

elements_before = ['A','B']
elements_after  = ['B','A']
move_vectors    = [(  0,[   1.000000e-01,  -8.000000e-01,  -2.100000e+00]),
                   (  1,[  -1.000000e-01,   8.000000e-01,   2.100000e+00])]
basis_sites     = [0]
rate_constant   =    3.500000e+00

process_0 = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    move_vectors=move_vectors,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

processes = [process_0]

my_kmc_interactions = KMCInteractions(
    processes=processes,
    implicit_wildcards=False)
"""
        self.assertEqual(script, ref_script)


if __name__ == '__main__':
    unittest.main()

