""" Module for testing the on-the-fly MSD functionality. """


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
class OnTheFlyMSDTest(unittest.TestCase):
    """ Class for testing 1D diffusion. """

    def test1DRandomWalk(self):
        """ Run the model and check the results. """

        # Setup a system, a periodic 10 atoms long 1D chain.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[1.0,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0]])

        # And a lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(10,1,1),
                             periodic=(True,False,False))

        # Setup an initial configuration with one B in a sea of A:s.
        types = ["A"]*10
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

        interactions = KMCInteractions(processes=[p0, p1],
                                       implicit_wildcards=True)

        model = KMCLatticeModel(configuration=config,
                                interactions=interactions)

        # Setup the analysis.
        msd = OnTheFlyMSD(history_steps=400,
                          n_bins=1000,
                          t_max=2500.0,
                          track_type="B")

        # Setup the control parameters.
        control_parameters = KMCControlParameters(number_of_steps=400000,
                                                  dump_interval=1000,
                                                  analysis_interval=10)
        # Run the model.
        model.run(control_parameters=control_parameters,
                  analysis=[msd])

        with open('msd.data1', 'w') as f:
            msd.printResult(f)

        # NEEDS IMPLEMENTATION

    def test1DLongRandomWalk(self):
        """ Run the model and check the results. """
        # Setup a system, a periodic 1000 atoms long 1D chain.
        unit_cell = KMCUnitCell(cell_vectors=numpy.array([[1.0,0.0,0.0],
                                                          [0.0,1.0,0.0],
                                                          [0.0,0.0,1.0]]),
                                basis_points=[[0.0,0.0,0.0]])

        # And a lattice.
        lattice = KMCLattice(unit_cell=unit_cell,
                             repetitions=(1000,1,1),
                             periodic=(True,False,False))

        # Setup an initial configuration with one B in a sea of A:s.
        types = ["A"]*1000
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

        interactions = KMCInteractions(processes=[p0, p1],
                                       implicit_wildcards=True)

        model = KMCLatticeModel(configuration=config,
                                interactions=interactions)

        # Setup the analysis.
        msd = OnTheFlyMSD(history_steps=4000,
                          n_bins=1000,
                          t_max=2500.0,
                          track_type="B")

        # Setup the control parameters.
        control_parameters = KMCControlParameters(number_of_steps=40000,
                                                  dump_interval=1000,
                                                  analysis_interval=1)
        # Run the model.
        model.run(control_parameters=control_parameters,
                  trajectory_filename="traj.py",
                  analysis=[msd])

        with open('msd.data2', 'w') as f:
            msd.printResult(f)

        # NEEDS IMPLEMENTATION


if __name__ == '__main__':
    unittest.main()


