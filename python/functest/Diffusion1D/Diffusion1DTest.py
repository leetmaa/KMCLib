""" Module for testing a 1D diffusion system. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import numpy

# Import the interface.
from KMCLib import *

# Setup a system, a periodic 10 atoms long 1D chain with 5.2 Angstrom
# between adjacent sites.
unit_cell = KMCUnitCell(cell_vectors=numpy.array([[5.2,0.0,0.0],
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
                rate_constant=1.5)

# Setup a diffusion process to the right.
coordinates_p1 = [[0.0, 0.0, 0.0],[1.0, 0.0, 0.0]]
p1 = KMCProcess(coordinates=coordinates_p1,
                elements_before=["B","A"],
                elements_after=["A","B"],
                move_vectors=None,
                basis_sites=[0],
                rate_constant=1.5)

# Construct the interactions object.
interactions = KMCInteractions(processes=[p0, p1],
                               implicit_wildcards=True)

# Generate the KMC model to run.
model = KMCLatticeModel(configuration=config,
                        interactions=interactions)

# Setup the control parameters, note that not specifting
# a seed value will result in the wall clock time seeding,
# so we would expect slightly different results each time
# we run this test.
control_parameters = KMCControlParameters(number_of_steps=250000000,
                                          dump_interval=1000,
                                          analysis_interval=100,
                                          seed=100)

# Setup the mean square displacement analysis.
msd_analysis = OnTheFlyMSD(history_steps=2000,
                           n_bins=1000,
                           t_max=250000.0,
                           track_type="B")

# Implement the test.
class Diffusion1DTest(unittest.TestCase):
    """ Class for testing 1D diffusion. """

    def testRun(self):
        """ Run the model and check the results. """
        # Run the model.
        model.run(control_parameters=control_parameters,
                  trajectory_filename="traj.py",
                  analysis=[msd_analysis])

        # Save the analysis data to a file.
        with open('msd.data', 'w') as f:
            msd_analysis.printResults(f)

        # Read in the file content.
        with open('msd.data', 'r') as f:
            content = f.readlines()
        raw_data = []
        for row in content[1:]:
            row = [float(r) for r in row.split('\n')[0].split(' ') if r != '']
            raw_data.append(row)
        raw_data = numpy.array(raw_data)

        # Fit a straight line to the data to get the diffusion coefficient.
        time = raw_data[:,0]
        msd  = raw_data[:,1]
        std  = raw_data[:,4]

        # Setup the properly weighted least square fit problem.
        A = numpy.vstack([time, numpy.ones(len(time))]).T
        A = numpy.vstack([A[:,0]/std, A[:,1]/std]).T
        b = msd / std

        # Get the diffusion coefficient and offset.
        D,offset = numpy.linalg.lstsq(A, b)[0]

        print D, offset

        # Results from running 250000 steps with 2000 steps history and
        # n_bins=1000, t_max=25000.0
        #
        #results = numpy.array([[82.6837068639,  54.261838519],
        #                       [86.5362326057, -55.9715804318],
        #                       [84.9635281076, -47.879847881],
        #                       [79.916652932 ,  35.4019518935],
        #                       [78.8206354785,  55.6999374782],
        #                       [85.1208671296, -34.9492897414],
        #                       [81.1387647664,  31.1402457678],
        #                       [80.6873741906,  25.3224272086],
        #                       [79.8158843209,  35.7711225873],
        #                       [87.4714115363, -73.1463067339]])
        # mean_D = sum(results[:,0])/len(results[:,0])
        # mean_offset = sum(results[:,1])/len(results[:,1])
        # 82.7155057931 2.56504986663

        # Results from running 2500000 steps with 2000 steps history and
        # n_bins=1000, t_max=25000.0
        # results = numpy.array([[79.0072155514, 47.7091837968],
        #                       [80.1695629233, 39.5531038871],
        #                       [83.0929585558, 6.45145889059],
        #                       [82.4971868124, 11.6262487783],
        #                       [79.9680707384, 36.152047566],
        #                       [81.6999382529, 18.8548968346],
        #                       [80.8642220369, 33.8065773959],
        #                       [82.1722742903, 16.8027196243],
        #                       [81.9871679636, 20.1542031522],
        #                       [79.9211352831, 31.2985453246]])
        #mean_D = sum(results[:,0])/len(results[:,0])
        #mean_offset = sum(results[:,1])/len(results[:,1])
        #print mean_D, mean_offset
        # 81.1379732408 26.240898525

if __name__ == '__main__':
    unittest.main()

