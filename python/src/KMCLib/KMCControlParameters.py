""" Module for the KMCControlParameters """


# Copyright (c)  2012-2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

from KMCLib.Utilities.CheckUtilities import checkPositiveInteger

class KMCControlParameters(object):
    """
    Class for handling the control parameters that goes in to a KMC simulation.
    """

    def __init__(self,
                 number_of_steps=None,
                 dump_interval=None):
        """
        Constructuor for the KMCControlParameters object that
        holds all parameters controlling the flow of the KMC simulation.

        :param number_of_steps: The number of KMC steps to take. If not provided
                                the default value 0 will  be used.
        :type number_of_steps: int

        :param dump_interval: The number of steps between subsequent trajectory
                              dumps. The default value is 1, i.e. dump every step.
        :type dump_interval: int
        """
        # Check and set the number of steps.
        self.__number_of_steps = checkPositiveInteger(number_of_steps,
                                                      0,
                                                      "number_of_steps")

        self.__dump_interval = checkPositiveInteger(dump_interval,
                                                    1,
                                                    "dump_interval")

    def numberOfSteps(self):
        """
        Query for the number of steps.

        :returns: The number of steps.
        """
        return self.__number_of_steps

    def dumpInterval(self):
        """
        Query for the dump interval.

        :returns: The dump interval.
        """
        return self.__dump_interval



