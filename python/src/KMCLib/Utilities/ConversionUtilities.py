""" Module for holding common type conversion utility functions. """


# Copyright (c)  2012-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy

from KMCLib.Backend import Backend
from KMCLib.Exceptions.Error import Error


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


def stdVectorStringToStringList(cpp_list):
    """
    Converts a std::vector<std::string> to a list of strings.

    :param cpp_list: The std::vector to convert.

    :returns: A corresponding list of strings.
    """
    # Get the size.
    size = cpp_list.size()

    # Loop over the vector and append.
    string_list = []
    for i in range(size):
        string_list.append(cpp_list[i])

    # Done.
    return string_list


def stringListToStdVectorStdVectorString(string_list):
    """
    Converts a list of strings to a std::vector<std::vector<std::string> >
    object with one elemnt in each of the inner vectors.

    :param string_list: The list of strings.

    :returns: A corresponding std::vector<std::vector<std::string> > object.
    """
    # Get the size.
    size = len(string_list)
    # Setup the c++ object.
    cpp_list = Backend.StdVectorStdVectorString()

    # Copy values over.
    for s in string_list:
        cpp_list.push_back(Backend.StdVectorString(1, s));
    # Done.
    return cpp_list

def bucketListToStdVectorStdVectorString(bucket_list):
    """
    Converts a list of the format [[(n,"A"), ...], ...]
    to a std::vector< std::vector<std::string> > representation.

    :param bucket_list: The list to convert.

    :returns: A corresponding std::vector<std::vector<std::string> > object.
    """
    cpp_list = Backend.StdVectorStdVectorString()

    # For each site.
    for ss in bucket_list:
        # For all types at this site.
        site_list = Backend.StdVectorString()
        for s in ss:
            # Add the number of types.
            for i in range(s[0]):
                site_list.push_back(s[1])
        cpp_list.push_back(site_list);

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


def stdVectorPairCoordinateToNumpy2DArray(cpp_vector):
    """
    Convert a std::vector< std::pair<Coordinate, Coordinate> >
    to a nx6 numpy array.
    """
    size = len(cpp_vector)
    py_data = numpy.zeros((size, 6))

    for i in range(size):
        py_data[i][0] = cpp_vector[i][0].x()
        py_data[i][1] = cpp_vector[i][0].y()
        py_data[i][2] = cpp_vector[i][0].z()
        py_data[i][3] = cpp_vector[i][1].x()
        py_data[i][4] = cpp_vector[i][1].y()
        py_data[i][5] = cpp_vector[i][1].z()

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

# NEEDS TESTING
def typeBucketToList(type_bucket):
    """
    Convert a TypeBucket to a numpy list.

    :param type_bucket: The type bucket to convert.

    :returns: A corresponding python list.
    """
    # Get the size.
    size = type_bucket.size()

    # Set up the return list.
    ret = [0]*size

    # Copy the values over.
    for i in range(size):
        ret[i] = type_bucket[i]

    # Done.
    return ret

def toShortBucketsFormat(types_list):
    """
    Convert any valid types information to the short buckets format,
    i.e. on the form [[(n,"A"), ...], ...]

    :param types_list: The list to convert.

    :returns: The types list in the short buckets format.
    """
    ret_types = []
    for t in types_list:

        # Set up the list for this site.
        t_list = []

        # Three cases.
        if isinstance(t, str):
            t_list.append((1, t))

        elif isinstance(t, list):
            # Two cases in the list.
            for tt in t:
                if isinstance(tt, str):
                    t_list.append((1, tt))
                elif isinstance(tt, tuple):
                    t_list.append(tt)

        elif isinstance(t, tuple):
            # Append the tuple.
            t_list.append(t)

        # Done with this site.
        ret_types.append(t_list)

    # Loop through again and collect identical types.
    final_types = []
    for t in ret_types:

        t_type = []
        taken  = []

        # For each site, collect the numbers of the same type.
        for i,t0 in enumerate(t):
            occupation = t0[0]
            name       = t0[1]
            if not name in taken:
                for j, t1 in enumerate(t):
                    if i < j:
                        if t1[1] == name:
                            occupation += t1[0]

                t_type.append( (occupation, name) )
                taken.append(name)
        final_types.append(t_type)

    # Done.
    return final_types

def stdVectorTypeBucketToPython(cpp_vector, cpp_type_map):
    """
    Translate a std::vector<TypeBucket> object to the python
    format [[(n, "A"), ... ], ... ] by using a map of the type
    std::vector<std::string>.

    :param cpp_vector: The C++ vector to translate.
    :param cpp_type_map: The type map to use.
    """
    # The return data.
    types = []

    # Loop over each site.
    for i in range(len(cpp_vector)):

        site_types = []

        for j in range(cpp_vector[i].size() - 1):

            # Skip the wildcard.
            j += 1

            # Translate.
            t = cpp_type_map[j]
            n = cpp_vector[i][j]

            # Add.
            if n > 0:
                site_types.append((n,t))

        types.append(site_types)

    # Done.
    return types



