""" Module containing error handling. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy


from KMCLib.Utilities.CheckUtilities import checkCellVectors
from KMCLib.Utilities.CheckUtilities import checkCoordinateList

class UnitCell:
    """ Class for describing a unitcell. """

    def __init__(self,
                 cell_vectors=None,
                 basis_points=None):
        """
        Constructor for the UnitCell class.

        :param cell_vectors: A 3x3 matrix, nested lists or numpy array with
                             rows corresponding to the a, b and c vectors in
                             cartesian coordinates.

        :param basis_points: A Nx3 matrix, nested lists or numpy array with
                             each row corresponding to the fractional coordinates
                             of a basis point.
        """
        # Check the cell vectors.
        self.__cell_vectors = checkCellVectors(cell_vectors)

        # Check the basis points.
        self.__basis_points = checkCoordinateList(basis_points, varname="basis_points")

