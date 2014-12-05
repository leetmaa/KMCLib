""" Module for testing a 3D diffusion system. """

# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import numpy
import os

# Import the interface.
from KMCLib import *
from KMCLib.Backend import Backend


# Implement the test.
class Diffusion3DTest(unittest.TestCase):
    """ Class for testing 3D diffusion. """

    def testRun(self):
        """ Run the model and check the results. """
        # Load the basic interactions and the initial configuration.
        directory = os.path.abspath(os.path.dirname(__file__))
        p_file    = os.path.join(directory, "processes.py")
        c_file    = os.path.join(directory,"config.py")
        interactions  = KMCInteractionsFromScript(p_file)
        configuration = KMCConfigurationFromScript(c_file)

        # Set the parameters.
        control_parameters = KMCControlParameters(number_of_steps=300000,
                                                  dump_interval=1000,
                                                  analysis_interval=1,
                                                  seed=1994669)

        # Setup the MSD analysis for tracking vacancy diffusion.
        msd_analysis = OnTheFlyMSD(history_steps=200,
                                   n_bins=100,
                                   t_max=100.0,
                                   track_type="V")

        # Put the model together and run.
        model = KMCLatticeModel(configuration, interactions)
        model.run(control_parameters,
                  trajectory_filename="traj.py",
                  analysis=[msd_analysis])

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
        time = raw_data[:,0]

        # X direction.
        msd = raw_data[:,1]
        std = raw_data[:,8]
        # Confined dynamics in the X direction. Slope at large time lag
        # significantly smaller than slope at small time lag.
        D_x1 = fit(time[0:15],  msd[0:15],  std[0:15])
        D_x2 = fit(time[16:29], msd[16:29], std[16:29])

        # Y direction.
        msd = raw_data[:,2]
        std = raw_data[:,9]
        # Flow motion in the Y direction. Slope is contant.
        D_y1 = fit(time[0:15],  msd[0:15],  std[0:15])
        D_y2 = fit(time[16:29], msd[16:29], std[16:29])

        # Z direction.
        msd = raw_data[:,3]
        std = raw_data[:,10]
        # Flow motion in the Z direction. Slope at large time lag
        # significantly larger than slope at small time lag.
        D_z1 = fit(time[0:15],  msd[0:15],  std[0:15])
        D_z2 = fit(time[16:29], msd[16:29], std[16:29])

        self.assertAlmostEqual(D_x1,  9.6904604529, 10)
        self.assertAlmostEqual(D_x2, 5.28485172992, 10)
        self.assertAlmostEqual(D_y1, 12.2434598296, 10)
        self.assertAlmostEqual(D_y2, 12.4080915647, 10)
        self.assertAlmostEqual(D_z1, 13.2233073435, 10)
        self.assertAlmostEqual(D_z2, 19.1751372532, 10)


def fit(time, msd, std):
    """ Helper function to get the slope of a msd curve with given errors."""
    # Setup the properly weighted least square fit problem.
    A = numpy.vstack([time, numpy.ones(len(time))]).T
    A = numpy.vstack([A[:,0]/std, A[:,1]/std]).T
    covariance = numpy.linalg.inv(numpy.dot(A.T,A))
    b = msd / std

    # Make the fit.
    fit = numpy.linalg.lstsq(A, b)

    # Get the diffusion coefficient and offset.
    D,offset  = fit[0]
    return D


if __name__ == '__main__':
    unittest.main()
