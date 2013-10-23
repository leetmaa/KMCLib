""" Module containing the KMCUnitCell class. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy

from KMCLib.Exceptions.Error import Error
from KMCLib.Utilities.CheckUtilities import checkCellVectors
from KMCLib.Utilities.CheckUtilities import checkCoordinateList

class KMCUnitCell(object):
    """ Class for describing a unitcell. """

    def __init__(self,
                 cell_vectors=None,
                 basis_points=None):
        """
        Constructor for the KMCUnitCell class.

        :param cell_vectors: A 3x3 matrix, nested lists or numpy array with
                             rows corresponding to the a, b and c vectors in
                             cartesian coordinates.

        :param basis_points: A Nx3 matrix, nested lists or numpy array with
                             each row corresponding to the fractional coordinates
                             of a basis point.
        """
        # Check the cell vectors.
        self.__cell_vectors = checkCellVectors(cell_vectors)

        # Check the basis points are of correct format.
        basis_points = checkCoordinateList(basis_points, varname="basis_points")

        # Check that the basis points are given in fractional coordinates.
        for point in basis_points:
            if not all([b < 1.0 and b >= 0.0 for b in point]):
                raise Error("All basis points must be given in fractional coordinates in the range 0.0 <= basis_point < 1.0.")

        self.__basis_points = basis_points

    def basis(self):
        """
        Query for the bais points.

        :returns: The basis points stored on the class.
        """
        return self.__basis_points

    def cellVectors(self):
        """
        Query for the cell vectors.

        :returns: The cell vectors stored on the class.
        """
        return self.__cell_vectors

    def transformToCartesian(self, r):
        """
        Transform an internal coordinate list to cartesian coordinates.

        :returns: The coordinates in cartesian units.
        """
        return numpy.dot(r, self.__cell_vectors)

    def _script(self, variable_name="unit_cell"):
        """
        Generate a script representation of an instance.

        :param variable_name: A name to use as variable name for
                              the KMCUnitCell in the generated script.
        :type variable_name: str

        :returns: A script that can generate this unit cell.
        """
        # Define the float format string.
        ff = "%15.6e"

        # Setup the cell vectors script part.
        indent = " "*16
        cell_vector_a_str = "[" + ff + "," + ff + "," + ff + "],\n"
        cell_vector_b_str = indent + "[" + ff + "," + ff + "," + ff + "],\n"
        cell_vector_c_str = indent + "[" + ff + "," + ff + "," + ff + "]]\n"

        cell_vector_a = cell_vector_a_str%(self.__cell_vectors[0][0], self.__cell_vectors[0][1], self.__cell_vectors[0][2])
        cell_vector_b = cell_vector_b_str%(self.__cell_vectors[1][0], self.__cell_vectors[1][1], self.__cell_vectors[1][2])
        cell_vector_c = cell_vector_c_str%(self.__cell_vectors[2][0], self.__cell_vectors[2][1], self.__cell_vectors[2][2])
        cell_string = "cell_vectors = [" + cell_vector_a + cell_vector_b + cell_vector_c

        # Setup the basis points scipt part.
        basis_string = "basis_points = ["

        # Loop over and add the basis points.
        basis_point_str = "[" + ff + "," + ff + "," + ff + "]"
        for i,point in enumerate(self.__basis_points):
            if (i == 0):
                basis_string += basis_point_str%(point[0], point[1], point[2])
            else:
                basis_string += indent + basis_point_str%(point[0], point[1], point[2])

            if len(self.__basis_points) > i+1:
                basis_string += ",\n"
            else:
                basis_string += "]\n"

        # Construct the KMCUnitCell script part.
        unit_cell_string = variable_name + """ = KMCUnitCell(
    cell_vectors=cell_vectors,
    basis_points=basis_points)
"""
        # Add the comment.
        comment_string = """
# -----------------------------------------------------------------------------
# Unit cell

"""
        # Return the script.
        return comment_string + cell_string + "\n" + basis_string + "\n" + unit_cell_string



