""" Module for the KMCRateCalculator API class """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.Backend import Backend
from KMCLib.Exceptions.Error import Error


class KMCRateCalculator(Backend.RateCalculator):
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
        self.intialize()

    def backendRateCallback(self):
        """
        Function called from C++ to get the rate.
        """
        # This function should recieve the data from
        # C++ and parse it to a Python friendly format.

        # NEEDS IMPLEMENTATION
        cpp_rate = 1.0

        # Translate data types if needed.
        rate = cpp_rate
        custom_rate = self.rate()

        # Call the custom rate function.
        if self.useAdditiveRate():
            rate += custom_rate
        else:
            rate = custom_rate

        # Return the rate.
        return rate

    def initialize(self):
        """
        Called as the last statement in the base class constructor
        to allow for custom setup of the object.
        """
        pass

    def rate(self):
        """
        Called from the base class to get the rate for a particular
        local geometry. This function must be overloaded.
        """
        raise Error("The rate(self,...) API function in the 'KMCRateCalculator' base class must be overloaded when using a custom rate calculator.")

    def useAdditiveRate(self):
        """
        Called from the base class to determine if the custom rate should
        be added to the rate defined on the process (return True, default),
        or if the rate should be determined completely from the custom
        rate (return False).

        :returns: The flag indicaing if the rate should be added (True) or
                  if the rate rate represents the full rate (False). The default
                  is True.
        """
        return True

