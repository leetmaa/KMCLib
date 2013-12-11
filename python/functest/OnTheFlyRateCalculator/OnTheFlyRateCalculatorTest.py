""" Module for testing the use of on-the-fly cutom rates. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import numpy

# Import the interface.
from KMCLib import *

# Implement the test.
class OnTheFlyRateCalculatorTest(unittest.TestCase):
    """ Class for testing usage of the on-the-fly rate calculator. """

    def testBattaileModel(self):
        """ Test that we can reproduce the first Battaile model. """

        # Move to the right.
        p0 = KMCProcess(coordinates=[[0.0, 0.0, 0.0],[1.0, 0.0, 0.0]],
                        elements_before=["A","B"],
                        elements_after=["B","A"],
                        basis_sites=[0],
                        rate_constant=1.0)
        # Move up.
        p1 = KMCProcess(coordinates=[[0.0, 0.0, 0.0],[0.0,-1.0, 0.0]],
                        elements_before=["A","B"],
                        elements_after=["B","A"],
                        basis_sites=[0],
                        rate_constant=1.0)

        # Move down.
        p2 = KMCProcess(coordinates=[[0.0, 0.0, 0.0],[0.0, 1.0, 0.0]],
                        elements_before=["A","B"],
                        elements_after=["B","A"],
                        basis_sites=[0],
                        rate_constant=1.0)

        # Move to the left.
        p3 = KMCProcess(coordinates=[[0.0, 0.0, 0.0],[-1.0, 0.0, 0.0]],
                        elements_before=["A","B"],
                        elements_after=["B","A"],
                        basis_sites=[0],
                        rate_constant=1.0)

        interactions = KMCInteractions(processes=[p0, p1, p2, p3])


        # Setup the structure.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[1.0,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0]])

        # And a lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(100,100,1),
                             periodic=(True,True,False))

        # Setup the initial configuration. A random 20% A on a background of B.
        types = ["B"]*100*100

        nTot = 10000
        nA   = 2000
        n    = 0

        # To work in parallel we need to have the seed set to the same value
        # for all processes.
        numpy.random.seed( 143605 )

        for i in range(nA):
            # find a site which is not yet occupied by an "A" type.
            pos = int(numpy.random.rand()*nTot)
            while (types[pos] == "A"):
                pos = int(numpy.random.rand()*nTot)

            # Set the type.
            types[pos] = "A"

        config = KMCConfiguration(lattice, types)

        # Set the custom rate calculator.
        interactions.setRateCalculator(rate_calculator=BattaileModelRates)

        # Put the model together and run.
        model = KMCLatticeModel(config, interactions)

        control_parameters = KMCControlParameters(number_of_steps=100000,
                                                  dump_interval=1000,
                                                  seed=13324591)

        model.run(control_parameters, trajectory_filename="traj.py")

        # Load the first and the last images in the trajectory and
        # calculate the cluster sizes.

        trajfile="traj.py"
        global_dict = {}
        local_dict  = {}
        execfile(trajfile, global_dict, local_dict)
        sites = local_dict["sites"]
        elem = local_dict["types"]

        e0 = numpy.array(elem[0]).reshape(100,100)
        e1 = numpy.array(elem[100]).reshape(100,100)

        e0_sum = 0
        e1_sum = 0

        # Loop over all positions.
        for i in range(100):
            for j in range(100):

                if e0[i,j] == "A" or e1[i,j] == "A":

                    # Get the indices of the neighbours.
                    i_minus_one = i-1
                    i_plus_one = i+1

                    if i == 0:
                        i_minus_one = 99
                    elif i == 99:
                        i_plus_one = 0

                    j_minus_one = j-1
                    j_plus_one = j+1
                    if j == 0:
                        j_minus_one = 99
                    elif j == 99:
                        j_plus_one = 0

                    if e0[i,j] == "A":
                        # Get all nearest neighbours of A type.
                        neighbours = [e0[i_plus_one, j], e0[i_minus_one, j],
                                      e0[i, j_plus_one], e0[i, j_minus_one]]
                        e0_sum += len([a for a in neighbours if a == "A"])

                    if e1[i,j] == "A":
                        # Get all nearest neighbours of A type.
                        neighbours = [e1[i_plus_one, j], e1[i_minus_one, j],
                                      e1[i, j_plus_one], e1[i, j_minus_one]]
                        e1_sum += len([a for a in neighbours if a == "A"])

        # Test the clustering simply by requiering that the number of nearest neighbours
        # in the final image is at least twice as large as in the first image.
        self.assertTrue(e0_sum*2.0 < e1_sum)


# Define the custom rates calculator.
class BattaileModelRates(KMCRateCalculatorPlugin):
    """ Class for defining the custom rates function for the KMCLib paper. """

    def rate(self, geometry, elements_before, elements_after, rate_constant, process_number, global_coordinate):
        """
        Rate function that implements the vacancy diffusion example of
        [Corbett C. Battaile, Comput. Methods Appl. Mech. Engrg. 197 (2008) 3386]
        """
        # The number of bonds before is the same number as the number
        # neighbours of A type. It can be between zero and three.

        # Before move the central particle has neighbour indices 1, 2, 3, 4.

        # After the move the central particle has meighbour indices,
        # if we move to the right, 0, 7, 8, 12
        # if we move up,           0, 6, 8, 11
        # if we move down,         0, 5, 7, 10
        # if we move left,         0, 5, 6, 9

        diff = 1.0
        # Move right.
        if process_number == 0:
            diff = len([ e for e in [elements_after[0], elements_after[7], elements_after[8], elements_after[12]] if e == "A"]) - len([ e for e in [elements_before[1], elements_before[2], elements_before[3], elements_before[4]] if e == "A"])

        # Move dowm.
        elif process_number == 1:
            diff = len([ e for e in [elements_after[0], elements_after[5], elements_after[7], elements_after[10]] if e == "A"]) - len([ e for e in [elements_before[1], elements_before[2], elements_before[3], elements_before[4]] if e == "A"])

        # Move up.
        elif process_number == 2:
            diff = len([ e for e in [elements_after[0], elements_after[6], elements_after[8], elements_after[11]] if e == "A"]) - len([ e for e in [elements_before[1], elements_before[2], elements_before[3], elements_before[4]] if e == "A"])

        # Move left.
        elif process_number == 3:
            diff = len([ e for e in [elements_after[0], elements_after[5], elements_after[6], elements_after[9]] if e == "A"]) - len([ e for e in [elements_before[1], elements_before[2], elements_before[3], elements_before[4]] if e == "A"])

        if diff < 0.0:
            # If diff is negative, DeltaE is positive (since the bond energy
            # is also negative) This implements the model in Batallie's paper.
            return numpy.exp(diff*2.0)
        else:
            return 1.0

    def cutoff(self):
        """ Overloaded base class API function """
        return 2.0

if __name__ == '__main__':
    unittest.main()


