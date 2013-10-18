""" Module for holding common type conversion utility functions. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy

from KMCLib.Backend import Backend


def stringListToStdVectorString(string_list):
    """
    Converts a list of strings to a std::vector<std::string> object.

    :param string_list: The list of strings.

    :returns: A corresponding std::vector<std::string> object.
    """
    # Get the size.
    size = len(string_list)
    # Setup the c++ object.
    cpp_list = Backend.StdVectorString()
    # Copy values over.
    for s in string_list:
        cpp_list.push_back(s);
    # Done.
    return cpp_list


def numpy2DArrayToStdVectorStdVectorDouble(array):
    """
    Convert a 2D numpy array to a std::vector<std::vector<double> > representation.

    :param array: The array to convert.

    :returns: A corresponding std::vector<std::vector<double> > object.
    """
    # Get the shape of the array.
    shape = numpy.shape(array)
    nI = shape[0]
    nJ = shape[1]

    # Setup the c++ object.
    cpp_2D_vector = Backend.StdVectorStdVectorDouble(nI)

    # Copy the values over.
    for i in range(nI):
        cpp_vector = Backend.StdVectorDouble(nJ)
        for j in range(nJ):
            cpp_vector[j] = array[i][j]
        cpp_2D_vector[i] = cpp_vector

    # Done.
    return cpp_2D_vector


def stdVectorCoordinateToNumpy2DArray(cpp_vector):
    """
    Convert a std::vector<Coordinate> to a nx3 numpy array.
    """
    size = len(cpp_vector)
    py_data = numpy.zeros((size, 3))

    for i in range(size):
        py_data[i][0] = cpp_vector[i].x()
        py_data[i][1] = cpp_vector[i].y()
        py_data[i][2] = cpp_vector[i].z()

    return py_data


def numpy2DArrayToStdVectorCoordinate(array):
    """
    Convert a Nx3 2D numpy array to a std::vector<Coordinate> representation.

    :param array: The array to convert.

    :returns: A corresponding std::vector<Coordinate> object.
    """
    # Get the shape of the array.
    shape = numpy.shape(array)
    nI = shape[0]

    # Setup the c++ object.
    cpp_vector = Backend.StdVectorCoordinate()

    # Copy the values over.
    for i in range(nI):
        cpp_vector.push_back(Backend.Coordinate(array[i][0],
                                                array[i][1],
                                                array[i][2]))
    # Done.
    return cpp_vector
