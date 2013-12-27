""" Module for testing the common print utilities. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import StringIO
import sys

from KMCLib.Backend.Backend import MPICommons

# Import from the module we test.
from KMCLib.Utilities.PrintUtilities import prettyPrint


# Implement the test.
class PrintUtilitiesTest(unittest.TestCase):
    """ Class for testing the print utility functions. """

    def testPrettyPrint(self):
        """ Test that we can call the pretty print function. """
        # Print to stdout.
        original_sys_stdout = sys.stdout
        try:
            stream_1   = StringIO.StringIO()
            sys.stdout = stream_1

            # Print to stdout.
            ref_str = "This is what we print"
            prettyPrint(ref_str)

            # Check.
            if MPICommons.myRank() == 0:
                ref_str = ref_str + "\n"
            else:
                ref_str = ""
            self.assertEqual(stream_1.getvalue(), ref_str)

        finally:
            # Put the original stdout back.
            sys.stdout = original_sys_stdout

        # Print to another stream.
        stream_2 = StringIO.StringIO()
        ref_str = "This is what we print next time."
        prettyPrint(ref_str, output=stream_2)

        # Check.
        if MPICommons.myRank() == 0:
            ref_str = ref_str + "\n"
        else:
            ref_str = ""

        self.assertEqual(stream_2.getvalue(), ref_str)

if __name__ == '__main__':
    unittest.main()
