"""" Module for testing the KMCBreakerPlugin """


# Copyright (c)  2014  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest


# Import the module to test.
from KMCLib.PluginInterfaces.KMCBreakerPlugin import KMCBreakerPlugin

# Implementing the tests.
class KMCBreakerPluginTest(unittest.TestCase):
    """ Class for testing the KMCAnalysisPlugin class """

    def testConstructionBaseClass(self):
        """ Test that we can construct the base class. """
        b = KMCBreakerPlugin()
        self.assertTrue( isinstance(b, KMCBreakerPlugin) )

    def testInterfaceFunctions(self):
        """ Test that we can construct a derrived class. """
        b = KMCBreakerPlugin()

        # Check that the functions exist.
        self.assertTrue( hasattr(b, "setup") )
        self.assertTrue( hasattr(b, "evaluate") )
        self.assertTrue( hasattr(b, "interval") )

        # Call them with dummy arguments.
        b.setup("time-dummy", "step-dummy", "config-dummy")
        b.evaluate("time-dummy", "step-dummy", "config-dummy")

        # Check the default behaviour.
        self.assertEqual(b.interval(), 1)

        # Done.


if __name__ == '__main__':
    unittest.main()

