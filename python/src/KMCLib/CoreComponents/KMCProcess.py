""" Module for the KMCProcess class """


# Copyright (c)  2013-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import numpy

from KMCLib.Utilities.CoordinateUtilities import sortCoordinatesDistance
from KMCLib.Utilities.CheckUtilities import checkTypes
from KMCLib.Utilities.CheckUtilities import checkSequence
from KMCLib.Utilities.CheckUtilities import checkSequenceOfFloats
from KMCLib.Utilities.CheckUtilities import checkSequenceOfPositiveIntegers
from KMCLib.Utilities.CheckUtilities import checkPositiveFloat
from KMCLib.CoreComponents.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.CoreComponents.KMCBaseProcess import KMCBaseProcess
from KMCLib.Exceptions.Error import Error

class KMCProcess(KMCBaseProcess):
    """
    Class for representing a possible process in a lattice KMC simulation.
    """

    def __init__(self,
                 coordinates=None,
                 elements_before=None,
                 elements_after=None,
                 move_vectors=None,
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

        :param move_vectors: A set of vectors in the local coordinates that define
                             which elements are moved to which place in the move.
                             The vectors are given as a list of tuples, where the first
                             element in each tuple indicates which index is moved and
                             the second element in the tuple is a vector in local coordinates
                             indicating where the move is to.

        :param basis_sites: The basis sites in the lattice at which the process
                            can possibly be applied. Only if the length of this
                            list is 1 can implicit wildcards be used for the
                            matching.

        :param rate_constant: The rate constant associated with this process.
        :type rate_constant: float

        """
        # Call the base class constructor.
        super(KMCProcess, self).__init__(coordinates, basis_sites, rate_constant)

        # Check the types.
        elements_before = checkTypes(elements_before, len(coordinates))
        elements_after  = checkTypes(elements_after,  len(coordinates))

        # Check that the elements represents a valid move.
        self.__checkValidMoveElements(elements_before, elements_after)

        # All types checking done.
        self._elements_before = elements_before
        self._elements_after  = elements_after

        # Check that the move vectors are compatible with the elements.
        self._move_vectors = self.__checkValidMoveVectors(move_vectors)

        # Sort the coordinates and co-sort the elements and move vectors.
        self.__sortCoordinatesElementsAndMoveVectors()

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
        center = 0
        c1 = KMCLocalConfiguration(self._coordinates, self._elements_before, center)
        c2 = KMCLocalConfiguration(self._coordinates, self._elements_after,  center)
        self._local_configurations = (c1, c2)

        self._all_present_types = list(set(self.elementsBefore() + self.elementsAfter()))

    def __checkValidMoveElements(self, elements_before, elements_after):
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

        # Done.

    def __checkValidMoveVectors(self, move_vectors):
        """
        Private helper function to check if a set of move vectors
        is compatible with the elements before and after the move.

        :param move_vectors: The move vectors to check.
        :returns: The validated move vectors.
        """
        # Check the move vector format, and reconstruct if
        # possible.
        move_vectors = self.__checkMoveVectorsFormat(move_vectors)

        # Quick return if no move vectors.
        if move_vectors is None:
            move_vectors = []
            return move_vectors

        # Try to make the move with the move vectors and check that
        # we end up in the elements after list.
        moved_elements = [] + self._elements_before
        for (move_index, move_vector) in move_vectors:
            old_coord = self._coordinates[move_index]
            new_coord = numpy.array(old_coord) + numpy.array(move_vector)   #    <- Elementwise addition.

            # Find which index this corresponds to.
            subtracted = self._coordinates - new_coord
            reduced    = numpy.abs(sum(abs(subtracted.transpose())))
            boolean    = [ rr < 1.0e-8 for rr in reduced ]
            new_index  = numpy.where(boolean)
            if len(new_index[0]) == 0:
                raise Error("Each move_vector must move an atom to a valid lattice site.")
            new_index = new_index[0][0]

            # Check that the the element at this position in the new elements
            # vector corresponds to the move.
            if self._elements_before[move_index] != self._elements_after[new_index]:
                raise Error("The move vector for index %i does not match the elements after move."%(move_index))

            # Perform the move.
            moved_elements[new_index] = self._elements_before[move_index]

        # With all moves performed on the elements we check if we properly reconstructed
        # the elements after the move.
        if (self._elements_after != moved_elements):
            raise Error("Applying the move vectors to the elements_before does not generate the elements_after list.")

        return move_vectors

    def __checkMoveVectorsFormat(self, move_vectors):
        """
        Private helper function to check the format of the move vectors.

        :param move_vectors: The input to check.

        :returns: The validated move vectors, or None if None was given
                  and a reconstruction was not possible.
        """
        if move_vectors is None:
            # Try to reconstruct the move vectors from the before and after
            # elements information.
            move_vectors = self.__reconstructMoveVectors()

        # Return if still None.
        if move_vectors is None:
            return move_vectors

        # This is the error message.
        msg = """The 'move_vectors' input to the KMCProcess constructor must be a
list of tuples, where the first element of each tuple refers to an atom index
and the second element is a cartesian vector of length 3, in internal
coordinates defining where the moved index goes."""

        # Check that we have a sequence.
        move_vectors = checkSequence(move_vectors, msg)

        # Check the format of each element.
        for t in move_vectors:
            if not isinstance(t, tuple):
                raise Error(msg)
            elif not isinstance(t[0], int):
                raise Error(msg)
            # Check the vector.
            checkSequenceOfFloats(t[1], msg)
            if len(t[1]) != 3:
                raise Error(msg)
        # Passed all tests.
        return move_vectors

    def __reconstructMoveVectors(self):
        """
        Private helper function to reconstruct the move vectors from the
        elements information. This is only possible for simple moves where
        two atoms swap places.

        :returns: The reconstructed move vectors or None if no reconstruction
                  was possible.
        """
        # Find the differing elements.
        pairs = []
        for i, (e1, e2) in enumerate(zip(self._elements_before, self._elements_after)):
            if e1 != e2:
                pairs.append((e1,e2,i))

        # Check the size.
        if len(pairs) != 2:
            return None

        # Check the types.
        if not (pairs[0][0] == pairs[1][1] and pairs[0][1] == pairs[1][0]):
            return None

        # We can now reconstruct the move vectors.
        index_0 = pairs[0][2]
        index_1 = pairs[1][2]

        start = numpy.array(self._coordinates[index_0])
        end   = numpy.array(self._coordinates[index_1])

        vector_0 = numpy.array(end-start)
        vector_1 = numpy.array(start-end)

        return [(index_0, vector_0), (index_1, vector_1)]

    def __sortCoordinatesElementsAndMoveVectors(self):
        """
        Private helper to sort the validated coordinate input,
        and update the element order and move_vector indexing accordingly.
        """
        # Set up the original indexing.
        original_indexing = range(len(self._coordinates))

        # Sort, and co-sort coordinates and indices.
        (sorted_coords, dummy_distances, sorted_types_before, sorted_types_after, new_to_old_index) = \
            sortCoordinatesDistance(coordinates = self._coordinates,
                                    center      = 0,
                                    types1      = self._elements_before,
                                    types2      = self._elements_after,
                                    co_sort     = original_indexing)

        # Fix the move vectors.
        if len(self._move_vectors) > 0:
            old_to_new_index = []
            for i in range(len(new_to_old_index)):
                old_to_new_index.append(new_to_old_index.index(i))

            # Fixt the move vector indexing.
            move_vector_index = []
            for v in self._move_vectors:
                move_vector_index.append( old_to_new_index[v[0]] )

            # Setup and sort the backmapping.
            help_index = range(len(move_vector_index))
            to_sort = numpy.array(zip(help_index, move_vector_index))
            sorted_indices = to_sort[numpy.argsort(to_sort[:,1])]

            # Construct the new move vectors.
            new_move_vectors = []
            for idx in sorted_indices:
                new_move_vectors.append( (idx[1], self._move_vectors[idx[0]][1]) )

            # Set the move vectors.
            self._move_vectors = new_move_vectors

        # Set the new data on the class.
        self._coordinates = sorted_coords
        self._elements_before = sorted_types_before
        self._elements_after = sorted_types_after

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

        coords_self = self.localConfigurations()[0].coordinates()
        coords_other = other.localConfigurations()[0].coordinates()

        types_before_self = self.localConfigurations()[0].types()
        types_before_other = other.localConfigurations()[0].types()

        types_after_self = self.localConfigurations()[1].types()
        types_after_other = other.localConfigurations()[1].types()

        # Check the coordinates and types.
        if numpy.linalg.norm(coords_self - coords_other) > 0.00001:
            return False
        elif not all([s1 == s2 for s1,s2 in zip(types_before_self,
                                                types_before_other)]):
            return False
        elif not all([s1 == s2 for s1,s2 in zip(types_after_self,
                                                types_after_other)]):
            return False

        # Check the move vectors.
        if len(self._move_vectors) != len(other._move_vectors):
            return False

            # For each move vector, loop through the others and find the one that matches.
        for v1,v2 in zip(self._move_vectors,other._move_vectors):
            if v1[0] != v2[0]:
                return False
            elif numpy.linalg.norm(numpy.array(v1[1])-numpy.array(v2[1])) > 1.0e-8:
                return False

        # Passed all tests, return true.
        return True

    def elementsBefore(self):
        """
        Query for the elements before.

        :returns: The elements before stored on the class.
        """
        return self._elements_before

    def elementsAfter(self):
        """
        Query for the elements before.

        :returns: The elements before stored on the class.
        """
        return self._elements_after

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
        if len(self._coordinates) > 1:
            c = self._coordinates[0]
            coords_string += coord_template%(c[0],c[1],c[2])

            # And the middle coordinates.
            coord_template = indent + "[" + ff + "," + ff + "," + ff + "],\n"
            for c in self._coordinates[1:-1]:
                coords_string += coord_template%(c[0],c[1],c[2])

        # Add the last coordinate (which is also the first if there is only one coordinate).
        c = self._coordinates[-1]
        if len(self._coordinates) == 1:
            coord_template = "[" + ff + "," + ff + "," + ff + "]]\n"
        else:
            coord_template = indent + "[" + ff + "," + ff + "," + ff + "]]\n"
        coords_string += coord_template%(c[0],c[1],c[2])

        # Setup the elements before string.
        elements_before_string = "elements_before = "
        indent = " "*19
        line = "["
        nT = len(self._elements_before)
        for i,t in enumerate(self._elements_before):
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
        nT = len(self._elements_after)
        for i,t in enumerate(self._elements_after):
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

        # Setup the move vector string.
        if len(self._move_vectors) == 0:
            move_vectors_string = "move_vectors    = None\n"
        else:
            move_vectors_string = "move_vectors    = ["
            indent = " "*19
            vector_template = "[" + ff + "," + ff + "," + ff + "]"
            for i, (index, vector) in enumerate(self._move_vectors):

                move_vectors_string += "( %2i,"%(index) + vector_template%(vector[0], vector[1], vector[2])

                if i < len(self._move_vectors)-1:
                    move_vectors_string += "),\n" + indent
                else:
                    move_vectors_string += ")]\n"

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
            "    move_vectors=move_vectors,\n" +\
            "    basis_sites=basis_sites,\n" + \
            "    rate_constant=rate_constant)\n"

        return coords_string + "\n" + \
            elements_before_string + \
            elements_after_string  + \
            move_vectors_string    + \
            basis_sites_string     + "\n" + \
            rate_constant_string   + "\n\n" + \
            process_string + "\n"

