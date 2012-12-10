/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_matcher.h"

// Include the file to test.
#include "../src/matcher.h"

#include "../src/matchlistentry.h"
#include "../src/configuration.h"
#include "../src/latticemap.h"
#include "../src/process.h"

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
    Matcher m;

    // Setup two match lists to check.
    std::vector<MatchListEntry> process_match_list;
    process_match_list.push_back(MatchListEntry(3,2,1.2,Coordinate(0.1,0.2,0.34)));
    process_match_list.push_back(MatchListEntry(1,4,1.6,Coordinate(1.1,0.2,0.34)));
    process_match_list.push_back(MatchListEntry(8,2,1.9,Coordinate(0.1,5.2,0.34)));

    std::vector<MatchListEntry> index_match_list;
    index_match_list.push_back(MatchListEntry(3,2,1.2,Coordinate(0.1,0.2,0.34)));
    index_match_list.push_back(MatchListEntry(1,4,1.6,Coordinate(1.1,0.2,0.34)));
    index_match_list.push_back(MatchListEntry(8,2,1.9,Coordinate(0.1,5.2,0.34)));

    // These two are equal.
    CPPUNIT_ASSERT( m.isMatch(process_match_list, index_match_list) );

    // It works also if the index match list is longer.
    index_match_list.push_back(MatchListEntry(8,2,4.9,Coordinate(3.1,5.2,0.34)));

    // These two are still equal.
    CPPUNIT_ASSERT( m.isMatch(process_match_list, index_match_list) );

    // If we add another not matching element to the process_match_list vecctor
    // we get a non-match.
    process_match_list.push_back(MatchListEntry(1,2,4.9,Coordinate(3.1,5.2,0.34)));

    // These two are no longer equal.
    CPPUNIT_ASSERT( !m.isMatch(process_match_list, index_match_list) );

    // But if they differ in the update index they are still equal.
    process_match_list[3] = MatchListEntry(8,1,4.9,Coordinate(3.1,5.2,0.34));

    // These two are again equal.
    CPPUNIT_ASSERT( m.isMatch(process_match_list, index_match_list) );

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
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["C"] = 2;
    possible_types["D"] = 3;
    possible_types["E"] = 4;
    possible_types["F"] = 5;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // ---------------------------------------------------------------------
    // Setup a non periodic cooresponding lattice map.

    const std::vector<int> repetitions(3, 1);
    const std::vector<bool> periodicity(3, false);
    const int basis = 2;
    LatticeMap lattice_map(basis, repetitions, periodicity);

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
        Process process(config1, config2, barrier);

        // This is a match.
        CPPUNIT_ASSERT( m.isMatch(0, lattice_map.neighbourIndices(0), process, lattice_map, config) );

        // This is not a match.
        CPPUNIT_ASSERT( !m.isMatch(1, lattice_map.neighbourIndices(1), process, lattice_map, config) );
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
        Process process(config1, config2, barrier);

        // This is a match.
        CPPUNIT_ASSERT( m.isMatch(1, lattice_map.neighbourIndices(1), process, lattice_map, config) );

        // This is not a match.
        CPPUNIT_ASSERT( !m.isMatch(0, lattice_map.neighbourIndices(0), process, lattice_map, config) );
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
        Process process(config1, config2, barrier);

        // Not a match.
        CPPUNIT_ASSERT( !m.isMatch(1, lattice_map.neighbourIndices(1), process, lattice_map, config) );
        CPPUNIT_ASSERT( !m.isMatch(0, lattice_map.neighbourIndices(0), process, lattice_map, config) );
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
        Process process(config1, config2, barrier);

        // Not a match.
        CPPUNIT_ASSERT( !m.isMatch(1, lattice_map.neighbourIndices(1), process, lattice_map, config) );
        CPPUNIT_ASSERT( !m.isMatch(0, lattice_map.neighbourIndices(0), process, lattice_map, config) );
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
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["C"] = 2;
    possible_types["D"] = 3;
    possible_types["E"] = 4;
    possible_types["F"] = 5;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // ---------------------------------------------------------------------
    // Setup a periodic cooresponding lattice map.
    const std::vector<int> repetitions(3, 3);
    const std::vector<bool> periodicity(3, true);
    const int basis_size = 2;
    LatticeMap lattice_map(basis_size, repetitions, periodicity);

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
        Process process(config1, config2, barrier);

        // This process should match all odd numbered indices.
        for (int i = 0; i < 26; i += 2)
        {
            CPPUNIT_ASSERT( m.isMatch(i, lattice_map.neighbourIndices(i), process, lattice_map, config) );

            CPPUNIT_ASSERT( !m.isMatch(i+1, lattice_map.neighbourIndices(i+1), process, lattice_map, config) );
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
        Process process(config1, config2, barrier);

        // This process should match all odd numbered indices.
        for (int i = 0; i < 26; i += 2)
        {
            CPPUNIT_ASSERT( m.isMatch(i, lattice_map.neighbourIndices(i), process, lattice_map, config) );

            CPPUNIT_ASSERT( !m.isMatch(i+1, lattice_map.neighbourIndices(i+1), process, lattice_map, config) );
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
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["C"] = 2;
    possible_types["D"] = 3;
    possible_types["E"] = 4;
    possible_types["F"] = 5;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // ---------------------------------------------------------------------
    // Setup a periodic cooresponding lattice map.
    const std::vector<int> repetitions(3, 3);
    const std::vector<bool> periodicity(3, true);
    const int basis_size = 2;
    LatticeMap lattice_map(basis_size, repetitions, periodicity);

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
        Process process(config1, config2, barrier);

        // This process should match all except the first the odd numbered indices.
        CPPUNIT_ASSERT( !m.isMatch(0, lattice_map.neighbourIndices(0), process, lattice_map, config) );
        CPPUNIT_ASSERT( !m.isMatch(1, lattice_map.neighbourIndices(1), process, lattice_map, config) );
        for (int i = 2; i < 27; i += 2)
        {
            CPPUNIT_ASSERT( m.isMatch(i, lattice_map.neighbourIndices(i), process, lattice_map, config) );
            CPPUNIT_ASSERT(!m.isMatch(i+1, lattice_map.neighbourIndices(i+1), process, lattice_map, config) );

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
        Process process(config1, config2, barrier);

        // This process should only match the first index.
        CPPUNIT_ASSERT( m.isMatch(0, lattice_map.neighbourIndices(0), process, lattice_map, config) );
        for (int i = 1; i < 27; ++i)
        {
            CPPUNIT_ASSERT(!m.isMatch(i, lattice_map.neighbourIndices(i), process, lattice_map, config) );
        }
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testCalculateMatchingProcess()
{
    // Construct.
    Matcher m;

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

    // NEEDS IMPLEMENTATION
}

