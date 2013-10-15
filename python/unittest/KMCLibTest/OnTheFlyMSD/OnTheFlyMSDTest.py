""" Module for testing the on-the-fly MSD analysis. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import numpy
import StringIO

# Import from the module we test.
from KMCLib.OnTheFlyMSD.OnTheFlyMSD import OnTheFlyMSD
from KMCLib.KMCUnitCell import KMCUnitCell
from KMCLib.KMCLattice import KMCLattice
from KMCLib.KMCConfiguration import KMCConfiguration
from KMCLib.KMCProcess import KMCProcess
from KMCLib.KMCInteractions import KMCInteractions
from KMCLib.KMCLatticeModel import KMCLatticeModel
from KMCLib.KMCControlParameters import KMCControlParameters

# Implement the test.
class OnTheFlyMSDTest(unittest.TestCase):
    """ Class for testing the on-the-fly MSD analysis. """

    def testConstruction(self):
        """ Test the MSD analysis object can be constructed. """
        msd = OnTheFlyMSD(history_steps=13,
                          n_bins=512,
                          t_max = 1024.0,
                          track_type="A")

    def testCalculation(self):
        """ Test a calculation with the on-the-fly MSD analysis. """
        # Setup a system, a periodic 10 atoms long 1D chain.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[1.0,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0]])

        # And a lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(10,10,10),
                             periodic=(True,True,True))

        # Setup an initial configuration with one B in a sea of A:s.
        types = ["A"]*10*10*10
        types[5] = "B"

        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=["A","B"])

        # Setup a diffusion process to the left.
        coordinates_p0 = [[0.0, 0.0, 0.0],[-1.0, 0.0, 0.0]]
        p0 = KMCProcess(coordinates=coordinates_p0,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=1.0)

        coordinates_p1 = [[0.0, 0.0, 0.0],[1.0, 0.0, 0.0]]
        p1 = KMCProcess(coordinates=coordinates_p1,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=1.0)

        coordinates_p2 = [[0.0, 0.0, 0.0],[0.0,-1.0, 0.0]]
        p2 = KMCProcess(coordinates=coordinates_p2,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=1.0)

        coordinates_p3 = [[0.0, 0.0, 0.0],[0.0, 1.0, 0.0]]
        p3 = KMCProcess(coordinates=coordinates_p3,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=1.0)

        coordinates_p4 = [[0.0, 0.0, 0.0],[0.0, 0.0,-1.0]]
        p4 = KMCProcess(coordinates=coordinates_p4,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=1.0)

        coordinates_p5 = [[0.0, 0.0, 0.0],[0.0, 0.0, 1.0]]
        p5 = KMCProcess(coordinates=coordinates_p5,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=1.0)

        interactions = KMCInteractions(processes=[p0, p1, p2, p3, p4, p5],
                                       implicit_wildcards=True)

        model = KMCLatticeModel(configuration=config,
                                interactions=interactions)

        # Setup the analysis.
        msd = OnTheFlyMSD(history_steps=400,
                          n_bins=10,
                          t_max=25.0,
                          track_type="B")

        # Setup the control parameters.
        control_parameters = KMCControlParameters(number_of_steps=4000,
                                                  dump_interval=100,
                                                  analysis_interval=1,
                                                  seed=2013)
        # Run the model.
        model.run(control_parameters=control_parameters,
                  analysis=[msd])

        # Get the results out.
        results      = msd.results()
        time_steps   = msd.timeSteps()
        std_dev      = msd.stdDev()
        bin_counters = msd.binCounters()

        # Compare against references.
        ref_results = numpy.array([[  2.83900934,   8.52281124,  14.09108351,  20.00585490,  25.26096208,
                                     30.09109804,  36.62202664,  43.47434981,  50.79520993,  58.25866865],
                                   [  2.88482657,   8.20673616,  13.39956335,  17.50176483,  22.89894945,
                                     29.46099902,  36.60734674,  44.33478009,  52.14399436,  59.49159482],
                                   [  3.12507672,   8.84431286,  13.83414102,  18.58819987,  24.03051120,
                                     29.05110007,  33.65539622,  40.08318015,  46.38807125,  54.56025293]])
        diff = numpy.linalg.norm(ref_results - results)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_time_steps = numpy.array([  1.25,   3.75,   6.25,   8.75,  11.25,  13.75,  16.25,  18.75,  21.25,  23.75])
        diff = numpy.linalg.norm(ref_time_steps - time_steps)
        self.assertAlmostEqual(diff, 0.0, 8)

        ref_std_dev = numpy.array([[ 0.02767058,  0.13776852,  0.29226253,  0.49229675,  0.7037584 ,  0.92888939,
                                     1.23033191,  1.56870603,  1.95862026,  2.37873604],
                                   [ 0.02811714,  0.13265927,  0.27791975,  0.43067702,  0.63795385,  0.90943871,
                                     1.22983873,  1.59975336,  2.01062825,  2.42907714],
                                   [ 0.03045875,  0.14296549,  0.2869333 ,  0.45741161,  0.66947862,  0.89678544,
                                     1.13066675,  1.44634082,  1.78868473,  2.22772753]])

        diff = numpy.linalg.norm(ref_std_dev - std_dev)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_bin_counters = (60283, 60628, 60461, 59779, 59683, 59178, 58856, 58752, 58162, 57881)
        self.assertEqual(bin_counters, ref_bin_counters)

    def testPrintResults(self):
        """ Test that the results gets printed as expected. """
        # Fill an object with data.
        ref_results = numpy.array([[  2.83900934,   8.52281124,  14.09108351,  20.00585490,  25.26096208,
                                     30.09109804,  36.62202664,  43.47434981,  50.79520993,  58.25866865],
                                   [  2.88482657,   8.20673616,  13.39956335,  17.50176483,  22.89894945,
                                     29.46099902,  36.60734674,  44.33478009,  52.14399436,  59.49159482],
                                   [  3.12507672,   8.84431286,  13.83414102,  18.58819987,  24.03051120,
                                     29.05110007,  33.65539622,  40.08318015,  46.38807125,  54.56025293]])
        ref_time_steps = numpy.array([  1.25,   3.75,   6.25,   8.75,  11.25,  13.75,  16.25,  18.75,  21.25,  23.75])
        ref_std_dev = numpy.array([[ 0.02008562,  0.05432052,  0.08833069,  0.11053417,  0.12584537,  0.14901027,
                                     0.18265464,  0.20891115,  0.24045901,  0.27834904],
                                   [ 0.01911933,  0.04761810,  0.07830592,  0.10414501,  0.14034533,  0.18069990,
                                     0.23416218,  0.28725392,  0.34211121,  0.39464802],
                                   [ 0.02198474,  0.05167655,  0.08130953,  0.10885342,  0.14397243,  0.16894787,
                                     0.19585331,  0.22369909,  0.25162530,  0.29321875]])
        ref_bin_counters = (60283, 60628, 60461, 59779, 59683, 59178, 58856, 58752, 58162, 57881)

        ref_history_bin_counters = [(0, 0, 0, 0, 0, 1, 87, 58752, 58162, 57881)]

        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)
        msd._OnTheFlyMSD__results = ref_results
        msd._OnTheFlyMSD__time_steps = ref_time_steps
        msd._OnTheFlyMSD__std_dev = ref_std_dev
        msd._OnTheFlyMSD__bin_counters = ref_bin_counters
        msd._OnTheFlyMSD__history_bin_counters = ref_history_bin_counters

        # Print the results to a stream.
        stream = StringIO.StringIO()
        msd.printResults(stream)

        # Check against reference.

        ref_value = """      TIME      MSD_a      DSD_b      MSD_c      STD_a      STD_b      STD_c
   1.25000    2.83901    2.88483    3.12508    0.02009    0.01912    0.02198
   3.75000    8.52281    8.20674    8.84431    0.05432    0.04762    0.05168
   6.25000   14.09108   13.39956   13.83414    0.08833    0.07831    0.08131
   8.75000   20.00585   17.50176   18.58820    0.11053    0.10415    0.10885
  11.25000   25.26096   22.89895   24.03051    0.12585    0.14035    0.14397
"""
        self.assertEqual(stream.getvalue(), ref_value)

    def testFinalize(self):
        """ Test finalization. """
        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)

        # Replace the getBackendResults function.
        def dummy():
            pass
        msd._OnTheFlyMSD__getBackendResults = dummy

        # Set the values.
        msd._OnTheFlyMSD__raw_histogram = numpy.array([[  171144.,   173906.,   188389.],
                                                       [  516721.,   497558.,   536213.],
                                                       [  851961.,   810151.,   836426.],
                                                       [ 1195930.,  1046238.,  1111184.],
                                                       [ 1507650.,  1366678.,  1434213.],
                                                       [ 1780731.,  1743443.,  1719186.],
                                                       [ 2155426.,  2154562.,  1980822.],
                                                       [ 2554205.,  2604757.,  2354967.],
                                                       [ 2954351.,  3032799.,  2698023.],
                                                       [ 3372070.,  3443433.,  3158002.]])
        bin_counters = (60283, 60628, 60461, 59779, 59683, 59178, 58856, 58752, 58162, 57881)
        msd._OnTheFlyMSD__bin_counters = bin_counters
        msd._OnTheFlyMSD__n_bins = 10
        msd._OnTheFlyMSD__binsize = 2.5

        history_bin_counters = [(1, 1, 1, 1, 1, 1, 1, 1, 1, 1),
                                (0, 0, 0, 0, 0, 1, 87, 58752, 58162, 57881)]
        msd._OnTheFlyMSD__history_bin_counters = history_bin_counters
        msd._OnTheFlyMSD__n_eff = [1.12, 2.24, 3.36, 4.48, 5.60, 6.72, 7.84, 8.96, 9.0, 10.12]

        # Call the finalize function.
        msd.finalize()

        # Check a few results.
        result_0_3 =  1195930. / 59779.0
        self.assertAlmostEqual(result_0_3, msd._OnTheFlyMSD__results[0][3], 10)

        result_1_7 = 2604757.0 / 58752.0
        self.assertAlmostEqual(result_1_7, msd._OnTheFlyMSD__results[1][7], 10)

        result_2_0 = 188389.0 / 60283.0
        self.assertAlmostEqual(result_2_0, msd._OnTheFlyMSD__results[2][0], 10)

        # Check the corresponding standard deviation.
        n_eff = numpy.zeros(10)
        for i in range(len(n_eff)):
            for b in range(len(history_bin_counters)):
                fraction = float(history_bin_counters[b][i]) / float(bin_counters[i])
                n_eff[i] += fraction * (b+1.0)

        std_0_3 = result_0_3 * numpy.sqrt( ( 2 * n_eff[3] * n_eff[3] + 1 ) / ( 3 * n_eff[3] * 59779.0 ) )
        self.assertAlmostEqual(std_0_3, msd._OnTheFlyMSD__std_dev[0][3], 10)

        std_1_7 = result_1_7 * numpy.sqrt( ( 2 * n_eff[7] * n_eff[7] + 1 ) / ( 3 * n_eff[7] * 58752.0 ) )
        self.assertAlmostEqual(std_1_7, msd._OnTheFlyMSD__std_dev[1][7], 10)

        std_2_0 = result_2_0 * numpy.sqrt( ( 2 * n_eff[0] * n_eff[0] + 1 ) / ( 3 * n_eff[0] * 60283.0 ) )
        self.assertAlmostEqual(std_2_0, msd._OnTheFlyMSD__std_dev[2][0], 10)

    def testResultsQuery(self):
        """ Test the result query function. """
        # Setup a dummy object with data.
        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)
        results = "data"
        msd._OnTheFlyMSD__results = results
        # Check the adress.
        self.assertTrue( msd.results() == results )

    def testTimeStepsQuery(self):
        """ Test the time step query function. """
        # Setup a dummy object with data.
        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)
        time_steps = "data"
        msd._OnTheFlyMSD__time_steps = time_steps
        # Check the adress.
        self.assertTrue( msd.timeSteps() == time_steps )

    def testStdDevQuery(self):
        """ Test the standard deviation query function. """
        # Setup a dummy object with data.
        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)
        std_dev = "data"
        msd._OnTheFlyMSD__std_dev = std_dev
        # Check the adress.
        self.assertTrue( msd.stdDev() == std_dev )

    def testBinCountersQuery(self):
        """ Test the bin counters query function. """
        # Setup a dummy object with data.
        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)
        bin_counters = "data"
        msd._OnTheFlyMSD__bin_counters = bin_counters
        # Check the adress.
        self.assertTrue( msd.binCounters() == bin_counters )

    def testSafeCutoffQuery(self):
        """ Test the save cutoff query function. """
        # Setup a dummy object with data.
        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)
        history_bin_counters = numpy.array([[0,0,0,0,0],[0,0,0,6,5],[0,0,0,0,6]], dtype=int)
        msd._OnTheFlyMSD__history_bin_counters = history_bin_counters

        # Check the value.
        self.assertEqual( msd.safeCutoff(), 4 )

        history_bin_counters = numpy.array([[0,0,0,0,0],[0,0,0,6,5],[0,0,0,0,0]], dtype=int)
        msd._OnTheFlyMSD__history_bin_counters = history_bin_counters

        # Check the value.
        self.assertEqual( msd.safeCutoff(), 5 )

        history_bin_counters = numpy.array([[0]], dtype=int)
        msd._OnTheFlyMSD__history_bin_counters = history_bin_counters

        # Check the value.
        self.assertEqual( msd.safeCutoff(), 1 )


if __name__ == '__main__':
    unittest.main()
