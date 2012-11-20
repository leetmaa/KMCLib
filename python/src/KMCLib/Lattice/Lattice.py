""" Module containing the Lattice class. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.Lattice.UnitCell import UnitCell
from KMCLib.Utilities.CheckUtilities import checkSequence
from KMCLib.Exceptions.Error import Error

class Lattice:
    """ Class for describing the lattice used in a KMC simulation. """

    def __init__(self,
                 unit_cell=None,
                 repetitions=None,
                 periodic=None):
        """
        Constructor for the Lattice used in the KMC simulations.

        :param unit_cell: The unitcell object to represent the periodicity of the lattice.

        :param repetitions: The repetitions of the unitcell along the a,b and c directions
                            to create the full lattice, given as a list or tuple of three
                            integers > 0. If not given this defaults to (1,1,1)

        :param periodic: A list or tuple indicating if periodicity should be used along the
                         a, b and c directions. If not specified it defaults to (True,True,True)
        :type periodic: (bool,bool,bool)
        """
        # Check and store the unit cell.
        if not isinstance(unit_cell, UnitCell):
            raise Error("The 'unit_cell parameter to the Lattice constructor must be of type UnitCell.")
        self.__unit_cell = unit_cell

        # Passed the tests, store.
        self.__repetitions = self.__checkRepetitions(repetitions)

        # Check the periodic input.
        self.__periodic = self.__checkPeriodic(periodic)


    def __checkRepetitions(self, repetitions):
        """ """
        """
        Private helper routine to check the repetitions input.
        """
        # Handle the default case.
        if repetitions is None:
            repetitions = (1,1,1)

        # Check that it is a sequence.
        repetitions = checkSequence(repetitions, "The 'repetitions' input parametr is not given as a sequence of numbers.")

        # Check its length.
        if len(repetitions) != 3:
            raise Error("The 'repetitions' input parameter must have length == 3.")

        # Check the values.
        for val in repetitions:
            if not isinstance(val, int):
                raise Error("The 'repetitions' input paramter must be given as a list or tuple of three integers, e.g. (5,5,6)")

        # Done.
        return repetitions


    def __checkPeriodic(self, periodic):
        """ """
        """
        Private helper routine to check the periodic input.
        """
        # Handle the default case.
        if periodic is None:
            periodic = (True,True,True)

        # Check that it is a sequence.
        periodic = checkSequence(periodic, "The 'periodic' input parametr is not given as a sequence of bools.")

        # Check its length.
        if len(periodic) != 3:
            raise Error("The 'periodic' input parameter must have length == 3.")

        # Check the values.
        for val in periodic:
            if not isinstance(val, bool):
                raise Error("The 'periodic' input paramter must be given as a list or tuple of three bools, e.g. (True,True,False).")

        # Done.
        return periodic
