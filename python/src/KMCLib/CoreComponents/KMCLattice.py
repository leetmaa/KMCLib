""" Module containing the KMCLattice class. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import numpy

from KMCLib.Backend import Backend
from KMCLib.CoreComponents.KMCUnitCell import KMCUnitCell
from KMCLib.Utilities.CheckUtilities import checkSequence
from KMCLib.Exceptions.Error import Error


class KMCLattice(object):
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
        if not isinstance(unit_cell, KMCUnitCell):
            raise Error("The 'unit_cell parameter to the KMCLattice constructor must be of type KMCUnitCell.")
        self.__unit_cell = unit_cell

        # Passed the tests, store.
        self.__repetitions = self.__checkRepetitions(repetitions)

        # Check the periodic input.
        self.__periodic = self.__checkPeriodic(periodic)

        # Generate the lattice sites.
        self.__sites = self.__generateLatticeSites()

        # Set the lattice map to be generated at first query.
        self.__lattice_map = None

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

        # Check for the occurance of too small values.
        for val in repetitions:
            if val < 1:
                raise Error("The all elements in the 'repetitions' parameter must be larger than zero.")

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

    def __generateLatticeSites(self):
        """ """
        """
        Private helper function to generate the sites data based on the basis and repetitions
        stored on the class.
        """
        # Get the basis out of the unit cell.
        basis = self.__unit_cell.basis()

        # Loop through all repetitions and insert the sites.
        sites = []
        for i in range(self.__repetitions[0]):
            for j in range(self.__repetitions[1]):
                for k in range(self.__repetitions[2]):
                    # For each point in the basis, add the translation.
                    translation = numpy.array([i,j,k])
                    for b in basis:
                        site = b+translation
                        sites.append(site)

        # Return the data.
        return numpy.array(sites)

    def sites(self):
        """
        Query function for the sites data.

        :returns: The sites data in fractional units of the original unit cell.
        """
        return self.__sites

    def repetitions(self):
        """
        Query function for the repetitions.

        :returns: The repetitions used to construct the full lattice from the unit cell.
        """
        return self.__repetitions

    def periodic(self):
        """
        Query function for the periodicity.

        :returns: The periodicity information along the a b and c directions.
        """
        return self.__periodic

    def basis(self):
        """
        Query function for the basis.

        :returns: The basis of the unit cell.
        """
        return self.__unit_cell.basis()

    def unitCell(self):
        """
        Query function for the unit cell.

        :returns: The unit cell.
        """
        return self.__unit_cell

    def _globalIndex(self, i, j, k, b):
        """
        Utility function to calculate the index position in the full arrays (sites, types) given the cell indices.

        :param i: The cell number in the 'a' direction.
        :type i: int

        :param j: The cell number in the 'b' direction.
        :type j: int

        :param k: The cell number in the 'c' direction.
        :type k: int

        :param b: The index number of the basis point in the cell.
        :type b: int

        :returns: The global index.
        """
        nI = self.__repetitions[0]
        nJ = self.__repetitions[1]
        nK = self.__repetitions[2]
        nB = len(self.__unit_cell.basis())

        # Calculate the index and return.
        return i*nJ*nK*nB + j*nK*nB + k*nB + b

    def _map(self):
        """
        Query for the lattice map.
        """
        # Generate the lattice map if not done allready.
        if self.__lattice_map is None:
            self.__lattice_map = Backend.LatticeMap(len(self.__unit_cell.basis()),
                                                    Backend.StdVectorInt(self.__repetitions),
                                                    Backend.StdVectorBool(self.__periodic))
        # Return the lattice map.
        return self.__lattice_map

    def _script(self, variable_name="lattice"):
        """
        Generate a script representation of an instance.

        :param variable_name: A name to use as variable name for
                              the KMCLattice in the generated script.
        :type variable_name: str

        :returns: A script that can generate this lattice.
        """
        # Get the unit cell script.
        unit_cell_script = self.__unit_cell._script(variable_name="unit_cell")

        # To generate the repetitions string.
        nI = self.__repetitions[0]
        nJ = self.__repetitions[1]
        nK = self.__repetitions[2]

        # Generate the lattice string.
        lattice_string = variable_name + """ = KMCLattice(
    unit_cell=unit_cell,
    repetitions=(%i,%i,%i),
    periodic=%s)
"""%(nI, nJ, nK, str(self.__periodic))

        # Add the comment.
        comment_string = """
# -----------------------------------------------------------------------------
# Lattice

"""
        return unit_cell_script + comment_string + lattice_string

