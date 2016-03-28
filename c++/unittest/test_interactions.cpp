/*
  Copyright (c)  2012-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_interactions.h"

// Include the files to test.
#include "interactions.h"

#include "configuration.h"
#include "matchlistentry.h"
#include "random.h"
#include "latticemap.h"
#include "ratecalculator.h"

// -------------------------------------------------------------------------- //
//
void Test_Interactions::testConstruction()
{
    std::vector<Process> processes;
    processes.push_back(Process());
    processes.push_back(Process());
    const bool implicit_wildcards = false;
    Interactions interactions(processes, implicit_wildcards);
    CPPUNIT_ASSERT( !interactions.useCustomRates() );

    // Construct with a rate calculator.
    const RateCalculator rc;
    std::vector<CustomRateProcess> processes2;
    processes2.push_back(CustomRateProcess());
    processes2.push_back(CustomRateProcess());
    Interactions interactions2(processes2, implicit_wildcards, rc);
    CPPUNIT_ASSERT( interactions2.useCustomRates() );
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
        const std::vector<std::vector<std::string> > process_elements1(1, std::vector<std::string>(1, "B"));
        const std::vector<std::vector<std::string> > process_elements2(1, std::vector<std::string>(1, "V"));
        const std::vector<std::vector<double> > process_coordinates(1, std::vector<double>(3, 0.0));
        const double rate = 1.234;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, std::vector<int>(1,0));
        // Store twize.
        processes.push_back(p);
        processes.push_back(p);
    }

    // A process that finds an A between two B's in the 1,1,1 direction
    // and swap the A and the first B.
    {
        std::vector<std::vector<std::string> > process_elements1(3);
        process_elements1[0] = std::vector<std::string>(1, "A");
        process_elements1[1] = std::vector<std::string>(1, "B");
        process_elements1[2] = std::vector<std::string>(1, "B");

        std::vector<std::vector<std::string> > process_elements2(3);
        process_elements2[0] = std::vector<std::string>(1, "B");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "B");

        std::vector<std::vector<double> > process_coordinates(3, std::vector<double>(3, 0.0));

        process_coordinates[1][0] = -0.25;
        process_coordinates[1][1] = -0.25;
        process_coordinates[1][2] = -0.25;
        process_coordinates[2][0] =  0.25;
        process_coordinates[2][1] =  0.25;
        process_coordinates[2][2] =  0.25;

        const double rate = 13.7;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, std::vector<int>(1,0));
        processes.push_back(p);
    }

    // Setup the interactions object.
    Interactions interactions(processes, false);

    // Query for the processes.
    const std::vector<Process*> & queried_processes = interactions.processes();

    // Check the length of the list of processes.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(queried_processes.size()), 3 );

    // Get the types in the queried processes and check.
    CPPUNIT_ASSERT_EQUAL( queried_processes[0]->processMatchList()[0].match_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[0]->processMatchList()[0].match_types[1], 1 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[0]->processMatchList()[0].match_types[2], 0 );

    CPPUNIT_ASSERT_EQUAL( queried_processes[0]->processMatchList()[0].update_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[0]->processMatchList()[0].update_types[1],-1 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[0]->processMatchList()[0].update_types[2], 1 );

    CPPUNIT_ASSERT_EQUAL( queried_processes[2]->processMatchList()[2].match_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[2]->processMatchList()[2].match_types[1], 1 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[2]->processMatchList()[2].match_types[2], 0 );

    CPPUNIT_ASSERT_EQUAL( queried_processes[2]->processMatchList()[2].update_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[2]->processMatchList()[2].update_types[1], 0 );
    CPPUNIT_ASSERT_EQUAL( queried_processes[2]->processMatchList()[2].update_types[2], 0 );

    // Query for the total number of available sites. This is zero since no sites are added to
    // the processes.
    CPPUNIT_ASSERT_EQUAL( interactions.totalAvailableSites(), 0 );

    // Add sites to the processes and see that we get the correct number out.
    interactions.processes()[0]->addSite(12);
    interactions.processes()[0]->addSite(143);
    interactions.processes()[0]->addSite(1654);
    interactions.processes()[0]->addSite(177777);

    interactions.processes()[1]->addSite(12);
    interactions.processes()[1]->addSite(143);

    interactions.processes()[2]->addSite(1654);
    interactions.processes()[2]->addSite(177777);
    interactions.processes()[2]->addSite(177777);

    CPPUNIT_ASSERT_EQUAL( interactions.totalAvailableSites(), 9 );

    // Query for the rate calculator.
    const RateCalculator & rc = interactions.rateCalculator();
    CPPUNIT_ASSERT( &rc != NULL );

}

// -------------------------------------------------------------------------- //
//
void Test_Interactions::testUpdateAndPick()
{
    // Setup a list of processes.
    std::vector<Process> processes;

    // Setup a vector of dummy processes.
    std::vector<std::vector<std::string> > process_elements1(1);
    process_elements1[0] = std::vector<std::string>(1, "A");

    std::vector<std::vector<std::string> > process_elements2(1);
    process_elements2[0] = std::vector<std::string>(1, "B");

    std::vector<std::vector<double> > process_coordinates(1, std::vector<double>(3, 0.0));

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["V"] = 2;

    const double rate = 1.0/13.7;
    Configuration c1(process_coordinates, process_elements1, possible_types);
    Configuration c2(process_coordinates, process_elements2, possible_types);
    std::vector<int> sites_vector(1,0);
    processes.push_back(Process(c1,c2,rate,sites_vector));
    processes.push_back(Process(c1,c2,rate,sites_vector));
    processes.push_back(Process(c1,c2,rate/2.0,sites_vector));
    processes.push_back(Process(c1,c2,rate,sites_vector));
    processes.push_back(Process(c1,c2,rate,sites_vector));
    processes.push_back(Process(c1,c2,rate,sites_vector));

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
    Interactions interactions(processes, true);

    // Update the probability table.
    interactions.updateProbabilityTable();

    // Check the values of the probability table.
    const std::vector<std::pair<double, int> > & probability_table = \
        interactions.probabilityTable();

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(probability_table.size()),
                          static_cast<int>(processes.size()) );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[0].first,  3.0 * rate, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[1].first,  5.0 * rate, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[2].first,  7.0 * rate, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[3].first,  7.0 * rate, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[4].first,  7.0 * rate, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[5].first,  8.0 * rate, 1.0e-14 );

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
    // but with the double rate for the third process should halve
    // this entry.
    std::vector<int> picked(6,0);
    const int n_loop = 1000000;
    for (int i = 0; i < n_loop; ++i)
    {
        const int p = interactions.pickProcessIndex();

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

    // Check that picking the process twice with two different access methods and
    // a seed reset inbetween gives a reference to the same object.
    seedRandom(false, 87);
    const int p = interactions.pickProcessIndex();
    const Process & proc1 = (*interactions.processes()[p]);

    seedRandom(false, 87);
    const Process & proc2 = (*interactions.pickProcess());
    CPPUNIT_ASSERT_EQUAL( &proc1, &proc2 );

}


// -------------------------------------------------------------------------- //
//
void Test_Interactions::testUpdateAndPickCustom()
{
    // Setup a list of custom rate processes.
    std::vector<CustomRateProcess> processes;

    // Setup a vector of dummy processes.
    std::vector<std::vector<std::string> > process_elements1(1);
    process_elements1[0] = std::vector<std::string>(1, "A");

    std::vector<std::vector<std::string> > process_elements2(1);
    process_elements2[0] = std::vector<std::string>(1, "B");

    std::vector<std::vector<double> > process_coordinates(1, std::vector<double>(3, 0.0));

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["V"] = 2;

    const double rate = 1.0/13.7;
    Configuration c1(process_coordinates, process_elements1, possible_types);
    Configuration c2(process_coordinates, process_elements2, possible_types);
    std::vector<int> sites_vector(1,0);
    processes.push_back(CustomRateProcess(c1,c2,rate,sites_vector, 1.0));
    processes.push_back(CustomRateProcess(c1,c2,rate,sites_vector, 1.0));
    processes.push_back(CustomRateProcess(c1,c2,rate/2.0,sites_vector, 1.0));
    processes.push_back(CustomRateProcess(c1,c2,rate,sites_vector, 1.0));
    processes.push_back(CustomRateProcess(c1,c2,rate,sites_vector, 1.0));
    processes.push_back(CustomRateProcess(c1,c2,rate,sites_vector, 1.0));

    // Fake a matching by adding sites to the processes.

    // First process, 3 sites, total rate 12
    processes[0].addSite(12,  4.0, 1.0);
    processes[0].addSite(123, 7.0, 1.0);
    processes[0].addSite(332, 1.0, 1.0);

    // Second process, 2 sites, total rate 4
    processes[1].addSite(19, 1.0, 1.0);
    processes[1].addSite(12, 3.0, 1.0);

    // Third process, 4 sites, total rate  3
    processes[2].addSite(19,  1.0/4.0, 1.0);
    processes[2].addSite(12,  5.0/4.0, 1.0);
    processes[2].addSite(234, 2.0/4.0, 1.0);
    processes[2].addSite(991, 4.0/4.0, 1.0);

    // The sixth process, one site, total rate 12.
    processes[5].addSite(992, 12.0, 1.0);

    // Setup the interactions object.
    RateCalculator rc;
    Interactions interactions(processes, true, rc);

    // Update the probability table.
    interactions.updateProbabilityTable();

    // Check the values of the probability table.
    const std::vector<std::pair<double, int> > & probability_table = \
        interactions.probabilityTable();

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(probability_table.size()),
                          static_cast<int>(processes.size()) );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[0].first,  12.0, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[1].first,  16.0, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[2].first,  19.0, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[3].first,  19.0, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[4].first,  19.0, 1.0e-14 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( probability_table[5].first,  31.0, 1.0e-14 );

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
    // but with the double rate for the third process should halve
    // this entry.
    std::vector<int> picked(6,0);
    const int n_loop = 1000000;
    for (int i = 0; i < n_loop; ++i)
    {
        const int p = interactions.pickProcessIndex();

        // Make sure the picked process is not negative or too large.
        CPPUNIT_ASSERT( p >= 0 );
        CPPUNIT_ASSERT( p < static_cast<int>(probability_table.size()) );
        ++picked[p];
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked[0]/n_loop,
                                  12.0/31.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked[1]/n_loop,
                                  4.0/31.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked[2]/n_loop,
                                  3.0/31.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked[3]/n_loop,
                                  0.0/31.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked[4]/n_loop,
                                  0.0/31.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked[5]/n_loop,
                                  12.0/31.0,
                                  1.0e-2 );

    // Check that picking the process twice with two different access methods and
    // a seed reset inbetween gives a reference to the same object.
    seedRandom(false, 87);
    const int p = interactions.pickProcessIndex();
    const Process & proc1 = (*interactions.processes()[p]);

    seedRandom(false, 87);
    const Process & proc2 = (*interactions.pickProcess());
    CPPUNIT_ASSERT_EQUAL( &proc1, &proc2 );

    // Alter the total rate in one of the processes and re-run the picking.
    interactions.processes()[5]->removeSite(992);
    interactions.processes()[5]->addSite(992, 24.0, 1.0);

    // Update the probability table.
    interactions.updateProbabilityTable();
    std::vector<int> picked2(6,0);
    for (int i = 0; i < n_loop; ++i)
    {
        const int p = interactions.pickProcessIndex();

        // Make sure the picked process is not negative or too large.
        CPPUNIT_ASSERT( p >= 0 );
        CPPUNIT_ASSERT( p < static_cast<int>(probability_table.size()) );
        ++picked2[p];
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked2[0]/n_loop,
                                  12.0/43.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked2[1]/n_loop,
                                  4.0/43.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked2[2]/n_loop,
                                  3.0/43.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked2[3]/n_loop,
                                  0.0/43.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked2[4]/n_loop,
                                  0.0/43.0,
                                  1.0e-2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0*picked2[5]/n_loop,
                                  24.0/43.0,
                                  1.0e-2 );

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_Interactions::testMaxRange()
{
    // Setup two valid processes.
    std::vector<Process> processes;

    std::vector<std::vector<std::string> > process_elements1(3);
    process_elements1[0] = std::vector<std::string>(1, "A");
    process_elements1[1] = std::vector<std::string>(1, "A");
    process_elements1[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<std::string> > process_elements2(3);
    process_elements2[0] = std::vector<std::string>(1, "B");
    process_elements2[1] = std::vector<std::string>(1, "A");
    process_elements2[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<double> > process_coordinates(3, std::vector<double>(3, 0.0));
    process_coordinates[1][0] = -0.1;
    process_coordinates[1][1] = -0.1;
    process_coordinates[1][2] = -0.1;

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["V"] = 2;

    const double rate = 13.7;
    const Configuration c1(process_coordinates, process_elements1, possible_types);
    const Configuration c2(process_coordinates, process_elements2, possible_types);

    std::vector<int> sites_vector(0);
    processes.push_back(Process(c1,c2,rate,sites_vector));
    processes.push_back(Process(c1,c2,rate,sites_vector));

    // Setup the interactions object.
    const Interactions interactions(processes, true);

    // This should have the max range 1.
    CPPUNIT_ASSERT_EQUAL( interactions.maxRange(), 1 );

    // Adding a process with a -1.1 site should make the max range become 2.
    process_coordinates[1][0] =  0.0;
    process_coordinates[1][1] =  0.0;
    process_coordinates[1][2] = -1.1;

    const Configuration c3(process_coordinates, process_elements1, possible_types);
    const Configuration c4(process_coordinates, process_elements2, possible_types);
    processes.push_back(Process(c3,c4,rate,sites_vector));

    // This should have the max range 2.
    const Interactions interactions2(processes, true);
    CPPUNIT_ASSERT_EQUAL( interactions2.maxRange(), 2 );

    // Check the same thing in y.
    process_coordinates[1][0] =  0.0;
    process_coordinates[1][1] = -2.1;
    process_coordinates[1][2] =  0.0;

    const Configuration c5(process_coordinates, process_elements1, possible_types);
    const Configuration c6(process_coordinates, process_elements2, possible_types);
    processes[2] = Process(c5,c6,rate,sites_vector);

    // This should have the max range 3.
    const Interactions interactions3(processes, true);
    CPPUNIT_ASSERT_EQUAL( interactions3.maxRange(), 3 );

    // And in x.
    process_coordinates[1][0] = -3.1;
    process_coordinates[1][1] =  0.0;
    process_coordinates[1][2] =  0.0;

    const Configuration c7(process_coordinates, process_elements1, possible_types);
    const Configuration c8(process_coordinates, process_elements2, possible_types);
    processes[2] = Process(c7,c8,rate,sites_vector);

    // This should have the max range 4.
    const Interactions interactions4(processes, true);
    CPPUNIT_ASSERT_EQUAL( interactions4.maxRange(), 4 );

    // And in the positive direction.

    // Adding a process with a 5.1 site should make the max range become 5.
    process_coordinates[1][0] =  0.0;
    process_coordinates[1][1] =  0.0;
    process_coordinates[1][2] =  5.1;

    const Configuration c9(process_coordinates, process_elements1, possible_types);
    const Configuration c10(process_coordinates, process_elements2, possible_types);
    processes[2] = Process(c9,c10,rate,sites_vector);

    // This should have the max range 5.
    const Interactions interactions5(processes, true);
    CPPUNIT_ASSERT_EQUAL( interactions5.maxRange(), 5 );


    // Check the same thing in y.
    process_coordinates[1][0] =  0.0;
    process_coordinates[1][1] =  2.1;
    process_coordinates[1][2] =  0.0;

    const Configuration c11(process_coordinates, process_elements1, possible_types);
    const Configuration c12(process_coordinates, process_elements2, possible_types);
    processes[2] = Process(c11,c12,rate,sites_vector);

    // This should have the max range 2.
    const Interactions interactions6(processes, true);
    CPPUNIT_ASSERT_EQUAL( interactions6.maxRange(), 2 );

    // And in x.
    process_coordinates[1][0] =  4.1;
    process_coordinates[1][1] =  0.0;
    process_coordinates[1][2] =  0.0;

    const Configuration c13(process_coordinates, process_elements1, possible_types);
    const Configuration c14(process_coordinates, process_elements2, possible_types);
    processes[2] = Process(c13,c14,rate,sites_vector);

    // This should have the max range 4.
    const Interactions interactions7(processes, true);
    CPPUNIT_ASSERT_EQUAL( interactions7.maxRange(), 4 );

}


// -------------------------------------------------------------------------- //
//
void Test_Interactions::testUpdateProcessMatchLists()
{
    // Setup two valid processes.
    std::vector<Process> processes;

    std::vector<std::vector<std::string> > process_elements1(3);
    process_elements1[0] = std::vector<std::string>(1, "A");
    process_elements1[1] = std::vector<std::string>(1, "B");
    process_elements1[2] = std::vector<std::string>(1, "V");

    std::vector<std::vector<std::string> > process_elements2(3);
    process_elements2[0] = std::vector<std::string>(1, "B");
    process_elements2[1] = std::vector<std::string>(1, "A");
    process_elements2[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<double> > process_coordinates1(3, std::vector<double>(3, 0.0));
    process_coordinates1[1][0] = -1.0;
    process_coordinates1[1][1] =  0.0;
    process_coordinates1[1][2] =  0.0;

    process_coordinates1[2][0] =  0.3;
    process_coordinates1[2][1] =  0.3;
    process_coordinates1[2][2] =  0.3;

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    const double rate = 13.7;
    const Configuration c1(process_coordinates1, process_elements1, possible_types);
    const Configuration c2(process_coordinates1, process_elements2, possible_types);

    // Let this process be valid at site 0.
    processes.push_back(Process(c1,c2,rate,std::vector<int>(1,0)));

    // Let this process be valid at sites 0 and 2.
    std::vector<int> sites_vector(2,0);
    sites_vector[1] = 2;
    processes.push_back(Process(c1,c2,rate,sites_vector));

    std::vector<std::vector<double> > process_coordinates2(3, std::vector<double>(3, 0.0));

    process_coordinates2[1][0] =  0.7;
    process_coordinates2[1][1] =  0.7;
    process_coordinates2[1][2] = -0.3;

    process_coordinates2[2][0] =  1.0;
    process_coordinates2[2][1] =  1.0;
    process_coordinates2[2][2] =  1.0;

    const Configuration c3(process_coordinates2, process_elements1, possible_types);
    const Configuration c4(process_coordinates2, process_elements2, possible_types);

    // Let the process be valid at site 1.
    processes.push_back(Process(c3,c4,rate,std::vector<int>(1,1)));

    Interactions interactions(processes, true);

    // Generate a corresponding configuration.
    std::vector<std::vector<double> > config_coordinates;
    std::vector<std::vector<std::string> > elements;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            for (int k = 0; k < 5; ++k)
            {
                std::vector<double> coord(3);
                coord[0] = 0.0 + i*1.0;
                coord[1] = 0.0 + j*1.0;
                coord[2] = 0.0 + k*1.0;
                config_coordinates.push_back(coord);
                elements.push_back(std::vector<std::string>(1, "V"));

                coord[0] = 0.3 + i*1.0;
                coord[1] = 0.3 + j*1.0;
                coord[2] = 0.3 + k*1.0;
                elements.push_back(std::vector<std::string>(1, "B"));
                config_coordinates.push_back(coord);
            }
        }
    }

    // Get the config and lattice map.
    Configuration config(config_coordinates, elements, possible_types);

    // Make sure we do this on a non-periodic lattice map, to make sure
    // the most central cell is choosen for determining wildcard positions.
    const LatticeMap lattice_map(2, std::vector<int>(3,5), std::vector<bool>(3,false));

    // Now, setup the matchlists in the configuration.
    config.initMatchLists(lattice_map, interactions.maxRange());

    // Check the process match lists before we start.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[0]->processMatchList().size()), 3);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[1]->processMatchList().size()), 3);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[2]->processMatchList().size()), 3);

    // Update the interactions according to the configuration match lists.
    interactions.updateProcessMatchLists(config, lattice_map);

    // Check a few coordinates and match types.
    {
        const ProcessBucketMatchList & match = interactions.processes()[0]->processMatchList();

        CPPUNIT_ASSERT_EQUAL( static_cast<int>(match.size()), 6 );

        // Wildcards are now added.
        CPPUNIT_ASSERT_EQUAL( match[0].match_types[0],  0 );
        CPPUNIT_ASSERT_EQUAL( match[0].match_types[1],  1 );
        CPPUNIT_ASSERT_EQUAL( match[0].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[0].match_types[3],  0 );

        CPPUNIT_ASSERT_EQUAL( match[1].match_types[0],  0 );
        CPPUNIT_ASSERT_EQUAL( match[1].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[1].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[1].match_types[3],  1 );

        CPPUNIT_ASSERT_EQUAL( match[2].match_types[0],  1 );
        CPPUNIT_ASSERT_EQUAL( match[2].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[2].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[2].match_types[3],  0 );

        CPPUNIT_ASSERT_EQUAL( match[3].match_types[0],  1 );
        CPPUNIT_ASSERT_EQUAL( match[3].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[3].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[3].match_types[3],  0 );

        CPPUNIT_ASSERT_EQUAL( match[4].match_types[0],  1 );
        CPPUNIT_ASSERT_EQUAL( match[4].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[4].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[4].match_types[3],  0 );

        CPPUNIT_ASSERT_EQUAL( match[5].match_types[0],  0 );
        CPPUNIT_ASSERT_EQUAL( match[5].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[5].match_types[2],  1 );
        CPPUNIT_ASSERT_EQUAL( match[5].match_types[3],  0 );

        // These are not changed - but sorted.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[0].coordinate.x(),  process_coordinates1[0][0], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[0].coordinate.y(),  process_coordinates1[0][1], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[0].coordinate.z(),  process_coordinates1[0][2], 1.0e-10 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[1].coordinate.x(),  process_coordinates1[2][0], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[1].coordinate.y(),  process_coordinates1[2][1], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[1].coordinate.z(),  process_coordinates1[2][2], 1.0e-10 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[5].coordinate.x(),  process_coordinates1[1][0], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[5].coordinate.y(),  process_coordinates1[1][1], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[5].coordinate.z(),  process_coordinates1[1][2], 1.0e-10 );

        // Here are the added wildcards.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[2].coordinate.x(), -0.7, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[2].coordinate.y(),  0.3, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[2].coordinate.z(),  0.3, 1.0e-10 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[3].coordinate.x(),  0.3, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[3].coordinate.y(), -0.7, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[3].coordinate.z(),  0.3, 1.0e-10 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[4].coordinate.x(),  0.3, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[4].coordinate.y(),  0.3, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[4].coordinate.z(), -0.7, 1.0e-10 );

    }


    {
        // This one is not changed, since it was applicable to more than one basis site.
        const ProcessBucketMatchList & match = interactions.processes()[1]->processMatchList();
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(match.size()), 3 );
        // Not changed - but sorted.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[0].coordinate.x(),  process_coordinates1[0][0], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[0].coordinate.y(),  process_coordinates1[0][1], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[0].coordinate.z(),  process_coordinates1[0][2], 1.0e-10 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[1].coordinate.x(),  process_coordinates1[2][0], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[1].coordinate.y(),  process_coordinates1[2][1], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[1].coordinate.z(),  process_coordinates1[2][2], 1.0e-10 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[2].coordinate.x(),  process_coordinates1[1][0], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[2].coordinate.y(),  process_coordinates1[1][1], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[2].coordinate.z(),  process_coordinates1[1][2], 1.0e-10 );

    }


    {
        const ProcessBucketMatchList & match = interactions.processes()[2]->processMatchList();
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(match.size()), 47 );

        // Not changed.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[0].coordinate.x(),  process_coordinates2[0][0], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[0].coordinate.y(),  process_coordinates2[0][1], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[0].coordinate.z(),  process_coordinates2[0][2], 1.0e-10 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[13].coordinate.x(),  process_coordinates2[1][0], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[13].coordinate.y(),  process_coordinates2[1][1], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[13].coordinate.z(),  process_coordinates2[1][2], 1.0e-10 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[46].coordinate.x(),  process_coordinates2[2][0], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[46].coordinate.y(),  process_coordinates2[2][1], 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[46].coordinate.z(),  process_coordinates2[2][2], 1.0e-10 );


        // These are the added wildcards.
        for (int i = 0; i < 47; ++i)
        {
            if (i != 0 && i != 13 && i != 46)
            {
                CPPUNIT_ASSERT_EQUAL( match[i].match_types[0], 1 );
            }
        }

        // Check one coordinate.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[17].coordinate.x(),-0.3, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[17].coordinate.y(),-0.3, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[17].coordinate.z(),-1.3, 1.0e-10 );

        // Check another one.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[18].coordinate.x(),-1.0, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[18].coordinate.y(),-1.0, 1.0e-10 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( match[18].coordinate.z(), 0.0, 1.0e-10 );

    }
}


// -------------------------------------------------------------------------- //
//
void Test_Interactions::testUpdateProcessIDMoves()
{
    // Setup two valid processes.
    std::vector<Process> processes;

    std::vector<std::vector<std::string> > process_elements1(3);
    process_elements1[0] = std::vector<std::string>(1, "A");
    process_elements1[1] = std::vector<std::string>(1, "B");
    process_elements1[2] = std::vector<std::string>(1, "V");

    std::vector<std::vector<std::string> > process_elements2(3);
    process_elements2[0] = std::vector<std::string>(1, "B");
    process_elements2[1] = std::vector<std::string>(1, "A");
    process_elements2[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<double> > process_coordinates1(3, std::vector<double>(3, 0.0));
    process_coordinates1[1][0] = -1.0;
    process_coordinates1[1][1] =  0.0;
    process_coordinates1[1][2] =  0.0;

    process_coordinates1[2][0] =  0.3;
    process_coordinates1[2][1] =  0.3;
    process_coordinates1[2][2] =  0.3;

    std::vector<int> move_origins;
    move_origins.push_back(0);
    move_origins.push_back(1);

    std::vector<Coordinate> move_vectors;
    move_vectors.push_back( Coordinate(-1.0, 0.0, 0.0) );
    move_vectors.push_back( Coordinate( 1.0, 0.0, 0.0) );

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    const double rate = 13.7;
    const Configuration c1(process_coordinates1, process_elements1, possible_types);
    const Configuration c2(process_coordinates1, process_elements2, possible_types);

    // Let this process be valid at site 0.
    processes.push_back(Process(c1,c2,rate,std::vector<int>(1,0), move_origins, move_vectors));

    // Let this process be valid at sites 0 and 2.
    std::vector<int> sites_vector(2,0);
    sites_vector[1] = 2;
    processes.push_back(Process(c1,c2,rate,sites_vector, move_origins, move_vectors));

    std::vector<std::vector<double> > process_coordinates2(3, std::vector<double>(3, 0.0));

    process_coordinates2[1][0] =  0.7;
    process_coordinates2[1][1] =  0.7;
    process_coordinates2[1][2] = -0.3;

    process_coordinates2[2][0] =  1.0;
    process_coordinates2[2][1] =  1.0;
    process_coordinates2[2][2] =  1.0;

    move_vectors[0] = Coordinate( 0.7, 0.7, -0.3);
    move_vectors[1] = Coordinate(-0.7,-0.7,  0.3);


    const Configuration c3(process_coordinates2, process_elements1, possible_types);
    const Configuration c4(process_coordinates2, process_elements2, possible_types);

    // Let the process be valid at site 1.
    processes.push_back(Process(c3,c4,rate,std::vector<int>(1,1), move_origins, move_vectors));

    Interactions interactions(processes, true);

    // Generate a corresponding configuration.
    std::vector<std::vector<double> > config_coordinates;
    std::vector<std::vector<std::string> > elements;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            for (int k = 0; k < 5; ++k)
            {
                std::vector<double> coord(3);
                coord[0] = 0.0 + i*1.0;
                coord[1] = 0.0 + j*1.0;
                coord[2] = 0.0 + k*1.0;
                config_coordinates.push_back(coord);
                elements.push_back(std::vector<std::string>(1, "V"));

                coord[0] = 0.3 + i*1.0;
                coord[1] = 0.3 + j*1.0;
                coord[2] = 0.3 + k*1.0;
                elements.push_back(std::vector<std::string>(1, "B"));
                config_coordinates.push_back(coord);
            }
        }
    }

    // Get the config and lattice map.
    Configuration config(config_coordinates, elements, possible_types);
    const LatticeMap lattice_map(2, std::vector<int>(3,5), std::vector<bool>(3,true));

    // Now, setup the matchlists in the configuration.
    config.initMatchLists(lattice_map, interactions.maxRange());

    // Check the process match lists before we start.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[0]->processMatchList().size()), 3);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[1]->processMatchList().size()), 3);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[2]->processMatchList().size()), 3);

    // Check the id moves before update.
    {
        const std::vector< std::pair<int, int> > & id_moves = interactions.processes()[0]->idMoves();
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(id_moves.size()), 2 );
        CPPUNIT_ASSERT_EQUAL( id_moves[0].first,  0 );
        CPPUNIT_ASSERT_EQUAL( id_moves[0].second, 2 );
        CPPUNIT_ASSERT_EQUAL( id_moves[1].first,  2 );
        CPPUNIT_ASSERT_EQUAL( id_moves[1].second, 0 );
    }

    // Update the interactions according to the configuration match lists.
    // This also updates the id moves on the processes.
    interactions.updateProcessMatchLists(config, lattice_map);

    // Check the id moves after update.
    {
        const ProcessBucketMatchList & match = interactions.processes()[0]->processMatchList();
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(match.size()), 6 );

        const std::vector< std::pair<int, int> > & id_moves = interactions.processes()[0]->idMoves();
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(id_moves.size()), 2 );
        CPPUNIT_ASSERT_EQUAL( id_moves[0].first,  0 );
        CPPUNIT_ASSERT_EQUAL( id_moves[0].second, 5 );
        CPPUNIT_ASSERT_EQUAL( id_moves[1].first,  5 );
        CPPUNIT_ASSERT_EQUAL( id_moves[1].second, 0 );

        // Wildcards are now added.


        CPPUNIT_ASSERT_EQUAL( match[0].match_types[0],  0 );
        CPPUNIT_ASSERT_EQUAL( match[0].match_types[1],  1 );
        CPPUNIT_ASSERT_EQUAL( match[0].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[0].match_types[3],  0 );

        CPPUNIT_ASSERT_EQUAL( match[1].match_types[0],  0 );
        CPPUNIT_ASSERT_EQUAL( match[1].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[1].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[1].match_types[3],  1 );

        CPPUNIT_ASSERT_EQUAL( match[2].match_types[0],  1 );
        CPPUNIT_ASSERT_EQUAL( match[2].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[2].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[2].match_types[3],  0 );

        CPPUNIT_ASSERT_EQUAL( match[3].match_types[0],  1 );
        CPPUNIT_ASSERT_EQUAL( match[3].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[3].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[3].match_types[3],  0 );

        CPPUNIT_ASSERT_EQUAL( match[4].match_types[0],  1 );
        CPPUNIT_ASSERT_EQUAL( match[4].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[4].match_types[2],  0 );
        CPPUNIT_ASSERT_EQUAL( match[4].match_types[3],  0 );

        CPPUNIT_ASSERT_EQUAL( match[5].match_types[0],  0 );
        CPPUNIT_ASSERT_EQUAL( match[5].match_types[1],  0 );
        CPPUNIT_ASSERT_EQUAL( match[5].match_types[2],  1 );
        CPPUNIT_ASSERT_EQUAL( match[5].match_types[3],  0 );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Interactions::testClearMatching()
{
    // Setup two valid processes.
    std::vector<Process> processes;

    std::vector<std::vector<std::string> > process_elements1(3);
    process_elements1[0] = std::vector<std::string>(1, "A");
    process_elements1[1] = std::vector<std::string>(1, "B");
    process_elements1[2] = std::vector<std::string>(1, "V");

    std::vector<std::vector<std::string> > process_elements2(3);
    process_elements2[0] = std::vector<std::string>(1, "B");
    process_elements2[1] = std::vector<std::string>(1, "A");
    process_elements2[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<double> > process_coordinates1(3, std::vector<double>(3, 0.0));
    process_coordinates1[1][0] = -1.0;
    process_coordinates1[1][1] =  0.0;
    process_coordinates1[1][2] =  0.0;

    process_coordinates1[2][0] =  0.3;
    process_coordinates1[2][1] =  0.3;
    process_coordinates1[2][2] =  0.3;

    std::vector<int> move_origins;
    move_origins.push_back(0);
    move_origins.push_back(1);

    std::vector<Coordinate> move_vectors;
    move_vectors.push_back( Coordinate(-1.0, 0.0, 0.0) );
    move_vectors.push_back( Coordinate( 1.0, 0.0, 0.0) );

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    const double rate = 13.7;
    const Configuration c1(process_coordinates1, process_elements1, possible_types);
    const Configuration c2(process_coordinates1, process_elements2, possible_types);

    // Let this process be valid at site 0.
    processes.push_back(Process(c1,c2,rate,std::vector<int>(1,0), move_origins, move_vectors));

    // Let this process be valid at sites 0 and 2.
    std::vector<int> sites_vector(2,0);
    sites_vector[1] = 2;
    processes.push_back(Process(c1, c2, rate, sites_vector, move_origins, move_vectors));

    std::vector<std::vector<double> > process_coordinates2(3, std::vector<double>(3, 0.0));

    process_coordinates2[1][0] =  0.7;
    process_coordinates2[1][1] =  0.7;
    process_coordinates2[1][2] = -0.3;

    process_coordinates2[2][0] =  1.0;
    process_coordinates2[2][1] =  1.0;
    process_coordinates2[2][2] =  1.0;

    move_vectors[0] = Coordinate( 0.7, 0.7, -0.3);
    move_vectors[1] = Coordinate(-0.7,-0.7,  0.3);

    const Configuration c3(process_coordinates2, process_elements1, possible_types);
    const Configuration c4(process_coordinates2, process_elements2, possible_types);

    // Let the process be valid at site 1.
    processes.push_back(Process(c3,c4,rate,std::vector<int>(1,1), move_origins, move_vectors));

    processes[0].addSite(32);
    processes[1].addSite(432);
    processes[1].addSite(443);
    processes[1].addSite(431);
    processes[2].addSite(112);
    processes[2].addSite(124);

    Interactions interactions(processes, true);

    // Check that the processes have the expected number of sites listed.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[0]->nSites()),
                         1);

    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[1]->nSites()),
                         3);

    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[2]->nSites()),
                         2);

    // Call the function to test.
    interactions.clearMatching();


    // These should now be zero.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[0]->nSites()),
                         0);

    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[1]->nSites()),
                         0);

    CPPUNIT_ASSERT_EQUAL(static_cast<int>(interactions.processes()[2]->nSites()),
                         0);

}

