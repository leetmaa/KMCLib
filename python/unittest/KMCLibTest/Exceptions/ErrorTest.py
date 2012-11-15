""" Module for testing the Error class. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


import unittest

from exceptions import Exception

# Import from the module we test.
from KMCLib.Exceptions.Error import Error


# Implement the test.
class ErrorTest(unittest.TestCase):
    """ Class for testing the Error class. """

    def testBaseClass(self):
        """ Make sure we inherits the correct way. """
        self.assertTrue(isinstance(Error('msg'), Exception))

    def testRaise(self):
        """ Test that we can raise the error. """
        # Try to raise the Error class as an error.
        with self.assertRaises(Error):
            raise Error("msg")

    def testMessageToString(self):
        """ Test that the message is returned in the __str__ method. """
        # Generate an error with message.
        original_message = "An error message"
        err0 = Error(original_message)

        # Check.
        self.assertEqual(str(err0), original_message)

        # Override the __str__ method.
        class NewError(Error):
            def __str__(self):
                return "WRONG MSG"

        # Setup a new error from the derived class.
        err1 = NewError(original_message)

        # Check.
        self.assertNotEqual(str(err1), original_message)



if __name__ == '__main__':
    unittest.main()

