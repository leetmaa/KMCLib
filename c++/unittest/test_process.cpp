/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_process.h"

// Include the files to test.
#include "process.h"

#include "configuration.h"
#include "random.h"

#include <cmath>

// -------------------------------------------------------------------------- //
//
void Test_Process::testConstruction()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    std::vector<int> basis_sites(3,0);
    basis_sites[1] = 23;
    basis_sites[2] = 11;
    const double rate = 13.7;
    Process process(config1, config2, rate, basis_sites);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Check that the basis sites can be returned correctly.
    const std::vector<int> ret_basis_sites = process.basisSites();
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[0], basis_sites[0] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[1], basis_sites[1] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[2], basis_sites[2] );

}


// -------------------------------------------------------------------------- //
//
void Test_Process::testConstruction2()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Get the list of move origins.
    std::vector<int> move_origins;
    move_origins.push_back(0);
    move_origins.push_back(1);

    // Setup the move vectors.
    std::vector<Coordinate> move_vectors;
    move_vectors.push_back(Coordinate(1.0, 1.3, -4.4));
    move_vectors.push_back(Coordinate(-1.0, -1.3, 4.4));

    // Construct the process.
    std::vector<int> basis_sites(3,0);
    basis_sites[1] = 23;
    basis_sites[2] = 11;
    const double rate = 13.7;
    Process process(config1, config2, rate, basis_sites, move_origins, move_vectors);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Check that the basis sites can be returned correctly.
    const std::vector<int> ret_basis_sites = process.basisSites();
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[0], basis_sites[0] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[1], basis_sites[1] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[2], basis_sites[2] );

    // Check that the list of id-moves was correctly constructed
    // based on the move vectors.
    const std::vector< std::pair<int,int> > & id_moves = process.idMoves();

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(id_moves.size()), 2 );
    CPPUNIT_ASSERT_EQUAL( id_moves[0].first,  0 );
    CPPUNIT_ASSERT_EQUAL( id_moves[0].second, 1 );
    CPPUNIT_ASSERT_EQUAL( id_moves[1].first,  1 );
    CPPUNIT_ASSERT_EQUAL( id_moves[1].second, 0 );
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testConstruction3()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[0][0] =   0.0;
    coords[0][1] =   0.0;
    coords[0][2] = 100.0;

    coords[1][0] = -1.0;
    coords[1][1] = -1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Get the list of move origins.
    std::vector<int> move_origins;
    move_origins.push_back(0);
    move_origins.push_back(1);

    // Setup the move vectors.
    std::vector<Coordinate> move_vectors;
    move_vectors.push_back(Coordinate(-1.0, -1.3, -104.4));
    move_vectors.push_back(Coordinate(1.0, 1.3, 104.4));

    // Construct the process.
    std::vector<int> basis_sites(3,0);
    basis_sites[1] = 23;
    basis_sites[2] = 11;
    const double rate = 13.7;
    Process process(config1, config2, rate, basis_sites, move_origins, move_vectors);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Check that the basis sites can be returned correctly.
    const std::vector<int> ret_basis_sites = process.basisSites();
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[0], basis_sites[0] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[1], basis_sites[1] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[2], basis_sites[2] );

    // Check that the list of id-moves was correctly constructed
    // based on the move vectors.
    const std::vector< std::pair<int,int> > & id_moves = process.idMoves();

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(id_moves.size()), 2 );
    CPPUNIT_ASSERT_EQUAL( id_moves[0].first,  0 );
    CPPUNIT_ASSERT_EQUAL( id_moves[0].second, 1 );
    CPPUNIT_ASSERT_EQUAL( id_moves[1].first,  1 );
    CPPUNIT_ASSERT_EQUAL( id_moves[1].second, 0 );
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testConstructionMoveVectors()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["C"] = 2;
    possible_types["D"] = 3;
    possible_types["E"] = 4;
    possible_types["F"] = 5;
    possible_types["G"] = 6;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "D"));
    elements1.push_back(std::vector<std::string>(1, "E"));
    elements1.push_back(std::vector<std::string>(1, "F"));
    elements1.push_back(std::vector<std::string>(1, "G"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "A"));
    elements2.push_back(std::vector<std::string>(1, "G"));
    elements2.push_back(std::vector<std::string>(1, "F"));
    elements2.push_back(std::vector<std::string>(1, "D"));
    elements2.push_back(std::vector<std::string>(1, "E"));
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(7,std::vector<double>(3,0.0));
    coords[0][0] =  0.0;
    coords[0][1] =  0.0;
    coords[0][2] =  0.0;

    coords[1][0] =  1.0;
    coords[1][1] =  1.0;
    coords[1][2] =  1.0;

    coords[2][0] =  2.0;
    coords[2][1] =  2.0;
    coords[2][2] =  2.0;

    coords[3][0] =  3.0;
    coords[3][1] =  3.0;
    coords[3][2] =  3.0;

    coords[4][0] =  4.0;
    coords[4][1] =  4.0;
    coords[4][2] =  4.0;

    coords[5][0] =  5.0;
    coords[5][1] =  5.0;
    coords[5][2] =  5.0;

    coords[6][0] =  6.0;
    coords[6][1] =  6.0;
    coords[6][2] =  6.0;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Get the list of move origins.
    std::vector<int> move_origins;
    move_origins.push_back(1);
    move_origins.push_back(2);
    move_origins.push_back(5);
    move_origins.push_back(6);

    // Setup the move vectors.
    std::vector<Coordinate> move_vectors;
    move_vectors.push_back(Coordinate( 5.0, 5.0, 5.0));
    move_vectors.push_back(Coordinate( 3.0, 3.0, 3.0));
    move_vectors.push_back(Coordinate(-3.0,-3.0,-3.0));
    move_vectors.push_back(Coordinate(-5.0,-5.0,-5.0));

    // Construct the process.
    std::vector<int> basis_sites(3,0);
    basis_sites[1] = 23;
    basis_sites[2] = 11;
    const double rate = 13.7;
    Process process(config1, config2, rate, basis_sites, move_origins, move_vectors);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Check that the basis sites can be returned correctly.
    const std::vector<int> ret_basis_sites = process.basisSites();
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[0], basis_sites[0] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[1], basis_sites[1] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[2], basis_sites[2] );

    // Check that the list of id-moves was correctly constructed
    // based on the move vectors.
    const std::vector< std::pair<int,int> > & id_moves = process.idMoves();

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(id_moves.size()), 4 );
    CPPUNIT_ASSERT_EQUAL( id_moves[0].first,  1 );
    CPPUNIT_ASSERT_EQUAL( id_moves[0].second, 6 );

    CPPUNIT_ASSERT_EQUAL( id_moves[1].first,  2 );
    CPPUNIT_ASSERT_EQUAL( id_moves[1].second, 5 );

    CPPUNIT_ASSERT_EQUAL( id_moves[2].first,  5 );
    CPPUNIT_ASSERT_EQUAL( id_moves[2].second, 2 );

    CPPUNIT_ASSERT_EQUAL( id_moves[3].first,  6 );
    CPPUNIT_ASSERT_EQUAL( id_moves[3].second, 1 );
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testConstructionMoveVectors2()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["C"] = 2;
    possible_types["D"] = 3;
    possible_types["E"] = 4;
    possible_types["F"] = 5;
    possible_types["G"] = 6;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "F"));
    elements1.push_back(std::vector<std::string>(1, "G"));
    elements1.push_back(std::vector<std::string>(1, "D"));
    elements1.push_back(std::vector<std::string>(1, "E"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "C"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "A"));
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "D"));
    elements2.push_back(std::vector<std::string>(1, "E"));
    elements2.push_back(std::vector<std::string>(1, "G"));
    elements2.push_back(std::vector<std::string>(1, "F"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(7,std::vector<double>(3,0.0));
    coords[0][0] =  0.0;
    coords[0][1] =  0.0;
    coords[0][2] =  0.0;

    coords[1][0] =  5.0;
    coords[1][1] =  5.0;
    coords[1][2] =  5.0;

    coords[2][0] =  6.0;
    coords[2][1] =  6.0;
    coords[2][2] =  6.0;

    coords[3][0] =  3.0;
    coords[3][1] =  3.0;
    coords[3][2] =  3.0;

    coords[4][0] =  4.0;
    coords[4][1] =  4.0;
    coords[4][2] =  4.0;

    coords[5][0] =  1.0;
    coords[5][1] =  1.0;
    coords[5][2] =  1.0;

    coords[6][0] =  2.0;
    coords[6][1] =  2.0;
    coords[6][2] =  2.0;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Get the list of move origins.
    std::vector<int> move_origins;
    move_origins.push_back(1);
    move_origins.push_back(2);
    move_origins.push_back(5);
    move_origins.push_back(6);

    // Setup the move vectors.
    std::vector<Coordinate> move_vectors;
    move_vectors.push_back(Coordinate(-3.0,-3.0,-3.0));
    move_vectors.push_back(Coordinate(-5.0,-5.0,-5.0));
    move_vectors.push_back(Coordinate( 5.0, 5.0, 5.0));
    move_vectors.push_back(Coordinate( 3.0, 3.0, 3.0));

    // Construct the process.
    std::vector<int> basis_sites(3,0);
    basis_sites[1] = 23;
    basis_sites[2] = 11;
    const double rate = 13.7;
    Process process(config1, config2, rate, basis_sites, move_origins, move_vectors);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Check that the basis sites can be returned correctly.
    const std::vector<int> ret_basis_sites = process.basisSites();
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[0], basis_sites[0] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[1], basis_sites[1] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[2], basis_sites[2] );

    // Check that the list of id-moves was correctly constructed
    // based on the move vectors.
    const std::vector< std::pair<int,int> > & id_moves = process.idMoves();

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(id_moves.size()), 4 );
    CPPUNIT_ASSERT_EQUAL( id_moves[0].first,  1 );
    CPPUNIT_ASSERT_EQUAL( id_moves[0].second, 6 );

    CPPUNIT_ASSERT_EQUAL( id_moves[1].first,  2 );
    CPPUNIT_ASSERT_EQUAL( id_moves[1].second, 5 );

    CPPUNIT_ASSERT_EQUAL( id_moves[2].first,  5 );
    CPPUNIT_ASSERT_EQUAL( id_moves[2].second, 2 );

    CPPUNIT_ASSERT_EQUAL( id_moves[3].first,  6 );
    CPPUNIT_ASSERT_EQUAL( id_moves[3].second, 1 );
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testMatchList()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 2;
    possible_types["B"] = 1;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "C"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "A"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(3,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;
    coords[2][0] =  0.1;
    coords[2][1] =  0.3;
    coords[2][2] = -0.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const std::vector<int> basis_sites(1,0);
    const double rate = 13.7;
    Process process(config1, config2, rate, basis_sites);

    // Get the match list out.
    const ProcessBucketMatchList & match_list = process.processMatchList();

    // Check the size of the match list.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(match_list.size()), 3);

    // Get the first entry out and check.
    {
        const ProcessBucketMatchListEntry entry = match_list[0];
        CPPUNIT_ASSERT_EQUAL(entry.index, -1);
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(entry.match_types.size()),  3);
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(entry.update_types.size()), 3);

        CPPUNIT_ASSERT_EQUAL(entry.match_types[0], 0);
        CPPUNIT_ASSERT_EQUAL(entry.match_types[1], 0);
        CPPUNIT_ASSERT_EQUAL(entry.match_types[2], 1);

        CPPUNIT_ASSERT_EQUAL(entry.update_types[0], 1);
        CPPUNIT_ASSERT_EQUAL(entry.update_types[1], 0);
        CPPUNIT_ASSERT_EQUAL(entry.update_types[2],-1);


        // Make sure these coordinates are equal.
        Coordinate check_coord(coords[0][0],coords[0][1],coords[0][2]);
        CPPUNIT_ASSERT( !(entry.coordinate < check_coord) );
        CPPUNIT_ASSERT( !(check_coord      < entry.coordinate) );

        // Check the distance.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(entry.distance, 0.0, 1.0e-14);

    }

    // Get the third entry out and check.
    {
        const ProcessBucketMatchListEntry entry = match_list[2];

        CPPUNIT_ASSERT_EQUAL(entry.index, -1);
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(entry.match_types.size()),  3);
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(entry.update_types.size()), 3);

        CPPUNIT_ASSERT_EQUAL(entry.match_types[0], 0);
        CPPUNIT_ASSERT_EQUAL(entry.match_types[1], 1);
        CPPUNIT_ASSERT_EQUAL(entry.match_types[2], 0);

        CPPUNIT_ASSERT_EQUAL(entry.update_types[0], 0);
        CPPUNIT_ASSERT_EQUAL(entry.update_types[1], 0);
        CPPUNIT_ASSERT_EQUAL(entry.update_types[2], 0);

        // Make sure these coordinates are equal.
        Coordinate check_coord(coords[1][0],coords[1][1],coords[1][2]);
        CPPUNIT_ASSERT( !(entry.coordinate < check_coord) );
        CPPUNIT_ASSERT( !(check_coord      < entry.coordinate) );

        // Check the distance.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(entry.distance, check_coord.distance(Coordinate(0.0,0.0,0.0)), 1.0e-14);

    }

    // Get the second entry out and check.
    {
        const ProcessBucketMatchListEntry entry = match_list[1];

        CPPUNIT_ASSERT_EQUAL(entry.index, -1);
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(entry.match_types.size()),  3);
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(entry.update_types.size()), 3);

        CPPUNIT_ASSERT_EQUAL(entry.match_types[0], 1);
        CPPUNIT_ASSERT_EQUAL(entry.match_types[1], 0);
        CPPUNIT_ASSERT_EQUAL(entry.match_types[2], 0);

        CPPUNIT_ASSERT_EQUAL(entry.update_types[0],-1);
        CPPUNIT_ASSERT_EQUAL(entry.update_types[1], 0);
        CPPUNIT_ASSERT_EQUAL(entry.update_types[2], 1);

        // Make sure these coordinates are equal.
        Coordinate check_coord(coords[2][0],coords[2][1],coords[2][2]);
        CPPUNIT_ASSERT( !(entry.coordinate < check_coord) );
        CPPUNIT_ASSERT( !(check_coord      < entry.coordinate) );

        // Check the distance.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(entry.distance, check_coord.distance(Coordinate(0.0,0.0,0.0)), 1.0e-14);

    }
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testMatchListLong()
{
    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["C"] = 2;
    possible_types["D"] = 3;
    possible_types["E"] = 4;
    possible_types["F"] = 5;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > process_coords(9,std::vector<double>(3,0.0));
    process_coords[1][0] =  0.5;
    process_coords[1][1] =  0.5;
    process_coords[1][2] =  0.5;

    process_coords[2][0] = -0.5;
    process_coords[2][1] =  0.5;
    process_coords[2][2] =  0.5;

    process_coords[3][0] =  0.5;
    process_coords[3][1] = -0.5;
    process_coords[3][2] =  0.5;

    process_coords[4][0] =  0.5;
    process_coords[4][1] =  0.5;
    process_coords[4][2] = -0.5;

    process_coords[5][0] = -0.5;
    process_coords[5][1] = -0.5;
    process_coords[5][2] =  0.5;

    process_coords[6][0] = -0.5;
    process_coords[6][1] =  0.5;
    process_coords[6][2] = -0.5;

    process_coords[7][0] =  0.5;
    process_coords[7][1] = -0.5;
    process_coords[7][2] = -0.5;

    process_coords[8][0] = -0.5;
    process_coords[8][1] = -0.5;
    process_coords[8][2] = -0.5;

    // The configurations.
    const Configuration config1(process_coords, elements1, possible_types);
    const Configuration config2(process_coords, elements2, possible_types);


    // Check the length of the coordinates from the configuration.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(config1.coordinates().size()),
                          static_cast<int>(process_coords.size()));

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Get the match list out.
    const ProcessBucketMatchList match_list = process.processMatchList();

    // Check the size of the match list.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(process_coords.size()),
                          static_cast<int>(match_list.size()));

    // Check the coordinates.
    const Coordinate c0 = match_list[0].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c0.x(), 0.0, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c0.y(), 0.0, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c0.z(), 0.0, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[0].distance, 0.0, 1.0e-14 );

    const Coordinate c1 = match_list[1].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[1].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c2 = match_list[2].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c2.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c2.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c2.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[2].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c3 = match_list[3].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c3.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c3.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c3.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[3].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c4 = match_list[4].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c4.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c4.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c4.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[4].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c5 = match_list[5].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c5.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c5.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c5.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[5].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c6 = match_list[6].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c6.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c6.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c6.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[6].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c7 = match_list[7].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c7.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c7.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c7.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[7].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c8 = match_list[8].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c8.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c8.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c8.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[8].distance, std::sqrt(0.25*3.0), 1.0e-14 );

}


// -------------------------------------------------------------------------- //
//
void Test_Process::testAddAndRemoveSite()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Add a few indices.
    process.addSite(1234);
    process.addSite(3);
    process.addSite(11);
    process.addSite(-123);

    // Check that these indices are now there.
    CPPUNIT_ASSERT( process.isListed(1234) );
    CPPUNIT_ASSERT( process.isListed(3)    );
    CPPUNIT_ASSERT( process.isListed(11)   );
    CPPUNIT_ASSERT( process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 4);

    // Remove two of the indices.
    process.removeSite(3);
    process.removeSite(11);

    // Check again.
    CPPUNIT_ASSERT( process.isListed(1234) );
    CPPUNIT_ASSERT( !process.isListed(3)   );
    CPPUNIT_ASSERT( !process.isListed(11)  );
    CPPUNIT_ASSERT( process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 2);

    // Remove the remaining two indices.
    process.removeSite(-123);
    process.removeSite(1234);

    // Check.
    CPPUNIT_ASSERT( !process.isListed(1234) );
    CPPUNIT_ASSERT( !process.isListed(3)    );
    CPPUNIT_ASSERT( !process.isListed(11)   );
    CPPUNIT_ASSERT( !process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // This would crash with a segfault since it is illegal.
    // process.removeSite(-123);
    // process.removeSite(1234);

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testClearSites()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Add a few indices.
    process.addSite(1234, 1.0,  1.0);
    process.addSite(3,    1.0,  5.0);
    process.addSite(11,   1.0, 11.0);
    process.addSite(-123, 1.0,  9.0);

    // Check that these indices are now there.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 4);

    // Check that the total rate is the rate times the multiplicities.
    const double rate_should_be = (1.0 + 5.0 + 11.0 + 9.0) * rate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( process.totalRate(), rate_should_be, 1.0e-10);

    // Clear.
    process.clearSites();

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( process.totalRate(), 0.0, 1.0e-10);

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testAddAndRemoveSiteMultiplicity()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.0;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Add a few indices.
    process.addSite(1234, 1.0,  1.0);
    process.addSite(3,    1.0,  5.0);
    process.addSite(11,   1.0, 11.0);
    process.addSite(-123, 1.0,  9.0);

    // Check that these indices are now there.
    CPPUNIT_ASSERT( process.isListed(1234) );
    CPPUNIT_ASSERT( process.isListed(3)    );
    CPPUNIT_ASSERT( process.isListed(11)   );
    CPPUNIT_ASSERT( process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 4);

    // Check that the total rate is the rate times the multiplicities.
    double rate_should_be = (1.0 + 5.0 + 11.0 + 9.0) * rate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( process.totalRate(), rate_should_be, 1.0e-10);

    // Remove two of the indices.
    process.removeSite(3);
    process.removeSite(11);

    // Check again.
    CPPUNIT_ASSERT( process.isListed(1234) );
    CPPUNIT_ASSERT( !process.isListed(3)   );
    CPPUNIT_ASSERT( !process.isListed(11)  );
    CPPUNIT_ASSERT( process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 2);

    // Check that the total rate is the rate times the multiplicities.
    rate_should_be = (1.0 + 9.0) * rate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( process.totalRate(), rate_should_be, 1.0e-10);

    // Remove the remaining two indices.
    process.removeSite(-123);
    process.removeSite(1234);

    // Check.
    CPPUNIT_ASSERT( !process.isListed(1234) );
    CPPUNIT_ASSERT( !process.isListed(3)    );
    CPPUNIT_ASSERT( !process.isListed(11)   );
    CPPUNIT_ASSERT( !process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Check that the total rate is the rate times the multiplicities.
    rate_should_be = (0.0) * rate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( process.totalRate(), rate_should_be, 1.0e-10);


    // This would crash with a segfault since it is illegal.
    // process.removeSite(-123);
    // process.removeSite(1234);

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testPickSite()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 1.0;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Add sites.
    process.addSite(12);
    process.addSite(199);
    process.addSite(19);
    process.updateRateTable();

    // Get the cite.
    int counter12  = 0;
    int counter19  = 0;
    int counter199 = 0;

    seedRandom(false, 97);
    const int n_loop = 1000000;

    for (int i = 0; i < n_loop; ++i)
    {
        const int site = process.pickSite();
        CPPUNIT_ASSERT( ! (site != 12 && site != 199 && site != 19) );

        // Count how often each gets selected.
        if (site == 12)
        {
            ++counter12;
        }

        if (site == 199)
        {
            ++counter199;
        }

        if (site == 19)
        {
            ++counter19;
        }
    }

    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/3.0 , 1.0 * counter12 / n_loop,  1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/3.0 , 1.0 * counter19 / n_loop,  1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/3.0 , 1.0 * counter199 / n_loop, 1.0e-2);

}


// -------------------------------------------------------------------------- //
//
void Test_Process::testPickSiteMultiplicity()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 1.0;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Add sites.
    process.addSite(12,  1.0, 3.0);
    process.addSite(199, 1.0, 5.0);
    process.addSite(19,  1.0, 7.0);
    process.updateRateTable();

    // Get the cite.
    int counter12  = 0;
    int counter19  = 0;
    int counter199 = 0;

    seedRandom(false, 97);
    const int n_loop = 1000000;

    for (int i = 0; i < n_loop; ++i)
    {
        const int site = process.pickSite();
        CPPUNIT_ASSERT( ! (site != 12 && site != 199 && site != 19) );

        // Count how often each gets selected.
        if (site == 12)
        {
            ++counter12;
        }

        if (site == 199)
        {
            ++counter199;
        }

        if (site == 19)
        {
            ++counter19;
        }
    }

    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0/15.0, 1.0 * counter12  / n_loop, 1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0/15.0, 1.0 * counter199 / n_loop, 1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.0/15.0, 1.0 * counter19  / n_loop, 1.0e-2);

}


// -------------------------------------------------------------------------- //
//
void Test_Process::testAffectedIndices()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Check that the size of the affected is correct.
    size_t one = 1;
    CPPUNIT_ASSERT_EQUAL( process.affectedIndices().size(), one );

    // Make sure we can add at some position.
    process.affectedIndices()[1] = 123;

    // And access.
    CPPUNIT_ASSERT_EQUAL( process.affectedIndices()[1], 123 );

}


// -------------------------------------------------------------------------- //
//
void Test_Process::testCutoffAndRange()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Test that the cutoff is set to the longest distance.
    const double d = std::sqrt(std::pow(1.0,2) + std::pow(1.3,2) + std::pow(-4.4,2));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( process.cutoff(), d, 1.0e-12 );

    // Test that the range is set by the largest index. (-4.4)
    CPPUNIT_ASSERT_EQUAL( process.range(), 5 );

}


// -------------------------------------------------------------------------- //
//
void Test_Process::testProcessNumber()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Setup an empty move vector.
    const std::vector<int> move_origins(0);
    const std::vector<Coordinate> move_vectors(0);

    // Give the process its number.
    const int p_number = 198;

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites, move_origins, move_vectors, p_number);

    // Check that the number that is returned is the same.
    CPPUNIT_ASSERT_EQUAL( process.processNumber(), p_number );


    // Construct another process without the process number.
    Process process2(config1, config2, rate, basis_sites, move_origins, move_vectors);

    // Check that the process number is -1 as default.
    CPPUNIT_ASSERT_EQUAL( process2.processNumber(), -1 );

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testCacheRate()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Setup an empty move vector.
    const std::vector<int> move_origins(0);
    const std::vector<Coordinate> move_vectors(0);

    // Give the process its number.
    const int p_number = 198;

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    const Process p1(config1, config2, rate, basis_sites, move_origins, move_vectors, p_number);

    // Check that the cache rate return is 'false' after constructor call.
    CPPUNIT_ASSERT( !p1.cacheRate() );

    // DONE
}
