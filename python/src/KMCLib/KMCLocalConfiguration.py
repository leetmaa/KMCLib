""" Module for the KMCLocalConfiguration """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.Utilities.CheckUtilities import checkCoordinateList
from KMCLib.Utilities.CheckUtilities import checkIndexWithinBounds
from KMCLib.Utilities.CheckUtilities import checkTypes

from KMCLib.Utilities.CoordinateUtilities import centerCoordinates
from KMCLib.Utilities.CoordinateUtilities import sortCoordinates


class KMCLocalConfiguration:
    """
    Class for representing the local environment around a central site
    in a Lattice KMC simulation.
    """

    def __init__(self,
                 coordinates=None,
                 types=None,
                 center=None):
        """
        Constructor for the KMCLocalConfiguration.

        :param coordinates: The coordinates of the configuration.
        :type coordinates:  A 3xN sequence of floating point numbers, where N is the number of sites.

        :param types: The lattice site types at the specified coordinates.
        :type types:  A sequence of strings of length N.

        :param center: The coordinate in the list to treat as the central site. If not given it will default to the first coordinate.
        :type center:  int
        """
        # Check the coordinates.
        coordinates = checkCoordinateList(coordinates)

        # Set the center coordinate if not given.
        if center is None:
            center = 0

        # Check the bounds of the center coordinate.
        center = checkIndexWithinBounds(center, coordinates, msg="The 'center' index paramter must be one in the coordinate list.")

        # Center the coordinates.
        coordinates = centerCoordinates(coordinates, center)

        # Check the tyeps.
        types = checkTypes(types, len(coordinates))

        # Sort the coordinates with respect to distance from the center and store on the class.
        (self.__coordinates, self.__distances, self.__types) = sortCoordinates(coordinates, center, types)

    def coordinates(self):
        """
        Query function for the coordinates stored on the class.

        :returns: The stored coordinates.
        """
        return self.__coordinates

    def types(self):
        """
        Query function for the types stored on the class.

        :returns: The stored types.
        """
        return self.__types

    def distances(self):
        """
        Query function for the distances from the center.
        """
        return self.__distances

    def _script(self, variable_name="local_configuration"):
        """
        Generate a script reperesentation of an isntance.

        :param variable_name: A name to use as variable name for KMCLocalConfiguration in the generated script.
        :type variable_name: string

        :returns: A script that can generate this local configuration.
        """
        # Define the float format string.
        ff = "%15.6e"

        # Add the coordinates.
        varname = "coordinates"
        indent = " "*len(varname) + "    "

        coords_string = varname + " = ["

        # Loop through all coordinates but the last.
        coord_template = "[" + ff + "," + ff + "," + ff + "],\n"

        # For the first coordinate, if there are more than one coordinate.
        if len(self.__coordinates) > 1:
            c = self.__coordinates[0]
            coords_string += coord_template%(c[0],c[1],c[2])

            # And the middle coordinates.
            coord_template = indent + "[" + ff + "," + ff + "," + ff + "],\n"
            for c in self.__coordinates[1:-1]:
                coords_string += coord_template%(c[0],c[1],c[2])

        # Add the last coordinate (which is also the first if there is only one coordinate).
        c = self.__coordinates[-1]
        if len(self.__coordinates) == 1:
            coord_template = "[" + ff + "," + ff + "," + ff + "]]\n"
        else:
            coord_template = indent + "[" + ff + "," + ff + "," + ff + "]]\n"
        coords_string += coord_template%(c[0],c[1],c[2])

        # Types
        types_string  = "types = " + str(self.__types) + "\n"

        # Add the configuration.
        config_string = variable_name + """ = KMCLocalConfiguration(
    coordinates=coordinates,
    types=types)
"""

        # Return the script.
        return "\n" + coords_string + "\n" + types_string + "\n" + config_string

