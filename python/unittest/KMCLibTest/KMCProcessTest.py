"""" Module for testing KMCProcess class """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest


from KMCLib.Exceptions.Error import Error

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

        ref_script = """
# -----------------------------------------------------------------------------
# Process

coordinates = [[   0.000000e+00,   0.000000e+00,   0.000000e+00],
               [   1.000000e+00,   2.000000e+00,   3.000000e+00]]

elements_before = ['A','B']

elements_after = ['B','A']

basis_sites = [0]

rate_constant =    1.000000e+00

process = KMCProcess(
    coordinates=coordinates,
    elements_before=elements_before,
    elements_after=elements_after,
    basis_sites=basis_sites,
    rate_constant=rate_constant)
"""
        self.assertEqual(ref_script, script)


if __name__ == '__main__':
    unittest.main()

