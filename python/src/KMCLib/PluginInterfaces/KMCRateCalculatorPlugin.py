""" Module for the KMCRateCalculatorPlugin class """


# Copyright (c)  2013-2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy

from KMCLib.Backend import Backend
from KMCLib.Exceptions.Error import Error
from KMCLib.Utilities.ConversionUtilities import stdVectorTypeBucketToPython

class KMCRateCalculatorPlugin(Backend.RateCalculator):
    """
    Class for providing an interface to easily extend and customize
    the behaviour of the calculation of individual rates in the KMC simulation.
    """

    def __init__(self, configuration):
        """
        Base class constructor.

        :param configuration: The KMCConfiguration
        """
        # Call the C++ base class constructor.
        Backend.RateCalculator.__init__(self)

        # Store member data on the class.
        self.configuration = configuration

        # Call the custom setup.
        self.initialize()

    def backendRateCallbackBuckets(self,
                                   cpp_coords,
                                   coords_len,
                                   occupations,
                                   update,
                                   types_map,
                                   rate_constant,
                                   process_number,
                                   global_x,
                                   global_y,
                                   global_z):
        """
        Function called from C++ to get the rate. It function recieves
        the data from C++ and parse it to a Python friendly format to send it
        forward to the custom rate function.
        """
        # PERFORMME: move operations to C++.

        # Determine the occupations after the move.
        occupations_after = Backend.StdVectorTypeBucket()

        for i in range(len(update)):
            occupations_after.push_back(occupations[i].add(update[i]))

        # Call and return the custom rate.
        global_coordinate = (global_x, global_y, global_z)
        return self.rate(numpy.array(cpp_coords).reshape(coords_len,3),
                         stdVectorTypeBucketToPython(occupations, types_map),
                         stdVectorTypeBucketToPython(occupations_after, types_map),
                         rate_constant,
                         process_number,
                         global_coordinate)

    def backendRateCallback(self,
                            cpp_coords,
                            coords_len,
                            types_before,
                            types_after,
                            rate_constant,
                            process_number,
                            global_x,
                            global_y,
                            global_z):
        """
        Function called from C++ to get the rate. It function recieves
        the data from C++ and parse it to a Python friendly format to send it
        forward to the custom rate function.
        """
        # Call and return the custom rate.
        # PERFORMME: Consider creating the numpy array in C++ if possible.
        global_coordinate = (global_x, global_y, global_z)
        return self.rate(numpy.array(cpp_coords).reshape(coords_len,3),
                         types_before,
                         types_after,
                         rate_constant,
                         process_number,
                         global_coordinate)

    def initialize(self):
        """
        Called as the last statement in the base class constructor
        to allow for custom setup of the object.
        """
        pass

    def rate(self,
             coords,
             types_before,
             types_after,
             rate_constant,
             process_number,
             global_coordinate):
        """
        Called from the base class to get the rate for a particular
        local geometry. Any class inheriting from the plugin base class
        must provide an implementation of this function.

        :param coords: The coordinates of the configuration as a Nx3 numpy array
                       in fractional units of the primitive cell.

        :param types_before: The types before the process, as tuple of strings.

        :param types_after: The types after the process, as tuple of strings.

        :param rate_constant: The rate constant associated with the process
                              to either update or replace.

        :param process_number: The process id number.

        :param global_coordinate: The global coordinate of the central index.

        :returns: The custom rate of the process. Note that the returned rate must
                  not be negative or zero.
        """
        raise Error("The rate(self,...) API function in the 'KMCRateCalculator' base class must be overloaded when using a custom rate calculator.")

    def cutoff(self):
        """
        To determine the radial cutoff of the geometry around the central
        lattice site to cut out and send down to the rustom rate function.
        If not implemented by derrived classes the default is to use
        the cutoff of the largetst process local geometry.

        :returns: The desiered cutoff in primitive cell internal coordinates.
        :rtype: float
        """
        # Returning None results in default behaviour.
        return None

    def cacheRates(self):
        """
        Method for determining if custom rates should be cached.
        If caching is used for a process, the process will only be evaluated once
        for each particular environment, not allowing for changes of the rate with
        the global index or with the simulation time. If cached rates are used
        processes can be exluded from the caching using the excludeFromCaching function.

        :returns: True for caching or False for no caching. Defaults to False.
        :rtype: bool
        """
        return False

    def excludeFromCaching(self):
        """
        Method for exluding processes from the rate caching.
        Overload for custom behavior. The method only takes effect if caching
        is enabled with the cacheRates function.

        :returns: A tuple of process numbers to exclue from caching.
        :rtype: tuple
        """
        return ()





