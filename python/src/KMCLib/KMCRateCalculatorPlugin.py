""" Module for the KMCRateCalculatorPlugin class """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy

from KMCLib.Backend import Backend
from KMCLib.Exceptions.Error import Error

class KMCRateCalculatorPlugin(Backend.RateCalculator):
    """
    Class for providing an interface to easily extend and customize
    the behaviour of the calculation of individual rates in the KMC simulation.
    """

    def __init__(self):
        """
        Base class constructor.
        """
        # Call the C++ base class constructor.
        Backend.RateCalculator.__init__(self)

        # Call the custom setup.
        self.initialize()

    def backendRateCallback(self,
                            cpp_coords,
                            coords_len,
                            types_before,
                            types_after,
                            rate_constant,
                            process_number):
        """
        Function called from C++ to get the rate. It function recieves
        the data from C++ and parse it to a Python friendly format to send it
        forward to the custom rate function.
        """
        # Call and return the custom rate.
        # PERFORMME: Consider creating the numpy array in C++ if possible.
        return self.rate(numpy.array(cpp_coords).reshape(coords_len,3),
                         types_before,
                         types_after,
                         rate_constant,
                         process_number)

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
             process_number):
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

