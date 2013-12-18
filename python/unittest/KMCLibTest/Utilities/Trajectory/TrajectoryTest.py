""" Module for testing the Trajectory base class. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import time

# Import from the module we test.
from KMCLib.Utilities.Trajectory.Trajectory import Trajectory


# Implement the test.
class TrajectoryTest(unittest.TestCase):
    """ Class for testing the Trajectory base class. """

    def testConstruction(self):
        """ Test that we can instantiate the Trajectory base class. """
        t = Trajectory(trajectory_filename="filename",
                       max_buffer_size=100,
                       max_buffer_time=30.0)
        # Check the member data.
        self.assertEqual(t._trajectory_filename, "filename")
        self.assertEqual(t._Trajectory__max_buffer_size, 100)
        self.assertEqual(t._Trajectory__max_buffer_time, 30.0)

    def testDefaultConstruction(self):
        """ Test default construction. """
        t = Trajectory(trajectory_filename="filename")

        # Check the member data.
        self.assertEqual(t._trajectory_filename, "filename")
        self.assertEqual(t._Trajectory__max_buffer_size, 10485760) # 10 MB
        self.assertEqual(t._Trajectory__max_buffer_time, 1800.0)   # 30 min

    def testAppendInterface(self):
        """ Test that the append function can not be called directly from the baseclass. """
        # Setup data.
        simulation_time = 12.3
        step = 14
        class Config:
            def __init__(self):
                pass
        configuration = Config()

        # Instantiate a trajectory.
        t = Trajectory(trajectory_filename="filename")

        # Call append.
        self.assertRaises(AttributeError,
                          lambda: t.append(simulation_time, step, configuration))

        # Add the missing function.
        def storeData(*args, **kwargs):
            pass
        t._storeData = storeData

        self.assertRaises(AttributeError,
                          lambda: t.append(simulation_time, step, configuration))

        # Add the missing function.
        def bufferSize(*args, **kwargs):
            pass
        t._bufferSize = bufferSize

        self.assertRaises(AttributeError,
                          lambda: t.append(simulation_time, step, configuration))

        # Add the missing function.
        def flush(*args, **kwargs):
            pass
        t.flush = flush

        # Now it works.
        t.append(simulation_time, step, configuration)

    def testAppendAndFlushBufferSize(self):
        """ Test that the append function flushes when the buffer size is large. """
        # Setup data.
        simulation_time = 12.3
        step = 14
        class Config:
            def __init__(self):
                pass
        configuration = Config()

        # Instantiate a trajectory.
        t = Trajectory(trajectory_filename="filename")

        # Add the needed functions.
        def storeData(*args, **kwargs):
            pass
        t._storeData = storeData

        ret_buffer = 10.0
        def bufferSize(*args, **kwargs):
            return ret_buffer
        t._bufferSize = bufferSize

        class Dummy:
            self.fc = False

        d = Dummy()
        def flush():
            d.fc = True

        t.flush = flush

        # Append.
        t.append(simulation_time, step, configuration)

        # The first time allways flushes.
        self.assertTrue(d.fc)

        # Append again.
        d.fc = False
        t.append(simulation_time, step, configuration)
        self.assertFalse(d.fc)

        # Change the value of the buffer size so that flush() gets called.
        ret_buffer = t._Trajectory__max_buffer_size + 1

        # Append.
        t.append(simulation_time, step, configuration)

        # Check.
        self.assertTrue(d.fc)

    def testAppendAndFlushTime(self):
        """ Test that the append function flushes when enough time has passed. """
        # Setup data.
        simulation_time = 12.3
        step = 14
        class Config:
            def __init__(self):
                pass
        configuration = Config()

        # Instantiate a trajectory.
        t = Trajectory(trajectory_filename="filename")

        # Add the needed functions.
        def storeData(*args, **kwargs):
            pass
        t._storeData = storeData

        ret_buffer = 10.0
        def bufferSize(*args, **kwargs):
            return ret_buffer
        t._bufferSize = bufferSize

        class Dummy:
            self.fc = False

        d = Dummy()
        def flush():
            d.fc = True

        t.flush = flush

        # Append.
        t.append(simulation_time, step, configuration)

        # The first time allways flushes.
        self.assertTrue(d.fc)

        # Append again.
        d.fc = False
        t.append(simulation_time, step, configuration)
        self.assertFalse(d.fc)

        # Change the value of the max buffer time so that flush() gets called.
        t._Trajectory__max_buffer_time = 0.0

        # Append.
        t.append(simulation_time, step, configuration)

        # Check.
        self.assertTrue(d.fc)

    def testAppendTimeUpdate(self):
        """ Test that the append function updates the time only when it flushes. """
        # Setup data.
        simulation_time = 12.3
        step = 14
        class Config:
            def __init__(self):
                pass
        configuration = Config()

        # Instantiate a trajectory.
        t = Trajectory(trajectory_filename="filename")

        # Add the needed functions.
        def storeData(*args, **kwargs):
            pass
        t._storeData = storeData

        ret_buffer = 10.0
        def bufferSize(*args, **kwargs):
            return ret_buffer
        t._bufferSize = bufferSize

        class Dummy:
            self.fc = False

        d = Dummy()
        def flush():
            d.fc = True

        t.flush = flush

        # Append.
        t.append(simulation_time, step, configuration)

        # The first time allways flushes.
        self.assertTrue(d.fc)

        # This updates the time last dump variable.
        ref_time_last_dump = t._Trajectory__time_last_dump

        # Sleep 10 ms.
        time.sleep(0.01)

        # Append again.
        d.fc = False
        t.append(simulation_time, step, configuration)
        self.assertFalse(d.fc)

        # This does not update the time last dump.
        self.assertAlmostEqual(ref_time_last_dump,  t._Trajectory__time_last_dump, 10)

        # Change the value of the max buffer time so that flush() gets called.
        t._Trajectory__max_buffer_time = 0.0

        # Sleep 10 ms.
        time.sleep(0.01)

        # Append.
        t.append(simulation_time, step, configuration)

        # Check.
        self.assertTrue(d.fc)

        # The time last dump should have been updated.
        self.assertTrue( (t._Trajectory__time_last_dump - ref_time_last_dump) > 0.0001 )


if __name__ == '__main__':
    unittest.main()
