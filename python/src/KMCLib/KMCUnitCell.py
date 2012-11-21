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

class KMCUnitCell:
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
        Qury for the bais points.

        :returns: The basis points stored on the class.
        """
        return self.__basis_points

