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
#include "../src/random.h"

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


// -------------------------------------------------------------------------- //
//
void Test_Interactions::testUpdateAndPick()
{
    // Setup two valid processes.
    std::vector<Process> processes;

    // Setup a vector of dummy processes.
    std::vector<std::string> process_elements1(1);
    process_elements1[0] = "A";

    std::vector<std::string> process_elements2(1);
    process_elements2[0] = "B";

    std::vector<std::vector<double> > process_coordinates(1, std::vector<double>(3, 0.0));

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["V"] = 2;

    const double barrier = 13.7;
    Configuration c1(process_coordinates, process_elements1, possible_types);
    Configuration c2(process_coordinates, process_elements2, possible_types);
    processes.push_back(Process(c1,c2,barrier));
    processes.push_back(Process(c1,c2,barrier));
    processes.push_back(Process(c1,c2,barrier+barrier));
    processes.push_back(Process(c1,c2,barrier));
    processes.push_back(Process(c1,c2,barrier));
    processes.push_back(Process(c1,c2,barrier));

    // Fake a matching by adding sites to the processes.

    // First process, 3 sites.
    processes[0].addSite(12);
    processes[0].addSite(123);
    processes[0].addSite(332);

    // Second process, 2 sites.
    processes[1].addSite(19);
    processes[1].addSite(12);

    // Third process, 4 sites.
    processes[2].addSite(19);
    processes[2].addSite(12);
    processes[2].addSite(234);
    processes[2].addSite(991);

    // The sixth process, one site.
    processes[5].addSite(992);

    // Setup the interactions object.
    Interactions interactions(processes);

    // Update the probability table.
    interactions.updateProbabilityTable();

    // Check the values of the probability table.
    const std::vector<std::pair<double, int> > & probability_table = \
        interactions.probabilityTable();

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(probability_table.size()),
                          static_cast<int>(processes.size()) );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[0].first,  3.0/barrier, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[1].first,  5.0/barrier, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[2].first,  7.0/barrier, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[3].first,  7.0/barrier, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[4].first,  7.0/barrier, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[5].first,  8.0/barrier, 1.0e-14 );

    CPPUNIT_ASSERT_EQUAL( probability_table[0].second, 3 );
    CPPUNIT_ASSERT_EQUAL( probability_table[1].second, 2 );
    CPPUNIT_ASSERT_EQUAL( probability_table[2].second, 4 );
    CPPUNIT_ASSERT_EQUAL( probability_table[3].second, 0 );
    CPPUNIT_ASSERT_EQUAL( probability_table[4].second, 0 );
    CPPUNIT_ASSERT_EQUAL( probability_table[5].second, 1 );


    // Make sure to seed the random number generator before we test any
    // random dependent stuff.
    seedRandom(false, 131);

    // Pick processes from this table with enough statistics should give
    // the distribution proportional to the number of available sites,
    // but with the double barrier for the third process should halve
    // this entry.
    std::vector<int> picked(6,0);
    const int n_loop = 1000000;
    for (int i = 0; i < n_loop; ++i)
    {
        const int p = interactions.pickProcess();
        // Make sure the picked process is not negative or too large.
        CPPUNIT_ASSERT( p >= 0 );
        CPPUNIT_ASSERT( p < static_cast<int>(probability_table.size()) );
        ++picked[p];
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.0*picked[0]/n_loop,
                                  1.0*probability_table[0].second,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.0*picked[1]/n_loop,
                                  1.0*probability_table[1].second,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.0*picked[2]/n_loop,
                                  1.0*probability_table[2].second/2.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.0*picked[3]/n_loop,
                                  1.0*probability_table[3].second,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.0*picked[4]/n_loop,
                                  1.0*probability_table[4].second,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.0*picked[5]/n_loop,
                                  1.0*probability_table[5].second,
                                  1.0e-2 );
}

