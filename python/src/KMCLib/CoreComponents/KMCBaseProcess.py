""" Module for the KMCBaseProcess class """


# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import numpy

from KMCLib.Utilities.CoordinateUtilities import centerCoordinates
from KMCLib.Utilities.CheckUtilities import checkCoordinateList
from KMCLib.Utilities.CheckUtilities import checkSequenceOfPositiveIntegers
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat

class KMCBaseProcess(object):
    """
    Class for representing a process base class in a lattice KMC simulation.
    """

    def __init__(self,
                 coordinates=None,
                 basis_sites=None,
                 rate_constant=None):
        """
        Constructor for the KMCBaseProcess.

        :param coordinates: The local coordinates, corresponding to the lattice
                            positions of the surrounding of the center where
                            the process will be preformed.

        :param basis_sites: The basis sites in the lattice at which the process
                            can possibly be applied. Only if the length of this
                            list is 1 can implicit wildcards be used for the
                            matching.

        :param rate_constant: The rate constant associated with this process.
        :type rate_constant: float

        """
        # Check the coordinates.
        coordinates = checkCoordinateList(coordinates)

        # Center the coordinates on the first entry.
        center = 0
        self._coordinates = centerCoordinates(coordinates, center)

        # Check the list of basis sites.
        basis_sites = checkSequenceOfPositiveIntegers(basis_sites,
                                                      msg="The basis_sites must be given as a list of positive integers.")

        if len(basis_sites) == 0:
            msg = "The list of available basis sites for a process may not be empty."

        # Passed the  tests.
        self._basis_sites = basis_sites

        # Check the rate constant.
        self._rate_constant = checkPositiveFloat(rate_constant,
                                                 default_parameter=None,
                                                 parameter_name="rate_constant")

        # To be updated by the derrived classes.
        self._elements_before      = None
        self._elements_after       = None
        self._move_vectors         = None
        self._all_present_types    = None
        self._local_configurations = None

    def basisSites(self):
        """
        Query for the basis sites.

        :returns: The basis sites as stored on the class. Returns None if
                  no basis sites were given on construction.
        """
        return self._basis_sites

    def rateConstant(self):
        """
        Query for the rate constant.

        :returns: The rate constant stored on the class.
        :rtype: float
        """
        return self._rate_constant

    def allPresentTypes(self):
        """
        Query for the list of all present element types.
        """
        return self._all_present_types

    def localConfigurations(self):
        """
        Query for the local configurations.

        :returns: The two local configurations in a tuple.
        """
        return self._local_configurations

    def moveVectors(self):
        """
        Query for the move vectors.

        :returns: The move vectors stored on the class.
        """
        return self._move_vectors
