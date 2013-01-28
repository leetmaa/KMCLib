/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_matcher.h"

// Include the file to test.
#include "matcher.h"

#include "matchlistentry.h"
#include "configuration.h"
#include "latticemap.h"
#include "process.h"
#include "interactions.h"

// -------------------------------------------------------------------------- //
//
void Test_Matcher::testConstruction()
{
    // Construct.
    Matcher m;
}

// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchMatchList()
{
    // Construct.
    Matcher matcher;

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
    CPPUNIT_ASSERT( matcher.isMatch(process_match_list, index_match_list) );

    // It works also if the index match list is longer.
    m.match_type  = 8;
    m.update_type = 2;
    m.distance     = 1.93;
    m.coordinate   = Coordinate(0.13,5.2,0.34);
    index_match_list.push_back(m);

    // These two are still equal.
    CPPUNIT_ASSERT( matcher.isMatch(process_match_list, index_match_list) );

    // If we add another not matching element to the process_match_list vecctor
    // we get a non-match.
    m.match_type  = 3;
    m.update_type = 2;
    m.distance     = 1.93;
    m.coordinate   = Coordinate(0.13,5.2,0.34);
    process_match_list.push_back(m);

    // These two are no longer equal.
    CPPUNIT_ASSERT( !matcher.isMatch(process_match_list, index_match_list) );

    // But if they differ in the update index they are still equal.
    m.match_type  = 8;
    m.update_type = 3;
    m.distance     = 1.93;
    m.coordinate   = Coordinate(0.13,5.2,0.34);
    process_match_list[3] = m;

    // These two are again equal.
    CPPUNIT_ASSERT( matcher.isMatch(process_match_list, index_match_list) );
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchWildcard()
{
    // Construct.
    Matcher matcher;

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
    CPPUNIT_ASSERT( matcher.isMatch(process_match_list, index_match_list) );

    // But the wildcard is only skipped in the process match list, so swapping
    // place creates a mismatch.
    CPPUNIT_ASSERT( !matcher.isMatch(index_match_list, process_match_list) );

}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchIndexListMinimal()
{
    // Construct.
    Matcher m;

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
    std::vector<std::string> elements(2);
    elements[0] = "A";
    elements[1] = "B";

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
        std::vector<std::string> elements1;
        elements1.push_back("A");
        elements1.push_back("B");
        std::vector<std::string> elements2;
        elements2.push_back("C");
        elements2.push_back("B");
        // Setup coordinates.
        std::vector<std::vector<double> > process_coords(2,std::vector<double>(3,0.0));
        process_coords[1][0] =  0.5;
        process_coords[1][1] =  0.5;
        process_coords[1][2] =  0.5;
        // The configurations.
        const Configuration config1(process_coords, elements1, possible_types);
        const Configuration config2(process_coords, elements2, possible_types);

        // Construct the process.
        const double barrier = 13.7;
        Process process(config1, config2, barrier, basis_sites);

        // This is a match.
        CPPUNIT_ASSERT( m.isMatch(process.minimalMatchList(), config.minimalMatchList(0)) );

        // This is not a match.
        CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(1)) );
    }

    // Construct another process that should match the second index.
    {
        // Setup the two configurations.
        std::vector<std::string> elements1;
        elements1.push_back("B");
        elements1.push_back("A");
        std::vector<std::string> elements2;
        elements2.push_back("C");
        elements2.push_back("A");
        // Setup coordinates.
        std::vector<std::vector<double> > process_coords(2,std::vector<double>(3,0.0));
        process_coords[1][0] =  -0.5;
        process_coords[1][1] =  -0.5;
        process_coords[1][2] =  -0.5;
        // The configurations.
        const Configuration config1(process_coords, elements1, possible_types);
        const Configuration config2(process_coords, elements2, possible_types);

        // Construct the process.
        const double barrier = 13.7;
        Process process(config1, config2, barrier, basis_sites);

        // This is a match.
        CPPUNIT_ASSERT( m.isMatch(process.minimalMatchList(), config.minimalMatchList(1)) );

        // This is not a match.
        CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(0)) );
    }

    // This process does not match any. Note that the symmetry / direction is important.
    {
        // Setup the two configurations.
        std::vector<std::string> elements1;
        elements1.push_back("B");
        elements1.push_back("A");
        std::vector<std::string> elements2;
        elements2.push_back("C");
        elements2.push_back("A");
        // Setup coordinates.
        std::vector<std::vector<double> > process_coords(2,std::vector<double>(3,0.0));
        process_coords[1][0] =  -0.5;
        process_coords[1][1] =  -0.5;
        process_coords[1][2] =   0.5;
        // The configurations.
        const Configuration config1(process_coords, elements1, possible_types);
        const Configuration config2(process_coords, elements2, possible_types);

        // Construct the process.
        const double barrier = 13.7;
        Process process(config1, config2, barrier, basis_sites);

        // Not a match.
        CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(1)) );
        CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(0)) );
    }

    // This process does not match any site since it is to long.
    {
        // Setup the two configurations.
        std::vector<std::string> elements1;
        elements1.push_back("B");
        elements1.push_back("A");
        elements1.push_back("C");
        std::vector<std::string> elements2;
        elements2.push_back("C");
        elements2.push_back("A");
        elements2.push_back("C");
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
        const double barrier = 13.7;
        Process process(config1, config2, barrier, basis_sites);

        // Not a match.
        CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(1)) );
        CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(0)) );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchIndexListMinimalPeriodic()
{
    // Note that a minimal periodic configuration is a 3x3x3 cell.
    // The reason for this is that it simplifies boundary condition
    // treatment alot, and it is highly unlikely that a real lattice KMC
    // simulation would be meaningfull on a smaller system any way.

    // Construct.
    Matcher m;

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

    std::vector<std::string> elements;
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
                elements.push_back("A");

                coord = basis[1];
                coord[0] += i;
                coord[1] += j;
                coord[2] += k;
                coords.push_back(coord);
                elements.push_back("B");
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
        std::vector<std::string> elements1;
        elements1.push_back("A");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        std::vector<std::string> elements2;
        elements2.push_back("C");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
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
        const double barrier = 13.7;
        Process process(config1, config2, barrier, basis_sites);

        // This process should match all even numbered indices.
        for (int i = 0; i < 26; i += 2)
        {
            CPPUNIT_ASSERT(  m.isMatch(process.minimalMatchList(), config.minimalMatchList(i)) );
            CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(i+1)) );
        }
    }

    // ---------------------------------------------------------------------
    // Test an even larger local environment.
    {
        // Setup the two configurations.
        std::vector<std::string> elements1;
        elements1.push_back("A");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("A");
        elements1.push_back("A");
        elements1.push_back("A");
        elements1.push_back("A");
        elements1.push_back("A");
        elements1.push_back("A");

        std::vector<std::string> elements2;
        elements2.push_back("C");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");

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
        const double barrier = 13.7;
        Process process(config1, config2, barrier, basis_sites);

        // This process should match all even numbered indices.
        for (int i = 0; i < 26; i += 2)
        {
            CPPUNIT_ASSERT(  m.isMatch(process.minimalMatchList(), config.minimalMatchList(i)) );
            CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(i+1)) );
        }
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchIndexListComplicatedPeriodic()
{
    // Construct.
    Matcher m;

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

    std::vector<std::string> elements;
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
                elements.push_back("C");

                coord = basis[1];
                coord[0] += i;
                coord[1] += j;
                coord[2] += k;
                coords.push_back(coord);
                elements.push_back("B");
            }
        }
    }

    // Modify the elements such that the central cell has an A in center.
    elements[0] = "A";

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
        std::vector<std::string> elements1;
        elements1.push_back("C");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        std::vector<std::string> elements2;
        elements2.push_back("D");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
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
        const double barrier = 13.7;
        Process process(config1, config2, barrier, basis_sites);

        // This process should match all except the first the even numbered indices.
        CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(0)) );
        CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(1)) );

        for (int i = 2; i < 27; i += 2)
        {
            CPPUNIT_ASSERT(  m.isMatch(process.minimalMatchList(), config.minimalMatchList(i)) );
            CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(i+1)) );
        }
    }

    // ---------------------------------------------------------------------
    // Test an even larger local environment.
    {
        // Setup the two configurations.
        std::vector<std::string> elements1;
        elements1.push_back("A");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("C");
        elements1.push_back("C");
        elements1.push_back("C");
        elements1.push_back("C");
        elements1.push_back("C");
        elements1.push_back("C");

        std::vector<std::string> elements2;
        elements2.push_back("F");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");

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
        const double barrier = 13.7;
        Process process(config1, config2, barrier, basis_sites);

        // This process should only match the first index.
        CPPUNIT_ASSERT(  m.isMatch(process.minimalMatchList(), config.minimalMatchList(0)) );

        for (int i = 1; i < 27; ++i)
        {
            CPPUNIT_ASSERT( !m.isMatch(process.minimalMatchList(), config.minimalMatchList(i)) );
        }
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testCalculateMatchingProcess()
{
    // Construct.
    Matcher m;

    // ---------------------------------------------------------------------
    // 27 cells cell with two atoms in the basis.
    std::vector<std::vector<double> > basis(2, std::vector<double>(3,0.0));
    basis[1][0] = 0.5;
    basis[1][1] = 0.5;
    basis[1][2] = 0.5;
    std::vector<int> basis_sites;
    basis_sites.push_back(1);
    basis_sites.push_back(0);

    std::vector<std::string> elements;
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
                elements.push_back("C");

                coord = basis[1];
                coord[0] += i;
                coord[1] += j;
                coord[2] += k;
                coords.push_back(coord);
                elements.push_back("B");
            }
        }
    }

    // Modify the elements such that the central cell has an A in center.
    elements[0] = "A";

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

    // Calculate the match lists.
    config.initMatchLists(lattice_map, 1);

    // ---------------------------------------------------------------------
    // Setup a process that matches all even indexed sites except the first.
    std::vector<std::string> elements1;
    elements1.push_back("C");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    std::vector<std::string> elements2;
    elements2.push_back("D");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
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
    const double barrier = 13.7;
    Process process(config1, config2, barrier, basis_sites);

    // Make sure the process' available sites is empty.
    CPPUNIT_ASSERT( process.sites().empty());

    // Call the matching function.
    m.calculateMatching(process, config, 0);

    // Check that the process' available sites is still empty.
    CPPUNIT_ASSERT( process.sites().empty());

    // Call the matching function.
    m.calculateMatching(process, config, 2);

    // Check that the index was added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(process.sites().size()), 1 );
    CPPUNIT_ASSERT( process.isListed(2) );

    // Match again.
    m.calculateMatching(process, config, 2);

    // This should not have changed the availability of the index.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(process.sites().size()), 1 );
    CPPUNIT_ASSERT( process.isListed(2) );

    // Match another index.
    m.calculateMatching(process, config, 4);

    // This should not have changed the availability of the index.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(process.sites().size()), 2 );
    CPPUNIT_ASSERT( process.isListed(2) );
    CPPUNIT_ASSERT( process.isListed(4) );

    // Change the configuration and match.
    elements[2] = "D";
    config = Configuration(coords, elements, possible_types);
    config.initMatchLists(lattice_map, 1);

    // Match.
    m.calculateMatching(process, config, 2);

    // Now index 2 should be removed but 4 still there.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(process.sites().size()), 1 );
    CPPUNIT_ASSERT( !process.isListed(2) );
    CPPUNIT_ASSERT(  process.isListed(4) );

    // Match again - this should not change any thing.
    m.calculateMatching(process, config, 2);
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(process.sites().size()), 1 );
    CPPUNIT_ASSERT( !process.isListed(2) );
    CPPUNIT_ASSERT(  process.isListed(4) );

    // Match against a changed configuration. This removes index 4.
    elements[4] = "D";
    config = Configuration(coords, elements, possible_types);
    config.initMatchLists(lattice_map, 1);
    m.calculateMatching(process, config, 4);
    CPPUNIT_ASSERT( process.sites().empty() );
    CPPUNIT_ASSERT( !process.isListed(2) );
    CPPUNIT_ASSERT( !process.isListed(4) );



    /*
    for( ; it1 != process_match_list.end(); ++it1, ++it2)
    {
        printf("              process  config\n");
        printf("MatchType   : %i %i\n",(*it1).matchType(), (*it2).matchType());
        printf("Distance    : %20.10e %20.10e\n", (*it1).distance(), (*it2).distance());

        printf("Coordinate x: %20.10e %20.10e\n", (*it1).coordinate().x(), (*it2).coordinate().x());
        printf("Coordinate y: %20.10e %20.10e\n", (*it1).coordinate().y(), (*it2).coordinate().y());
        printf("Coordinate z: %20.10e %20.10e\n", (*it1).coordinate().z(), (*it2).coordinate().z());

    }
    //*/

    // NEEDS IMPLEMENTATION
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testCalculateMatchingInteractions()
{
    // Construct.
    Matcher m;

    // Setup the processes to test with.
    std::vector<Process> processes;

    // ---------------------------------------------------------------------
    // 27 cells cell with two atoms in the basis.
    std::vector<std::vector<double> > basis(2, std::vector<double>(3,0.0));
    basis[1][0] = 0.5;
    basis[1][1] = 0.5;
    basis[1][2] = 0.5;
    std::vector<int> basis_sites;
    basis_sites.push_back(1);
    basis_sites.push_back(0);

    std::vector<std::string> elements;
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
                elements.push_back("C");

                coord = basis[1];
                coord[0] += i;
                coord[1] += j;
                coord[2] += k;
                coords.push_back(coord);
                elements.push_back("B");
            }
        }
    }

    // Modify the elements such that the central cell has an A in center.
    elements[0] = "A";

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

    // Calculate the match lists.
    config.initMatchLists(lattice_map, 1);

    // ---------------------------------------------------------------------
    // Now the processes.

    {
        // Setup the two configurations.
        std::vector<std::string> elements1;
        elements1.push_back("C");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        std::vector<std::string> elements2;
        elements2.push_back("D");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
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
        const double barrier = 13.7;
        const Process process(config1, config2, barrier, basis_sites);
        processes.push_back(process);

    }

    {
        // Setup the two configurations.
        std::vector<std::string> elements1;
        elements1.push_back("A");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("B");
        elements1.push_back("C");
        elements1.push_back("C");
        elements1.push_back("C");
        elements1.push_back("C");
        elements1.push_back("C");
        elements1.push_back("C");

        std::vector<std::string> elements2;
        elements2.push_back("F");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("B");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");
        elements2.push_back("A");

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
        const double barrier = 13.7;
        const Process process(config1, config2, barrier, basis_sites);
        processes.push_back(process);
    }

    // ---------------------------------------------------------------------

    // Setup the interactions object.
    Interactions interactions(processes, true);

    // Setup a list of indices to test. Their order should not matter.
    std::vector<int> indices;
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(4);
    indices.push_back(3);
    indices.push_back(14);
    indices.push_back(9);

    // Call the matching function and check that all processes have been
    // updated correctly.
    m.calculateMatching(interactions, config, lattice_map, indices);

    // The first process should match all even indices except the first.
    CPPUNIT_ASSERT( !interactions.processes()[0].isListed(0) );
    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[0].isListed(3) );
    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[0].isListed(9) );
    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(14) );

    // The second process should match the first index only.
    CPPUNIT_ASSERT(  interactions.processes()[1].isListed(0) );
    CPPUNIT_ASSERT( !interactions.processes()[1].isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[1].isListed(3) );
    CPPUNIT_ASSERT( !interactions.processes()[1].isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[1].isListed(5) );

    // Call the matching function again does not change any thing.
    m.calculateMatching(interactions, config, lattice_map, indices);

    CPPUNIT_ASSERT( !interactions.processes()[0].isListed(0) );
    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[0].isListed(3) );
    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[0].isListed(9) );
    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(14) );
    CPPUNIT_ASSERT(  interactions.processes()[1].isListed(0) );
    CPPUNIT_ASSERT( !interactions.processes()[1].isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[1].isListed(3) );
    CPPUNIT_ASSERT( !interactions.processes()[1].isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[1].isListed(5) );

    // Change the configuration such that the first proces
    // match with the first (0:th) index, and the second process
    // does not match any.
    elements[0] = "C";
    config = Configuration(coords, elements, possible_types);
    config.initMatchLists(lattice_map, 1);
    m.calculateMatching(interactions, config, lattice_map, indices);

    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(0) );
    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[0].isListed(3) );
    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[0].isListed(9) );
    CPPUNIT_ASSERT(  interactions.processes()[0].isListed(14) );
    CPPUNIT_ASSERT( interactions.processes()[1].sites().empty() );

}

