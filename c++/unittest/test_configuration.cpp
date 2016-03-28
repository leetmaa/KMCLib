/*
  Copyright (c)  2012-2016  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_configuration.h"

// Include the files to test.
#include "configuration.h"

#include "latticemap.h"
#include "process.h"

// -------------------------------------------------------------------------- //
//
void Test_Configuration::testConstruction()
{
    // Setup coordinates.
    std::vector<std::vector<double> > coords(5, std::vector<double>(3, 0.0));
    coords[0][0]  = 0.1;
    coords[0][1]  = 0.2;
    coords[0][2]  = 0.3;
    coords[1][0]  = 0.4;
    coords[1][1]  = 0.5;
    coords[1][2]  = 0.6;
    coords[2][0]  = 0.7;
    coords[2][1]  = 0.8;
    coords[2][2]  = 0.9;
    coords[3][0]  = 1.1;
    coords[3][1] = 1.2;
    coords[3][2] = 1.3;
    coords[4][0] = 3.6;
    coords[4][1] = 3.5;
    coords[4][2] = 3.4;

    // Setup elements.
    std::vector< std::vector<std::string> > elements(5);
    elements[0] = std::vector<std::string>(1, "A");
    elements[1] = std::vector<std::string>(1, "B");
    elements[2] = std::vector<std::string>(1, "D");
    elements[3] = std::vector<std::string>(1, "H");
    elements[4] = std::vector<std::string>(1, "J");

    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["D"] = 3;
    possible_types["H"] = 4;
    possible_types["J"] = 5;
    possible_types["G"] = 6;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);


    // Extract the member data and check that it is the same as what whent in.
    std::vector<Coordinate> const & ret_coords = config.coordinates();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(ret_coords.size()),
                         static_cast<int>(coords.size()));

    for (size_t i = 0; i < coords.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ret_coords[i][0], coords[i][0], 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ret_coords[i][1], coords[i][1], 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ret_coords[i][2], coords[i][2], 1.0e-12);
    }

    // Check the size of the elements.
    std::vector<std::vector<std::string> > const & ret_elements = config.elements();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(ret_coords.size()),
                         static_cast<int>(ret_elements.size()));

    for (size_t i = 0; i < elements.size(); ++i)
    {
        for (size_t j = 0; j < elements[i].size(); ++j)
        {
            CPPUNIT_ASSERT_EQUAL(ret_elements[i][j], elements[i][j]);
        }
    }

    // DONE

}


// ---------------------------------------------------------------------------//
// Description: Test moved_atom_id_ and recent_move_vector
//              lengths and memories allocation.
// Author     : pytlab <shaozhengjiang@gmail.com>
// Date       : 2016-03-28
// Reviewed and accepted as a contribution by Mikael Leetmaa 2016-03-28.
// ---------------------------------------------------------------------------//
void Test_Configuration::testMovedAtomIDsRecentMoveVectorsSize()
{
    // Make a 3x3x3 structure.
    const int nI = 3;
    const int nJ = 3;
    const int nK = 3;

    // Number of basis.
    const int nB = 1;

    // Coordinates and elements.
    std::vector<std::vector<double> > coordinates;
    std::vector<std::vector<std::string> > elements;

    for (int i = 0; i < nI; ++i)
    {
        for (int j = 0; j < nJ; ++j)
        {
            for (int k = 0; k < nK; ++k)
            {
                std::vector<double> c(3);
                c[0] = static_cast<double>(i);
                c[1] = static_cast<double>(j);
                c[2] = static_cast<double>(k);
                coordinates.push_back(c);
                elements.push_back({"A"});
            }
        }
    }

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    // Setup a configuration.
    Configuration configuration(coordinates, elements, possible_types);

    // Setup a lattice map.
    std::vector<int> repetitions = {3, 3, 3};
    // No periodic for minimal matchlist testing.
    std::vector<bool> periodic = {false, false, false};
    LatticeMap lattice_map(nB, repetitions, periodic);

    // Initialize matchlists of configuration.
    const int range = 1;
    configuration.initMatchLists(lattice_map, range);

    // Test lengths of minimal_matchlists.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(configuration.configMatchList(0).size()), 8);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(configuration.configMatchList(1).size()), 12);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(configuration.configMatchList(13).size()), 27);

    // Test size of moved_atom_ids_ before process performing.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(configuration.movedAtomIDs().size()), 0);
    // Test size of recent_move_vectors_ before process performing
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(configuration.recentMoveVectors().size()), 0);


    // Now perform a process.

    // Setup a process that can be performed at site 1.
    std::vector<std::vector<std::string> > process_elements1 = \
        {{"A"}, {"A"}, {"A"}, {"A"}, {"A"}, {"A"},
         {"A"}, {"A"}, {"A"}, {"A"}, {"A"}, {"A"}};
    std::vector<std::vector<std::string> > process_elements2 = \
        {{"B"}, {"V"}, {"V"}, {"B"}, {"V"}, {"B"},
         {"B"}, {"V"}, {"V"}, {"B"}, {"V"}, {"B"}};
    std::vector<std::vector<double> > process_coordinates = {
        {0.0, 0.0, 0.0}, {0.0, 0.0, -1.0}, {0.0, 0.0, 1.0},
        {0.0, 1.0, -1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
        {1.0, 0.0, -1.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0},
        {1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0},
    };
    const double rate = 13.9;
    std::vector<int> basis_sites(1, 0);
    Configuration c1(process_coordinates, process_elements1, possible_types);
    Configuration c2(process_coordinates, process_elements2, possible_types);

    Process p(c1, c2, rate, basis_sites);

    // We know the process can take place at site 1.
    p.addSite(1, 0.0);

    // Perform the process.

    // NOTE: The number of atoms affected here is 12.
    //       If the moved_atoms_ids_ vector had the size of the first matchlist,
    //       (as before the fix this test checks) then the size would be 8 and
    //       there would not be enough space in moved_atom_ids_ and
    //       recent_moved_vectors_ since causing a segmentation fault.
    configuration.performBucketProcess(p, 1, lattice_map);

    // Test entries in moved_atom_ids_.
    const int atom_ids[12] = {1, 0, 2, 4, 10, 3, 5, 9, 11, 13, 12, 14};
    for (int i = 0; i < 12; ++i)
    {
        CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[i], atom_ids[i]);
    }

    // Test recent_moved_vectors_.
    for (int i = 0; i < 12; ++i)
    {
        const Coordinate & coord = configuration.recentMoveVectors()[i];
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.x(), 0.0, 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.y(), 0.0, 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.z(), 0.0, 1.0e-12);
    }

}


// -------------------------------------------------------------------------- //
//
void Test_Configuration::testPerformProcess()
{
    // Setup a realistic system.
    std::vector< std::vector<double> > basis(3, std::vector<double>(3,0.0));
    basis[1][0] = 0.25;
    basis[1][1] = 0.25;
    basis[1][2] = 0.25;
    basis[2][0] = 0.75;
    basis[2][1] = 0.75;
    basis[2][2] = 0.75;

    std::vector<int> basis_sites(3);
    basis_sites[0] = 0;
    basis_sites[1] = 1;
    basis_sites[2] = 2;
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

    elements[0]    = std::vector<std::string>(1, "V");
    elements[216]  = std::vector<std::string>(1, "V");
    elements[1434] = std::vector<std::string>(1, "V");
    elements[2101] = std::vector<std::string>(1, "V");

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

    // Init the match lists.
    configuration.initMatchLists(lattice_map, 1);

    // Get a process that finds a V between two B and turns one of
    // the Bs into an A.
    std::vector<std::vector<std::string> > process_elements1(3);
    process_elements1[0] = std::vector<std::string>(1,"V");
    process_elements1[1] = std::vector<std::string>(1,"B");
    process_elements1[2] = std::vector<std::string>(1,"B");

    std::vector<std::vector<std::string> > process_elements2(3);
    process_elements2[0] = std::vector<std::string>(1,"B");
    process_elements2[1] = std::vector<std::string>(1,"A");
    process_elements2[2] = std::vector<std::string>(1,"B");

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

    // Now, add index 1434 to the process.
    // We know by construction that these match.
    p.addSite(1434, 0.0);
    // For site 1434
    // 350 changes from 1 to 0
    // 1434 changes from 2 to 1
    // All other must remain unchanged.
    CPPUNIT_ASSERT( configuration.types()[1434] == 3 );
    CPPUNIT_ASSERT( configuration.types()[350]  == 2 );
    CPPUNIT_ASSERT( configuration.types()[1433] == 2 );
    CPPUNIT_ASSERT( configuration.types()[349]  == 2 );
    CPPUNIT_ASSERT( configuration.types()[351]  == 1 );
    CPPUNIT_ASSERT( configuration.types()[2517] == 1 );

    // Check that the latest index and process numbers are zero before we perform the process.
    CPPUNIT_ASSERT_EQUAL( configuration.latestEventProcess(), 0 );
    CPPUNIT_ASSERT_EQUAL( configuration.latestEventSite(), 0 );

    // Peform the process.
    configuration.performBucketProcess(p, 1434, lattice_map);

    // Check that the index and process number got saved on the configuration.
    CPPUNIT_ASSERT_EQUAL( configuration.latestEventProcess(), p.processNumber() );
    CPPUNIT_ASSERT_EQUAL( configuration.latestEventSite(), 1434 );

    // Check that the types were correctly updated.
    CPPUNIT_ASSERT( configuration.types()[1434] == 2 );
    CPPUNIT_ASSERT( configuration.types()[350]  == 1 );
    CPPUNIT_ASSERT( configuration.types()[1433] == 2 );
    CPPUNIT_ASSERT( configuration.types()[349]  == 2 );
    CPPUNIT_ASSERT( configuration.types()[351]  == 1 );
    CPPUNIT_ASSERT( configuration.types()[2517] == 1 );

    // Check that the correct indices were added to the list of affected.
    const std::vector<int> affected = p.affectedIndices();

    CPPUNIT_ASSERT_EQUAL( affected[0], 1434 );
    CPPUNIT_ASSERT_EQUAL( affected[1], 350  );

}


// -------------------------------------------------------------------------- //
//
void Test_Configuration::testPerformProcessVectors()
{
    // Setup a realistic system.
    std::vector< std::vector<double> > basis(3, std::vector<double>(3,0.0));
    basis[1][0] = 0.25;
    basis[1][1] = 0.25;
    basis[1][2] = 0.25;
    basis[2][0] = 0.75;
    basis[2][1] = 0.75;
    basis[2][2] = 0.75;

    std::vector<int> basis_sites(3);
    basis_sites[0] = 0;
    basis_sites[1] = 1;
    basis_sites[2] = 2;
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

    // Init the match lists.
    configuration.initMatchLists(lattice_map, 1);

    // Get a process that finds a V between two B and moves it
    // to a nearby site.
    std::vector<std::vector<std::string> > process_elements1(3);
    process_elements1[0] = std::vector<std::string>(1, "V");
    process_elements1[1] = std::vector<std::string>(1, "B");
    process_elements1[2] = std::vector<std::string>(1, "B");

    std::vector<std::vector<std::string> > process_elements2(3);
    process_elements2[0] = std::vector<std::string>(1, "B");
    process_elements2[1] = std::vector<std::string>(1, "V");
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

    // Setup the move vectors.
    std::vector<int> move_origins;
    move_origins.push_back(0);
    move_origins.push_back(1);

    std::vector<Coordinate> move_vectors;
    move_vectors.push_back( Coordinate(-0.25, -0.25, -0.25) );
    move_vectors.push_back( Coordinate( 0.25,  0.25,  0.25) );

    // Construct the process.
    Process p(c1, c2, rate, basis_sites, move_origins, move_vectors);

    // Now, add index 1434 to the process.
    // We know by construction that these match.
    p.addSite(1434, 0.0);

    // Check that the atom ID's and coordinates are unchanged.
    const std::vector<int> & atom_id = configuration.atomID();
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(atom_id.size()),
                          static_cast<int>(configuration.elements().size()) );

    for (int i = 0; i < static_cast<int>(atom_id.size()); ++i)
    {
        CPPUNIT_ASSERT_EQUAL( configuration.atomID()[i], i );

        const Coordinate & coord = configuration.coordinates()[i];
        const Coordinate & id_coord = configuration.atomIdCoordinates()[i];
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.x(), id_coord.x(), 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.y(), id_coord.y(), 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.z(), id_coord.z(), 1.0e-12);
    }

    // Check that the recent move vectors are of length zero.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(configuration.recentMoveVectors().size()), 0);

    // Peform the process.
    configuration.performBucketProcess(p, 1434, lattice_map);

    // This move changes place on atom IDs 1434 and 350.
    CPPUNIT_ASSERT_EQUAL( configuration.atomID()[1434],  350 );
    CPPUNIT_ASSERT_EQUAL( configuration.atomID()[350],  1434 );

    // Check that the recent move vectors are of length two.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(configuration.recentMoveVectors().size()), 2);

    // Check that the norm of the difference is zero.
    const double norm0 = Coordinate(move_vectors[0] - configuration.recentMoveVectors()[0]).norm();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(norm0, 0.0, 1.0e-12);
    const double norm1 = Coordinate(move_vectors[1] - configuration.recentMoveVectors()[1]).norm();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(norm1, 0.0, 1.0e-12);

    // Now, check the ID's again.
    for (int i = 0; i < static_cast<int>(atom_id.size()); ++i)
    {
        if (i != 1434 && i != 350)
        {
            CPPUNIT_ASSERT_EQUAL( configuration.atomID()[i], i );

            const Coordinate & coord = configuration.coordinates()[i];
            const Coordinate & id_coord = configuration.atomIdCoordinates()[i];
            CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.x(), id_coord.x(), 1.0e-12);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.y(), id_coord.y(), 1.0e-12);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.z(), id_coord.z(), 1.0e-12);
        }
    }

    {
        const Coordinate & coord = configuration.coordinates()[1434];
        const Coordinate & id_coord = configuration.atomIdCoordinates()[350];
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.x(), id_coord.x(), 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.y(), id_coord.y(), 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.z(), id_coord.z(), 1.0e-12);
    }

    {
        const Coordinate & coord = configuration.coordinates()[350];
        const Coordinate & id_coord = configuration.atomIdCoordinates()[1434];
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.x(), id_coord.x(), 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.y(), id_coord.y(), 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(coord.z(), id_coord.z(), 1.0e-12);
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Configuration::testAtomID()
{
    // Setup coordinates.
    std::vector<std::vector<double> > coords(5, std::vector<double>(3, 0.0));
    coords[0][0]  = 0.1;
    coords[0][1]  = 0.2;
    coords[0][2]  = 0.3;
    coords[1][0]  = 0.4;
    coords[1][1]  = 0.5;
    coords[1][2]  = 0.6;
    coords[2][0]  = 0.7;
    coords[2][1]  = 0.8;
    coords[2][2]  = 0.9;
    coords[3][0]  = 1.1;
    coords[3][1]  = 1.2;
    coords[3][2]  = 1.3;
    coords[4][0]  = 3.6;
    coords[4][1]  = 3.5;
    coords[4][2]  = 3.4;

    // Setup elements.
    std::vector<std::vector<std::string> > elements(5);
    elements[0] = std::vector<std::string>(1, "A");
    elements[1] = std::vector<std::string>(1, "B");
    elements[2] = std::vector<std::string>(1, "D");
    elements[3] = std::vector<std::string>(1, "H");
    elements[4] = std::vector<std::string>(1, "J");

    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["D"] = 3;
    possible_types["H"] = 4;
    possible_types["J"] = 5;
    possible_types["G"] = 6;

    // Construct the configuration.
    Configuration configuration(coords, elements, possible_types);

    // Check that the atom ID's are unchanged.
    const std::vector<int> & atom_id = configuration.atomID();
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(atom_id.size()),
                          static_cast<int>(configuration.elements().size()) );

    for (int i = 0; i < static_cast<int>(atom_id.size()); ++i)
    {
        CPPUNIT_ASSERT_EQUAL( configuration.atomID()[i], i );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Configuration::testMatchLists()
{
    // Setup a configuration.
    std::vector< std::vector<double> > basis(3, std::vector<double>(3,0.0));
    basis[1][0] = 0.25;
    basis[1][1] = 0.25;
    basis[1][2] = 0.25;
    basis[2][0] = 0.75;
    basis[2][1] = 0.75;
    basis[2][2] = 0.75;

    std::vector<int> basis_sites(3);
    basis_sites[0] = 0;
    basis_sites[1] = 1;
    basis_sites[2] = 2;

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

    elements[0]    = std::vector<std::string>(1, "V");
    elements[216]  = std::vector<std::string>(1, "V");
    elements[1434] = std::vector<std::string>(1, "V");
    elements[2101] = std::vector<std::string>(1, "V");

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

    // Try to access the match lists before initialization. They should be
    // empty.
    CPPUNIT_ASSERT( configuration.configMatchList(10).empty()   );
    CPPUNIT_ASSERT( configuration.configMatchList(2101).empty() );
    CPPUNIT_ASSERT( configuration.configMatchList(1434).empty() );

    // Init the match lists.
    configuration.initMatchLists(lattice_map, 1);

    // This did something.
    CPPUNIT_ASSERT( !configuration.configMatchList(10).empty()   );
    CPPUNIT_ASSERT( !configuration.configMatchList(2101).empty() );
    CPPUNIT_ASSERT( !configuration.configMatchList(1434).empty() );

    // Get the match list the hard way.
    const ConfigBucketMatchList ref_1434 = \
        configuration.configMatchList( 1434,
                                       lattice_map.neighbourIndices(1434),
                                       lattice_map);
    // Check the size.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ref_1434.size()),
                          static_cast<int>(configuration.configMatchList(1434).size()) );

    // Check the values.
    for (size_t i = 0; i < ref_1434.size(); ++i)
    {

        CPPUNIT_ASSERT_EQUAL( ref_1434[i].match_types,
                              configuration.configMatchList(1434)[i].match_types );

        CPPUNIT_ASSERT_EQUAL( ref_1434[i].index,
                              configuration.configMatchList(1434)[i].index );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_1434[i].distance,
                                      configuration.configMatchList(1434)[i].distance,
                                      1.0e-14 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_1434[i].x,
                                      configuration.configMatchList(1434)[i].x,
                                      1.0e-14 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_1434[i].y,
                                      configuration.configMatchList(1434)[i].y,
                                      1.0e-14 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_1434[i].z,
                                      configuration.configMatchList(1434)[i].z,
                                      1.0e-14 );

    }

    // Setup a process that changes V to B.

    // Get a process that finds a V between two B and turns one of
    // the Bs into an A.
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

    // Now, add index 1434 to the process.
    // We know by construction that these match.
    p.addSite(1434, 0.0);

    // For site 1434
    // 350 changes from 1 to 0
    // 1434 changes from 2 to 1
    // All other must remain unchanged.
    CPPUNIT_ASSERT( configuration.types()[1434] == 3 );
    CPPUNIT_ASSERT( configuration.types()[350]  == 2 );
    CPPUNIT_ASSERT( configuration.types()[1433] == 2 );
    CPPUNIT_ASSERT( configuration.types()[349]  == 2 );
    CPPUNIT_ASSERT( configuration.types()[351]  == 1 );
    CPPUNIT_ASSERT( configuration.types()[2517] == 1 );

    // Peform the process.
    configuration.performBucketProcess(p, 1434, lattice_map);

    // Check that the types were correctly updated.
    CPPUNIT_ASSERT( configuration.types()[1434] == 2 );
    CPPUNIT_ASSERT( configuration.types()[350]  == 1 );
    CPPUNIT_ASSERT( configuration.types()[1433] == 2 );
    CPPUNIT_ASSERT( configuration.types()[349]  == 2 );
    CPPUNIT_ASSERT( configuration.types()[351]  == 1 );
    CPPUNIT_ASSERT( configuration.types()[2517] == 1 );

    // Check that updating the matchlist gets us the correct values.
    configuration.updateMatchList(1434);

    // Reference.
    const ConfigBucketMatchList ref2_1434 =        \
        configuration.configMatchList( 1434,
                                       lattice_map.neighbourIndices(1434),
                                       lattice_map);
    // Check the size.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ref2_1434.size()),
                          static_cast<int>(configuration.configMatchList(1434).size()) );

    // Check the values.
    for (size_t i = 0; i < ref2_1434.size(); ++i)
    {

        CPPUNIT_ASSERT_EQUAL( ref2_1434[i].match_types,
                              configuration.configMatchList(1434)[i].match_types );

        CPPUNIT_ASSERT_EQUAL( ref2_1434[i].index, configuration.configMatchList(1434)[i].index );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref2_1434[i].distance,
                                      configuration.configMatchList(1434)[i].distance,
                                      1.0e-14 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref2_1434[i].x,
                                      configuration.configMatchList(1434)[i].x,
                                      1.0e-14 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref2_1434[i].y,
                                      configuration.configMatchList(1434)[i].y,
                                      1.0e-14 );

        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref2_1434[i].z,
                                      configuration.configMatchList(1434)[i].z,
                                      1.0e-14 );

    }
}


// -------------------------------------------------------------------------- //
//
void Test_Configuration::testTypeNameQuery()
{
    // Setup coordinates.
    const std::vector<std::vector<double> > coords(1, std::vector<double>(3, 0.0));

    // Setup elements.
    const std::vector<std::vector<std::string> > elements(1, std::vector<std::string>(1,"A"));

    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["D"] = 3;
    possible_types["H"] = 4;
    possible_types["J"] = 5;
    possible_types["G"] = 6;

    // Construct the configuration.
    const Configuration config(coords, elements, possible_types);

    // Query for the type names.
    CPPUNIT_ASSERT_EQUAL( config.typeName(0), std::string("*") );
    CPPUNIT_ASSERT_EQUAL( config.typeName(1), std::string("A") );
    CPPUNIT_ASSERT_EQUAL( config.typeName(2), std::string("B") );
    CPPUNIT_ASSERT_EQUAL( config.typeName(3), std::string("D") );
    CPPUNIT_ASSERT_EQUAL( config.typeName(4), std::string("H") );
    CPPUNIT_ASSERT_EQUAL( config.typeName(5), std::string("J") );
    CPPUNIT_ASSERT_EQUAL( config.typeName(6), std::string("G") );

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_Configuration::testAtomIDElementsCoordinatesMovedIDs()
{
    // Setup a configuration.
    const std::vector< std::vector<double> > basis(0, std::vector<double>(3,0.0));
    const std::vector<int> basis_sites(1, 0);
    const std::vector<std::string> basis_elements(1, "A");

    // Make a 37x18x19 structure.
    const int nI = 37;
    const int nJ = 18;
    const int nK = 19;
    const int nB = 1;

    // Coordinates and elements.
    std::vector<std::vector<double> > coordinates;
    std::vector<std::vector<std::string> > elements;

    for (int i = 0; i < nI; ++i)
    {
        for (int j = 0; j < nJ; ++j)
        {
            for (int k = 0; k < nK; ++k)
            {
                std::vector<double> c(3);
                c[0] = static_cast<double>(i);
                c[1] = static_cast<double>(j);
                c[2] = static_cast<double>(k);
                coordinates.push_back(c);
                elements.push_back(std::vector<std::string>(1, basis_elements[0]));
            }
        }
    }

    elements[0]    = std::vector<std::string>(1, "V");
    elements[216]  = std::vector<std::string>(1, "V");
    elements[1434] = std::vector<std::string>(1, "V");
    elements[2101] = std::vector<std::string>(1, "V");

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    // Setup the configuration.
    Configuration configuration(coordinates, elements, possible_types);

    // Test the atom id coordinates. Should initially be the same as the coordinates of
    // the configuration.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(configuration.coordinates().size()),
                          static_cast<int>(configuration.atomIDCoordinates().size()) );

    for (size_t i = 0; i < configuration.coordinates().size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[i].x(),
                                     configuration.atomIDCoordinates()[i].x(),
                                     1.0e-12);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[i].y(),
                                     configuration.atomIDCoordinates()[i].y(),
                                     1.0e-12);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[i].z(),
                                     configuration.atomIDCoordinates()[i].z(),
                                     1.0e-12);
    }

    // Test the atom id elements. Should initially be the same as the types of
    // the lattice configuration.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(configuration.elements().size()),
                          static_cast<int>(configuration.atomIDElements().size()) );

    for (size_t i = 0; i < configuration.elements().size(); ++i)
    {
        // FIXME - this is expected behavior at the moment, but incorrect.
        //         Atom IDs can only be used when there is exactly one atom per site.
        //         This will be updated in the release version.
        CPPUNIT_ASSERT_EQUAL(configuration.elements()[i][0],
                             configuration.atomIDElements()[i]);
    }

    // Setup the lattice map.
    std::vector<int> repetitions(3);
    repetitions[0] = nI;
    repetitions[1] = nJ;
    repetitions[2] = nK;
    std::vector<bool> periodicity(3, true);
    LatticeMap lattice_map(nB, repetitions, periodicity);

    // Init the match lists.
    configuration.initMatchLists(lattice_map, 1);

    // Get a process that finds a V surrounded by A,
    // moves two of the A's and changing the V to B,
    // keeping all other atoms in place.
    std::vector<std::vector<std::string> > process_elements1(7);
    process_elements1[0] = std::vector<std::string>(1, "V");
    process_elements1[1] = std::vector<std::string>(1, "A");
    process_elements1[2] = std::vector<std::string>(1, "A");
    process_elements1[3] = std::vector<std::string>(1, "A");
    process_elements1[4] = std::vector<std::string>(1, "A");
    process_elements1[5] = std::vector<std::string>(1, "A");
    process_elements1[6] = std::vector<std::string>(1, "A");

    std::vector<std::vector<std::string> > process_elements2(7);
    process_elements2[0] = std::vector<std::string>(1, "B");
    process_elements2[1] = std::vector<std::string>(1, "V");
    process_elements2[2] = std::vector<std::string>(1, "V");
    process_elements2[3] = std::vector<std::string>(1, "A");
    process_elements2[4] = std::vector<std::string>(1, "A");
    process_elements2[5] = std::vector<std::string>(1, "A");
    process_elements2[6] = std::vector<std::string>(1, "A");

    std::vector<std::vector<double> > process_coordinates(7, std::vector<double>(3, 0.0));

    process_coordinates[1][0] = -1.0;
    process_coordinates[1][1] =  0.0;
    process_coordinates[1][2] =  0.0;

    process_coordinates[2][0] =  1.0;
    process_coordinates[2][1] =  0.0;
    process_coordinates[2][2] =  0.0;

    process_coordinates[3][0] =  0.0;
    process_coordinates[3][1] = -1.0;
    process_coordinates[3][2] =  0.0;

    process_coordinates[4][0] =  0.0;
    process_coordinates[4][1] =  1.0;
    process_coordinates[4][2] =  0.0;

    process_coordinates[5][0] =  0.0;
    process_coordinates[5][1] =  0.0;
    process_coordinates[5][2] = -1.0;

    process_coordinates[6][0] =  0.0;
    process_coordinates[6][1] =  0.0;
    process_coordinates[6][2] =  1.0;

    const double rate = 13.7;
    Configuration c1(process_coordinates, process_elements1, possible_types);
    Configuration c2(process_coordinates, process_elements2, possible_types);

    // Setup the move vectors for the first process.
    std::vector<int> move_origins_p1;
    move_origins_p1.push_back(1);
    move_origins_p1.push_back(2);

    std::vector<Coordinate> move_coordinates_p1;

    // Move 1 to 2 (atom_id 1092 moved from lattice site 1092 to
    // lattice site 1776)
    move_coordinates_p1.push_back(Coordinate( 2.0, 0.0, 0.0));

    // Move 2 to 1 (atom_id 1776 moved from lattice site 1776 to
    // lattice site 1092)
    move_coordinates_p1.push_back(Coordinate(-2.0, 0.0, 0.0));

    Process p(c1, c2, rate, basis_sites, move_origins_p1, move_coordinates_p1);

    // Now, add index 1434 to the process.
    // We know by construction that these match.
    p.addSite(1434, 0.0);

    // Peform the process.
    configuration.performBucketProcess(p, 1434, lattice_map);

    // Get the moved atom_id:s out.
    const std::vector<int> moved_atom_ids1 = configuration.movedAtomIDs();

    // Check marked ids.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(moved_atom_ids1.size()), 3 );
    CPPUNIT_ASSERT_EQUAL( moved_atom_ids1[0], 1434 );
    CPPUNIT_ASSERT_EQUAL( moved_atom_ids1[1], 1092 );
    CPPUNIT_ASSERT_EQUAL( moved_atom_ids1[2], 1776 );

    // Check the corresponding atom id coordinates.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1776].x(),
                                 configuration.atomIDCoordinates()[1092].x(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1776].y(),
                                 configuration.atomIDCoordinates()[1092].y(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1776].z(),
                                 configuration.atomIDCoordinates()[1092].z(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1092].x(),
                                 configuration.atomIDCoordinates()[1776].x(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1092].y(),
                                 configuration.atomIDCoordinates()[1776].y(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1092].z(),
                                 configuration.atomIDCoordinates()[1776].z(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1434].x(),
                                 configuration.atomIDCoordinates()[1434].x(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1434].y(),
                                 configuration.atomIDCoordinates()[1434].y(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1434].z(),
                                 configuration.atomIDCoordinates()[1434].z(),
                                 1.0e-12);

    // FIXME: Again, only matchig the first atom found ([0]).

    // Check the corresponding atom ID elements.
    CPPUNIT_ASSERT_EQUAL(configuration.elements()[1776][0],
                         configuration.atomIDElements()[1092]);
    CPPUNIT_ASSERT_EQUAL(configuration.elements()[1092][0],
                         configuration.atomIDElements()[1776]);
    CPPUNIT_ASSERT_EQUAL(configuration.elements()[1434][0],
                         configuration.atomIDElements()[1434]);

    // Get a process that finds a B surrounded by two V's in the
    // positive and negative x directions and the rest A.

    // Moves atoms. Make sure we change the atom_id
    // at the central location.

    process_elements1[0] = std::vector<std::string>(1, "B");
    process_elements1[1] = std::vector<std::string>(1, "V");
    process_elements1[2] = std::vector<std::string>(1, "V");
    process_elements1[3] = std::vector<std::string>(1, "A");
    process_elements1[4] = std::vector<std::string>(1, "A");
    process_elements1[5] = std::vector<std::string>(1, "A");
    process_elements1[6] = std::vector<std::string>(1, "A");

    process_elements2[0] = std::vector<std::string>(1, "V");
    process_elements2[1] = std::vector<std::string>(1, "B");
    process_elements2[2] = std::vector<std::string>(1, "A");
    process_elements2[3] = std::vector<std::string>(1, "A");
    process_elements2[4] = std::vector<std::string>(1, "V");
    process_elements2[5] = std::vector<std::string>(1, "A");
    process_elements2[6] = std::vector<std::string>(1, "A");

    // Setup the move vectors for the second process.
    std::vector<int> move_origins_p2;
    move_origins_p2.push_back(0);
    move_origins_p2.push_back(1);
    move_origins_p2.push_back(2);
    move_origins_p2.push_back(4);

    std::vector<Coordinate> move_coordinates_p2;

    // Move 0 to 1 (atom_id 1434 moved from lattice site 1434 to
    // lattice site 1092)
    move_coordinates_p2.push_back(Coordinate(-1.0, 0.0, 0.0));

    // Move 1 to 4 (atom_id 1776 moved from lattice site 1092 to
    // lattice site 1453)
    move_coordinates_p2.push_back(Coordinate( 1.0, 1.0, 0.0));

    // Move 2 to 0 (atom_id 1092 moved from lattice site 1776 to
    // lattice site 1434)
    move_coordinates_p2.push_back(Coordinate(-1.0, 0.0, 0.0));

    // Move 4 to 2 (atom_id 1453 moved from lattice site 1453 to
    // lattice site 1776)
    move_coordinates_p2.push_back(Coordinate( 1.0,-1.0, 0.0));

    Configuration c3(process_coordinates, process_elements1, possible_types);
    Configuration c4(process_coordinates, process_elements2, possible_types);
    Process p2(c3, c4, rate, basis_sites, move_origins_p2, move_coordinates_p2);

    // Now, add index 1434 to the process.
    // We know by construction that these match.
    p2.addSite(1434, 0.0);

    // Peform the process.
    configuration.updateMatchList(1434);
    configuration.performBucketProcess(p2, 1434, lattice_map);

    // Get the moved atom_id:s out.
    const std::vector<int> moved_atom_ids2 = configuration.movedAtomIDs();

    // Check marked ids.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(moved_atom_ids2.size()), 4 );
    CPPUNIT_ASSERT_EQUAL( moved_atom_ids2[0], 1434 );
    CPPUNIT_ASSERT_EQUAL( moved_atom_ids2[1], 1776 );
    CPPUNIT_ASSERT_EQUAL( moved_atom_ids2[2], 1453 );
    CPPUNIT_ASSERT_EQUAL( moved_atom_ids2[3], 1092 );

    // Check that the ids sit where we think they should.
    CPPUNIT_ASSERT_EQUAL( configuration.atomID()[1434], 1092 );
    CPPUNIT_ASSERT_EQUAL( configuration.atomID()[1092], 1434 );
    CPPUNIT_ASSERT_EQUAL( configuration.atomID()[1453], 1776 );
    CPPUNIT_ASSERT_EQUAL( configuration.atomID()[1776], 1453 );

    // Check that they have the types they should have.
    CPPUNIT_ASSERT_EQUAL(configuration.elements()[1092][0],
                         configuration.atomIDElements()[1434]);
    CPPUNIT_ASSERT_EQUAL(configuration.elements()[1434][0],
                         configuration.atomIDElements()[1092]);
    CPPUNIT_ASSERT_EQUAL(configuration.elements()[1776][0],
                         configuration.atomIDElements()[1453]);
    CPPUNIT_ASSERT_EQUAL(configuration.elements()[1453][0],
                         configuration.atomIDElements()[1776]);

    // Check the coordinates.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1434].x(),
                                 configuration.atomIDCoordinates()[1092].x(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1434].y(),
                                 configuration.atomIDCoordinates()[1092].y(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1434].z(),
                                 configuration.atomIDCoordinates()[1092].z(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1092].x(),
                                 configuration.atomIDCoordinates()[1434].x(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1092].y(),
                                 configuration.atomIDCoordinates()[1434].y(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1092].z(),
                                 configuration.atomIDCoordinates()[1434].z(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1453].x(),
                                 configuration.atomIDCoordinates()[1776].x(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1453].y(),
                                 configuration.atomIDCoordinates()[1776].y(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1453].z(),
                                 configuration.atomIDCoordinates()[1776].z(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1776].x(),
                                 configuration.atomIDCoordinates()[1453].x(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1776].y(),
                                 configuration.atomIDCoordinates()[1453].y(),
                                 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(configuration.coordinates()[1776].z(),
                                 configuration.atomIDCoordinates()[1453].z(),
                                 1.0e-12);
}

// -------------------------------------------------------------------------- //
//
void Test_Configuration::testUpdateInfo()
{
    // Setup coordinates.
    std::vector<std::vector<double> > coords(5, std::vector<double>(3, 0.0));
    coords[0][0]  = 0.1;
    coords[0][1]  = 0.2;
    coords[0][2]  = 0.3;
    coords[1][0]  = 0.4;
    coords[1][1]  = 0.5;
    coords[1][2]  = 0.6;
    coords[2][0]  = 0.7;
    coords[2][1]  = 0.8;
    coords[2][2]  = 0.9;
    coords[3][0]  = 1.1;
    coords[3][1] = 1.2;
    coords[3][2] = 1.3;
    coords[4][0] = 3.6;
    coords[4][1] = 3.5;
    coords[4][2] = 3.4;

    // Setup elements.
    std::vector< std::vector<std::string> > elements(5);
    elements[0] = std::vector<std::string>(1, "A");
    elements[1] = std::vector<std::string>(1, "B");
    elements[2] = std::vector<std::string>(1, "D");
    elements[3] = std::vector<std::string>(1, "H");
    elements[4] = std::vector<std::string>(1, "J");

    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["D"] = 3;
    possible_types["H"] = 4;
    possible_types["J"] = 5;
    possible_types["G"] = 6;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // Create update info.
    std::vector<std::map<std::string, int> > update(5);
    update[0]["*"] =  1;

    update[1]["A"] =  2;
    update[1]["B"] = -2;

    update[2]["A"] = -2;
    update[2]["B"] =  2;

    update[3]["C"] =  1;

    update[4]["*"] =  1;

    // Set the update info on the class.
    config.setUpdateInfo(update);

    // Check that the update info is there.
    std::vector<std::map<std::string, int> > ret_update = config.updateInfo();

    CPPUNIT_ASSERT_EQUAL(update[0]["*"], ret_update[0]["*"]);
    CPPUNIT_ASSERT_EQUAL(update[1]["A"], ret_update[1]["A"]);
    CPPUNIT_ASSERT_EQUAL(update[1]["B"], ret_update[1]["B"]);
    CPPUNIT_ASSERT_EQUAL(update[2]["B"], ret_update[2]["B"]);
}


// -------------------------------------------------------------------------- //
//
void Test_Configuration::testParticlesPerType()
{
    // Setup coordinates.
    std::vector<std::vector<double> > coords(5, std::vector<double>(3, 0.0));
    coords[0][0]  = 0.1;
    coords[0][1]  = 0.2;
    coords[0][2]  = 0.3;
    coords[1][0]  = 0.4;
    coords[1][1]  = 0.5;
    coords[1][2]  = 0.6;
    coords[2][0]  = 0.7;
    coords[2][1]  = 0.8;
    coords[2][2]  = 0.9;
    coords[3][0]  = 1.1;
    coords[3][1] = 1.2;
    coords[3][2] = 1.3;
    coords[4][0] = 3.6;
    coords[4][1] = 3.5;
    coords[4][2] = 3.4;

    // Setup elements.
    std::vector< std::vector<std::string> > elements(5);
    elements[0] = std::vector<std::string>(2, "A");
    elements[0].push_back("G");

    elements[1] = std::vector<std::string>(3, "B");
    elements[1].push_back("G");
    elements[1].push_back("G");

    elements[2] = std::vector<std::string>(4, "D");
    elements[2].push_back("G");
    elements[2].push_back("G");
    elements[2].push_back("G");

    elements[3] = std::vector<std::string>(5, "H");
    elements[3].push_back("G");
    elements[3].push_back("G");
    elements[3].push_back("G");
    elements[3].push_back("G");

    elements[4] = std::vector<std::string>(1, "J");
    elements[4].push_back("G");
    elements[4].push_back("J");

    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["D"] = 3;
    possible_types["H"] = 4;
    possible_types["J"] = 5;
    possible_types["G"] = 6;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // Get the particles per type.
    const std::vector<int> particles_per_type_1 = config.particlesPerType();

    // Check the length.
    CPPUNIT_ASSERT_EQUAL( particles_per_type_1.size(), possible_types.size() );

    // Check the content.
    CPPUNIT_ASSERT_EQUAL( particles_per_type_1[0], 0);
    CPPUNIT_ASSERT_EQUAL( particles_per_type_1[1], 2);
    CPPUNIT_ASSERT_EQUAL( particles_per_type_1[2], 3);
    CPPUNIT_ASSERT_EQUAL( particles_per_type_1[3], 4);
    CPPUNIT_ASSERT_EQUAL( particles_per_type_1[4], 5);
    CPPUNIT_ASSERT_EQUAL( particles_per_type_1[5], 2);
    CPPUNIT_ASSERT_EQUAL( particles_per_type_1[6], 11);

    // DONE
}
