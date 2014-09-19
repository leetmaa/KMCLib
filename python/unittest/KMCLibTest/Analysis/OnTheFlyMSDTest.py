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
        ref_results = numpy.array([[   2.48411677 ,   6.9316136  ,  11.15965253 ,  15.61525881 ,  20.79577824,
                                       26.17127015,   31.27058662,   35.79435927,   40.29549292,   44.87584877],
                                   [   2.73213241 ,   7.30730216 ,  11.57837564 ,  15.9480526  ,  20.17306843,
                                       23.52580107,   26.69678715,   30.13962601,   34.01181903,   39.10129726],
                                   [   2.76485136 ,   7.55726857 ,  12.02714009 ,  16.80741865 ,  22.31998028,
                                       26.5859072 ,   31.59849655,   36.11618314,   39.94591847,   44.25651668],
                                   [   5.21624919 ,  14.23891576 ,  22.73802817 ,  31.56331141 ,  40.96884666,
                                       49.69707123,   57.96737377,   65.93398529,   74.30731195,   83.97714603],
                                   [   5.24896813 ,  14.48888217 ,  23.18679261 ,  32.42267746 ,  43.11575852,
                                       52.75717736,   62.86908317,   71.91054242,   80.24141139,   89.13236546],
                                   [   5.49698377 ,  14.86457074 ,  23.60551573 ,  32.75547125 ,  42.49304871,
                                       50.11170828,   58.2952837 ,   66.25580916,   73.9577375 ,   83.35781394],
                                   [   7.98110055 ,  21.79618433 ,  34.76516826 ,  48.37073006 ,  63.28882695,
                                       76.28297843,   89.56587032,  102.05016843,  114.25323042,  128.23366271]])

        diff = numpy.linalg.norm(ref_results - results)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_time_steps = numpy.array([  1.25,   3.75,   6.25,   8.75,  11.25,  13.75,  16.25,  18.75,  21.25,  23.75])
        diff = numpy.linalg.norm(ref_time_steps - time_steps)
        self.assertAlmostEqual(diff, 0.0, 8)

        ref_std_dev = numpy.array([[  0.1327733 ,   0.6147092 ,   1.2656102 ,   2.09129612,   3.15588011,
                                      4.39286372,   5.70998744,   7.02342351,   8.42827969,   9.93305955],
                                   [  0.14602946,   0.64802601,   1.31309737,   2.13586601,   3.06138028,
                                      3.94882011,   4.87481483,   5.91387476,   7.11397486,   8.65488954],
                                   [  0.14777825,   0.67019352,   1.3639915 ,   2.25095785,   3.38718662,
                                      4.4624608 ,   5.76986356,   7.08657048,   8.35516205,   9.79597328],
                                   [  0.19714332,   0.89288863,   1.82342161,   2.989055  ,   4.39626698,
                                      5.8984612 ,   7.48458546,   9.14805134,  10.99003359,  13.14366485],
                                   [  0.1983799 ,   0.90856343,   1.8594092 ,   3.07043722,   4.62664685,
                                      6.26166002,   8.11748049,   9.97727244,  11.86768547,  13.9505329 ],
                                   [  0.20775342,   0.93212197,   1.89298769,   3.1019529 ,   4.55982538,
                                      5.94767377,   7.52692427,   9.19270299,  10.93833161,  13.04673022],
                                   [  0.24628666,   1.11597692,   2.27631837,   3.74014431,   5.54513006,
                                      7.39247635,   9.44237072,  11.56078602,  13.79717991,  16.38746522]])

        diff = numpy.linalg.norm(ref_std_dev - std_dev)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_bin_counters = (59843, 59544, 59285, 59310, 58838, 58796, 58266, 58184, 58042, 57583)
        self.assertEqual(bin_counters, ref_bin_counters)

    def testCalculationFlipped(self):
        """ Test a calculation with the on-the-fly MSD analysis. """
        # FIXME
        return
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
        ref_results = numpy.array([[   3.12507672 ,   8.84431286 ,  13.83414102 ,  18.58819987 ,  24.0305112,
                                       29.05110007,   33.65539622,   40.08318015,   46.38807125,   54.56025293],
                                   [   2.83900934 ,   8.52281124 ,  14.09108351 ,  20.0058549  ,  25.26096208,
                                       30.09109804,   36.62202664,   43.47434981,   50.79520993,   58.25866865],
                                   [   2.88482657 ,   8.20673616 ,  13.39956335 ,  17.50176483 ,  22.89894945,
                                       29.46099902,   36.60734674,   44.33478009,   52.14399436,   59.49159482],
                                   [   5.96408606 ,  17.3671241  ,  27.92522452 ,  38.59405477 ,  49.29147328,
                                       59.14219811,   70.27742286,   83.55752996,   97.18328118,  112.81892158],
                                   [   6.00990329 ,  17.05104902 ,  27.23370437 ,  36.0899647  ,  46.92946065,
                                       58.51209909,   70.26274297,   84.41796024,   98.53206561,  114.05184776],
                                   [   5.72383591 ,  16.7295474  ,  27.49064686 ,  37.50761973 ,  48.15991153,
                                       59.55209706,   73.22937339,   87.8091299 ,  102.93920429,  117.75026347],
                                   [   8.84891263 ,  25.57386026 ,  41.32478788 ,  56.0958196  ,  72.19042273,
                                       88.60319713,  106.88476961,  127.89231005,  149.32727554,  172.3105164 ]])

        diff = numpy.linalg.norm(ref_results - results)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_time_steps = numpy.array([  1.25,   3.75,   6.25,   8.75,  11.25,  13.75,  16.25,  18.75,  21.25,  23.75])
        diff = numpy.linalg.norm(ref_time_steps - time_steps)
        self.assertAlmostEqual(diff, 0.0, 8)

        ref_std_dev = numpy.array([[  0.16736707,   0.78892572,   1.58362091,   2.51376711,   3.68145254,
                                      4.91747367,   6.19186581,   7.92487428,   9.76530836,  12.15024843],
                                   [  0.1520464 ,   0.7602473 ,   1.61303362,   2.70548307,   3.8699565,
                                      5.09351392,   6.73766171,   8.59534486,  10.6930699 ,  12.97386393],
                                   [  0.15450019,   0.73205294,   1.53387397,   2.36684354,   3.5080983,
                                      4.98685718,   6.73496093,   8.76546115,  10.97700703,  13.24842936],
                                   [  0.22585943,   1.09543074,   2.2603761 ,   3.6905672 ,   5.33965254,
                                      7.07883721,   9.14255658,  11.68155898,  14.466258  ,  17.76543022],
                                   [  0.22759452,   1.07549432,   2.20440177,   3.45111289,   5.08378016,
                                      7.00341951,   9.14064684,  11.80184936,  14.66703187,  17.9595773 ],
                                   [  0.21676118,   1.05521562,   2.22519969,   3.58667655,   5.21707258,
                                      7.12789876,   9.52658283,  12.27594366,  15.32305835,  18.5419614 ],
                                   [  0.27361418,   1.31706642,   2.7311719 ,   4.37983325,   6.38520954,
                                      8.65900972,  11.3532977 ,  14.59869432,  18.14922816,  22.15439729]])

        diff = numpy.linalg.norm(ref_std_dev - std_dev)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_bin_counters = (60283, 60628, 60461, 59779, 59683, 59178, 58856, 58752, 58162, 57881)
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
        ref_results = numpy.array([[  11.35603736 ,  34.09124497 ,  56.36433403 ,  80.0234196  , 101.04384833,
                                      120.36439217,  146.48810657,  173.89739924,  203.18083972,  233.03467459],
                                   [  17.77565815 ,  49.86008115 ,  77.47314798 , 103.31671657 , 127.78442773,
                                      152.7615668 ,  178.05358842,  213.64137391,  244.77727726,  276.28601786],
                                   [   3.12507672 ,   8.84431286 ,  13.83414102 ,  18.58819987 ,  24.0305112,
                                       29.05110007,   33.65539622,   40.08318015,   46.38807125,   54.56025293],
                                   [  29.1316955  ,  83.95132612 , 133.83748201 , 183.34013617 , 228.82827606,
                                      273.12595897,  324.54169498,  387.53877315,  447.95811698,  509.32069246],
                                   [  14.48111408 ,  42.93555783 ,  70.19847505 ,  98.61161947 , 125.07435953,
                                      149.41549224,  180.14350279,  213.98057938,  249.56891097,  287.59492752],
                                   [  20.90073487 ,  58.70439401 ,  91.307289   , 121.90491644 , 151.81493893,
                                      181.81266687,  211.70898464,  253.72455406,  291.16534851,  330.8462708 ],
                                   [  32.25677222 ,  92.79563898 , 147.67162303 , 201.92833604 , 252.85878726,
                                      302.17705904,  358.19709121,  427.6219533 ,  494.34618823,  563.88094539]])

        ref_results = numpy.array([[   9.93646709  , 27.72645439 ,  44.6386101  ,  62.46103524 ,  83.18311295,
                                       104.68508062, 125.08234648,  143.1774371 ,  161.18197168,  179.5033951 ],
                                   [  16.70019885  , 45.94185812 ,  72.47576959 ,  99.61918732 , 133.25906727,
                                      158.32367848 , 188.40639481,  216.07120514,  239.50020675,  264.75473664],
                                   [   2.76485136  ,  7.55726857 ,  12.02714009 ,  16.80741865 ,  22.31998028,
                                       26.5859072  ,  31.59849655,   36.11618314,   39.94591847,   44.25651668],
                                   [  26.63666594  , 73.66831251 , 117.11437969 , 162.08022256 , 216.44218022,
                                      263.0087591  , 313.48874129,  359.24864224,  400.68217842,  444.25813174],
                                   [  12.70131845  , 35.28372296 ,  56.66575019 ,  79.26845389 , 105.50309324,
                                      131.27098782 , 156.68084303,  179.29362024,  201.12789015,  223.75991178],
                                   [  19.46505021  , 53.4991267  ,  84.50290967 , 116.42660597 , 155.57904755,
                                      184.90958569 , 220.00489136,  252.18738829,  279.44612522,  309.01125332],
                                   [  29.4015173   , 81.22558108 , 129.14151978 , 178.88764121 , 238.76216051,
                                      289.5946663  , 345.08723784,  395.36482538,  440.6280969 ,  488.51464842]])

        diff = numpy.linalg.norm(ref_results - results)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_time_steps = numpy.array([  1.25,   3.75,   6.25,   8.75,  11.25,  13.75,  16.25,  18.75,  21.25,  23.75])
        diff = numpy.linalg.norm(ref_time_steps - time_steps)
        self.assertAlmostEqual(diff, 0.0, 8)

        ref_std_dev = numpy.array([[  0.53109319 ,  2.45883681 ,  5.06244082 ,  8.36518447 , 12.62352042,
                                      17.57145487,  22.83994975,  28.09369405,  33.71311876,  39.7322382 ],
                                   [  0.89260718 ,  4.07421483 ,  8.21943813 , 13.34164372 , 20.22283727,
                                      26.57472636,  34.40287708,  42.39661257,  50.09430539,  58.60222452],
                                   [  0.14777825 ,  0.67019352 ,  1.3639915  ,  2.25095785 ,  3.38718662,
                                      4.4624608  ,  5.76986356 ,  7.08657048 ,  8.35516205 ,  9.79597328],
                                   [  1.00670818 ,  4.61956511 ,  9.39170667 , 15.34904541 , 23.22588227,
                                      31.21606411,  40.47679103,  49.84417382,  59.26079793,  69.53296541],
                                   [  0.4800346  ,  2.21255856 ,  4.54417387 ,  7.50674622 , 11.32127952,
                                      15.58033118,  20.23019301,  24.87620361,  29.74676663,  35.0217342 ],
                                   [  0.73566359 ,  3.35480332 ,  6.77650808 , 11.0256343  , 16.694808,
                                      21.94660551,  28.40641733,  34.98989429,  41.33001478,  48.36482949],
                                   [  0.9072936  ,  4.15879553 ,  8.45579725 , 13.83203422 , 20.91944656,
                                      28.06421256,  36.38039377,  44.78903088,  53.21009395,  62.42913633]])

        diff = numpy.linalg.norm(ref_std_dev - std_dev)
        self.assertAlmostEqual(diff, 0.0, 6)

        ref_bin_counters = (59843, 59544, 59285, 59310, 58838, 58796, 58266, 58184, 58042, 57583)
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
