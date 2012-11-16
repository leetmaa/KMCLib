""" Module for holding common coordinate manipulating utility functions. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy


def centerCoordinates(coordinates, index):
    """ Center the coordinates around the given index. """
    # Extract the centeral coordinate.
    center = coordinates[index]
    # Subtract.
    coordinates = numpy.array([ coord - center for coord in coordinates ])

    # Done.
    return coordinates


def sortCoordinates(coordinates, center, types):
    """ Sort the coordinates with respect to distance form the provided center index and type."""
    # Get the types.
    dt = coordinates.dtype
    dtype = [('x',dt),('y',dt),('z',dt),('d',dt),('type', numpy.array(types).dtype)]

    # Calculate the distance form the center.
    origin = coordinates[center]
    distances = numpy.array([ numpy.linalg.norm(coord) for coord in coordinates ])

    # Setup the data to sort.
    to_sort = numpy.array([ (c[0],c[1],c[2],d,type) for (c,d,type) in zip(coordinates,distances,types)],
                          dtype=dtype)

    # Sort.
    sorted = numpy.sort(to_sort, order=['d','type','x','y','z'])

    # Extract the info.
    coordinates = numpy.array([[c[0],c[1],c[2]] for c in sorted])
    distances   = numpy.array([c[3] for c in sorted])
    types       = [c[4] for c in sorted]

    # Done.
    return (coordinates, distances, types)

