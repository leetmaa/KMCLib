/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_latticemodel.h"

// Include the files to test.
#include "../src/latticemodel.h"

// Other inclusions.
#include "../src/configuration.h"
#include "../src/latticemap.h"
#include "../src/interactions.h"

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

    std::vector<std::string> elements(2);
    elements[0] = "A";
    elements[1] = "V";

    std::map<std::string, int> possible_types;
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
    Interactions interactions(processes);

    // construct.
    LatticeModel model(config, lattice_map, interactions);

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
    std::vector<std::string> elements;

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
                    elements.push_back(basis_elements[b]);
                }
            }
        }
    }

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["V"] = 2;

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

    // A process that finds a V between two A in the 1,1,1 direction
    // and turn the V into B.
    {
        std::vector<std::string> process_elements1(3);
        process_elements1[0] = "V";
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

    Interactions interactions(processes);

    // Construct the lattice model to test.
    LatticeModel lattice_model(configuration, lattice_map, interactions);

    // Get the interactions out and check that they are setup as expected.
    const Interactions ret_interactions = lattice_model.interactions();
    const std::vector<Process> ret_processes = ret_interactions.processes();

    // Check the number of processes.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_processes.size()), 4);

    // Check the number of listed sites at each process.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions.processes()[0].sites().size()), nI*nJ*nK*nB*2/3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions.processes()[1].sites().size()), nI*nJ*nK*nB*2/3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions.processes()[2].sites().size()), nI*nJ*nK*nB/3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions.processes()[3].sites().size()), 0 );

    // Check the first process.
    const Process & p0 = ret_processes[0];

    // Check that all "B" sites are listed,
    // and that all "V" and "A" sites are not.
    for (int i = 0; i < nI*nJ*nK*nB; i += nB)
    {
        CPPUNIT_ASSERT(!p0.isListed(i)   );
        CPPUNIT_ASSERT( p0.isListed(i+1) );
        CPPUNIT_ASSERT( p0.isListed(i+2) );
    }

    // Check the second process.
    const Process & p1 = ret_processes[1];

    // This process is identical to the first.
    for (int i = 0; i < nI*nJ*nK*nB; i += nB)
    {
        CPPUNIT_ASSERT(!p1.isListed(i)   );
        CPPUNIT_ASSERT( p1.isListed(i+1) );
        CPPUNIT_ASSERT( p1.isListed(i+2) );
    }

    // Check the third process has all "A" sites listed.
    const Process & p2 = ret_processes[2];

    // This process is identical to the first.
    for (int i = 0; i < nI*nJ*nK*nB; i += nB)
    {
        CPPUNIT_ASSERT( p2.isListed(i)   );
        CPPUNIT_ASSERT(!p2.isListed(i+1) );
        CPPUNIT_ASSERT(!p2.isListed(i+2) );
    }

    // Check the last process has no sites listed.
    const Process & p3 = ret_processes[3];
    CPPUNIT_ASSERT( p3.sites().empty() );

    // Introduce a few different typed sites.
    elements[0]    = "V";
    elements[216]  = "V";   // These affects process 0,1 and 3
    elements[1434] = "V";
    elements[2101] = "V";   // This affects process 0,1 and 2

    // Get a new configuration.
    configuration = Configuration(coordinates, elements, possible_types);

    // Get a new lattice model to test.
    LatticeModel lattice_model_2(configuration, lattice_map, interactions);

    // Get the interactions out and check.
    const Interactions ret_interactions_2 = lattice_model_2.interactions();

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions_2.processes()[0].sites().size()), (nI*nJ*nK*nB*2/3)-1);
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions_2.processes()[1].sites().size()), (nI*nJ*nK*nB*2/3)-1);
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions_2.processes()[2].sites().size()), (nI*nJ*nK*nB/3)-4);
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ret_interactions_2.processes()[3].sites().size()), 3 );


}
