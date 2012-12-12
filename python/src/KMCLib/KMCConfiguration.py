""" Module for the KMCConfiguration """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.KMCLattice import KMCLattice
from KMCLib.Utilities.CheckUtilities import checkTypes
from KMCLib.Utilities.ConversionUtilities import stringListToStdVectorString
from KMCLib.Utilities.ConversionUtilities import numpy2DArrayToStdVectorStdVectorDouble
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

        :param lattice: The lattice of the configurartion.
        :type lattice: Lattice

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

        # Check and set the types.
        self.__checkAndSetTypes(types, default_type, possible_types)

        # Wait with setting up the backend until we need it.
        self.__backend = None

    def __checkAndSetTypes(self, types, default_type, possible_types):
        """ """
        """
        Private helper routine to check and set the types input.
        """
        # Check that the types is a list.
        if not isinstance(types, list):
            raise Error("The 'types' given to the KMCConfiguration constructor must be \na list of type strings, e.g. ['a','b','c'] or tuples e.g. [(0,0,1,0,'a'), (0,0,2,0,'b'), ...].")

        # Check the first element to get the assumed format.
        use_long_format = not isinstance(types[0],str)

        # Set the use of default type flag.
        use_default_type = (default_type is not None)

        # We can not have both a short format and default type.
        if use_default_type and not use_long_format:
            raise Error("A default type can only be used in combination with the long types format\nwhen constructing a KMCConfiguration object.")

        # If we use the long format, check that each entry is of the form (int,int,int,int,string)
        if use_long_format:

            # We must have a default type for the long format.
            if not use_default_type:
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

        else:
            # Otherwise, check the types the normal way.
            types_to_set = checkTypes(types,self.__n_lattice_sites)

        # Setup the list of possible types.
        all_types_present = list(set(types_to_set))
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

        # Every thing is checked - store the data on the class.
        self.__types = types_to_set

        # Setup the possible types as a dict.
        self.__possible_types = dict(zip(possible_types, range(len(possible_types))))

    def types(self):
        """
        Query function for the types of the configuration.

        :returns: The stored types list.
        """
        # Update the types with what ever has been changed in the backend.
        self.__types = list(self._backend().elements())

        # Return the types.
        return self.__types

    def possibleTypes(self):
        """
        Query function for the possible types dict.

        :returns: The stored possible types dict.
        """
        return self.__possible_types

    def _backend(self):
        """
        Query function for the c++ backend object.
        """
        if self.__backend is None:
            # Construct the c++ backend object.
            cpp_types  = stringListToStdVectorString(self.__types)
            cpp_coords = numpy2DArrayToStdVectorStdVectorDouble(self.__lattice.sites())
            cpp_possible_types = Backend.StdMapStringInt(self.__possible_types)

            # Send in the coordinates and types to construct the backend configuration.
            self.__backend = Backend.Configuration(cpp_coords,
                                                   cpp_types,
                                                   cpp_possible_types)

        # Return the backend.
        return self.__backend

    def _latticeMap(self):
        """
        Get a c++ lattice map describing the lattice.
        """
        return self.__lattice._map()



