""" Module for testing the Trajectory object. """


# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

import unittest
import os
import numpy
import time
import sys

# Import from the module we test.
from KMCLib.Utilities.Trajectory.Trajectory import Trajectory


# Implement the test.
class TrajectoryTest(unittest.TestCase):
    """ Class for testing the Trajectory object. """

    def setUp(self):
        """ The setUp method for test fixtures. """
        self.__files_to_remove = []

    def tearDown(self):
        """ The tearDown method for test fixtures. """
#        for f in self.__files_to_remove:
#            os.remove(f)

    def testConstruction(self):
        """ Test the Trajectory object can be constructed. """
        # Setup input.
        sites = [[1.0,1.0,2.3],
                 [3.4,4.5,4.3],
                 [3.7,7.5,6.5]]
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory_file.py")
        self.__files_to_remove.append(trajectory_filename)

        # Construct with default values.
        dt = Trajectory(trajectory_filename, sites)

        # Check the defaults.
        self.assertAlmostEqual( dt._Trajectory__max_buffer_time,  60*30 )
        self.assertEqual( dt._Trajectory__max_buffer_size, 1024*1024*10 )

        # Construct.
        t = Trajectory(trajectory_filename,
                       sites,
                       max_buffer_time=100.0,
                       max_buffer_size=100000)

        # Check the stored values.
        self.assertEqual( t._Trajectory__max_buffer_time,  100.0 )
        self.assertEqual( t._Trajectory__max_buffer_size, 100000 )
        self.assertEqual( t._Trajectory__trajectory_filename, trajectory_filename )

        # Open the trajectory file and check that we can read the meta
        # information and sites.
        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        # Get the version and creation time.
        read_version  = local_dict["version"]
        read_creation = local_dict["creation_time"]

        # Check that they are of corect type.
        self.assertTrue( isinstance(read_version, str) )
        self.assertTrue( isinstance(read_creation, str) )

        # Check the sites.
        read_sites = numpy.array(local_dict["sites"])
        ref_sites  = numpy.array(sites)
        self.assertAlmostEqual( numpy.linalg.norm(read_sites - ref_sites), 0.0, 10 )

        # Check the empty lists.
        read_times = local_dict["times"]
        read_steps = local_dict["steps"]
        read_types = local_dict["types"]
        empty_list = []
        self.assertEqual( read_times, empty_list )
        self.assertEqual( read_steps, empty_list )
        self.assertEqual( read_types, empty_list )

    def testVersion(self):
        """ Test the Trajectory file version number string. """
        # Setup input.
        sites = [[1.0,1.0,2.3],
                 [3.4,4.5,4.3],
                 [3.7,7.5,6.5]]
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory_file.py")
        self.__files_to_remove.append(trajectory_filename)

        # Construct.
        t = Trajectory(trajectory_filename, sites)

        # Open the trajectory file and check that we can read the meta
        # information and sites.
        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        # Get the version and check.
        read_version  = local_dict["version"]
        ref_version = "2013.1.0"
        self.assertEqual( read_version, ref_version )

    def testWriteToFile(self):
        """ Test writing the buffers to file. """
        # Setup input.
        sites = [[0.0,1.0,2.3],
                 [1.0,0.0,2.3],
                 [1.0,1.0,0.3],
                 [1.0,1.0,2.3],
                 [3.4,4.5,4.3],
                 [3.4,4.3,4.3],
                 [3.4,5.5,4.3],
                 [3.7,7.5,6.5]]
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory_file.py")
        self.__files_to_remove.append(trajectory_filename)

        # Construct.
        t = Trajectory(trajectory_filename, sites)

        # Write times, steps and typers.
        times = [1.10045, 2.334156, 3.4516410]
        steps = [12, 25, 52]
        types = [["ThisIsTheLongestTypeNameIHaveEverEncounteredPerhaps",
                  "here", "is", "Next", "Long", "List", "Offffffff", "Names", "now", "this", "one", "is", "longer", "still"],
                 ["A", "B", "C", "D", "E", "F", "G", "H"],
                 ["1", "2", "4", "5", "6", "5" ,"43", "243r2424"]]

        # Check that the time is zero before we start.
        self.assertAlmostEqual( t._Trajectory__time_last_dump, 0.0, 10 )

        t._Trajectory__writeToFile(times, steps, types)

        # Check that the time stamp was updated.
        self.assertTrue( 1357651850 < t._Trajectory__time_last_dump )
        last_time = t._Trajectory__time_last_dump

        # Check the info stored in the file.
        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        # Check the types.
        ret_types = local_dict['types']
        ref_types = [['ThisIsTheLongestTypeNameIHaveEverEncounteredPerhaps',
                      'here', 'is', 'Next', 'Long', 'List', 'Offffffff',
                      'Names', 'now', 'this', 'one', 'is', 'longer', 'still'],
                     ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'],
                     ['1', '2', '4', '5', '6', '5', '43', '243r2424']]
        self.assertEqual( ret_types, ref_types )

        # Check the steps.
        ret_steps = local_dict['steps']
        ref_steps = [12, 25, 52]
        self.assertEqual( ret_steps, ref_steps )

        # Check the times.
        ret_times = local_dict['times']
        ref_times = [1.10045, 2.334156, 3.451641]
        self.assertEqual( ret_times, ref_times )

        # Sleep for two seconds before we add again.
        time.sleep(1)
        t._Trajectory__writeToFile(times, steps, types)

        # Check the time.
        self.assertTrue( (t._Trajectory__time_last_dump - last_time > 1) )

        # Now, check the file again.
        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        # Check the types.
        ret_types = local_dict['types']
        ref_types = [['ThisIsTheLongestTypeNameIHaveEverEncounteredPerhaps',
                      'here', 'is', 'Next', 'Long', 'List', 'Offffffff',
                      'Names', 'now', 'this', 'one', 'is', 'longer', 'still'],
                     ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'],
                     ['1', '2', '4', '5', '6', '5', '43', '243r2424'],
                     ['ThisIsTheLongestTypeNameIHaveEverEncounteredPerhaps',
                      'here', 'is', 'Next', 'Long', 'List', 'Offffffff',
                      'Names', 'now', 'this', 'one', 'is', 'longer', 'still'],
                     ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'],
                     ['1', '2', '4', '5', '6', '5', '43', '243r2424']]
        self.assertEqual( ret_types, ref_types )

        # Check the steps.
        ret_steps = local_dict['steps']
        ref_steps = [12, 25, 52, 12, 25, 52]
        self.assertEqual( ret_steps, ref_steps )

        # Check the times.
        ret_times = local_dict['times']
        ref_times = [1.10045, 2.334156, 3.451641, 1.10045, 2.334156, 3.451641]
        self.assertEqual( ret_times, ref_times )

    def testAppend(self):
        """ Test appending to the trajectory. """
        # Setup input.
        sites = [[0.0,1.0,2.3],
                 [1.0,0.0,2.3],
                 [1.0,1.0,0.3],
                 [1.0,1.0,2.3],
                 [3.4,4.5,4.3],
                 [3.4,4.3,4.3],
                 [3.4,5.5,4.3],
                 [3.7,7.5,6.5]]
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory_file.py")
        self.__files_to_remove.append(trajectory_filename)

        # Construct.
        t = Trajectory(trajectory_filename, sites)

        # Append times, steps and typers.
        t.append(1.10045, 12,
                 ["A", "A", "A", "A", "A", "A"] )

        # Since this was the first time it should have triggered a dump to file.
        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        # Check the types.
        ret_types = local_dict['types']
        ref_types = [["A", "A", "A", "A", "A", "A"]]
        self.assertEqual( ret_types, ref_types )

        # Check the steps.
        ret_steps = local_dict['steps']
        ref_steps = [12]
        self.assertEqual( ret_steps, ref_steps )

        # Check the times.
        ret_times = local_dict['times']
        ref_times = [1.10045]
        self.assertEqual( ret_times, ref_times )

        # Appending again directly makes no dump.
        t.append(1.1993, 14,
                 ["B", "B", "B", "B", "B", "B"] )

        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        # Check.
        ret_types = local_dict['types']
        self.assertEqual( ret_types, ref_types )
        ret_steps = local_dict['steps']
        self.assertEqual( ret_steps, ref_steps )
        ret_times = local_dict['times']
        self.assertEqual( ret_times, ref_times )

        # But if we dump again and set the time limit to zero we will trigger a dump.
        t._Trajectory__max_buffer_time = 0.0
        t.append(1.199, 19,
                 ["C", "C", "C", "C", "C", "C"] )

        # Reset the time to some thing large.
        t._Trajectory__max_buffer_time = 100000000000.0

        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        # Check the types.
        ret_types = local_dict['types']
        ref_types = [["A","A","A","A","A","A"],
                     ["B","B","B","B","B","B"],
                     ["C","C","C","C","C","C"]]
        self.assertEqual( ret_types, ref_types )

        # Check the steps.
        ret_steps = local_dict['steps']
        ref_steps = [12,14,19]
        self.assertEqual( ret_steps, ref_steps )

        # Check the times.
        ret_times = local_dict['times']
        ref_times = [1.10045, 1.1993, 1.199]
        self.assertEqual( ret_times, ref_times )

        # The buffers are reset after each dump. If we make the
        # max size limit smaller than the size of the appended types
        # list this must trigger a dump.
        types = ["A","A","A","A","A","B"]
        size = sys.getsizeof(types)
        t._Trajectory__max_buffer_size = size

        # Append.
        t.append(1.1995, 43, types )

        # Check.
        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        ret_types = local_dict['types']
        ref_types = [["A","A","A","A","A","A"],
                     ["B","B","B","B","B","B"],
                     ["C","C","C","C","C","C"]]
        self.assertEqual( ret_types, ref_types )

        # Append.
        t.append(1.1995, 43, types )
        t.append(1.1995, 43, types )
        t.append(1.1995, 43, types )
        # This last one triggers the dump.
        t.append(1.1995, 43, types )

        # Check.
        global_dict = {}
        local_dict  = {}
        execfile(trajectory_filename, global_dict, local_dict)

        ret_types = local_dict['types']
        ref_types = [["A","A","A","A","A","A"],
                     ["B","B","B","B","B","B"],
                     ["C","C","C","C","C","C"],
                     ["A","A","A","A","A","B"],
                     ["A","A","A","A","A","B"],
                     ["A","A","A","A","A","B"],
                     ["A","A","A","A","A","B"],
                     ["A","A","A","A","A","B"]]
        self.assertEqual( ret_types, ref_types )


if __name__ == '__main__':
    unittest.main()
