"""" Module for testing the KMCRateCalculatorPlugin """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy


# Import the module to test.
from KMCLib.KMCRateCalculatorPlugin import KMCRateCalculatorPlugin
from KMCLib.Backend import Backend

# Implementing the tests.
class KMCRateCalculatorPluginTest(unittest.TestCase):
    """ Class for testing the KMCRateCalculatorPlugin class """

    def testConstructionBaseClass(self):
        """ Test that we can construct the base class. """
        calculator = KMCRateCalculatorPlugin()
        self.assertTrue( isinstance(calculator, KMCRateCalculatorPlugin) )
        self.assertTrue( isinstance(calculator, Backend.RateCalculator) )

    def testConstructionDerrivedClass(self):
        """ Test that we can construct a derrived class. """
        # Define a derrived class.
        class RateCalc(KMCRateCalculatorPlugin):
            # Overload the initialize function.
            def initialize(self):
                # Save something on the class here.
                self._variable = 123
        # Construct.
        calculator = RateCalc()

        # Check the type.
        self.assertTrue( isinstance(calculator, RateCalc) )
        self.assertTrue( isinstance(calculator, KMCRateCalculatorPlugin) )
        self.assertTrue( isinstance(calculator, Backend.RateCalculator) )

        # Check that the initialize function has been called on construction.
        self.assertTrue( hasattr(calculator, "_variable") )
        self.assertEqual( calculator._variable, 123 )

    def testCutoff(self):
        """ Test that the base class has the cutoff function. """
        rc = KMCRateCalculatorPlugin()
        self.assertTrue(hasattr(rc, "cutoff"))
        self.assertTrue(rc.cutoff() is None)

    def testUsage(self):
        """ Test that the KMCRateCalculatorPlugin can be used in a simulation. """
        # To get the random numbers and process numbers returned.
        ref_randoms = []
        ref_process_numbers = []
        # Define a derrived class.
        class RateCalc(KMCRateCalculatorPlugin):
            # Overload the initialize function.
            def initialize(self):
                # Save something on the class here.
                self._times_called = 0
            # Overload the rate function.
            def rate(self, coords, types_befpre, types_after, rate_constant, process_number):
                # Do some simple counting and return the random number.
                self._times_called += 1
                rnd = numpy.random.uniform(0.0,1.0)
                ref_randoms.append(rnd)
                ref_process_numbers.append(process_number)
                return rnd
            # Overload the additive rate function.
            def useAdditiveRate(self):
                return False

        # Construct.
        calculator = RateCalc()

        # Send it to C++ to get the rate out, call it 4 times.
        cpp_coords = Backend.StdVectorCoordinate()
        cpp_coords.push_back(Backend.Coordinate(1.0,2.9,3.4))
        cpp_types1 = Backend.StdVectorString()
        cpp_types1.push_back("A")
        cpp_types2 = Backend.StdVectorString()
        cpp_types2.push_back("B")
        rate_constant = 3.1415927
        ret_randoms = []
        ret_randoms.append(Backend.getRate(calculator,
                                           cpp_coords,
                                           cpp_types1,
                                           cpp_types2,
                                           rate_constant,
                                           21))
        ret_randoms.append(Backend.getRate(calculator,
                                           cpp_coords,
                                           cpp_types1,
                                           cpp_types2,
                                           rate_constant,
                                           12))
        ret_randoms.append(Backend.getRate(calculator,
                                           cpp_coords,
                                           cpp_types1,
                                           cpp_types2,
                                           rate_constant,
                                           10))
        ret_randoms.append(Backend.getRate(calculator,
                                           cpp_coords,
                                           cpp_types1,
                                           cpp_types2,
                                           rate_constant,
                                           2))

        # Check that it was called 4 times.
        self.assertEqual( calculator._times_called, 4 )

        # Check the values.
        self.assertAlmostEqual( ret_randoms, ref_randoms, 12 )

        # Check the reference process numbers.
        self.assertEqual( ref_process_numbers[0], 21 )
        self.assertEqual( ref_process_numbers[1], 12 )
        self.assertEqual( ref_process_numbers[2], 10 )
        self.assertEqual( ref_process_numbers[3], 2  )

if __name__ == '__main__':
    unittest.main()

