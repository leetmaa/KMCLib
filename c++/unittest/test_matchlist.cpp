/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_matchlist.h"

// Include the files to test.
#include "matchlist.h"
#include "process.h"
#include "configuration.h"
#include "latticemap.h"

// -------------------------------------------------------------------------- //
//
void Test_MatchList::testCall()
{
    // Construct.
    ProcessBucketMatchList m1(1);
    m1[0].match_types = TypeBucket(3);
    m1[0].match_types[2] = 2;
    m1[0].distance = 1.2;
    m1[0].coordinate = Coordinate(0.1, 0.2, 0.3);
    m1[0].index = 0;

    ConfigBucketMatchList m2(1);
    m2[0].match_types = TypeBucket(3);
    m2[0].match_types[2] = 2;
    m2[0].distance = 1.2;
    m2[0].x = 0.1;
    m2[0].y = 0.2;
    m2[0].z = 0.3;
    m2[0].index = 0;

    whateverMatch(m1, m2);
}


// -------------------------------------------------------------------------- //
//
void Test_MatchList::testIsMatchMatchList()
{
    // NEEDS IMPLEMENTATION

    /*
    // Setup two match lists to check.
    std::vector<MinimalMatchListEntry> process_match_list;
    MinimalMatchListEntry m;
    m.match_type  = 3;
    m.update_type = 2;
    m.distance     = 1.2;
    m.coordinate   = Coordinate(0.1,2.0,0.34);
    process_match_list.push_back(m);

    m.match_type  = 1;
    m.update_type = 4;
    m.distance     = 1.6;
    m.coordinate   = Coordinate(1.1,2.0,0.34);
    process_match_list.push_back(m);

    m.match_type  = 8;
    m.update_type = 2;
    m.distance     = 1.9;
    m.coordinate   = Coordinate(0.1,5.2,0.34);
    process_match_list.push_back(m);

    std::vector<MinimalMatchListEntry> index_match_list;
    m.match_type  = 3;
    m.update_type = 2;
    m.distance     = 1.2;
    m.coordinate   = Coordinate(0.1,2.0,0.34);
    index_match_list.push_back(m);

    m.match_type  = 1;
    m.update_type = 4;
    m.distance     = 1.6;
    m.coordinate   = Coordinate(1.1,2.0,0.34);
    index_match_list.push_back(m);

    m.match_type  = 8;
    m.update_type = 2;
    m.distance     = 1.9;
    m.coordinate   = Coordinate(0.1,5.2,0.34);
    index_match_list.push_back(m);

    // These two are equal.
    CPPUNIT_ASSERT( whateverMatch(process_match_list, index_match_list) );

    // It works also if the index match list is longer.
    m.match_type  = 8;
    m.update_type = 2;
    m.distance     = 1.93;
    m.coordinate   = Coordinate(0.13,5.2,0.34);
    index_match_list.push_back(m);

    // These two are still equal.
    CPPUNIT_ASSERT( whateverMatch(process_match_list, index_match_list) );

    // If we add another not matching element to the process_match_list vecctor
    // we get a non-match.
    m.match_type  = 3;
    m.update_type = 2;
    m.distance     = 1.93;
    m.coordinate   = Coordinate(0.13,5.2,0.34);
    process_match_list.push_back(m);

    // These two are no longer equal.
    CPPUNIT_ASSERT( !whateverMatch(process_match_list, index_match_list) );

    // But if they differ in the update index they are still equal.
    m.match_type  = 8;
    m.update_type = 3;
    m.distance     = 1.93;
    m.coordinate   = Coordinate(0.13,5.2,0.34);
    process_match_list[3] = m;

    // These two are again equal.
    CPPUNIT_ASSERT( whateverMatch(process_match_list, index_match_list) );
    */
}


// -------------------------------------------------------------------------- //
//
void Test_MatchList::testIsMatchWildcard()
{
    // NEEDS IMPLEMENTATION

    /*

    // Setup two match lists to check.
    std::vector<MinimalMatchListEntry> process_match_list;
    MinimalMatchListEntry m;
    m.match_type  = 3;
    m.update_type = 2;
    m.distance     = 1.2;
    m.coordinate   = Coordinate(0.1,2.0,0.34);
    process_match_list.push_back(m);

    // Add a wildcard, 0.
    m.match_type  = 0;
    m.update_type = 4;
    m.distance     = 1.6;
    m.coordinate   = Coordinate(1.1,2.0,0.34);
    process_match_list.push_back(m);

    m.match_type  = 8;
    m.update_type = 2;
    m.distance     = 1.9;
    m.coordinate   = Coordinate(0.1,5.2,0.34);
    process_match_list.push_back(m);

    std::vector<MinimalMatchListEntry> index_match_list;
    m.match_type  = 3;
    m.update_type = 2;
    m.distance     = 1.2;
    m.coordinate   = Coordinate(0.1,2.0,0.34);
    index_match_list.push_back(m);

    // Note that no wildcard here.
    m.match_type  = 1;
    m.update_type = 4;
    m.distance     = 1.6;
    m.coordinate   = Coordinate(1.1,2.0,0.34);
    index_match_list.push_back(m);

    m.match_type  = 8;
    m.update_type = 2;
    m.distance     = 1.9;
    m.coordinate   = Coordinate(0.1,5.2,0.34);
    index_match_list.push_back(m);

    // These two are equal.
    CPPUNIT_ASSERT( whateverMatch(process_match_list, index_match_list) );

    // But the wildcard is only skipped in the process match list, so swapping
    // place creates a mismatch.
    CPPUNIT_ASSERT( !whateverMatch(index_match_list, process_match_list) );
    */
}


// -------------------------------------------------------------------------- //
//
void Test_MatchList::testIsMatchIndexListMinimal()
{
    // ---------------------------------------------------------------------
    // Setup a valid configuration.
    std::vector<std::vector<double> > coords(2, std::vector<double>(3, 0.0));

    // One cell with two atoms.
    coords[0][0] = 0.0;
    coords[0][1] = 0.0;
    coords[0][2] = 0.0;
    coords[1][0] = 0.5;
    coords[1][1] = 0.5;
    coords[1][2] = 0.5;

    // Setup elements.
    std::vector<std::vector<std::string> > elements(2);
    elements[0] = std::vector<std::string>(1, "A");
    elements[1] = std::vector<std::string>(1, "B");

    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 3;
    possible_types["D"] = 4;
    possible_types["E"] = 5;
    possible_types["F"] = 6;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // ---------------------------------------------------------------------
    // Setup a non periodic cooresponding lattice map.

    const std::vector<int> repetitions(3, 1);
    const std::vector<bool> periodicity(3, false);
    const int basis = 2;
    std::vector<int> basis_sites;
    basis_sites.push_back(1);
    basis_sites.push_back(0);

    LatticeMap lattice_map(basis, repetitions, periodicity);
    config.initMatchLists(lattice_map, 1);

    // ---------------------------------------------------------------------
    // Construct a processes and test.
    {
        // Setup the two configurations.
        std::vector<std::vector<std::string> > elements1;
        elements1.push_back(std::vector<std::string>(1, "A"));
        elements1.push_back(std::vector<std::string>(1, "B"));
        std::vector<std::vector<std::string> > elements2;
        elements2.push_back(std::vector<std::string>(1, "C"));
        elements2.push_back(std::vector<std::string>(1, "B"));
        // Setup coordinates.
        std::vector<std::vector<double> > process_coords(2,std::vector<double>(3,0.0));
        process_coords[1][0] =  0.5;
        process_coords[1][1] =  0.5;
        process_coords[1][2] =  0.5;
        // The configurations.
        const Configuration config1(process_coords, elements1, possible_types);
        const Configuration config2(process_coords, elements2, possible_types);

        // Construct the process.
        const double rate = 13.7;
        Process process(config1, config2, rate, basis_sites);

        // This is a match.
        CPPUNIT_ASSERT( whateverMatch(process.processMatchList(), config.configMatchList(0)) );

        // This is not a match.
        CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(1)) );
    }

    // Construct another process that should match the second index.
    {
        // Setup the two configurations.
        std::vector<std::vector<std::string> > elements1;
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "A"));
        std::vector<std::vector<std::string> > elements2;
        elements2.push_back(std::vector<std::string>(1, "C"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        // Setup coordinates.
        std::vector<std::vector<double> > process_coords(2,std::vector<double>(3,0.0));
        process_coords[1][0] =  -0.5;
        process_coords[1][1] =  -0.5;
        process_coords[1][2] =  -0.5;
        // The configurations.
        const Configuration config1(process_coords, elements1, possible_types);
        const Configuration config2(process_coords, elements2, possible_types);

        // Construct the process.
        const double rate = 13.7;
        Process process(config1, config2, rate, basis_sites);

        // This is a match.
        CPPUNIT_ASSERT( whateverMatch(process.processMatchList(), config.configMatchList(1)) );

        // This is not a match.
        CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(0)) );
    }

    // This process does not match any. Note that the symmetry / direction is important.
    {
        // Setup the two configurations.
        std::vector<std::vector<std::string> > elements1;
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "A"));
        std::vector<std::vector<std::string> > elements2;
        elements2.push_back(std::vector<std::string>(1, "C"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        // Setup coordinates.
        std::vector<std::vector<double> > process_coords(2,std::vector<double>(3,0.0));
        process_coords[1][0] =  -0.5;
        process_coords[1][1] =  -0.5;
        process_coords[1][2] =   0.5;
        // The configurations.
        const Configuration config1(process_coords, elements1, possible_types);
        const Configuration config2(process_coords, elements2, possible_types);

        // Construct the process.
        const double rate = 13.7;
        Process process(config1, config2, rate, basis_sites);

        // Not a match.
        CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(1)) );
        CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(0)) );
    }

    // This process does not match any site since it is to long.
    {
        // Setup the two configurations.
        std::vector<std::vector<std::string> > elements1;
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "A"));
        elements1.push_back(std::vector<std::string>(1, "C"));
        std::vector<std::vector<std::string> > elements2;
        elements2.push_back(std::vector<std::string>(1, "C"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "C"));
        // Setup coordinates.
        std::vector<std::vector<double> > process_coords(3,std::vector<double>(3,0.0));
        process_coords[1][0] =  -0.5;
        process_coords[1][1] =  -0.5;
        process_coords[1][2] =  -0.5;
        process_coords[2][0] =   0.5;
        process_coords[2][1] =   0.5;
        process_coords[2][2] =   0.5;
        // The configurations.
        const Configuration config1(process_coords, elements1, possible_types);
        const Configuration config2(process_coords, elements2, possible_types);

        // Construct the process.
        const double rate = 13.7;
        Process process(config1, config2, rate, basis_sites);

        // Not a match.
        CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(1)) );
        CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(0)) );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MatchList::testIsMatchIndexListMinimalPeriodic()
{
    // Note that a minimal periodic configuration is a 3x3x3 cell.
    // The reason for this is that it simplifies boundary condition
    // treatment alot, and it is highly unlikely that a real lattice KMC
    // simulation would be meaningfull on a smaller system any way.

    // ---------------------------------------------------------------------
    // Setup a valid configuration.

    // 27 cells cell with two atoms in the basis.
    std::vector<std::vector<double> > basis(2, std::vector<double>(3,0.0));
    basis[1][0] = 0.5;
    basis[1][1] = 0.5;
    basis[1][2] = 0.5;

    std::vector<int> basis_sites;
    basis_sites.push_back(1);
    basis_sites.push_back(0);

    std::vector<std::vector<std::string> > elements;
    std::vector<std::vector<double> > coords;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                std::vector<double> coord = basis[0];
                coord[0] += i;
                coord[1] += j;
                coord[2] += k;
                coords.push_back(coord);
                elements.push_back(std::vector<std::string>(1, "A"));

                coord = basis[1];
                coord[0] += i;
                coord[1] += j;
                coord[2] += k;
                coords.push_back(coord);
                elements.push_back(std::vector<std::string>(1, "B"));
            }
        }
    }

    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 3;
    possible_types["D"] = 4;
    possible_types["E"] = 5;
    possible_types["F"] = 6;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // ---------------------------------------------------------------------
    // Setup a periodic cooresponding lattice map.
    const std::vector<int> repetitions(3, 3);
    const std::vector<bool> periodicity(3, true);
    const int basis_size = 2;

    LatticeMap lattice_map(basis_size, repetitions, periodicity);
    config.initMatchLists(lattice_map, 1);

    // ---------------------------------------------------------------------
    // Construct a processes and test.
    {
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

        // Construct the process.
        const double rate = 13.7;
        Process process(config1, config2, rate, basis_sites);

        // This process should match all even numbered indices.
        for (int i = 0; i < 26; i += 2)
        {
            CPPUNIT_ASSERT(  whateverMatch(process.processMatchList(), config.configMatchList(i)) );
            CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(i+1)) );
        }
    }

    // ---------------------------------------------------------------------
    // Test an even larger local environment.
    {
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
        elements1.push_back(std::vector<std::string>(1, "A"));
        elements1.push_back(std::vector<std::string>(1, "A"));
        elements1.push_back(std::vector<std::string>(1, "A"));
        elements1.push_back(std::vector<std::string>(1, "A"));
        elements1.push_back(std::vector<std::string>(1, "A"));
        elements1.push_back(std::vector<std::string>(1, "A"));

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
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));

        // Setup coordinates.
        std::vector<std::vector<double> > process_coords(15,std::vector<double>(3,0.0));
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

        process_coords[9][0] =  1.0;
        process_coords[9][1] =  0.0;
        process_coords[9][2] =  0.0;

        process_coords[10][0] =  0.0;
        process_coords[10][1] =  1.0;
        process_coords[10][2] =  0.0;

        process_coords[11][0] =  0.0;
        process_coords[11][1] =  0.0;
        process_coords[11][2] =  1.0;

        process_coords[12][0] = -1.0;
        process_coords[12][1] =  0.0;
        process_coords[12][2] =  0.0;

        process_coords[13][0] =  0.0;
        process_coords[13][1] = -1.0;
        process_coords[13][2] =  0.0;

        process_coords[14][0] =  0.0;
        process_coords[14][1] =  0.0;
        process_coords[14][2] = -1.0;


        // The configurations.
        const Configuration config1(process_coords, elements1, possible_types);
        const Configuration config2(process_coords, elements2, possible_types);

        // Construct the process.
        const double rate = 13.7;
        Process process(config1, config2, rate, basis_sites);

        // This process should match all even numbered indices.
        for (int i = 0; i < 26; i += 2)
        {
            CPPUNIT_ASSERT(  whateverMatch(process.processMatchList(), config.configMatchList(i)) );
            CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(i+1)) );
        }
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MatchList::testIsMatchIndexListComplicatedPeriodic()
{
    // ---------------------------------------------------------------------
    // Setup a valid configuration.

    // 27 cells cell with two atoms in the basis.
    std::vector<std::vector<double> > basis(2, std::vector<double>(3,0.0));
    basis[1][0] = 0.5;
    basis[1][1] = 0.5;
    basis[1][2] = 0.5;
    std::vector<int> basis_sites;
    basis_sites.push_back(1);
    basis_sites.push_back(0);

    std::vector<std::vector<std::string> > elements;
    std::vector<std::vector<double> > coords;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                std::vector<double> coord = basis[0];
                coord[0] += i;
                coord[1] += j;
                coord[2] += k;
                coords.push_back(coord);
                elements.push_back(std::vector<std::string>(1, "C"));

                coord = basis[1];
                coord[0] += i;
                coord[1] += j;
                coord[2] += k;
                coords.push_back(coord);
                elements.push_back(std::vector<std::string>(1, "B"));
            }
        }
    }

    // Modify the elements such that the central cell has an A in center.
    elements[0] = std::vector<std::string>(1, "A");

    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 3;
    possible_types["D"] = 4;
    possible_types["E"] = 5;
    possible_types["F"] = 6;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // ---------------------------------------------------------------------
    // Setup a periodic cooresponding lattice map.
    const std::vector<int> repetitions(3, 3);
    const std::vector<bool> periodicity(3, true);
    const int basis_size = 2;

    LatticeMap lattice_map(basis_size, repetitions, periodicity);
    config.initMatchLists(lattice_map, 1);

    // ---------------------------------------------------------------------
    // Construct a processes and test.
    {
        // Setup the two configurations.
        std::vector<std::vector<std::string> > elements1;
        elements1.push_back(std::vector<std::string>(1, "C"));
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "B"));
        elements1.push_back(std::vector<std::string>(1, "B"));
        std::vector<std::vector<std::string> > elements2;
        elements2.push_back(std::vector<std::string>(1, "D"));
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

        // Construct the process.
        const double rate = 13.7;
        Process process(config1, config2, rate, basis_sites);

        // This process should match all except the first the even numbered indices.
        CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(0)) );
        CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(1)) );

        for (int i = 2; i < 27; i += 2)
        {
            CPPUNIT_ASSERT(  whateverMatch(process.processMatchList(), config.configMatchList(i)) );
            CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(i+1)) );
        }
    }

    // ---------------------------------------------------------------------
    // Test an even larger local environment.
    {
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
        elements1.push_back(std::vector<std::string>(1, "C"));
        elements1.push_back(std::vector<std::string>(1, "C"));
        elements1.push_back(std::vector<std::string>(1, "C"));
        elements1.push_back(std::vector<std::string>(1, "C"));
        elements1.push_back(std::vector<std::string>(1, "C"));
        elements1.push_back(std::vector<std::string>(1, "C"));

        std::vector<std::vector<std::string> > elements2;
        elements2.push_back(std::vector<std::string>(1, "F"));
        elements2.push_back(std::vector<std::string>(1, "B"));
        elements2.push_back(std::vector<std::string>(1, "B"));
        elements2.push_back(std::vector<std::string>(1, "B"));
        elements2.push_back(std::vector<std::string>(1, "B"));
        elements2.push_back(std::vector<std::string>(1, "B"));
        elements2.push_back(std::vector<std::string>(1, "B"));
        elements2.push_back(std::vector<std::string>(1, "B"));
        elements2.push_back(std::vector<std::string>(1, "B"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));
        elements2.push_back(std::vector<std::string>(1, "A"));

        // Setup coordinates.
        std::vector<std::vector<double> > process_coords(15,std::vector<double>(3,0.0));
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

        process_coords[9][0] =  1.0;
        process_coords[9][1] =  0.0;
        process_coords[9][2] =  0.0;

        process_coords[10][0] =  0.0;
        process_coords[10][1] =  1.0;
        process_coords[10][2] =  0.0;

        process_coords[11][0] =  0.0;
        process_coords[11][1] =  0.0;
        process_coords[11][2] =  1.0;

        process_coords[12][0] = -1.0;
        process_coords[12][1] =  0.0;
        process_coords[12][2] =  0.0;

        process_coords[13][0] =  0.0;
        process_coords[13][1] = -1.0;
        process_coords[13][2] =  0.0;

        process_coords[14][0] =  0.0;
        process_coords[14][1] =  0.0;
        process_coords[14][2] = -1.0;


        // The configurations.
        const Configuration config1(process_coords, elements1, possible_types);
        const Configuration config2(process_coords, elements2, possible_types);

        // Construct the process.
        const double rate = 13.7;
        Process process(config1, config2, rate, basis_sites);

        // This process should only match the first index.
        CPPUNIT_ASSERT(  whateverMatch(process.processMatchList(), config.configMatchList(0)) );

        for (int i = 1; i < 27; ++i)
        {
            CPPUNIT_ASSERT( !whateverMatch(process.processMatchList(), config.configMatchList(i)) );
        }
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MatchList::testConfigurationsToMatchList()
{
    // Set up two configurations.
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
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "F"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "A"));
    elements2.push_back(std::vector<std::string>(1, "A"));
    elements2.push_back(std::vector<std::string>(1, "A"));
    elements2.push_back(std::vector<std::string>(1, "A"));
    elements2.push_back(std::vector<std::string>(1, "A"));
    elements2.push_back(std::vector<std::string>(1, "A"));

    // Setup coordinates.
    std::vector<std::vector<double> > process_coords(15,std::vector<double>(3,0.0));
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

    process_coords[9][0] =  1.0;
    process_coords[9][1] =  0.0;
    process_coords[9][2] =  0.0;

    process_coords[10][0] =  0.0;
    process_coords[10][1] =  1.0;
    process_coords[10][2] =  0.0;

    process_coords[11][0] =  0.0;
    process_coords[11][1] =  0.0;
    process_coords[11][2] =  1.0;

    process_coords[12][0] = -1.0;
    process_coords[12][1] =  0.0;
    process_coords[12][2] =  0.0;

    process_coords[13][0] =  0.0;
    process_coords[13][1] = -1.0;
    process_coords[13][2] =  0.0;

    process_coords[14][0] =  0.0;
    process_coords[14][1] =  0.0;
    process_coords[14][2] = -1.0;

    // Mapping form string to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 3;
    possible_types["D"] = 4;
    possible_types["E"] = 5;
    possible_types["F"] = 6;

    // The configurations.
    const Configuration first(process_coords, elements1, possible_types);
    const Configuration second(process_coords, elements2, possible_types);

    // This is output from the config to matchlist generation routine.
    int range = 0;
    double cutoff = 0.0;
    ProcessBucketMatchList match_list;
    std::vector<int> affected_indices;

    // Generate the match list.
    configurationsToMatchList(first,
                              second,
                              range,
                              cutoff,
                              match_list,
                              affected_indices);

    // Check that the match list correct.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(match_list.size()), 15 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(affected_indices.size()), 7 );

    // Check the size of the match types and update types.
    for (size_t i = 0; i < match_list.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(match_list[i].match_types.size()),
                             static_cast<int>(possible_types.size()));

        CPPUNIT_ASSERT_EQUAL(static_cast<int>(match_list[i].update_types.size()),
                             static_cast<int>(possible_types.size()));
    }

    // Check the content of the match types.
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[1], 1 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[3], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[6], 0 );

    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[1], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[2], 1 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[3], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[6], 0 );

    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[1], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[3], 1 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[6], 0 );

    // Check the content of the update types.
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[1],-1 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[3], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[6], 1 );

    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[1], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[3], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[6], 0 );

    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[1], 1 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[3],-1 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[6], 0 );

    // Check the cutoff and range.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( cutoff, 1.0, 1.0e-10);
    CPPUNIT_ASSERT_EQUAL( range, 1);

    // DONE.

}

// -------------------------------------------------------------------------- //
//
void Test_MatchList::testConfigurationsToMatchList2()
{
    // Set up two configurations.
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
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "C"));

    std::vector<std::vector<std::string> > elements2 = elements1;

    // Setup coordinates.
    std::vector<std::vector<double> > process_coords(15,std::vector<double>(3,0.0));
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

    process_coords[9][0] =  1.0;
    process_coords[9][1] =  0.0;
    process_coords[9][2] =  0.0;

    process_coords[10][0] =  0.0;
    process_coords[10][1] =  1.0;
    process_coords[10][2] =  0.0;

    process_coords[11][0] =  0.0;
    process_coords[11][1] =  0.0;
    process_coords[11][2] =  1.0;

    process_coords[12][0] = -1.0;
    process_coords[12][1] =  0.0;
    process_coords[12][2] =  0.0;

    process_coords[13][0] =  0.0;
    process_coords[13][1] = -1.0;
    process_coords[13][2] =  0.0;

    process_coords[14][0] =  0.0;
    process_coords[14][1] =  0.0;
    process_coords[14][2] = -1.0;

    // Mapping form string to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 3;
    possible_types["D"] = 4;
    possible_types["E"] = 5;
    possible_types["F"] = 6;

    // The configurations.
    const Configuration first(process_coords, elements1, possible_types);
    Configuration second(process_coords, elements2, possible_types);


    // The update info on the second process.
    std::vector< std::map<std::string, int> > update_info(15);
    update_info[0]["F"]  =  1;
    update_info[0]["A"]  = -1;
    update_info[1]["*"]  =  0;
    update_info[2]["*"]  =  0;
    update_info[3]["*"]  =  0;
    update_info[4]["*"]  =  0;
    update_info[5]["*"]  =  0;
    update_info[6]["*"]  =  0;
    update_info[7]["*"]  =  0;
    update_info[8]["*"]  =  0;
    update_info[9]["A"]  =  1;
    update_info[9]["C"]  = -1;
    update_info[10]["A"] =  1;
    update_info[10]["C"] = -1;
    update_info[11]["A"] =  1;
    update_info[11]["C"] = -1;
    update_info[12]["A"] =  1;
    update_info[12]["C"] = -1;
    update_info[13]["A"] =  1;
    update_info[13]["C"] = -1;
    update_info[14]["A"] =  1;
    update_info[14]["C"] = -1;

    second.setUpdateInfo(update_info);

    // This is output from the config to matchlist generation routine.
    int range = 0;
    double cutoff = 0.0;
    ProcessBucketMatchList match_list;
    std::vector<int> affected_indices;

    // Generate the match list.
    configurationsToMatchList(first,
                              second,
                              range,
                              cutoff,
                              match_list,
                              affected_indices);

    // Check that the match list correct.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(match_list.size()), 15 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(affected_indices.size()), 7 );

    // Check the size of the match types and update types.
    for (size_t i = 0; i < match_list.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(match_list[i].match_types.size()),
                             static_cast<int>(possible_types.size()));

        CPPUNIT_ASSERT_EQUAL(static_cast<int>(match_list[i].update_types.size()),
                             static_cast<int>(possible_types.size()));
    }

    // Check the content of the match types.
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[1], 1 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[3], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].match_types[6], 0 );

    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[1], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[2], 1 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[3], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].match_types[6], 0 );

    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[1], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[3], 1 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].match_types[6], 0 );

    // Check the content of the update types.
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[1],-1 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[3], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[0].update_types[6], 1 );

    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[1], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[3], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[1].update_types[6], 0 );

    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[0], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[1], 1 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[2], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[3],-1 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[4], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[5], 0 );
    CPPUNIT_ASSERT_EQUAL( match_list[9].update_types[6], 0 );

    // Check the cutoff and range.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( cutoff, 1.0, 1.0e-10);
    CPPUNIT_ASSERT_EQUAL( range, 1);

    // DONE.

}


// -------------------------------------------------------------------------- //
//
void Test_MatchList::testMultiplicity()
{
    // Two match lists.
    ProcessBucketMatchList process_match_list(3);
    ConfigBucketMatchList  config_match_list(3);

    process_match_list[0].match_types = TypeBucket(3);
    process_match_list[0].match_types[0] = 0;
    process_match_list[0].match_types[1] = 2;
    process_match_list[0].match_types[2] = 1;

    process_match_list[1].match_types = TypeBucket(3);
    process_match_list[1].match_types[0] = 1;
    process_match_list[1].match_types[1] = 0;
    process_match_list[1].match_types[2] = 4;

    process_match_list[2].match_types = TypeBucket(3);
    process_match_list[2].match_types[0] = 0;
    process_match_list[2].match_types[1] = 2;
    process_match_list[2].match_types[2] = 0;

    config_match_list[0].match_types = TypeBucket(3);
    config_match_list[0].match_types[0] = 0;
    config_match_list[0].match_types[1] = 2;
    config_match_list[0].match_types[2] = 1;

    config_match_list[1].match_types = TypeBucket(3);
    config_match_list[1].match_types[0] = 0;
    config_match_list[1].match_types[1] = 5;
    config_match_list[1].match_types[2] = 4;

    config_match_list[2].match_types = TypeBucket(3);
    config_match_list[2].match_types[0] = 0;
    config_match_list[2].match_types[1] = 2;
    config_match_list[2].match_types[2] = 0;

    // Their multiplicity should be 1.0.
    const double m0 = multiplicity(process_match_list, config_match_list);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m0, 1.0, 1.0e-10);

    // Generate two other lists.
    config_match_list[0].match_types[0] = 0;
    config_match_list[0].match_types[1] = 3;
    config_match_list[0].match_types[2] = 4;

    config_match_list[1].match_types[0] = 0;
    config_match_list[1].match_types[1] = 5;
    config_match_list[1].match_types[2] = 4;

    config_match_list[2].match_types[0] = 0;
    config_match_list[2].match_types[1] = 7;
    config_match_list[2].match_types[2] = 2;

    // Their multiplicity should be 252.
    const double m1 = multiplicity(process_match_list, config_match_list);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m1, 252.0, 1.0e-10);

}


// -------------------------------------------------------------------------- //
//
void Test_MatchList::testMultiplicity2()
{
    // A case with larger numbers.
    ProcessBucketMatchList process_match_list(1);
    ConfigBucketMatchList  config_match_list(1);

    process_match_list[0].match_types = TypeBucket(3);
    process_match_list[0].match_types[0] = 0;
    process_match_list[0].match_types[1] = 15;
    process_match_list[0].match_types[2] = 35;

    config_match_list[0].match_types = TypeBucket(3);
    config_match_list[0].match_types[0] = 0;
    config_match_list[0].match_types[1] = 20;
    config_match_list[0].match_types[2] = 36;

    // Check the multiplicity.
    const double m0 = multiplicity(process_match_list, config_match_list);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m0, 558144.0, 1.0e-10);
}
