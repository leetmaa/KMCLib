/*
  Copyright (c)  2012-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_latticemodel.h"

// Include the files to test.
#include "latticemodel.h"

// Other inclusions.
#include "configuration.h"
#include "latticemap.h"
#include "interactions.h"
#include "random.h"
#include "simulationtimer.h"

#include <ctime>

// -------------------------------------------------------------------------- //
//
void Test_LatticeModel::testConstruction()
{
    // Construct a configuration.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[0][0] = 1.4;
    coords[0][1] = 2.5;
    coords[0][2] = 4.6;
    coords[1][0] = 5.7;
    coords[1][1] = 3.5;
    coords[1][2] = 2.1;

    std::vector<std::vector<std::string> > elements(2);
    elements[0] = std::vector<std::string>(1, "A");
    elements[1] = std::vector<std::string>(1, "V");

    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["V"] = 2;

    Configuration config(coords,elements,possible_types);

    // And a corresponding lattice map.
    std::vector<int> rep(3, 1);
    rep[0] = 2;
    const std::vector<bool> per(3, true);
    LatticeMap lattice_map(1, rep, per);

    // We also need the interactions.
    const std::vector<Process> processes;
    Interactions interactions(processes, true);

    // And a timer.
    SimulationTimer timer;

    // construct.
    LatticeModel model(config, timer, lattice_map, interactions);

}


// -------------------------------------------------------------------------- //
//
void Test_LatticeModel::testSetupAndQuery()
{
    // Setup a realistic system and check.
    std::vector< std::vector<double> > basis(3, std::vector<double>(3,0.0));
    basis[1][0] = 0.25;
    basis[1][1] = 0.25;
    basis[1][2] = 0.25;
    basis[2][0] = 0.75;
    basis[2][1] = 0.75;
    basis[2][2] = 0.75;

    std::vector<int> basis_sites;
    basis_sites.push_back(0);
    basis_sites.push_back(1);
    basis_sites.push_back(2);

    std::vector<std::string> basis_elements(3);
    basis_elements[0] = "A";
    basis_elements[1] = "B";
    basis_elements[2] = "B";

    // Make a 37x18x19 structure.
    const int nI = 37;
    const int nJ = 18;
    const int nK = 19;
    const int nB = 3;

    // Coordinates and elements.
    std::vector<std::vector<double> > coordinates;
    std::vector<std::vector<std::string> > elements;

    for (int i = 0; i < nI; ++i)
    {
        for (int j = 0; j < nJ; ++j)
        {
            for (int k = 0; k < nK; ++k)
            {
                for (int b = 0; b < nB; ++b)
                {
                    std::vector<double> c(3);
                    c[0] = i + basis[b][0];
                    c[1] = j + basis[b][1];
                    c[2] = k + basis[b][2];
                    coordinates.push_back(c);
                    elements.push_back(std::vector<std::string>(1, basis_elements[b]));
                }
            }
        }
    }

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    // Setup the configuration.
    Configuration configuration(coordinates, elements, possible_types);

    // Setup the lattice map.
    std::vector<int> repetitions(3);
    repetitions[0] = nI;
    repetitions[1] = nJ;
    repetitions[2] = nK;
    std::vector<bool> periodicity(3, true);
    LatticeMap lattice_map(nB, repetitions, periodicity);

    // Setup the interactions object.
    std::vector<Process> processes;

    // A process that independent of local environment swaps a "B" to "V"
    {
        const std::vector<std::vector<std::string> > process_elements1(1,std::vector<std::string>(1,"B"));
        const std::vector<std::vector<std::string> > process_elements2(1,std::vector<std::string>(1,"V"));
        const std::vector<std::vector<double> > process_coordinates(1, std::vector<double>(3, 0.0));
        const double rate = 1.234;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
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
        Process p(c1, c2, rate, basis_sites);
        processes.push_back(p);
    }

    // A process that finds a V between two A in the 1,1,1 direction
    // and turn the V into B.
    {
        std::vector<std::vector<std::string> > process_elements1(3);
        process_elements1[0] = std::vector<std::string>(1, "V");
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
        Process p(c1, c2, rate, basis_sites);
        processes.push_back(p);
    }

    Interactions interactions(processes, false);

    // We need a timer.
    SimulationTimer timer;

    // Construct the lattice model to test.
    LatticeModel lattice_model(configuration, timer, lattice_map, interactions);

    // Get the interactions out and check that they are setup as expected.
    const Interactions ret_interactions = lattice_model.interactions();
    const std::vector<Process*> ret_processes = ret_interactions.processes();

    // Check the number of processes.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_processes.size()), 4);

    // Check the number of listed sites at each process.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions.processes()[0]->sites().size()), nI*nJ*nK*nB*2/3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions.processes()[1]->sites().size()), nI*nJ*nK*nB*2/3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions.processes()[2]->sites().size()), nI*nJ*nK*nB/3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions.processes()[3]->sites().size()), 0 );

    // Check the first process.
    const Process & p0 = (*ret_processes[0]);

    // Check that all "B" sites are listed,
    // and that all "V" and "A" sites are not.
    for (int i = 0; i < nI*nJ*nK*nB; i += nB)
    {
        CPPUNIT_ASSERT(!p0.isListed(i)   );
        CPPUNIT_ASSERT( p0.isListed(i+1) );
        CPPUNIT_ASSERT( p0.isListed(i+2) );
    }

    // Check the second process.
    const Process & p1 = (*ret_processes[1]);

    // This process is identical to the first.
    for (int i = 0; i < nI*nJ*nK*nB; i += nB)
    {
        CPPUNIT_ASSERT(!p1.isListed(i)   );
        CPPUNIT_ASSERT( p1.isListed(i+1) );
        CPPUNIT_ASSERT( p1.isListed(i+2) );
    }

    // Check the third process has all "A" sites listed.
    const Process & p2 = (*ret_processes[2]);

    // This process is identical to the first.
    for (int i = 0; i < nI*nJ*nK*nB; i += nB)
    {
        CPPUNIT_ASSERT( p2.isListed(i)   );
        CPPUNIT_ASSERT(!p2.isListed(i+1) );
        CPPUNIT_ASSERT(!p2.isListed(i+2) );
    }

    // Check the last process has no sites listed.
    const Process & p3 = (*ret_processes[3]);
    CPPUNIT_ASSERT( p3.sites().empty() );

    // Introduce a few different typed sites.
    elements[0]    = std::vector<std::string>(1, "V");
    elements[216]  = std::vector<std::string>(1, "V");   // These affects process 0,1 and 3
    elements[1434] = std::vector<std::string>(1, "V");
    elements[2101] = std::vector<std::string>(1, "V");   // This affects process 0,1 and 2

    // Get a new configuration.
    configuration = Configuration(coordinates, elements, possible_types);

    // Get a new lattice model to test.
    const LatticeModel lattice_model_2(configuration, timer, lattice_map, interactions);

    // Get the interactions out and check.
    const Interactions ret_interactions_2 = lattice_model_2.interactions();

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions_2.processes()[0]->sites().size()), (nI*nJ*nK*nB*2/3)-1);
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions_2.processes()[1]->sites().size()), (nI*nJ*nK*nB*2/3)-1);
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions_2.processes()[2]->sites().size()), (nI*nJ*nK*nB/3)-4);
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions_2.processes()[3]->sites().size()), 3 );

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_LatticeModel::testSingleStepFunction()
{
    // Setup a realistic system and check.
    std::vector< std::vector<double> > basis(3, std::vector<double>(3,0.0));
    basis[1][0] = 0.25;
    basis[1][1] = 0.25;
    basis[1][2] = 0.25;
    basis[2][0] = 0.75;
    basis[2][1] = 0.75;
    basis[2][2] = 0.75;

    std::vector<int> basis_sites;
    basis_sites.push_back(0);
    basis_sites.push_back(1);
    basis_sites.push_back(2);

    std::vector<std::string> basis_elements(3);
    basis_elements[0] = "A";
    basis_elements[1] = "B";
    basis_elements[2] = "B";

    // Make a 37x18x19 structure.
    const int nI = 37;
    const int nJ = 18;
    const int nK = 19;
    const int nB = 3;

    // Coordinates and elements.
    std::vector<std::vector<double> > coordinates;
    std::vector<std::vector<std::string> > elements;

    for (int i = 0; i < nI; ++i)
    {
        for (int j = 0; j < nJ; ++j)
        {
            for (int k = 0; k < nK; ++k)
            {
                for (int b = 0; b < nB; ++b)
                {
                    std::vector<double> c(3);
                    c[0] = i + basis[b][0];
                    c[1] = j + basis[b][1];
                    c[2] = k + basis[b][2];
                    coordinates.push_back(c);
                    elements.push_back(std::vector<std::string>(1, basis_elements[b]));
                }
            }
        }
    }

    // Introduce a few different typed sites.
    elements[0]    = std::vector<std::string>(1, "V");
    elements[216]  = std::vector<std::string>(1, "V");   // These affects process 0,1 and 3
    elements[1434] = std::vector<std::string>(1, "V");
    elements[2101] = std::vector<std::string>(1, "V");   // This affects process 0,1 and 2

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    // Setup the configuration.
    Configuration configuration(coordinates, elements, possible_types);

    // Setup the lattice map.
    std::vector<int> repetitions(3);
    repetitions[0] = nI;
    repetitions[1] = nJ;
    repetitions[2] = nK;
    std::vector<bool> periodicity(3, true);
    LatticeMap lattice_map(nB, repetitions, periodicity);
    configuration.initMatchLists(lattice_map, 1);

    // Setup the interactions object.
    std::vector<Process> processes;

    // A process that independent of local environment swaps a "B" to "V"
    {
        const std::vector<std::vector<std::string> > process_elements1(1,std::vector<std::string>(1,"B"));
        const std::vector<std::vector<std::string> > process_elements2(1,std::vector<std::string>(1,"V"));
        const std::vector<std::vector<double> > process_coordinates(1, std::vector<double>(3, 0.0));
        const double rate = 1.234;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
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
        Process p(c1, c2, rate, basis_sites);
        processes.push_back(p);
    }

    // A process that finds a V between two A in the 1,1,1 direction
    // and turn the V into B.
    {
        std::vector<std::vector<std::string> > process_elements1(3);
        process_elements1[0] = std::vector<std::string>(1, "V");
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
        Process p(c1, c2, rate, basis_sites);
        processes.push_back(p);
    }

    Interactions interactions(processes, true);

    // Get a timer.
    SimulationTimer timer;

    // Construct the lattice model to test.
    LatticeModel lattice_model(configuration, timer, lattice_map, interactions);

    // Call the single step function a couple of times to make sure it is
    // stable - the rest of the testing of this function should be done on
    // a higher level.
    const int n_loop = 1000;
    for (int i = 0; i < n_loop; ++i)
    {
        lattice_model.singleStep();
    }
}

// -------------------------------------------------------------------------- //
//
void Test_LatticeModel::testTiming()
{
    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["V"] = 2;

    // Setup the interactions object.
    std::vector<Process> processes;

    // Get a basis sites list.
    std::vector<int> basis_sites;
    basis_sites.push_back(0);
    basis_sites.push_back(1);
    basis_sites.push_back(2);

    // This defines the moving directions.
    std::vector<std::vector<double> > process_coordinates(7, std::vector<double>(3, 0.0));
    process_coordinates[1][0] = -1.0;
    process_coordinates[2][0] =  1.0;
    process_coordinates[3][1] = -1.0;
    process_coordinates[4][1] =  1.0;
    process_coordinates[5][2] = -1.0;
    process_coordinates[6][2] =  1.0;

    // A process that moves a vacancy to the left with rate 10
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // down
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "V");
        process_elements2[2] = std::vector<std::string>(1, "A");
        process_elements2[3] = std::vector<std::string>(1, "A");
        process_elements2[4] = std::vector<std::string>(1, "A");
        process_elements2[5] = std::vector<std::string>(1, "A");
        process_elements2[6] = std::vector<std::string>(1, "A");

        const double rate = 10.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }


    // A process that moves a vacancy to the right with rate 10
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // down
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "V");
        process_elements2[3] = std::vector<std::string>(1, "A");
        process_elements2[4] = std::vector<std::string>(1, "A");
        process_elements2[5] = std::vector<std::string>(1, "A");
        process_elements2[6] = std::vector<std::string>(1, "A");

        const double rate = 10.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // A process that moves a vacancy to the front with rate 10
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // down
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "A");
        process_elements2[3] = std::vector<std::string>(1, "V");
        process_elements2[4] = std::vector<std::string>(1, "A");
        process_elements2[5] = std::vector<std::string>(1, "A");
        process_elements2[6] = std::vector<std::string>(1, "A");

        const double rate = 10.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // A process that moves a vacancy to the back with rate 10
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // donw
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "A");
        process_elements2[3] = std::vector<std::string>(1, "A");
        process_elements2[4] = std::vector<std::string>(1, "V");
        process_elements2[5] = std::vector<std::string>(1, "A");
        process_elements2[6] = std::vector<std::string>(1, "A");

        const double rate = 10.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // A process that moves a vacancy down with rate 10
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // down
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "A");
        process_elements2[3] = std::vector<std::string>(1, "A");
        process_elements2[4] = std::vector<std::string>(1, "A");
        process_elements2[5] = std::vector<std::string>(1, "V");
        process_elements2[6] = std::vector<std::string>(1, "A");

        const double rate = 10.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // A process that moves a vacancy up with rate 10
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // down
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "A");
        process_elements2[3] = std::vector<std::string>(1, "A");
        process_elements2[4] = std::vector<std::string>(1, "A");
        process_elements2[5] = std::vector<std::string>(1, "A");
        process_elements2[6] = std::vector<std::string>(1, "V");

        const double rate = 10.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // Processes that moves a vacancy away from other vacancies with rate 15.

    // Left.
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "V");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // down
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "V");
        process_elements2[2] = std::vector<std::string>(1, "V");
        process_elements2[3] = std::vector<std::string>(1, "A");
        process_elements2[4] = std::vector<std::string>(1, "A");
        process_elements2[5] = std::vector<std::string>(1, "A");
        process_elements2[6] = std::vector<std::string>(1, "A");

        const double rate = 15.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // Right.
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "V");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // down
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "V");
        process_elements2[2] = std::vector<std::string>(1, "V");
        process_elements2[3] = std::vector<std::string>(1, "A");
        process_elements2[4] = std::vector<std::string>(1, "A");
        process_elements2[5] = std::vector<std::string>(1, "A");
        process_elements2[6] = std::vector<std::string>(1, "A");

        const double rate = 15.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // Front.
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "V");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // down
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "A");
        process_elements2[3] = std::vector<std::string>(1, "V");
        process_elements2[4] = std::vector<std::string>(1, "V");
        process_elements2[5] = std::vector<std::string>(1, "A");
        process_elements2[6] = std::vector<std::string>(1, "A");

        const double rate = 15.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // Back.
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "V");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // donw
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "A");
        process_elements2[3] = std::vector<std::string>(1, "V");
        process_elements2[4] = std::vector<std::string>(1, "V");
        process_elements2[5] = std::vector<std::string>(1, "A");
        process_elements2[6] = std::vector<std::string>(1, "A");

        const double rate = 15.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // Down.
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "A");  // down
        process_elements1[6] = std::vector<std::string>(1, "V");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "A");
        process_elements2[3] = std::vector<std::string>(1, "A");
        process_elements2[4] = std::vector<std::string>(1, "A");
        process_elements2[5] = std::vector<std::string>(1, "V");
        process_elements2[6] = std::vector<std::string>(1, "V");

        const double rate = 15.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // Up.
    {
        std::vector<std::vector<std::string> > process_elements1(7);
        process_elements1[0] = std::vector<std::string>(1, "V");  // center
        process_elements1[1] = std::vector<std::string>(1, "A");  // left
        process_elements1[2] = std::vector<std::string>(1, "A");  // right
        process_elements1[3] = std::vector<std::string>(1, "A");  // front
        process_elements1[4] = std::vector<std::string>(1, "A");  // back
        process_elements1[5] = std::vector<std::string>(1, "V");  // down
        process_elements1[6] = std::vector<std::string>(1, "A");  // up

        std::vector<std::vector<std::string> > process_elements2(7);
        process_elements2[0] = std::vector<std::string>(1, "A");
        process_elements2[1] = std::vector<std::string>(1, "A");
        process_elements2[2] = std::vector<std::string>(1, "A");
        process_elements2[3] = std::vector<std::string>(1, "A");
        process_elements2[4] = std::vector<std::string>(1, "A");
        process_elements2[5] = std::vector<std::string>(1, "V");
        process_elements2[6] = std::vector<std::string>(1, "V");

        const double rate = 15.0;
        Configuration c1(process_coordinates, process_elements1, possible_types);
        Configuration c2(process_coordinates, process_elements2, possible_types);
        Process p(c1, c2, rate, basis_sites);
        // Store.
        processes.push_back(p);
    }

    // Setup a binary 3D system with voids and occupied.
    std::vector< std::vector<double> > basis(1, std::vector<double>(3,0.0));

    const int nI = 1000;
    const int nJ = 10;
    const int nK = 10;
    const int nB = 1;

    // Coordinates and elements.
    std::vector<std::vector<double> > coordinates;
    std::vector<std::vector<std::string> > elements;

    // Seed the random number generator to make the test reproducible.
    seedRandom(false, 14159265);

    for (int i = 0; i < nI; ++i)
    {
        for (int j = 0; j < nJ; ++j)
        {
            for (int k = 0; k < nK; ++k)
            {
                std::vector<double> c(3);
                c[0] = i;
                c[1] = j;
                c[2] = k;
                coordinates.push_back(c);

                if (randomDouble01() < 0.05)
                {
                    elements.push_back(std::vector<std::string>(1, "V"));
                }
                else
                {
                    elements.push_back(std::vector<std::string>(1, "A"));
                }
            }
        }
    }

    // Setup the configuration.
    Configuration configuration(coordinates, elements, possible_types);

    // Setup the lattice map.
    std::vector<int> repetitions(3);
    repetitions[0] = nI;
    repetitions[1] = nJ;
    repetitions[2] = nK;
    std::vector<bool> periodicity(3, true);
    LatticeMap lattice_map(nB, repetitions, periodicity);
    Interactions interactions(processes, false);
    SimulationTimer timer;

    // Construct the lattice model to test.
    LatticeModel lattice_model(configuration, timer, lattice_map, interactions);

    // Call the single step function a couple of times to make sure it is
    // stable - the rest of the testing of this function should be done on
    // a higher level.

    time_t seconds;
    time(&seconds);

    const int n_loop = 10000;
    for (int i = 0; i < n_loop; ++i)
    {
        lattice_model.singleStep();
    }

    time_t seconds2;
    time(&seconds2);

    int diff_t = static_cast<int>(seconds2-seconds);
    printf("\nTIMING: %i steps were performed in %i seconds (%f ms/step)\n",
           n_loop,
           diff_t,
           1000.0*static_cast<double>(diff_t)/n_loop);

    printf("        with %i processes (7 centers per process) for %i sites in the lattice.\n",
           static_cast<int>(processes.size()), nI*nJ*nK*nB);

}
