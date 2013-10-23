""" Module for the CustomRateCalculator plugin """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.PluginInterfaces.KMCRateCalculatorPlugin import KMCRateCalculatorPlugin

class CustomRateCalculator(KMCRateCalculatorPlugin):
    """ An example of a custom rate calculator plugin. """

    def rate(self,
             coords,
             types_before,
             types_after,
             rate_constant,
             process_number,
             global_coordinate):
        """ Overloaded API rate function. """
        if types_before[0] == "B":
            return 3.0
        else:
            return 1.0
