"""" Module for testing KMCProcess class """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest
import numpy

from KMCLib.Exceptions.Error import Error
from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration

# Import the module to test.
from KMCLib.KMCProcess import KMCProcess

# Implementing the tests.
class KMCProcessTest(unittest.TestCase):
    """ Class for testing the KMCProcess class """

    def testConstruction(self):
        """ Test that the KMCProcess class can be constructed. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        basis_sites = [0]

        # Construct.
        p = KMCProcess(coordinates=coordinates,
                       elements_before=elements_before,
                       elements_after=elements_after,
                       basis_sites=basis_sites,
                       rate_constant=1.0)

        # Check that we got a valid instance.
        self.assertTrue( isinstance(p, KMCProcess) )

    def testConstructionFailCoordinates(self):
        """ Test that construction fails with the wrong coordinates format. """
        # Set the input.
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        basis_sites = [0]

        # Fail on wrong dimension.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[[0.0, 0.0],[1.0, 2.0, 3.0]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )
        # Fail on wrong dimension.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[[0.0, 0.0, 1.3],[2.0, 3.0]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )
        # Fail on wrong dimension.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[[0.0, 0.0, 1.3, 3.4, 2.0, 3.0]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        # Fail on wrong type.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[[0, 0, 1], [3, 2, 3]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        # Fail on wrong type.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates="[[0, 0, 1], [3, 2, 3]]",
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        # Fail on wrong type.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=[["0.1", "0.4", "1.3"],
                                                            ["3.1", "2.1", "3"]],
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

    def testConstructionFailElements(self):
        """ Test that construction fails with the wrong elements format. """

        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        basis_sites = [0]

        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before="A,B",
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after="B,A",
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

    def testConstructionFailNumberOfSites(self):
        """ Test that construction fails mismatch in the number of sites. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0], [1.2,3.4,5.6]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A", "B"]
        basis_sites = [0]

        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        elements_before = ["A", "B", "B"]
        elements_after = ["B", "A"]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )


        coordinates = [[0.0, 0.0, 0.0], [1.2,3.4,5.6]]
        elements_before = ["A", "B", "D"]
        elements_after = ["B", "A", "B"]
        basis_sites = [0]

        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

    def testConstructionFailBasisSites(self):
        """ Test that construction fails with the wrong basis sites input. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]

        basis_sites = ["First"]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        basis_sites = 0
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

        basis_sites = [1.0, 2.0]
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=1.0) )

    def testConstructionFailRate(self):
        """ Test that construction fails with a wrong rate. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        basis_sites = [0]

        # Negative rate.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=-1.0) )
        # Integer rate.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=345) )
        # Wrong type.
        self.assertRaises( Error,
                           lambda : KMCProcess(coordinates=coordinates,
                                               elements_before=elements_before,
                                               elements_after=elements_after,
                                               basis_sites=basis_sites,
                                               rate_constant=[3.459]) )

    def testScript(self):
        """ Test that the process can generate its own valid script. """
        # Set the input.
        coordinates = [[0.0, 0.0, 0.0],[1.0,2.0,3.0]]
        elements_before = ["A", "B"]
        elements_after = ["B", "A"]
        basis_sites = [0]

        # Construct.
        p = KMCProcess(coordinates=coordinates,
                       elements_before=elements_before,
                       elements_after=elements_after,
                       basis_sites=basis_sites,
                       rate_constant=1.0)

        # Get the script.
        script = p._script()

        ref_script = """coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e+00,   2.000000e+00,   3.000000e+00]]

elements_before = ['A','B']
elements_after  = ['B','A']
basis_sites     = [0]
rate_constant   =    1.000000e+00

process = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)

"""

        self.assertEqual(ref_script, script)

    def testLocalConfigurations(self):
        """ Test that the local configurations are correctly set up """
        coordinates = numpy.array([[1.0,2.0,3.0], [2.3,5.5,3.2]])
        elements1 = ["First", "Second"]
        elements2 = ["Second", "One"]

        # Construct the process.
        process = KMCProcess(coordinates,
                             elements1,
                             elements2,
                             basis_sites=[9],
                             rate_constant=1.0)

        # Get the local configurations out.
        c1 = process.localConfigurations()[0]
        c2 = process.localConfigurations()[1]

        # Construct the two reference local configurations.
        ref_c1 = KMCLocalConfiguration(coordinates, elements1, center=0)
        ref_c2 = KMCLocalConfiguration(coordinates, elements2, center=0)

        # Check coordinates.
        self.assertAlmostEqual( numpy.linalg.norm(c1.coordinates() - ref_c1.coordinates()), 0.0, 12 )
        self.assertAlmostEqual( numpy.linalg.norm(c2.coordinates() - ref_c2.coordinates()), 0.0, 12 )

        # Check types.
        self.assertAlmostEqual( c1.types(), ref_c1.types() )
        self.assertAlmostEqual( c2.types(), ref_c2.types() )

    # FIXME : Needs implementation in the process class.
    def notestConstructionFailWildcardMove(self):
        """ Test for failure if wildcards change place in the move. """
        # This should fail.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["*","B"]
        types1 = ["B","*"]
        sites  = [0]
        rate_0_1 = 3.5
        self.assertRaises( Error,
                           lambda : KMCProcess(coords, types0, types1, sites, rate_0_1) )

    # FIXME : Needs implementation in the process class.
    def notestConstructionFailNoMove(self):
        """ Test for failure if no move takes place. """
        # This should fail.
        coords = [[1.0,2.0,3.4],[1.1,1.2,1.3]]
        types0 = ["A","B"]
        types1 = ["A","B"]
        sites  = [0]
        rate_0_1 = 3.5
        self.assertRaises( Error,
                           lambda : KMCProcess(coords, types0, types1, sites, rate_0_1) )


if __name__ == '__main__':
    unittest.main()

