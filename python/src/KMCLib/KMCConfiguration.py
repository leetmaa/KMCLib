""" Module for the KMCConfiguration """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


class KMCConfiguration:
    """
    Class for representing the configuration in a KMC simulation.
    """

    def __init__(self,
                 lattice=None,
                 types=None,
                 possible_types=None):
        """
        Constructor for the KMCConfiguration - the configuration object to use
        in the KMC simulations.

        :param lattice: The lattice of the configurartion.
        :type lattice: Lattice

        :param types: The lattice site types at the lattice points, ordered as
                      [a,b,c,i] with i being the fastest and a the slowest index,
                      where a, b and c refers to the cell repetitions and i
                      refers to the specific basis point in the cell.

        :type types:  A sequence of strings of length N corresponding to the N
                      lattice sites in the given lattice.

        :param possible_types: A list of possible types. This list is set to the
                               types present in the types list if not given.
        """
        # NEEDS IMPLEMENTATION
        pass



