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
from KMCLib.Backend import Backend

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
control_parameters = KMCControlParameters(number_of_steps=2500000,
                                          dump_interval=10000,
                                          analysis_interval=100,
                                          seed=None)

# Setup the mean square displacement analysis.
msd_analysis = OnTheFlyMSD(history_steps=200,
                           n_bins=100,
                           t_max=2500.0,
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

        # Only master does the testing.

        if Backend.MPICommons.isMaster():
            # Save the analysis data to a file.
            with open('msd.data', 'w') as f:
                msd_analysis.printResults(f)

        Backend.MPICommons.barrier()

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
        std  = raw_data[:,8]

        # Setup the properly weighted least square fit problem.
        A = numpy.vstack([time, numpy.ones(len(time))]).T
        A = numpy.vstack([A[:,0]/std, A[:,1]/std]).T
        covariance = numpy.linalg.inv(numpy.dot(A.T,A))
        b = msd / std

        # Make the fit.
        fit = numpy.linalg.lstsq(A, b)

        # Get the diffusion coefficient and offset.
        D,offset  = fit[0]

        # Get the residuals.
        residuals = fit[1][0]

        # Calculate the standard deviation of D.
        std_dev_D = numpy.sqrt(covariance[0,0] * residuals / (float(len(time)-2)))

        # Calculate the standard deviation of the offset.
        std_dev_offset = numpy.sqrt(covariance[1,1] * residuals / (float(len(time)-2)))

        # print "[", D,",", offset,",", std_dev_D,",",std_dev_offset, "],"
        # Results from running the test several times with different seed values.
        results = numpy.array([[78.0988867187,  755.171438570, 2.25791330688, 150.7354059410],
                               [72.7368256248,  880.147443244, 1.73843101852, 116.3536556430],
                               [70.5977713569,  941.310653009, 1.41401143957,  93.3614358073],
                               [77.6769956876,  772.810589646, 1.69869328439, 105.3189492430],
                               [73.1090345846,  966.323452557, 1.86000016312, 129.8734748640],
                               [76.2778051313,  694.090234485, 1.62207453876,  97.7777662451],
                               [75.0672695992,  661.516187893, 1.35782209771,  78.5319533459],
                               [77.0892757225,  671.603530818, 1.21776951195,  68.6146562997],
                               [72.0285316265,  991.900289461, 1.98703833274, 143.8695303770],
                               [76.0471461070,  884.490882096, 1.85763227041, 123.6274325190],
                               [73.9135364736,  831.627688526, 1.61333123625, 103.5172709000],
                               [73.0875151593,  869.207618413, 1.47134336245,  94.2281655223],
                               [75.1563895771,  902.245502697, 1.70793889981, 112.4333018870],
                               [74.4132715742,  711.818819874, 1.28334435742,  75.2930669159],
                               [76.6591469495,  610.081433489, 1.36589976011,  76.7434023114],
                               [72.9957345559,  883.962983177, 1.85286664489, 125.9164794120],
                               [74.5376479586,  842.171393324, 1.67062853156, 107.8071690770],
                               [78.1138320689,  648.900172236, 1.61903227321,  94.3144612738],
                               [75.8776117412,  844.597049474, 2.17588165704, 150.2954225890],
                               [71.6892473565, 1035.882536680, 2.04077221728, 151.2769147800],
                               [72.2184440838,  948.540399846, 1.72098059269, 117.8501570350],
                               [76.0425903175,  786.133121472, 1.61231054779, 100.3629115530],
                               [76.0534894055,  851.329911170, 2.04113245958, 137.9356021100],
                               [77.5446972928,  605.758653061, 1.42893357104,  80.3909427002],
                               [77.3792735847,  729.695855993, 1.58829977343,  95.8039934268],
                               [72.6246860882,  846.506663791, 1.45058932652,  92.2514872505],
                               [76.9296878453,  809.534804563, 2.25559389800, 154.8579531090],
                               [77.6296177486,  730.908052866, 1.75706746900, 108.2313254510],
                               [73.3512776008,  916.182818139, 1.64139624023, 108.9108582280]])

        # Get the mean slope and the standard deviation from the series of tests.
        mean_slope = numpy.mean(results[:,0])
        stdev_mean_slope = numpy.std(results[:,0])
        mean_stdev = numpy.mean(results[:,2])
        stdev_mean_stdev = numpy.std(results[:,2])

        # Check that the slope is within two standard deviations from the mean.
        self.assertTrue( numpy.abs(mean_slope - D) < stdev_mean_slope*3.0 )

        # Make the same check for the estimated standard deviation.
        self.assertTrue( numpy.abs(mean_stdev - std_dev_D) < stdev_mean_stdev*3.0 )

        # Finally, check that the estimated standard deviation is close enough to the
        # standard deviation from the series of tests.
        print "sigma D: ", std_dev_D, "mean sigma: ", stdev_mean_slope, "sigma mean sigma: ", stdev_mean_stdev
        self.assertTrue( numpy.abs(stdev_mean_slope - std_dev_D) < stdev_mean_stdev*4.0 )


if __name__ == '__main__':
    unittest.main()

