""" Module for testing a bucket type system. """

# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import numpy
import os
import time

# Import the interface.
from KMCLib import *
from KMCLib.Backend import Backend


# Implement the test.
class BucketsTest(unittest.TestCase):
    """ Class for testing bucket type simulations. """

    def test1D(self):
        """ Test a 1D system."""
        # Define the unit cell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.1,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0]])

        # And a lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(5,1,1),
                             periodic=(True,False,False))

        # Populate the lattice with types.
        types = [(12,"A"),
                 "A",
                 ["A", "A", "A"],
                 "A",
                 "B"]

        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=["A","B"])

        # Setup a processes that moves B to the left.
        coordinates_p0 = [[0.0, 0.0, 0.0],[-1.0, 0.0, 0.0]]
        p0 = KMCBucketProcess(coordinates=coordinates_p0,
                              minimum_match=[(1, "B"), "*"],
                              update=[[(-1,"B")], [(1,"B")]],
                              basis_sites=[0],
                              rate_constant=1.0)

        # B to the right.
        coordinates_p1 = [[0.0, 0.0, 0.0],[1.0, 0.0, 0.0]]
        p1 = KMCBucketProcess(coordinates=coordinates_p1,
                              minimum_match=[(1, "B"), "*"],
                              update=[[(-1,"B")], [(1,"B")]],
                              basis_sites=[0],
                              rate_constant=1.0)

        # Set up the interactions.
        interactions = KMCInteractions(processes=[p0, p1],
                                       implicit_wildcards=True)

        # Construct the model.
        model = KMCLatticeModel(configuration=config,
                                interactions=interactions)

        ref_types = [['A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'], ['A'], ['A', 'A', 'A'], ['A'], ['B']]

        self.assertEqual(ref_types, config.types())

        # Control parameters.
        control_parameters = KMCControlParameters(number_of_steps=1,
                                                  dump_interval=1,
                                                  analysis_interval=1,
                                                  seed=123)
        # Run the model.
        model.run(control_parameters=control_parameters,
                  trajectory_filename="traj1a.py")

        ref_types = [['A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'B'], ['A'], ['A', 'A', 'A'], ['A'], []]
        self.assertEqual(ref_types, config.types())

        # Keep running.
        control_parameters = KMCControlParameters(number_of_steps=1000,
                                                  dump_interval=1,
                                                  analysis_interval=1,
                                                  seed=123)
        model.run(control_parameters=control_parameters,
                  trajectory_filename="traj1b.py")

    def test1DCustom(self):
        """ Test a 1D system."""
        # Define the unit cell.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[2.1,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0]])

        # And a lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(5,1,1),
                             periodic=(True,False,False))

        # Populate the lattice with types.
        types = [(4,"A"),
                 "A",
                 ["A", "A"],
                 "A",
                 "B"]

        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=["A","B"])

        # Setup a processes that moves B to the left.
        coordinates_p0 = [[0.0, 0.0, 0.0],[-1.0, 0.0, 0.0]]
        p0 = KMCBucketProcess(coordinates=coordinates_p0,
                              minimum_match=[(1, "B"), "*"],
                              update=[[(-1,"B")], [(1,"B")]],
                              basis_sites=[0],
                              rate_constant=1.0)

        # Set up the interactions.
        interactions = KMCInteractions(processes=[p0],
                                       implicit_wildcards=True)

        # Use a rate calculator.
        class CustomRates(KMCRateCalculatorPlugin):
            def rate(self,
                     coords,
                     types_before,
                     types_after,
                     rate_constant,
                     process_number,
                     global_coordinate):
                # Store the
                self.global_types_before = types_before
                self.global_types_after  = types_after
                self.global_coords       = coords
                self.global_coordinate   = global_coordinate
                return rate_constant

        interactions.setRateCalculator(rate_calculator=CustomRates)

        # Construct the model.
        model = KMCLatticeModel(configuration=config,
                                interactions=interactions)

        ref_types = [['A', 'A', 'A', 'A'], ['A'], ['A', 'A'], ['A'], ['B']]
        self.assertEqual(ref_types, config.types())

        # Control parameters.
        control_parameters = KMCControlParameters(number_of_steps=1,
                                                  dump_interval=1,
                                                  analysis_interval=1,
                                                  seed=123)
        # Run the model - take one step to the left.
        model.run(control_parameters=control_parameters,
                  trajectory_filename="traj2a.py")

        ref_types = [['A', 'A', 'A', 'A'], ['A'], ['A', 'A'], ['A', 'B'], []]
        self.assertEqual(ref_types, config.types())

        # The 'B' now sits at global coordinate 3, so this is the last matched site.
        self.assertAlmostEqual(interactions.rateCalculator().global_coordinate[0], 3.0, 10)
        self.assertAlmostEqual(interactions.rateCalculator().global_coordinate[1], 0.0, 10)
        self.assertAlmostEqual(interactions.rateCalculator().global_coordinate[2], 0.0, 10)

        # Check the order of the coordinates.
        ref_coords = numpy.array([[ 0.,  0.,  0.],
                                  [-1.,  0.,  0.],
                                  [ 1.,  0.,  0.]])

        diff = numpy.linalg.norm(interactions.rateCalculator().global_coords - ref_coords)
        self.assertAlmostEqual(diff, 0.0, 10)

        # The types before should equal the reference types.
        ref_tb = [[(1, 'A'),(1, 'B')],[(2,'A')],[]]
        self.assertEqual(interactions.rateCalculator().global_types_before, ref_tb)

        # The types after should correspond to the 'B' moving one step to the left.
        ref_ta = [[(1, 'A')],[(2,'A'), (1, 'B')],[]]
        self.assertEqual(interactions.rateCalculator().global_types_after, ref_ta)

    def testReal2D(self):
        """
        Test a realistic 2D diffusion system.
        """
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[1.0,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0],
                                              [0.5,0.0,0.0],
                                              [0.0,0.5,0.0]])

        # And a lattice.
        rep_a = 100
        rep_b = 10
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(rep_a,rep_b,1),
                             periodic=(True,True,False))

        # Populate the lattice with types.
        types = [["d"],["d"],["d"]] * rep_a * rep_b

        # Add creation sites and block sites.
        for i,c in enumerate(lattice.sites()):
            if (c[0] == 1.0 or c[0] == rep_a - 1) and c[1]%1 == 0:
                types[i] = ["C", "d"]
            elif c[0] == 0.0:
                types[i] = ["x"]

        # Add a random distribution of roughly 20% ions "I".
        numpy.random.seed(8765621)
        for i,(c,t) in enumerate(zip(lattice.sites(), types)):
            if c[0]%1 == 0.5 or c[1]%1 == 0.5:
                if numpy.random.rand() < 0.2:
                    types[i] = ["I"]

        # The starting configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=["C","I", "M1", "d", "x"])

        # A processes that creates an "M1" at a creation site.
        coordinates = [[0.0, 0.0, 0.0]]
        p0 = KMCBucketProcess(coordinates=coordinates,
                              minimum_match=[[(1, "C")]],
                              update=[[(1,"M1")]],
                              basis_sites=[0],
                              rate_constant=1.0)

        # A processes that moves an "M1" in the bulk, to the right.
        coordinates = [[0.0, 0.0, 0.0], [1.0, 0.0, 0.0]]
        p1 = KMCBucketProcess(coordinates=coordinates,
                              minimum_match=[[(1, "M1")], [(1, "d")]],
                              update=[[(-1,"M1")], [(1, "M1")]],
                              basis_sites=[0],
                              rate_constant=1.0)

        # A processes that moves an "M1" in the bulk, to the left.
        coordinates = [[0.0, 0.0, 0.0], [-1.0, 0.0, 0.0]]
        p2 = KMCBucketProcess(coordinates=coordinates,
                              minimum_match=[[(1, "M1")], [(1, "d")]],
                              update=[[(-1,"M1")], [(1, "M1")]],
                              basis_sites=[0],
                              rate_constant=1.0)

        # A processes that moves an "M1" in the bulk, to up.
        coordinates = [[0.0, 0.0, 0.0], [0.0, 1.0, 0.0]]
        p3 = KMCBucketProcess(coordinates=coordinates,
                              minimum_match=[[(1, "M1")], [(1, "d")]],
                              update=[[(-1,"M1")], [(1, "M1")]],
                              basis_sites=[0],
                              rate_constant=1.0)

        # A processes that moves an "M1" in the bulk, to down.
        coordinates = [[0.0, 0.0, 0.0], [0.0,-1.0, 0.0]]
        p4 = KMCBucketProcess(coordinates=coordinates,
                              minimum_match=[[(1, "M1")], [(1, "d")]],
                              update=[[(-1,"M1")], [(1, "M1")]],
                              basis_sites=[0],
                              rate_constant=1.0)

        # Set up the interactions.
        interactions = KMCInteractions(processes=[p0, p1, p2, p3, p4],
                                       implicit_wildcards=True)
        interactions.setRateCalculator(rate_calculator="BucketsTestCalculator")

        # Construct the lattice mode.
        model = KMCLatticeModel(configuration=config,
                                interactions=interactions)

        # Control parameters.
        control_parameters = KMCControlParameters(number_of_steps=118817,
                                                  seed=12,
                                                  dump_time_interval=10.0)

        # Run the model.
        t1 = time.clock()
        model.run(control_parameters=control_parameters,
                  trajectory_filename="traj1.py")
        t2 = time.clock()

        #breakers=[Breaker()])

        # Make a simple analysis of the distribution of M1 in the structure.
        distribution = []

        types = config.types()
        row = numpy.array([0, 3, 6, 9, 12, 15, 18, 21, 24, 27])
        for i in range(len(config.types())/(len(row)*3)):
            this_row = row + numpy.ones(len(row), dtype=int)*i*30

            # One-liner to calculate the number of M1 on this row.
            distribution.append(len([t for i in this_row for t in types[i] if t == 'M1']))

        ref_distribution = [0, 104, 36, 17, 23, 10, 18, 6, 3, 7, 5, 3, 6, 5, 5, 3, 2, 4, 8, 4, 0, 1, 2, 0, 0, 4, 2, 1, 0, 0, 1, 0, 0, 0, 0, 3, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 1, 1, 0, 1, 1, 2, 1, 2, 2, 1, 2, 1, 3, 4, 5, 4, 10, 7, 5, 8, 10, 4, 10, 8, 17, 26, 20, 38, 13, 78]
        # Check.
        self.assertEqual( ref_distribution, distribution )

        # Populate the lattice with types.
        types = [["d"],["d"],["d"]] * rep_a * rep_b

        # Add creation sites and block sites.
        for i,c in enumerate(lattice.sites()):
            if (c[0] == 1.0 or c[0] == rep_a - 1) and c[1]%1 == 0:
                types[i] = ["C", "d"]
            elif c[0] == 0.0:
                types[i] = ["x"]

        # Add a random distribution of roughly 20% ions "I".
        numpy.random.seed(8765621)
        for i,(c,t) in enumerate(zip(lattice.sites(), types)):
            if c[0]%1 == 0.5 or c[1]%1 == 0.5:
                if numpy.random.rand() < 0.2:
                    types[i] = ["I"]

        # The starting configuration.
        config = KMCConfiguration(lattice=lattice,
                                  types=types,
                                  possible_types=["C","I", "M1", "d", "x"])

        # Set up the interactions.
        interactions = KMCInteractions(processes=[p0, p1, p2, p3, p4],
                                       implicit_wildcards=True)
        interactions.setRateCalculator(rate_calculator=Bucket2DRateCalculator)

        # Construct the lattice mode.
        model = KMCLatticeModel(configuration=config,
                                interactions=interactions)

        # Control parameters.
        control_parameters = KMCControlParameters(number_of_steps=118817,
                                                  seed=12,
                                                  dump_time_interval=10.0)

        # Run the model.
        t3 = time.clock()
        model.run(control_parameters=control_parameters,
                  trajectory_filename="traj2.py")
        t4 = time.clock()

        # Check the results.
        distribution = []
        types = config.types()
        row = numpy.array([0, 3, 6, 9, 12, 15, 18, 21, 24, 27])
        for i in range(len(config.types())/(len(row)*3)):
            this_row = row + numpy.ones(len(row), dtype=int)*i*30

            # One-liner to calculate the number of M1 on this row.
            distribution.append(len([t for i in this_row for t in types[i] if t == 'M1']))

        # Check.
        self.assertEqual( ref_distribution, distribution )

        # Print the times.
        print "Time for custom C++ run:    ", t2-t1
        print "Time for custom Python run: ", t4-t3

        # Check that the trajectories are identical.

class Breaker(KMCBreakerPlugin):
    """
    Breaker class for the 2D buckets test.
    """

    def evaluate(self, step, time, configuration):
        """
        Called from the KMC loop after the number of steps returned by the
        interval function.

        :return: True if the KMC loop should stop.
        :rtype: bool
        """
        # 50:th row.
        check_at = [1500, 1503, 1506, 1509, 1512,
                    1515, 1518, 1521, 1524, 1527]

        # Break if any M1 has reached the most central layer.
        types = configuration.types()

        if any([('M1' in types[i]) for i in check_at]):
            print "Breaking from saturation condition at step: ", step
            return True

        # No break yet.
        return False


class Bucket2DRateCalculator(KMCRateCalculatorPlugin):
    """
    Rate Calculator for the test.
    """

    def rate(self,
             coords,
             types_before,
             types_after,
             rate_constant,
             process_number,
             global_coordinate):
        """
        Recieves info from the simulations backend.
        """
        # Creation.
        if process_number == 0:
            return 0.1

        # Right.
        elif process_number == 1:
            # Movig M1 from site 0 to 8 with 4 halfway.
            if types_before[4][0][1] == 'I':
                return 0.1
            else:
                return 1.0

        # Left.
        elif process_number == 2:
            # Movig M1 from site 0 to 5 with 1 halfway.
            if types_before[1][0][1] == 'I':
                return 0.1
            else:
                return 1.0

        # Up.
        elif process_number == 3:
            # Movig M1 from site 0 to 7 with 3 halfway.
            if types_before[3][0][1] == 'I':
                return 0.1
            else:
                return 1.0

        # Down.
        elif process_number == 4:
            # Movig M1 from site 0 to 6 with 2 halfway.
            if types_before[2][0][1] == 'I':
                return 0.1
            else:
                return 1.0

        return 1.0

    def cutoff(self):
        """
        Set the cutoff.
        """
        return 1.0

    def cacheRates(self):
        """
        Cache the rates.
        """
        return True


if __name__ == '__main__':
    unittest.main()
