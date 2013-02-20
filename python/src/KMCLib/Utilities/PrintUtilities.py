""" Module for holding common printing utility functions. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


from KMCLib.Backend.Backend import MPICommons

def prettyPrint(msg):
    """
    Utility function for printing an output string to screen.
    :param msg: The message to print.
    :type msg: str
    """
    if MPICommons.isMaster():
        print msg
    MPICommons.barrier()
