""" Module for the KMCConfiguration """


# Copyright (c)  2012-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import numpy

from KMCLib.CoreComponents.KMCLattice import KMCLattice
from KMCLib.Utilities.CheckUtilities import checkTypes
from KMCLib.Utilities.CheckUtilities import checkAndNormaliseBucketEntry
from KMCLib.Utilities.ConversionUtilities import stringListToStdVectorStdVectorString
from KMCLib.Utilities.ConversionUtilities import numpy2DArrayToStdVectorStdVectorDouble
from KMCLib.Utilities.ConversionUtilities import stdVectorCoordinateToNumpy2DArray
from KMCLib.Utilities.ConversionUtilities import bucketListToStdVectorStdVectorString

from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend

class KMCConfiguration(object):
    """
    Class for representing the configuration in a KMC simulation.
    """

    def __init__(self,
                 lattice=None,
                 types=None,
                 possible_types=None,
                 default_type=None):
        """
        Constructor for the KMCConfiguration - the configuration object to use
        in the KMC simulations.

        :param lattice: The lattice of the configurartion as a KMCLattice.

        :param types: The site types at the lattice points as a list, e.g. ['type1','type2',..],
                      ordered as [a,b,c,i] with i being the fastest and a the slowest index and
                      a, b and c refers to the cell repetitions and i
                      refers to the specific basis point in the cell. When using this format
                      one cannot specify a default type, and the number of elements in the
                      types list must match the number of grid points in the lattice. Alternatively
                      one can specify the types input as a list of tuples specifying the a,b,c,i for each type,
                      e.g. [(0,0,1,0,'a'), (0,0,1,1,'b'), ...]. If one uses the longer tuple format
                      a default type should be given, which then will be used for all points not
                      explicitly specified in the list.

        :param possible_types: A list of possible types. If not given this list will be set to the
                               types present in the types list by default.

        :param default_type: This input parameter can only be given if the types are
                             given in long format i.e. [(0,0,1,0,'a'), (0,0,1,1,'b'), ...]
                             The default type will then be used for lattice sites
                             not specified in the types list.
        """
        # Check that the lattice is of the correct type.
        if not isinstance(lattice,KMCLattice):
            raise Error("The lattice given to the KMCConfiguration constructor must be of type KMCLattice.")
        self.__lattice = lattice

        # Extract the number of lattice sites.
        self.__n_lattice_sites = len(self.__lattice.sites())

        # To be updated if bucket types are used.
        self.__use_buckets = False

        # Check and set the types.
        self.__checkAndSetTypes(types, default_type, possible_types)

        # Wait with setting up the backend until we need it.
        self.__backend = None

    def __checkAndSetTypes(self, types, default_type, possible_types):
        """ """
        """
        Private helper routine to check and set the types input.
        """

        types_format = self.__detectTypesFormat(types)

        if types_format == "short":
            types_to_set = self.__checkAndSetShortTypes(types, default_type, possible_types)
            all_types_present = list(set(types_to_set))
        elif types_format == "long":
            types_to_set = self.__checkAndSetLongTypes(types, default_type, possible_types)
            all_types_present = list(set(types_to_set))
        else:
            types_to_set = self.__checkAndSetBucketsTypes(types, default_type, possible_types)
            self.__use_buckets = True
            # Calculate all types.
            all_types_present = []
            for tt in types_to_set:
                for t in tt:
                    all_types_present.append(t[1])
            all_types_present = list(set(all_types_present))

        # Setup the list of possible types.
        if possible_types is None:
            possible_types = all_types_present
        else:
            # Check that the possible types is a list of strings.
            if not isinstance(possible_types, list) or not all([isinstance(pt,str) for pt in possible_types]):
                raise Error("The possible types must be given as a list of strings.")

            # Check that each present type is in the list of possible types.
            possible_set = set(possible_types)
            present_set = set(all_types_present)
            if not present_set.issubset(possible_set):
                raise Error("There are types specified which are not in the given list of possible types.")

        # Check that the wildcard has not made it into the possible types allready.
        if "*" in possible_types:
            raise Error("The wildcard caracter '*' is not a valid type.")

        # Setup the possible types as a dict.
        possible_types = ["*"] + possible_types
        self.__possible_types = dict(zip(possible_types, range(len(possible_types))))

        # Every thing is checked - store the data on the class.
        self.__types = types_to_set

    def __detectTypesFormat(self, types):
        """ """
        """
        Private helper routine to detect what types format is given.

        :retrurns: "short", "long" or "buckets".
        """
        # Check that the types is a list.
        if not isinstance(types, list):
            raise Error("The 'types' given to the KMCConfiguration constructor must be a list of either 'short', 'long' or 'buckets' format. See the manual for details.")

        # Check all elements to see if we have the short format.
        if all([isinstance(t, str) for t in types]):
            return "short"

        # Check all elements to see if we have the long format.
        elif all([isinstance(t, tuple) for t in types]) and all([isinstance(tt, (int, int, int, int, str)[i]) for i,tt in enumerate(types[0])]):
            return "long"

        # No other option left but to assume the buckets format.
        else:
            return "buckets"

    def __checkAndSetShortTypes(self, types, default_type, possible_types):
        """ """
        """
        Private helper to check the types input for the 'short' format.
        """
        # Check the default types.
        if default_type is not None:
            raise Error("A default type can only be used in combination with the long types format\nwhen constructing a KMCConfiguration object.")

        # Check the types.
        return checkTypes(types,self.__n_lattice_sites)

    def __checkAndSetLongTypes(self, types, default_type, possible_types):
        """ """
        """
        Private helper to check the types input for the 'long' format.
        """
        # We must have a default type for the long format.
        if default_type is None:
            raise Error("A default type must be specified when using the long types format.")

        # Check and distribute the default type.
        if not isinstance(default_type, str):
            raise Error("The default type given to the KMCConfiguration constructor must be a string.")
        types_to_set = [default_type]*self.__n_lattice_sites

        # Check each element in the list.
        for t in types:
            # Check that it is ia tuple.
            if not isinstance(t,tuple) or len(t) != 5:
                raise Error("All elements in the types list must be of type (int,int,int,int,string) when\nusing the long type format.")

            # Check that the elements in the tuple have the correct type.
            if not all([isinstance(tt,(int,int,int,int,str)[i]) for i,tt in enumerate(t)]):
                raise Error("All elements in the types list must be of type (int,int,int,int,string) when\nusing the long type format.")

            # Check the bounds of the given indices.
            (nI, nJ, nK) = self.__lattice.repetitions()
            nB = len(self.__lattice.basis())

            if t[0] < 0 or t[0] >= nI:
                raise Error("The first index in the type tuple must be within the limits of the repetitions in the 'a' direction, indexed from 0.")
            if t[1] < 0 or t[1] >= nJ:
                raise Error("The second index in the type tuple must be within the limits of the repetitions in the 'b' direction, indexed from 0.")
            if t[2] < 0 or t[2] >= nK:
                raise Error("The third index in the type tuple must be within the limits of the repetitions in the 'c' direction, indexed from 0.")
            if t[3] < 0 or t[3] >= nB:
                raise Error("The fourth index in the type tuple must be withing the limits of the basis points in the original unit cell, indexed from 0.")

            # Set the type.
            index = self.__lattice._globalIndex(t[0],t[1],t[2],t[3])
            types_to_set[index] = t[4]

        # Return the types to set.
        return types_to_set

    def __checkAndSetBucketsTypes(self, types, default_type, possible_types):
        """ """
        """
        Private helper to check the types input for the 'buckets' format.
        """
        # Check the default types.
        if default_type is not None:
            raise Error("A default type can only be used in combination with the long types format\nwhen constructing a KMCConfiguration object.")

        # For each element, check that the foramt is any of the valid
        # "A" - single particle
        # (N,"A") - numbered single particle
        # ["A", (N1,"B")] - a list with a combination of the other two.
        return [checkAndNormaliseBucketEntry(t) for t in types]

    def types(self):
        """
        Query function for the types of the configuration.

        :returns: The stored types list.
        """
        # Update the types with what ever has been changed in the backend.
        if self.__use_buckets:

            self.__types = [[ee for ee in e] for e in self._backend().elements()]
        else:
            self.__types = [e[0] for e in self._backend().elements()]

        # Return the types.
        return self.__types

    def atomIDTypes(self):
        """
        Query for the types indexed according to the atom_ids.
        """
        return self._backend().atomIDElements()

    def atomIDCoordinates(self):
        """
        Query for the coordinates per atom id.
        """
        return stdVectorCoordinateToNumpy2DArray(self._backend().atomIDCoordinates())

    def sites(self):
        """
        Query function for the lattice sites.

        :returns: The lattice sites.
        """
        return self.__lattice.sites()

    def possibleTypes(self):
        """
        Query function for the possible types dict.

        :returns: The stored possible types dict.
        """
        return self.__possible_types

    def particlesPerType(self):
        """
        Query function for the number of particles per type.

        :returns: The current number of particles per type.
        """
        return self._backend().particlesPerType()

    def cellRepetitions(self):
        """
        Query for the primitive cell repetitions.
        """
        return self.__lattice.repetitions()

    def movedAtomIDs(self):
        """
        Query for the moved atom_id:s of the last move.
        """
        return self._backend().movedAtomIDs()

    def latestEventProcess(self):
        """
        Query for the process number of the latest event.
        """
        return self._backend().latestEventProcess()

    def latestEventSite(self):
        """
        Query for the site index of the latest event.
        """
        return self._backend().latestEventSite()

    def lattice(self):
        """
        Query for the lattice.
        """
        return self.__lattice

    def _backend(self):
        """
        Query function for the c++ backend object.
        """
        if self.__backend is None:
            # Construct the c++ backend object.
            if self.__use_buckets:
                cpp_types = bucketListToStdVectorStdVectorString(self.__types)
            else:
                cpp_types = stringListToStdVectorStdVectorString(self.__types)

            cpp_coords = numpy2DArrayToStdVectorStdVectorDouble(self.__lattice.sites())
            cpp_possible_types = Backend.StdMapStringInt(self.__possible_types)

            # Send in the coordinates and types to construct the backend configuration.
            self.__backend = Backend.Configuration(cpp_coords,
                                                   cpp_types,
                                                   cpp_possible_types)

        # Return the backend.
        return self.__backend

    # ML: NEEDS TEST
    def _backendTypeNames(self):
        """
        Get a tuple with the type names order the same way as the buckets
        backend implementation.
        """
        return self.__backend.typeNames()

    def _latticeMap(self):
        """
        Get a c++ lattice map describing the lattice.
        """
        return self.__lattice._map()

    def _script(self, variable_name="configuration"):
        """
        Generate a KMCLib Python script representation of this configuration.

        :param variable_name: A name to use as variable name for
                              the KMCConfiguration in the generated script.
        :type variable_name: str

        :returns: A KMCLib Python script, as a string,
                  that can generate this configuration.
        """
        # Get the lattice script.
        lattice_script = self.__lattice._script(variable_name="lattice")

        # Get the types string.
        types_string = "types = "
        indent = " "*9
        line = "["
        nT = len(self.__types)
        for i,t in enumerate(self.__types):
            # Add the type.
            line += "'" + t + "'"
            if i == nT-1:
                # Stop if we reach the end.
                line += "]\n"
                types_string += line
                break
            else:
            # Add the separator.
                line += ","

            # Check if we should add a new line.
            if len(line) > 50:
                types_string += line + "\n" + indent
                line = ""

        # Generate the possible types string.
        possible_types_string = "possible_types = "
        indent = " "*18
        line = "["
        possible_types = [t for t in list(set(self.__possible_types.keys())) if t != "*"]

        nT = len(possible_types)
        for i,t in enumerate(possible_types):
            # Add the type.
            line += "'" + t + "'"
            if i == nT-1:
                # Stop if we reach the end.
                line += "]\n"
                possible_types_string += line
                break
            else:
            # Add the separator.
                line += ","

            # Check if we should add a new line.
            if len(line) > 50:
                possible_types_string += line + "\n" + indent
                line = ""

        # Setup the configuration string.
        configuration_string = variable_name + """ = KMCConfiguration(
    lattice=lattice,
    types=types,
    possible_types=possible_types)
"""

        # Add the comment.
        comment_string = """
# -----------------------------------------------------------------------------
# Configuration

"""
        # Return the script.
        return lattice_script + comment_string + types_string + "\n" + \
            possible_types_string + "\n" + configuration_string

    def _atkScript(self, types_map):
        """
        Generate an ATK 12.8.2 compatible BulkConfiguration script of
        this configuration.

        :param types_map: A map between the KMCLib types used in this
                          configuration and ATK type names as a dict,
                          where keys are the type names used in this
                          configuration and the corresponding values are the
                          ATK types to be used given as strings. Any
                          type not specified in the mapping will be left out
                          in the ATK configuration.

        :returns: An ATK script as a string representing this configuration.
        """
        header = """# ----------------------------------------------------------
# ATK 12.8.2 BulkConfiguration script generated from KMCLib
# configuration version 1.0.
# ----------------------------------------------------------

"""
        # Get the primitive cell vectors.
        cell_vectors = self.__lattice.unitCell().cellVectors()

        # Generate the ATK lattice vectors by multiplying with the
        # repetitions.
        vector_a = cell_vectors[0] * self.__lattice.repetitions()[0]
        vector_b = cell_vectors[1] * self.__lattice.repetitions()[1]
        vector_c = cell_vectors[2] * self.__lattice.repetitions()[2]

        cell_script = "# Specify the lattice parameters.\n"
        cell_script += "vector_a = [ %12.8f, %12.8f, %12.8f]*Angstrom\n"%(vector_a[0], vector_a[1], vector_a[2])
        cell_script += "vector_b = [ %12.8f, %12.8f, %12.8f]*Angstrom\n"%(vector_b[0], vector_b[1], vector_b[2])
        cell_script += "vector_c = [ %12.8f, %12.8f, %12.8f]*Angstrom\n"%(vector_c[0], vector_c[1], vector_c[2])
        cell_script += "lattice = UnitCell(vector_a, vector_b, vector_c)\n\n"

        # Parse the types and positions to ATK format.
        one_over_rep = numpy.array([1.0/self.__lattice.repetitions()[0],
                                    1.0/self.__lattice.repetitions()[1],
                                    1.0/self.__lattice.repetitions()[2]], dtype=float)
        atoms = [ (types_map[t],c*one_over_rep) for t,c in zip(self.atomIDTypes(), self.atomIDCoordinates()) if t in types_map.keys() ]

        # Generate the ATK elements and coordinates script.
        elements_script    = "# Define the elements.\nelements = ["
        coordinates_script = "# Define the coordinates.\ncoordinates = ["
        last = len(atoms)-1
        this_row = 0
        full_row = 6
        for i,(t,c) in enumerate(atoms):
            this_row += 1
            if i == last:
                elements_script    += "%s]\n\n"%(t)
                coordinates_script += "[ %15.8e, %15.8e, %15.8e]]\n\n"%(c[0],c[1],c[2])
            else:
                coordinates_script += "[ %15.8e, %15.8e, %15.8e],\n               "%(c[0],c[1],c[2])

                if this_row == full_row:
                    elements_script += "%s,\n            "%(t)
                    this_row = 0
                else:
                    elements_script += "%s, "%(t)

        # Generate the configuration script.
        config_script ="""# Setup the configuration.
bulk_configuration = BulkConfiguration(
    bravais_lattice=lattice,
    elements=elements,
    fractional_coordinates=coordinates )

# ----------------------------------------------------------
"""

        # Setup and return the whole script.
        return header + cell_script + elements_script + coordinates_script + config_script

