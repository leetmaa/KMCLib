""" Module for the KMCBreakerPlugin class """


# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


class KMCBreakerPlugin(object):
    """
    Class for providing an interface to break the main KMC loop
    triggered by a custom criterion.
    """

    def __init__(self):
        """
        The constructor of the base-class.
        """
        pass

    def setup(self, step, time, configuration):
        """
        Function called right before the start of the KMC loop to allow for
        custom setup of the breaker object.

        :param step: The simulation step number.
        :type step: int

        :param time: The simulation time.
        :type time: float

        :param configuration: The up to date configuration of the simulation.
        :type configuration: KMCConfiguration
        """
        pass

    def evaluate(self, step, time, configuration):
        """
        Called from the KMC loop after the number of steps returned by the
        interval function.

        :param step: The simulation step number.
        :type step: int

        :param time: The simulation time.
        :type time: float

        :param configuration: The up to date configuration of the simulation.
        :type configuration: KMCConfiguration

        :return: True if the KMC loop should stop.
        :rtype: bool
        """
        pass

    def interval(self):
        """
        Called after initialization to get the interval for evaluation.

        :return: The evaluation interval. Defaults to 1.
        :rtype: int
        """
        return 1

