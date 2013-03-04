""" Module for the KMCProcess class """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.Utilities.CoordinateUtilities import centerCoordinates
from KMCLib.Utilities.CheckUtilities import checkCoordinateList
from KMCLib.Utilities.CheckUtilities import checkTypes
from KMCLib.Utilities.CheckUtilities import checkSequence
from KMCLib.Utilities.CheckUtilities import checkSequenceOfPositiveIntegers
from KMCLib.Utilities.CheckUtilities import checkPositiveInteger
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat

class KMCProcess(object):
    """
    Class for representing a possible process in a lattice KMC simulation.
    """

    def __init__(self,
                 coordinates=None,
                 elements_before=None,
                 elements_after=None,
                 basis_sites=None,
                 rate_constant=None):
        """
        Constructor for the KMCProcess.

        :param coordinates: The local coordinates, corresponding to the lattice
                            positions of the surrounding of the center where
                            the process will be preformed.

        :param elements_before: The elements, as a list of strings,
                                before the process is preformed.
                                This list of elements will be used to match the
                                local surroundings of each center in the
                                simulation, to determine at which sites the
                                process can be performed.

        :param elements_after: The elements, as a list of strings,
                               after the process is preformed.
                               This list of elements will be used to update the
                               local surrounding of the site where the process
                               is performed.

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
        self.__coordinates = centerCoordinates(coordinates, center)

        # Check the tyeps.
        self.__elements_before = checkTypes(elements_before, len(coordinates))
        self.__elements_after  = checkTypes(elements_after,  len(coordinates))

        # Check the list of basis sites.
        basis_sites = checkSequenceOfPositiveIntegers(basis_sites,
                                                      msg="The basis_sites must be given as a list of positive integers.")

        if len(basis_sites) == 0:
            msg = "The list of available basis sites for a process may not be empty."
        # Passed the  tests.
        self.__basis_Sites = basis_sites

        # Check the rate constant.
        self.__rate_constant = checkPositiveFloat(rate_constant,
                                                  default_parameter=None,
                                                  parameter_name="rate_constant")


