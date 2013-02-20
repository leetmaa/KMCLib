""" Module for holding common printing utility functions. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import sys

from KMCLib.Backend.Backend import MPICommons


def prettyPrint(msg, output=None):
    """
    Utility function for printing an output string to screen.

    :param msg: The message to print.
    :type msg: str

    :param out: The stream to write to. Defaults to sys.stdout.
    """
    # Set the default.
    if output is None:
        output = sys.stdout

    # Write.
    if MPICommons.isMaster():
        output.write(msg)
        output.write("\n")
    MPICommons.barrier()
