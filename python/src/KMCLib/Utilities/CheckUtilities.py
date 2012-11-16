""" Module for holding common checking utility functions. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy

from KMCLib.Exceptions.Error import Error


def checkIndexWithinBounds(index, list, msg=None):
    """
    Check that the given index is within the bounds of the list.

    @param index : The index to check.
    @type        : Integer
    @default     : None

    @param list  : The list to check against.
    @type        : Any sequence object with __len__ defined.
    @default     : None

    @param msg   : The error message to print.
    @type        : String
    @default     : "Index out of range."

    @returns     : The valid index.
    """
    # Set the default.
    if msg is None:
        msg = "Index out of range."

    # Stop if outside bounds.
    if (index < 0 or index >= len(list)):
        raise Error(msg)

    # Return if passed.
    return index


def checkCoordinateList(coordinates):
    """
    Check that the given coordinates is a valid 3xN sequence of numbers.

    @param coordinates : The object to test. To pass the test this must be
                         a 3xN array of floating point numbers.

    @returns           : A valid 3xN array of numbers.
    """
    # Check that it is a sequence.
    coordinates = checkSequence(coordinates)

    # Check that its length is not zero.
    if (len(coordinates) < 1):
        raise Error("The coordinates may not be empty.")

    # Check each coordinate.
    for coord in coordinates:

        # Check that it is a sequence.
        coord = checkSequence(coord)

        # Make sure the length of the coordinate is 3.
        if len(coord) != 3:
            raise Error("Each coordinate must have exactly three elements.")

        # Check that each element is a floating point number.
        if not all([isinstance(c,float) for c in coord]):
            raise Error("All coordinates must be given as floating point numbers.")

    # Convert to a numpy array and return.
    return numpy.array(coordinates)


def checkSequence(sequence):
    """
    Check that the given object is sequence.

    @param sequence : The object to test.

    @returns        : The valid sequence object.
    """
    # Check that this is a sequence.
    if not ('__len__' in dir(sequence)):
        raise Error("The tested object is not a sequence.")

    # Done.
    return sequence


def checkTypes(types, length):
    """
    Check that the types list is given as a list of strings with the correct
    length.

    @param types : The object to check.
    @param length: The size the list should have.
    @type        : Integer

    @returns     : The checked list.
    """
    # Check that it is a list.
    if not isinstance(types, list):
        raise Error("The 'types' parameter must be given as a list of strings.")

    # Check eachg element.
    for t in types:
        if not isinstance(t,str):
            raise Error("The 'types' parameter must be given as a list of strings.")

    # Check the length.
    if len(types) != length:
        raise Error("The length of the 'types' parameter must match the coordinates.")

    # Done.
    return types



