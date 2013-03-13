""" Module for the KMCProcess class """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import numpy

from KMCLib.Utilities.CoordinateUtilities import centerCoordinates
from KMCLib.Utilities.CheckUtilities import checkCoordinateList
from KMCLib.Utilities.CheckUtilities import checkTypes
from KMCLib.Utilities.CheckUtilities import checkSequence
from KMCLib.Utilities.CheckUtilities import checkSequenceOfPositiveIntegers
from KMCLib.Utilities.CheckUtilities import checkPositiveInteger
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat
from KMCLib.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.Exceptions.Error import Error

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
        elements_before = checkTypes(elements_before, len(coordinates))
        elements_after  = checkTypes(elements_after,  len(coordinates))

        # Check that the types represents a valid move.
        self.__checkValidMove(elements_before, elements_after)

        # All types checking done.
        self.__elements_before = elements_before
        self.__elements_after  = elements_after

        # Check the list of basis sites.
        basis_sites = checkSequenceOfPositiveIntegers(basis_sites,
                                                      msg="The basis_sites must be given as a list of positive integers.")

        if len(basis_sites) == 0:
            msg = "The list of available basis sites for a process may not be empty."
        # Passed the  tests.
        self.__basis_sites = basis_sites

        # Check the rate constant.
        self.__rate_constant = checkPositiveFloat(rate_constant,
                                                  default_parameter=None,
                                                  parameter_name="rate_constant")
        # Setup the local configurations.
        c1 = KMCLocalConfiguration(self.__coordinates, self.__elements_before, center)
        c2 = KMCLocalConfiguration(self.__coordinates, self.__elements_after,  center)
        self.__local_configurations = (c1, c2)

    def __checkValidMove(self, elements_before, elements_after):
        """
        Private helper function to check if a move is valid with respect
        to atom types and wildcards.

        :param elements_before: The list of elements before the move.

        :param elements_after: The list of elements after the move.
        """
        # Check that the wildcards, if any, are not moved.
        before = [ e == "*" for e in elements_before ]
        after  = [ e == "*" for e in elements_after ]

        if len(before) != len(after) or before != after:
            raise Error("Wildcards must not move during a valid process.")

        # Check that they are not identical.
        if elements_before == elements_after:
            raise Error("The atomic configuration before and after a move can not be identical.")

    def __eq__(self, other):
        """ Implements the equal oprator. """

        # Check the length of the basis sites.
        if len(other.basisSites()) != len(self.basisSites()):
            return False

        # Check the basis sites.
        elif not all([s1 == s2 for s1,s2 in zip(other.basisSites(),self.basisSites())]):
            return False

        # Check the number of atoms in the local configurations.
        elif numpy.shape(other.localConfigurations()[0].coordinates())[0] != \
                numpy.shape(self.localConfigurations()[0].coordinates())[0]:
            return False

        # Check the coordinates.
        elif numpy.linalg.norm(other.localConfigurations()[0].coordinates() - \
                                   self.localConfigurations()[0].coordinates()) > 0.00001:
            return False

        # Check the types.
        elif not all([s1 == s2 for s1,s2 in zip(other.localConfigurations()[0].types(),
                                                self.localConfigurations()[0].types())]):
            return False
        elif not all([s1 == s2 for s1,s2 in zip(other.localConfigurations()[1].types(),
                                                self.localConfigurations()[1].types())]):
            return False
        # Passed all tests, return true.
        else:
            return True

    def localConfigurations(self):
        """
        Query for the local configurations.

        :returns: The two local configurations in a tuple.
        """
        return self.__local_configurations

    def basisSites(self):
        """
        Query for the basis sites.

        :returns: The basis sites as stored on the class. Returns None if
                  no basis sites were given on construction.
        """
        return self.__basis_sites

    def rateConstant(self):
        """
        Query for the rate constant.

        :returns: The rate constant stored on the class.
        :rtype: float
        """
        return self.__rate_constant

    def _script(self, variable_name="process"):
        """
        Generate a script representation of an instance.

        :param variable_name: A name to use as variable name for
                              the KMCProcess in the generated script.
        :type variable_name: str

        :returns: A script that can generate this process object.
        """
        # Define the float format string.
        ff = "%15.6e"

        # Setup the coordinates string.
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

        # Setup the elements before string.
        elements_before_string = "elements_before = "
        indent = " "*19
        line = "["
        nT = len(self.__elements_before)
        for i,t in enumerate(self.__elements_before):
            # Add the type.
            line += "'" + t + "'"
            if i == nT-1:
                # Stop if we reach the end.
                line += "]\n"
                elements_before_string += line
                break
            else:
            # Add the separator.
                line += ","

            # Check if we should add a new line.
            if len(line) > 50:
                elements_before_string += line + "\n" + indent
                line = ""

        # Setup the elements after string.
        elements_after_string = "elements_after  = "
        indent = " "*19
        line = "["
        nT = len(self.__elements_after)
        for i,t in enumerate(self.__elements_after):
            # Add the type.
            line += "'" + t + "'"
            if i == nT-1:
                # Stop if we reach the end.
                line += "]\n"
                elements_after_string += line
                break
            else:
            # Add the separator.
                line += ","

            # Check if we should add a new line.
            if len(line) > 50:
                elements_after_string += line + "\n" + indent
                line = ""

        # Setup the basis sites list.
        basis_sites_string = "basis_sites     = ["
        for j,b in enumerate(self.__basis_sites):
            if j == (len(self.__basis_sites)-1):
                basis_sites_string += "%i]"%(b)
            else:
                basis_sites_string += "%i,"%(b)

        # Setup the rate constant string.
        rate_constant_string = "rate_constant   = " + ff
        rate_constant_string = rate_constant_string%(self.__rate_constant)

        # Get the script together.
        process_string = variable_name + " = KMCProcess(\n" + \
            "    coordinates=coordinates,\n" + \
            "    elements_before=elements_before,\n" + \
            "    elements_after=elements_after,\n" + \
            "    basis_sites=basis_sites,\n" + \
            "    rate_constant=rate_constant)\n"

        return coords_string + "\n" + \
            elements_before_string + \
            elements_after_string  + \
            basis_sites_string     + "\n" + \
            rate_constant_string   + "\n\n" + \
            process_string + "\n"

