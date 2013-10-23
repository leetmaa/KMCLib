"""" Module for testing the KMCAnalysisPlugin """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest


# Import the module to test.
from KMCLib.PluginInterfaces.KMCAnalysisPlugin import KMCAnalysisPlugin

# Implementing the tests.
class KMCAnalysisPluginTest(unittest.TestCase):
    """ Class for testing the KMCAnalysisPlugin class """

    def testConstructionBaseClass(self):
        """ Test that we can construct the base class. """
        ap = KMCAnalysisPlugin()
        self.assertTrue( isinstance(ap, KMCAnalysisPlugin) )

    def testInterfaceFunctions(self):
        """ Test that we can construct a derrived class. """
        ap = KMCAnalysisPlugin()

        # Check that the functions exist.
        self.assertTrue( hasattr(ap, "setup") )
        self.assertTrue( hasattr(ap, "finalize") )
        self.assertTrue( hasattr(ap, "registerStep") )

        # Call them with dummy arguments.
        ap.setup("time-dummy", "step-dummy", "config-dummy")
        ap.registerStep("time-dummy", "step-dummy", "config-dummy")
        ap.finalize()

        # Done.


if __name__ == '__main__':
    unittest.main()

