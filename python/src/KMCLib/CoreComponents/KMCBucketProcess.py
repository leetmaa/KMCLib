""" Module for the KMCBucketProcess class """


# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import numpy

from KMCLib.CoreComponents.KMCBaseProcess import KMCBaseProcess
from KMCLib.CoreComponents.KMCLocalConfiguration import KMCLocalConfiguration
from KMCLib.Utilities.CheckUtilities import checkAndNormaliseBucketEntry
from KMCLib.Exceptions.Error import Error
from KMCLib.Backend import Backend

class KMCBucketProcess(KMCBaseProcess):
    """
    Class for representing a process in a lattice KMC simulation where
    more than one particle is allowed on each lattice site.
    """

    def __init__(self,
                 coordinates=None,
                 minimum_match=None,
                 update=None,
                 basis_sites=None,
                 rate_constant=None):
        """
        Constructor for the KMCBucketProcess.

        :param coordinates:   The local coordinates, corresponding to the lattice
                              positions of the surrounding of the center where
                              the process will be preformed.

        :param minimum_match: The list of minimal occupations to match.

        :param update:        List of positive and negative changes in occupations
                              due to performing the process.

        :param basis_sites:   The basis sites in the lattice at which the process
                              can possibly be applied. Only if the length of this
                              list is 1 can implicit wildcards be used for the
                              matching.

        :param rate_constant: The rate constant associated with this process.
        :type rate_constant: float
        """
        # Call the base class constructor.
        super(KMCBucketProcess, self).__init__(coordinates, basis_sites, rate_constant)

        # Check the minimum match.
        self.__minimum_match = [checkAndNormaliseBucketEntry(m) for m in minimum_match]

        if not len(self.__minimum_match) == len(coordinates):
            raise Error("The types 'match' information to a KMCBucketProcess constructor must be of the same length as the coordinates.")

        # Check the update information.
        self.__update = self.__checkUpdateInfo(update)

        # Construct elements before based on the match and update.
        self._elements_before = self.__minimum_match

        # Get all present types.
        all_types = []
        for e in self._elements_before:
            if isinstance(e, str):
                all_types.append(e)
            else:
                for ee in e:
                    if isinstance(ee, str):
                        all_types.append(ee)
                    elif isinstance(ee, tuple):
                        all_types.append(ee[1])

        # Remove doublets and store on the class.
        self._all_present_types = list(set(all_types))

        # We are not allowed to use move vectors with bucket processes.
        self._move_vectors = []

        # Setup the local configurations.
        center = 0
        c1 = KMCLocalConfiguration(self._coordinates, self._elements_before, center)
        self._local_configurations = (c1,)

        # To be updated later.
        self.__cpp_update = None

    def __checkUpdateInfo(self, update_info):
        """ """
        """
        Private helper function to check the update information.

        :param update_info: The update information to check.
        :returns: The checked update information.
        """
        # FIXME
        return update_info

    def _update(self):
        """
        Query for the update info.

        :returns: The update stored on the class.
        """
        if self.__cpp_update is None:

            # Create the cpp update vector of maps.
            self.__cpp_update = Backend.StdVectorStdMapStringInt()

            # Take each entry in the update list.
            for update_entry in self.__update:

                # Convert to C++
                cpp_map = Backend.StdMapStringInt()
                for e in update_entry:
                    cpp_map[e[1]] = e[0]

                # Add.
                self.__cpp_update.push_back(cpp_map)

        return self.__cpp_update


