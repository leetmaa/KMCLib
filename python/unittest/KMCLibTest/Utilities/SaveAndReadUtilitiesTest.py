""" Module for testing the common save and read utilities. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import os
import numpy

from KMCLib.CoreComponents.KMCConfiguration import KMCConfiguration
from KMCLib.CoreComponents.KMCLattice import KMCLattice
from KMCLib.CoreComponents.KMCUnitCell import KMCUnitCell
from KMCLib.CoreComponents.KMCLatticeModel import KMCLatticeModel
from KMCLib.CoreComponents.KMCInteractions import KMCInteractions
from KMCLib.Exceptions.Error import Error

# Import from the module we test.
from KMCLib.Utilities.SaveAndReadUtilities import KMCConfigurationFromScript
from KMCLib.Utilities.SaveAndReadUtilities import KMCInteractionsFromScript
from KMCLib.Utilities.SaveAndReadUtilities import KMCLatticeModelFromScript
from KMCLib.Utilities.SaveAndReadUtilities import getScriptComponent


# Implement the test.
class SaveAndReadUtilitiesTest(unittest.TestCase):
    """ Class for testing the save and read utility functions. """

    def testKMCConfigurationFromScript(self):
        """ Test the function that reads a KMCConfiguration from a script. """
        # Set the path to the file to read from.
        module_path = os.path.abspath(os.path.dirname(__file__))
        script_file_path = os.path.join(module_path,"..","TestUtilities","Scripts")
        script_file_path = os.path.join(script_file_path, "kmc_configuration_script.py")

        # Make sure the file exists.
        self.assertTrue( os.path.exists(script_file_path) )

        # Now, get the KMCConfiguration out of the script.
        configuration = KMCConfigurationFromScript(script_file_path)

        # Make sure that it is of the correct type.
        self.assertTrue( isinstance(configuration, KMCConfiguration) )

    def testKMCInteractionsFromScript(self):
        """ Test the function that reads a KMCInteractions from a script. """
        # Set the path to the file to read from.
        module_path = os.path.abspath(os.path.dirname(__file__))
        script_file_path = os.path.join(module_path,"..","TestUtilities","Scripts")
        script_file_path = os.path.join(script_file_path, "kmc_lattice_model_script.py")

        # Make sure the file exists.
        self.assertTrue( os.path.exists(script_file_path) )

        # Now, get the KMCInteractions out of the script.
        configuration = KMCInteractionsFromScript(script_file_path)

        # Make sure that it is of the correct type.
        self.assertTrue( isinstance(configuration, KMCInteractions) )

    def testKMCLatticeModelFromScript(self):
        """ Test the function that reads a KMCLatticeModel from a script. """
        # Set the path to the file to read from.
        module_path = os.path.abspath(os.path.dirname(__file__))
        script_file_path = os.path.join(module_path,"..","TestUtilities","Scripts")
        script_file_path = os.path.join(script_file_path, "kmc_lattice_model_script.py")

        # Make sure the file exists.
        self.assertTrue( os.path.exists(script_file_path) )

        # Now, get the KMCLatticeModel out of the script.
        configuration = KMCLatticeModelFromScript(script_file_path)

        # Make sure that it is of the correct type.
        self.assertTrue( isinstance(configuration, KMCLatticeModel) )

    def testGetScriptComponent(self):
        """ Test the general get script component function. """
        # Set the path to the file to read from.
        module_path = os.path.abspath(os.path.dirname(__file__))
        script_file_path = os.path.join(module_path,"..","TestUtilities","Scripts")
        script_file_path = os.path.join(script_file_path, "kmc_configuration_script.py")
        # Make sure the file exists.
        self.assertTrue( os.path.exists(script_file_path) )

        # Get the KMCLattice out of this script.
        lattice = getScriptComponent(script_file_path, KMCLattice)

        # Check that it is the correct lattice by cheking its values
        # against these known references.
        unit_cell = KMCUnitCell(cell_vectors=[[2.8,0.0,0.0],
                                              [0.0,3.2,0.0],
                                              [0.0,0.5,3.0]],
                                basis_points=[[0.0,0.0,0.0],
                                              [0.5,0.5,0.5],
                                              [0.25,0.25,0.75]])
        ref_lattice = KMCLattice(unit_cell=unit_cell,
                                 repetitions=(4,4,1),
                                 periodic=(True,True,False))

        self.assertEqual( lattice.repetitions(), ref_lattice.repetitions() )
        self.assertEqual( lattice.periodic(), ref_lattice.periodic() )
        self.assertAlmostEqual( numpy.linalg.norm(numpy.array(lattice.sites())-numpy.array(ref_lattice.sites())), 0.0, 12 )
        self.assertEqual( numpy.linalg.norm(numpy.array(lattice.basis()) - numpy.array(ref_lattice.basis())), 0.0, 12 )

        # Check that the function returns None if no component of the correct type is found.
        self.assertTrue( (getScriptComponent(script_file_path, numpy.ndarray) is None) )

        # Check that we fail in a controlled way if there is a problme with the file.
        self.assertRaises( Error,
                           lambda: getScriptComponent("THIS IS NOT A VALID PATH", numpy.ndarray) )


if __name__ == '__main__':
    unittest.main()
