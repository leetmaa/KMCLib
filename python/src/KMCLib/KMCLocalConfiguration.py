""" Module for the KMCLocalConfiguration """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.Utilities.CheckUtilities import checkCoordinateList
from KMCLib.Utilities.CheckUtilities import checkIndexWithinBounds
from KMCLib.Utilities.CoordinateUtilities import centerCoordinates
from KMCLib.Utilities.CoordinateUtilities import sortCoordinates


class KMCLocalConfiguration:
    """
    Class for representing the local environment around a central site
    in a Lattice KMC simulation.
    """

    def __init__(self,
                 cartesian_coordinates=None,
                 types=None,
                 center=None):
        """
        Constructor for the KMCLocalConfiguration.

        @param cartesian_coordinates: The cartesian coordinates of the configuration.
        @type                       : A 3xN sequence of floating point numbers.
        @default                    : None

        @param types                : The lattice site types at the specified coordinates.
        @type                       : A sequence of strings of length N.
        @default                    : None

        @param center               : The coordinate in the list to treat as the central site.
        @type                       : Integer
        @default                    : 0
        """
        # Check the cartesian coordinates.
        self.__cartesian_coordinates = checkCoordinateList(cartesian_coordinates)

        # Set the center coordinate if not given.
        if center is None:
            center = 0

        # Check the bounds of the center coordinate.
        center = checkIndexWithinBounds(center, cartesian_coordinates)

        # Center the coordinates.
        cartesian_coordinates = centerCoordinates(cartesian_coordinates, center)

        # Sort the coordinates with respect to distance from the center and store on the class.
        (self.__cartesain_coordinates, self.__types) = sortCoordinates(cartesian_coordinates, center, types)


