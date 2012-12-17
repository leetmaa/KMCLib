/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_interactions.h"

// Include the files to test.
#include "../src/interactions.h"

#include "../src/configuration.h"
#include "../src/matchlistentry.h"

// -------------------------------------------------------------------------- //
//
void Test_Interactions::testConstruction()
{
    std::vector<Process> processes;
    Interactions interactions(processes);

    // DONE
}

// -------------------------------------------------------------------------- //
//
void Test_Interactions::testQuery()
{
    // Setup two valid processes.
    std::vector<Process> processes;

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["V"] = 2;

    // A process that independent of local environment swaps a "B" to "V"
    {
        const std::vector<std::string> process_elements1(1,"B");
        const std::vector<std::string> process_elements2(1,"V");
        const std::vector<std::vector<double> > process_coordinates(1, std::vector<double>(3, 0.0));
        const double barrier = 1.234;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, barrier);
        // Store twize.
        processes.push_back(p);
        processes.push_back(p);
    }

    // A process that finds an A between two B's in the 1,1,1 direction
    // and swap the A and the first B.
    {
        std::vector<std::string> process_elements1(3);
        process_elements1[0] = "A";
        process_elements1[1] = "B";
        process_elements1[2] = "B";

        std::vector<std::string> process_elements2(3);
        process_elements2[0] = "B";
        process_elements2[1] = "A";
        process_elements2[2] = "B";

        std::vector<std::vector<double> > process_coordinates(3, std::vector<double>(3, 0.0));

        process_coordinates[1][0] = -0.25;
        process_coordinates[1][1] = -0.25;
        process_coordinates[1][2] = -0.25;
        process_coordinates[2][0] =  0.25;
        process_coordinates[2][1] =  0.25;
        process_coordinates[2][2] =  0.25;

        const double barrier = 13.7;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, barrier);
        processes.push_back(p);
    }

    // Setup the interactions object.
    Interactions interactions(processes);

    // Query for the processes.
    const std::vector<Process> & queried_processes = interactions.processes();

    // Check the length of the list of processes.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(queried_processes.size()), 3 );

    // Get the types in the queried processes and check.
    CPPUNIT_ASSERT_EQUAL( queried_processes[0].matchList()[0].matchType(), 1 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[0].matchList()[0].updateType(), 2 );

    CPPUNIT_ASSERT_EQUAL( queried_processes[2].matchList()[2].matchType(), 1 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[2].matchList()[2].updateType(), 1 );


}

