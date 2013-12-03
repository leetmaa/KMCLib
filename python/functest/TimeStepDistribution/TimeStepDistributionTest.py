""" Module for testing the time step distribution analysis functionality. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import numpy

# Import the interface.
from KMCLib import *

# Import the time step distribution analysis plugin.
from KMCLib.Analysis.TimeStepDistribution import TimeStepDistribution

# Implement the test.
class TimeStepDistributionTest(unittest.TestCase):
    """ Class for testing the time step distribution analysis. """

    def testTimeStepDistribution(self):
        """ Test the distribution for a system with only one moving particle. """
        # Setup a system, a periodic 10x10x10 atoms long 3D block.
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

        # Setup a all diffusion processes.
        coordinates_p0 = [[0.0, 0.0, 0.0],[-1.0, 0.0, 0.0]]
        p0 = KMCProcess(coordinates=coordinates_p0,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=0.1)

        coordinates_p1 = [[0.0, 0.0, 0.0],[1.0, 0.0, 0.0]]
        p1 = KMCProcess(coordinates=coordinates_p1,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=0.2)

        coordinates_p2 = [[0.0, 0.0, 0.0],[0.0,-1.0, 0.0]]
        p2 = KMCProcess(coordinates=coordinates_p2,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=0.3)

        coordinates_p3 = [[0.0, 0.0, 0.0],[0.0, 1.0, 0.0]]
        p3 = KMCProcess(coordinates=coordinates_p3,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=0.4)

        coordinates_p4 = [[0.0, 0.0, 0.0],[0.0, 0.0,-1.0]]
        p4 = KMCProcess(coordinates=coordinates_p4,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=0.5)

        coordinates_p5 = [[0.0, 0.0, 0.0],[0.0, 0.0, 1.0]]
        p5 = KMCProcess(coordinates=coordinates_p5,
                        elements_before=["B","A"],
                        elements_after=["A","B"],
                        move_vectors=None,
                        basis_sites=[0],
                        rate_constant=0.6)

        interactions = KMCInteractions(processes=[p0, p1, p2, p3, p4, p5],
                                       implicit_wildcards=True)

        model = KMCLatticeModel(configuration=config,
                                interactions=interactions)

        # Setup the analysis.
        tsd = TimeStepDistribution(binsize=0.001)

        # Setup the control parameters.
        control_parameters = KMCControlParameters(number_of_steps=1000000,
                                                  dump_interval=10000,
                                                  analysis_interval=1)
        # Run the model.
        model.run(control_parameters=control_parameters,
                  analysis=[tsd])

        # Define the analytical reference function.
        # The exponent is the total rate and the coefficient is
        # chosen to normalize to one.
        total_rate = 2.1
        def ref(dt):
            return total_rate*0.001*numpy.exp(-total_rate * dt)

        hst  = tsd.normalizedHistogram()
        time = tsd.timeSteps()

        sum_diff = 0.0
        for t, h in zip(time, hst):
            sum_diff += numpy.abs(ref(t) - h)

        # Make sure the relative error is less than five percent.
        self.assertTrue( sum_diff*100.0 < 5.0 )


if __name__ == '__main__':
    unittest.main()


