""" Module for testing the LatticeTrajectory object. """


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
from KMCLib.Utilities.Trajectory.LatticeTrajectory import LatticeTrajectory

from KMCLib.Backend.Backend import MPICommons


class Config:
    def __init__(self, sites):
        self._sites = sites
    def sites(self):
        return self._sites

# Implement the test.
class LatticeTrajectoryTest(unittest.TestCase):
    """ Class for testing the LatticeTrajectory object. """

    def setUp(self):
        """ The setUp method for test fixtures. """
        self.__files_to_remove = []

    def tearDown(self):
        """ The tearDown method for test fixtures. """
        for f in self.__files_to_remove:
            os.remove(f)

    def testConstruction(self):
        """ Test the LatticeTrajectory object can be constructed. """
        # Setup input.
        sites = [[1.0,1.0,2.3],
                 [3.4,4.5,4.3],
                 [3.7,7.5,6.5]]
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory_file.py")

        if MPICommons.isMaster():
            self.__files_to_remove.append(trajectory_filename)

        # Construct with default values.
        dt = LatticeTrajectory(trajectory_filename, Config(sites))

        # Check the defaults.
        self.assertAlmostEqual( dt._Trajectory__max_buffer_time,  60*30 )
        self.assertEqual( dt._Trajectory__max_buffer_size, 1024*1024*10 )

        # Construct.
        t = LatticeTrajectory(trajectory_filename,
                              Config(sites),
                              max_buffer_time=100.0,
                              max_buffer_size=100000)

        # Check the stored values.
        self.assertEqual( t._Trajectory__max_buffer_time,  100.0 )
        self.assertEqual( t._Trajectory__max_buffer_size, 100000 )
        self.assertEqual( t._trajectory_filename, trajectory_filename )

        # Open the trajectory file and check that we can read the meta
        # information and sites.
        if MPICommons.isMaster():
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
        """ Test the LatticeTrajectory file version number string. """
        # Setup input.
        sites = [[1.0,1.0,2.3],
                 [3.4,4.5,4.3],
                 [3.7,7.5,6.5]]
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory_file.py")

        if MPICommons.isMaster():
            self.__files_to_remove.append(trajectory_filename)

        # Construct.
        t = LatticeTrajectory(trajectory_filename, Config(sites))

        # Open the trajectory file and check that we can read the meta
        # information and sites.
        if MPICommons.isMaster():
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

        if MPICommons.isMaster():
            self.__files_to_remove.append(trajectory_filename)

        # Construct.
        t = LatticeTrajectory(trajectory_filename, Config(sites))

        # Write times, steps and typers.
        times = [1.10045, 2.334156, 3.4516410]
        steps = [12, 25, 52]
        site_types = [["ThisIsTheLongestTypeNameIHaveEverEncounteredPerhaps",
                  "here", "is", "Next", "Long", "List", "Offffffff", "Names", "now", "this", "one", "is", "longer", "still"],
                 ["A", "B", "C", "D", "E", "F", "G", "H"],
                 ["1", "2", "4", "5", "6", "5" ,"43", "243r2424"]]

        # Check that the time is zero before we start.
        if MPICommons.isMaster():
            self.assertAlmostEqual( t._Trajectory__time_last_dump, 0.0, 10 )

        # This function should be MPI safe.
        t._LatticeTrajectory__writeToFile(times, steps, site_types)

        # Check the info stored in the file.
        if MPICommons.isMaster():
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

        # Write again - MPI safe.
        t._LatticeTrajectory__writeToFile(times, steps, site_types)

        # Now, check the file again.
        if MPICommons.isMaster():
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

    def testWriteToFileBuckets(self):
        """ Test writing buffers in buckets format to file. """
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
        trajectory_filename = os.path.join(name, "tmp_bucktes_traj.py")

        if MPICommons.isMaster():
            self.__files_to_remove.append(trajectory_filename)

        # Construct.
        t = LatticeTrajectory(trajectory_filename, Config(sites))

        # Write times, steps and typers.
        times = [1.10045, 2.334156, 3.4516410]
        steps = [12, 25, 52]
        site_types = [[["A"],[(3, "A"), "A", "B", "B"],["A", "B", "C"]],
                      [[],["A", (3, "A"), "A", "B", "B"],["A", "B", "C"]],
                      [[(2, "C")],["B", "B", "A"],["A", "B"]]]

        # Check that the time is zero before we start.
        if MPICommons.isMaster():
            self.assertAlmostEqual( t._Trajectory__time_last_dump, 0.0, 10 )

        # This function should be MPI safe.
        t._LatticeTrajectory__writeToFile(times, steps, site_types)

        # Check the info stored in the file.
        if MPICommons.isMaster():
            global_dict = {}
            local_dict  = {}
            execfile(trajectory_filename, global_dict, local_dict)

            # Check the types.
            ret_types = local_dict['types']
            ref_types = [[[(1, "A")],
                          [(4, "A"), (2, "B")],
                          [(1, "A"), (1, "B"), (1, "C")]],
                         [[],
                          [(5, "A"), (2, "B")],
                          [(1, "A"), (1, "B"), (1, "C")]],
                         [[(2, "C")],[(2, "B"), (1, "A")],
                          [(1, "A"), (1, "B")]]]

            self.assertEqual( ret_types, ref_types )

            # Check the steps.
            ret_steps = local_dict['steps']
            ref_steps = [12, 25, 52]
            self.assertEqual( ret_steps, ref_steps )

            # Check the times.
            ret_times = local_dict['times']
            ref_times = [1.10045, 2.334156, 3.451641]
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

        if MPICommons.isMaster():
            self.__files_to_remove.append(trajectory_filename)

        class DummyConfig:
            def types(self):
                return site_types

        # Construct.
        t = LatticeTrajectory(trajectory_filename, Config(sites))

        # Append times, steps and typers.
        site_types = ["A", "A", "A", "A", "A", "A"]
        t.append(1.10045, 12, DummyConfig())

        # Since this was the first time it should have triggered a dump to file.
        if MPICommons.isMaster():
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
        site_types = ["B", "B", "B", "B", "B", "B"]
        t.append(1.1993, 14, DummyConfig())

        if MPICommons.isMaster():
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
        site_types = ["C", "C", "C", "C", "C", "C"]
        t.append(1.199, 19, DummyConfig())

        # Reset the time to some thing large.
        t._Trajectory__max_buffer_time = 100000000000.0

        if MPICommons.isMaster():
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
        size = sys.getsizeof(site_types)
        t._Trajectory__max_buffer_size = size

        # Append.
        site_types = ["A","A","A","A","A","B"]
        t.append(1.1995, 43, DummyConfig())

        # Check.
        if MPICommons.isMaster():
            global_dict = {}
            local_dict  = {}
            execfile(trajectory_filename, global_dict, local_dict)

            ret_types = local_dict['types']
            ref_types = [["A","A","A","A","A","A"],
                         ["B","B","B","B","B","B"],
                         ["C","C","C","C","C","C"],
                         ["A","A","A","A","A","B"]]

            self.assertEqual( ret_types, ref_types )

        # Append.
        t.append(1.1995, 43, DummyConfig() )
        t.append(1.1995, 43, DummyConfig() )
        t.append(1.1995, 43, DummyConfig() )
        # This last one triggers the dump.
        t.append(1.1995, 43, DummyConfig() )

        # Check.
        if MPICommons.isMaster():
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

    def testFlush(self):
        """ Make sure we can flush the buffer as expected. """
        # Get a filename.
        name = os.path.abspath(os.path.dirname(__file__))
        name = os.path.join(name, "..", "..")
        name = os.path.join(name, "TestUtilities", "Scratch")
        trajectory_filename = os.path.join(name, "tmp_trajectory_file.py")

        if MPICommons.isMaster():
            self.__files_to_remove.append(trajectory_filename)

        # Construct the trajecctory object.
        sites = [[0.0,1.0,2.3],
                 [1.0,0.0,2.3],
                 [1.0,1.0,0.3],
                 [1.0,1.0,2.3],
                 [3.4,4.5,4.3],
                 [3.4,4.3,4.3],
                 [3.4,5.5,4.3],
                 [3.7,7.5,6.5]]

        # Construct.
        t = LatticeTrajectory(trajectory_filename, Config(sites))

        # To check against.
        empty_list = []

        # Check that the file is created but only with meta information.
        if MPICommons.isMaster():
            global_dict = {}
            local_dict  = {}
            execfile(trajectory_filename, global_dict, local_dict)

            ret_types = local_dict['types']
            self.assertEqual( ret_types, empty_list )

            ret_times = local_dict['times']
            self.assertEqual( ret_times, empty_list )

            ret_steps = local_dict['steps']
            self.assertEqual( ret_steps, empty_list )

        # Fill the buffers.
        t._LatticeTrajectory__types_buffer           = [["ABC", "123"],["123", "ABC"]]
        t._LatticeTrajectory__simulation_time_buffer = [1.234, 5.678]
        t._LatticeTrajectory__step_buffer = [1, 99]

        # Flush the buffers.
        t.flush()

        # Check that the buffers are empty.
        self.assertEqual( t._LatticeTrajectory__types_buffer, empty_list )
        self.assertEqual( t._LatticeTrajectory__simulation_time_buffer, empty_list )
        self.assertEqual( t._LatticeTrajectory__step_buffer, empty_list )

        # Check that the file has the flushed values.
        if MPICommons.isMaster():
            global_dict = {}
            local_dict  = {}
            execfile(trajectory_filename, global_dict, local_dict)

            ret_types = local_dict['types']
            self.assertEqual( ret_types, [["ABC", "123"],["123", "ABC"]] )

            ret_times = local_dict['times']
            self.assertAlmostEqual( ret_times, [1.234, 5.678], 10 )

            ret_steps = local_dict['steps']
            self.assertEqual( ret_steps, [1, 99] )


if __name__ == '__main__':
    unittest.main()
