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
                 dump_interval=None,
                 analysis_interval=None,
                 seed=None):
        """
        Constructuor for the KMCControlParameters object that
        holds all parameters controlling the flow of the KMC simulation.

        :param number_of_steps: The number of KMC steps to take. If not provided
                                the default value 0 will  be used.
        :type number_of_steps: int

        :param dump_interval: The number of steps between subsequent trajectory
                              dumps. The default value is 1, i.e. dump every step.
        :type dump_interval: int

        :param analysis_interval: The number of steps between subsequent calls to
                                  the custom analysis 'registerStep' functions.
                                  The default value is 1, i.e. analysis every step.
        :type analysis_interval: int

        :param seed: The seed value to use for the backend random number generator.
                     If no seed value is given the random numnber generator will be
                     seeded based on the wall-clock time.
        :type seed: int

        """
        # Check and set the number of steps.
        self.__number_of_steps = checkPositiveInteger(number_of_steps,
                                                      0,
                                                      "number_of_steps")

        self.__dump_interval = checkPositiveInteger(dump_interval,
                                                    1,
                                                    "dump_interval")

        self.__analysis_interval = checkPositiveInteger(analysis_interval,
                                                        1,
                                                        "analysis_interval")

        self.__time_seed = (seed is None)
        self.__seed = checkPositiveInteger(seed,
                                           1,
                                           "seed")

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

    def analysisInterval(self):
        """
        Query for the analysis interval.

        :returns: The analysis interval.
        """
        return self.__analysis_interval

    def seed(self):
        """
        Query for the seed value.

        :returns: The seed value.
        """
        return self.__seed

    def timeSeed(self):
        """
        Query for the time seed value.

        :returns: The time seed value.
        """
        return self.__time_seed

