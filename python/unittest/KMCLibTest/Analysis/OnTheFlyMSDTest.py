""" Module for testing the on-the-fly MSD analysis. """


# Copyright (c)  2013-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import numpy
import StringIO

# Import from the module we test.
from KMCLib.Analysis.OnTheFlyMSD import OnTheFlyMSD
from KMCLib.CoreComponents.KMCUnitCell import KMCUnitCell
from KMCLib.CoreComponents.KMCLattice import KMCLattice
from KMCLib.CoreComponents.KMCConfiguration import KMCConfiguration
from KMCLib.CoreComponents.KMCProcess import KMCProcess
from KMCLib.CoreComponents.KMCInteractions import KMCInteractions
from KMCLib.CoreComponents.KMCLatticeModel import KMCLatticeModel
from KMCLib.CoreComponents.KMCControlParameters import KMCControlParameters
from KMCLib.Backend.Backend import MPICommons
from KMCLib.Backend import Backend

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
        ref_results = numpy.array([[   2.59214168 ,   7.09292512 ,  11.09571441 ,  14.73593561 ,  18.02780434,
                                       21.25579701,   24.64588125,   28.81831092,   31.62393358,   34.6177501 ],
                                   [   2.65374493 ,   7.10317609 ,  10.90626886 ,  14.87948705 ,  18.25169973,
                                       21.00603548,   22.97611332,   25.57822115,   28.87848128,   32.2240879 ],
                                   [   2.59976568 ,   7.35085681 ,  11.8568078  ,  16.56678774 ,  21.77700882,
                                       26.34223625,   30.40786814,   35.09199292,   40.24434182,   46.48414664],
                                   [   5.24588661 ,  14.19610121 ,  22.00198327 ,  29.61542266 ,  36.27950408,
                                       42.2618325 ,   47.62199456,   54.39653208,   60.50241486,   66.841838  ],
                                   [   5.19190736 ,  14.44378193 ,  22.9525222  ,  31.30272335 ,  39.80481316,
                                       47.59803326,   55.05374939,   63.91030384,   71.8682754 ,   81.10189674],
                                   [   5.2535106  ,  14.45403291 ,  22.76307666 ,  31.44627479 ,  40.02870855,
                                       47.34827174,   53.38398146,   60.67021407,   69.1228231 ,   78.70823454],
                                   [   7.84565228 ,  21.54695802 ,  33.85879107 ,  46.1822104  ,  58.05651289,
                                       68.60406875,   78.02986271,   89.48852499,  100.74675668,  113.32598464]])


        diff = numpy.linalg.norm(ref_results - results)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_time_steps = numpy.array([  1.25,   3.75,   6.25,   8.75,  11.25,  13.75,  16.25,  18.75,  21.25,  23.75])
        diff = numpy.linalg.norm(ref_time_steps - time_steps)
        self.assertAlmostEqual(diff, 0.0, 8)

        ref_std_dev = numpy.array([[  0.13765582,   0.62432398,   1.24701335,   1.95661619,   2.7137435,
                                      3.53476554,   4.45846845,   5.60517366,   6.55120574,   7.58912539],
                                   [  0.14092726,   0.62522628,   1.22572215,   1.97567674,   2.7474467,
                                      3.49323106,   4.1564055 ,   4.97497483,   5.98245856,   7.06437139],
                                   [  0.13806069,   0.6470273 ,   1.33255031,   2.19971409,   3.27811502,
                                      4.38062279,   5.50081855,   6.82540746,   8.33700723,  10.19055301],
                                   [  0.19698798,   0.88356546,   1.74848804,   2.780551  ,   3.86164462,
                                      4.96954405,   6.09163579,   7.48129474,   8.86263902,  10.36158694],
                                   [  0.19496101,   0.89898111,   1.82402696,   2.93896933,   4.23688379,
                                      5.59702476,   7.04227937,   8.7897482 ,  10.52755635,  12.57213116],
                                   [  0.19727427,   0.89961913,   1.8089718 ,   2.95244717,   4.26071555,
                                      5.56765545,   6.82868861,   8.34413034,  10.12539137,  12.20107405],
                                   [  0.24054939,   1.09498957,   2.19698279,   3.54031591,   5.04564022,
                                      6.58676948,   8.14969886,  10.04910241,  12.04968783,  14.34371883]])

        diff = numpy.linalg.norm(ref_std_dev - std_dev)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_bin_counters = (58893, 58531, 57985, 58641, 57509, 57659, 57396, 57037, 56732, 56518)
        self.assertEqual(bin_counters, ref_bin_counters)

    def testCalculationFlipped(self):
        """ Test a calculation with the on-the-fly MSD analysis. """
        # Setup a system, a periodic 10 atoms long 1D chain.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[0.0,1.0,0.0],
                                                          [0.0,0.0,1.0],
                                                          [1.0,0.0,0.0]]),
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
        ref_results = numpy.array([[   2.59976568 ,   7.35085681 ,  11.8568078  ,  16.56678774 ,  21.77700882,
                                       26.34223625,   30.40786814,   35.09199292,   40.24434182,   46.48414664],
                                   [   2.59214168 ,   7.09292512 ,  11.09571441 ,  14.73593561 ,  18.02780434,
                                       21.25579701,   24.64588125,   28.81831092,   31.62393358,   34.6177501 ],
                                   [   2.65374493 ,   7.10317609 ,  10.90626886 ,  14.87948705 ,  18.25169973,
                                       21.00603548,   22.97611332,   25.57822115,   28.87848128,   32.2240879 ],
                                   [   5.19190736 ,  14.44378193 ,  22.9525222  ,  31.30272335 ,  39.80481316,
                                       47.59803326,   55.05374939,   63.91030384,   71.8682754 ,   81.10189674],
                                   [   5.2535106  ,  14.45403291 ,  22.76307666 ,  31.44627479 ,  40.02870855,
                                       47.34827174,   53.38398146,   60.67021407,   69.1228231 ,   78.70823454],
                                   [   5.24588661 ,  14.19610121 ,  22.00198327 ,  29.61542266 ,  36.27950408,
                                       42.2618325 ,   47.62199456,   54.39653208,   60.50241486,   66.841838  ],
                                   [   7.84565228 ,  21.54695802 ,  33.85879107 ,  46.1822104  ,  58.05651289,
                                       68.60406875,   78.02986271,   89.48852499,  100.74675668,  113.32598464]])

        diff = numpy.linalg.norm(ref_results - results)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_time_steps = numpy.array([  1.25,   3.75,   6.25,   8.75,  11.25,  13.75,  16.25,  18.75,  21.25,  23.75])
        diff = numpy.linalg.norm(ref_time_steps - time_steps)
        self.assertAlmostEqual(diff, 0.0, 8)

        ref_std_dev = numpy.array([[  0.13806069,   0.6470273 ,   1.33255031,   2.19971409,   3.27811502,
                                      4.38062279,   5.50081855,   6.82540746,   8.33700723,  10.19055301],
                                   [  0.13765582,   0.62432398,   1.24701335,   1.95661619,   2.7137435,
                                      3.53476554,   4.45846845,   5.60517366,   6.55120574,   7.58912539],
                                   [  0.14092726,   0.62522628,   1.22572215,   1.97567674,   2.7474467,
                                      3.49323106,   4.1564055 ,   4.97497483,   5.98245856,   7.06437139],
                                   [  0.19496101,   0.89898111,   1.82402696,   2.93896933,   4.23688379,
                                      5.59702476,   7.04227937,   8.7897482 ,  10.52755635,  12.57213116],
                                   [  0.19727427,   0.89961913,   1.8089718 ,   2.95244717,   4.26071555,
                                      5.56765545,   6.82868861,   8.34413034,  10.12539137,  12.20107405],
                                   [  0.19698798,   0.88356546,   1.74848804,   2.780551  ,   3.86164462,
                                      4.96954405,   6.09163579,   7.48129474,   8.86263902,  10.36158694],
                                   [  0.24054939,   1.09498957,   2.19698279,   3.54031591,   5.04564022,
                                      6.58676948,   8.14969886,  10.04910241,  12.04968783,  14.34371883]])

        diff = numpy.linalg.norm(ref_std_dev - std_dev)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_bin_counters = (58893, 58531, 57985, 58641, 57509, 57659, 57396, 57037, 56732, 56518)
        self.assertEqual(bin_counters, ref_bin_counters)

    def testCalculationNonOrthogonal(self):
        """ Test a calculation with the on-the-fly MSD analysis. """
        # Setup a system, a periodic 10 atoms long 1D chain.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.0,1.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,2.0,1.0]]),
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
        ref_results = numpy.array([[  10.36856672 ,  28.37170047 ,  44.38285764 ,  58.94374243 ,  72.11121737,
                                      85.02318805 ,  98.58352498 , 115.27324368 , 126.49573433 , 138.47100039],
                                   [  16.05138132 ,  44.90792913 ,  68.59720617 ,  93.35093194 , 123.68147594,
                                      148.63693439,  172.54794759,  203.0082052 ,  232.5195128 ,  267.73366007],
                                   [   2.59976568 ,   7.35085681 ,  11.8568078  ,  16.56678774 ,  21.77700882,
                                       26.34223625,   30.40786814,   35.09199292,   40.24434182,   46.48414664],
                                   [  26.41994804 ,  73.2796296  , 112.98006381 , 152.29467437 , 195.79269332,
                                      233.66012244,  271.13147258,  318.28144888,  359.01524713,  406.20466046],
                                   [  12.9683324  ,  35.72255728 ,  56.23966543 ,  75.51053018 ,  93.88822619,
                                      111.3654243 ,  128.99139313,  150.3652366 ,  166.74007615,  184.95514703],
                                   [  18.651147   ,  52.25878594 ,  80.45401397 , 109.91771968 , 145.45848476,
                                      174.97917064,  202.95581574,  238.10019812,  272.76385461,  314.21780672],
                                   [  29.01971372 ,  80.63048641 , 124.8368716  , 168.86146212 , 217.56970213,
                                      260.0023587 ,  301.53934072,  353.3734418 ,  399.25958894,  452.68880711]])

        diff = numpy.linalg.norm(ref_results - results)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_time_steps = numpy.array([  1.25,   3.75,   6.25,   8.75,  11.25,  13.75,  16.25,  18.75,  21.25,  23.75])
        diff = numpy.linalg.norm(ref_time_steps - time_steps)
        self.assertAlmostEqual(diff, 0.0, 8)

        ref_std_dev = numpy.array([[  0.55062327 ,  2.49729593 ,  4.98805342 ,  7.82646477 , 10.85497401,
                                      14.13906215,  17.83387379,  22.42069464,  26.20482295,  30.35650154],
                                   [  0.85240944 ,  3.95282576 ,  7.70942988 , 12.39500157 , 18.61789684,
                                      24.71780818,  31.21412347,  39.48518175,  48.16868093,  58.69429153],
                                   [  0.13806069 ,  0.6470273  ,  1.33255031 ,  2.19971409 ,  3.27811502,
                                      4.38062279 ,  5.50081855 ,  6.82540746 ,  8.33700723 , 10.19055301],
                                   [  0.99209394 ,  4.56092478 ,  8.97847654 , 14.29873598 , 20.84046684,
                                      27.47595651,  34.68217147,  43.77406499,  52.59000894,  62.96841965],
                                   [  0.4869731  ,  2.22337228 ,  4.46934176 ,  7.08957906 ,  9.99360309,
                                      13.09539481,  16.50011919,  20.68011712,  24.42476236,  28.67109723],
                                   [  0.70036814 ,  3.25258729 ,  6.3936455  , 10.32002241 , 15.48281847,
                                      20.57569786,  25.96138447,  32.74653167,  39.95555528,  48.70894069],
                                   [  0.88975069 ,  4.09754088 ,  8.10024368 , 12.94487456 , 18.90879051,
                                      24.96317829,  31.49377349,  39.68202523,  47.75293586,  57.29701788]])

        diff = numpy.linalg.norm(ref_std_dev - std_dev)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_bin_counters = (58893, 58531, 57985, 58641, 57509, 57659, 57396, 57037, 56732, 56518)
        self.assertEqual(bin_counters, ref_bin_counters)

    def testPrintResults(self):
        """ Test that the results gets printed as expected. """
        # Fill an object with data.
        ref_results = numpy.array([[  2.83900934,   8.52281124,  14.09108351,  20.00585490,  25.26096208,
                                     30.09109804,  36.62202664,  43.47434981,  50.79520993,  58.25866865],
                                   [  2.88482657,   8.20673616,  13.39956335,  17.50176483,  22.89894945,
                                     29.46099902,  36.60734674,  44.33478009,  52.14399436,  59.49159482],
                                   [ 22.88482657,  48.20673616,  53.39956335,  67.50176483,  72.89894945,
                                     39.46099902,  56.60734674,  54.33478009,  62.14399436,  79.49159482],
                                   [  3.12507672,   8.84431286,  13.83414102,  18.58819987,  24.03051120,
                                     29.05110007,  77.65539622,  40.08318015,  46.38807125,  54.56025293],
                                   [ 13.83900934,  66.52281124,  14.09108351,  20.00585490,  25.26096208,
                                     24.09109804,  88.62202664,  43.47434981,  30.79520993,  78.25866865],
                                   [ 35.88482657,   5.20673616,  13.39956335,  37.50176483,  82.89894945,
                                     45.46099902,  34.60734674,  44.33478009,  42.14399436,  99.49159482],
                                   [  56.12507672,  1.84431286,  13.83414102,  58.58819987,  84.03051120,
                                     29.05110007,  33.65539622,  40.08318015,  56.38807125,  74.56025293]])
        ref_time_steps = numpy.array([  1.25,   3.75,   6.25,   8.75,  11.25,  13.75,  16.25,  18.75,  21.25,  23.75])
        ref_std_dev = numpy.array([[ 0.02008562,  0.05432052,  0.08833069,  0.11053417,  0.12584537,  0.14901027,
                                     0.18265464,  0.20891115,  0.24045901,  0.27834904],
                                   [ 0.01911933,  0.04761810,  0.07830592,  0.10414501,  0.14034533,  0.18069990,
                                     0.23416218,  0.28725392,  0.34211121,  0.39464802],
                                   [ 0.02198474,  0.05167655,  0.08130953,  0.10885342,  0.14397243,  0.16894787,
                                     0.19585331,  0.22369909,  0.25162530,  0.29321875],
                                   [ 3.02198474,  4.05167655,  5.08130953,  5.10885342,  0.14397243,  4.16894787,
                                     3.19585331,  4.22369909, 50.25162530, 10.29321875],
                                   [ 1.02008562,  3.05432052,  0.08833069,  0.11053417, 44.12584537,  3.14901027,
                                     3.18265464,  4.20891115,  0.24045901,  0.27834904],
                                   [ 4.01911933,  5.04761810,  4.07830592,  0.10414501,  0.14034533,  1.18069990,
                                     5.23416218,  6.28725392,  4.34211121,  0.39464802],
                                   [ 6.02198474,  7.05167655,  3.08130953,  2.10885342,  4.14397243,  1.16894787,
                                     7.19585331,  8.22369909,  0.25162530,  0.29321875]])
        ref_bin_counters = (60283, 60628, 60461, 59779, 59683, 59178, 58856, 58752, 58162, 57881)

        ref_history_bin_counters = [(0, 0, 0, 0, 0, 1, 87, 58752, 58162, 57881)]

        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)
        msd._OnTheFlyMSD__results = ref_results
        msd._OnTheFlyMSD__time_steps = ref_time_steps
        msd._OnTheFlyMSD__std_dev = ref_std_dev
        msd._OnTheFlyMSD__bin_counters = ref_bin_counters
        msd._OnTheFlyMSD__history_bin_counters = ref_history_bin_counters
        msd._OnTheFlyMSD__n_eff = [0.1, 1.2, 3.4, 5.5, 6.6, 7.7, 0.8, 9.9, 4.3, 2.1]

        # Print the results to a stream.
        stream = StringIO.StringIO()
        msd.printResults(stream)

        # Check against reference.

        ref_value = """      TIME       MSD_x       DSD_y       MSD_z      MSD_xy      MSD_xz      MSD_yz     MSD_xyz       STD_x       STD_y       STD_z      STD_xy      STD_xz      STD_yz     STD_xyz        N_eff
1.25000e+00 2.83901e+00 2.88483e+00 2.28848e+01 3.12508e+00 1.38390e+01 3.58848e+01 5.61251e+01 2.00856e-02 1.91193e-02 2.19847e-02 3.02198e+00 1.02009e+00 4.01912e+00 6.02198e+00 1.00000e-01
3.75000e+00 8.52281e+00 8.20674e+00 4.82067e+01 8.84431e+00 6.65228e+01 5.20674e+00 1.84431e+00 5.43205e-02 4.76181e-02 5.16766e-02 4.05168e+00 3.05432e+00 5.04762e+00 7.05168e+00 1.20000e+00
6.25000e+00 1.40911e+01 1.33996e+01 5.33996e+01 1.38341e+01 1.40911e+01 1.33996e+01 1.38341e+01 8.83307e-02 7.83059e-02 8.13095e-02 5.08131e+00 8.83307e-02 4.07831e+00 3.08131e+00 3.40000e+00
8.75000e+00 2.00059e+01 1.75018e+01 6.75018e+01 1.85882e+01 2.00059e+01 3.75018e+01 5.85882e+01 1.10534e-01 1.04145e-01 1.08853e-01 5.10885e+00 1.10534e-01 1.04145e-01 2.10885e+00 5.50000e+00
1.12500e+01 2.52610e+01 2.28989e+01 7.28989e+01 2.40305e+01 2.52610e+01 8.28989e+01 8.40305e+01 1.25845e-01 1.40345e-01 1.43972e-01 1.43972e-01 4.41258e+01 1.40345e-01 4.14397e+00 6.60000e+00
1.37500e+01 3.00911e+01 2.94610e+01 3.94610e+01 2.90511e+01 2.40911e+01 4.54610e+01 2.90511e+01 1.49010e-01 1.80700e-01 1.68948e-01 4.16895e+00 3.14901e+00 1.18070e+00 1.16895e+00 7.70000e+00
1.62500e+01 3.66220e+01 3.66073e+01 5.66073e+01 7.76554e+01 8.86220e+01 3.46073e+01 3.36554e+01 1.82655e-01 2.34162e-01 1.95853e-01 3.19585e+00 3.18265e+00 5.23416e+00 7.19585e+00 8.00000e-01
"""
        if MPICommons.isMaster():
            self.assertEqual(stream.getvalue(), ref_value)
        else:
            self.assertEqual(stream.getvalue(), "")


    def testFinalizeNoCoordinateTransform(self):
        """ Test finalization. """
        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)

        # Replace the getBackendResults function.
        def dummy():
            pass
        msd._OnTheFlyMSD__getBackendResults = dummy

        # Set a unitcell on the class.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[1.0,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0]])
        msd._OnTheFlyMSD__unit_cell = unit_cell

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
        hstep_counts = [8919,8918,8917,8916,8915,8914,8913,8912,8911,8910,8099,8908,8907,8906,8905,8904,8903,8902,8901]
        msd._OnTheFlyMSD__hstep_counts = hstep_counts

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

        K1 = hstep_counts[int(n_eff[3]+1)]
        K2 = K1*K1
        std_0_3 = result_0_3 * numpy.sqrt(2.0) * numpy.sqrt((4*n_eff[3]*n_eff[3]*K1 + 2*K1 + n_eff[3] - n_eff[3]*n_eff[3]*n_eff[3])/(n_eff[3]*6*K2))
        self.assertAlmostEqual(std_0_3, msd._OnTheFlyMSD__std_dev[0][3], 10)

        K1 = hstep_counts[int(n_eff[7]+1)]
        K2 = K1*K1
        std_1_7 = result_1_7 * numpy.sqrt(2.0) * numpy.sqrt((4*n_eff[7]*n_eff[7]*K1 + 2*K1 + n_eff[7] - n_eff[7]*n_eff[7]*n_eff[7])/(n_eff[7]*6*K2))
        self.assertAlmostEqual(std_1_7, msd._OnTheFlyMSD__std_dev[1][7], 10)


        K1 = hstep_counts[int(n_eff[0]+1)]
        K2 = K1*K1
        std_2_0 = result_2_0 * numpy.sqrt(2.0) * numpy.sqrt((4*n_eff[0]*n_eff[0]*K1 + 2*K1 + n_eff[0] - n_eff[0]*n_eff[0]*n_eff[0])/(n_eff[0]*6*K2))
        self.assertAlmostEqual(std_2_0, msd._OnTheFlyMSD__std_dev[2][0], 10)

    def testFinalizeWithCoordinateTransform(self):
        """ Test finalization. """
        msd = OnTheFlyMSD.__new__(OnTheFlyMSD)

        # Replace the getBackendResults function.
        def dummy():
            pass
        msd._OnTheFlyMSD__getBackendResults = dummy

        # Set a unitcell on the class.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[1.0,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0]])
        msd._OnTheFlyMSD__unit_cell = unit_cell

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

        hstep_counts = [8919,8918,8917,8916,8915,8914,8913,8912,8911,8910,8099,8908,8907,8906,8905,8904,8903,8902,8901]
        msd._OnTheFlyMSD__hstep_counts = hstep_counts

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

        result_3_4 =  ( 1507650.0 +  1366678.0) / 59683.0
        self.assertAlmostEqual(result_3_4, msd._OnTheFlyMSD__results[3][4], 10)

        result_4_4 =  (  1507650.0 + 1434213.0) /  59683.0
        self.assertAlmostEqual(result_4_4, msd._OnTheFlyMSD__results[4][4], 10)

        result_5_6 =  ( 2154562.0 + 1980822.0) / 58856.0
        self.assertAlmostEqual(result_5_6, msd._OnTheFlyMSD__results[5][6], 10)

        result_6_5 =  ( 1780731.0 + 1743443.0 + 1719186.0)/ 59178.0
        self.assertAlmostEqual(result_6_5, msd._OnTheFlyMSD__results[6][5], 10)

        # Check the corresponding standard deviation against hardcoded values.
        std_0_3 = 42.291401180595834
        std_1_7 =  0.813230156401512
        std_2_0 =  6.634050132990601
        std_3_4 = 71.931090096615776
        std_4_4 = 73.621177716983027
        std_5_6 =  7.877909031669120
        std_6_5 = 62.119638306131414

        self.assertAlmostEqual(std_0_3, msd._OnTheFlyMSD__std_dev[0][3], 10)
        self.assertAlmostEqual(std_1_7, msd._OnTheFlyMSD__std_dev[1][7], 10)
        self.assertAlmostEqual(std_2_0, msd._OnTheFlyMSD__std_dev[2][0], 10)
        self.assertAlmostEqual(std_3_4, msd._OnTheFlyMSD__std_dev[3][4], 10)
        self.assertAlmostEqual(std_4_4, msd._OnTheFlyMSD__std_dev[4][4], 10)
        self.assertAlmostEqual(std_5_6, msd._OnTheFlyMSD__std_dev[5][6], 10)
        self.assertAlmostEqual(std_6_5, msd._OnTheFlyMSD__std_dev[6][5], 10)

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
