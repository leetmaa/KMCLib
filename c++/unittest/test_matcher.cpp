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
#include "random.h"





// -------------------------------------------------------------------------- //
//
void Test_Matcher::testConstruction()
{
    // Construct.
    Matcher m(1,2);
}

// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchMatchList()
{
    // Construct.
    Matcher matcher(1000, 10);

    // Setup two match lists to check.
    ProcessBucketMatchList process_match_list;
    ProcessBucketMatchListEntry m;

    const int n_types = 9;

    m.match_types  = TypeBucket(n_types);
    m.update_types = TypeBucket(n_types);

    m.match_types[3]  = 1;
    m.update_types[2] = 1;

    m.distance     = 1.2;
    m.coordinate   = Coordinate(0.1,2.0,0.34);
    process_match_list.push_back(m);

    // Reset.
    m.match_types[3]  = 0;
    m.update_types[2] = 0;

    // Add new values.
    m.match_types[1]  = 1;
    m.update_types[4] = 1;

    m.distance     = 1.6;
    m.coordinate   = Coordinate(1.1,2.0,0.34);
    process_match_list.push_back(m);

    // Reset.
    m.match_types[1]  = 0;
    m.update_types[4] = 0;

    // Add new values.
    m.match_types[8]  = 1;
    m.update_types[2] = 1;

    m.distance     = 1.9;
    m.coordinate   = Coordinate(0.1,5.2,0.34);
    process_match_list.push_back(m);

    // Reset.
    m.match_types[8]  = 0;
    m.update_types[2] = 0;

    ConfigBucketMatchList index_match_list;
    ConfigBucketMatchListEntry n;

    n.match_types = TypeBucket(n_types);
    n.match_types[3] = 1;
    n.distance = 1.2;
    n.x = 0.1;
    n.y = 2.0;
    n.z = 0.34;
    index_match_list.push_back(n);

    // Reset.
    n.match_types[3] = 0;

    // Add new value.
    n.match_types[1] = 1;
    n.distance = 1.6;
    n.x = 1.1;
    n.y = 2.0;
    n.z = 0.34;
    index_match_list.push_back(n);

    // Reset.
    n.match_types[1] = 0;

    // Add new value.
    n.match_types[8] = 1;

    n.distance = 1.9;
    n.x = 0.1;
    n.y = 5.2;
    n.z = 0.34;
    index_match_list.push_back(n);

    // These two are equal.
    CPPUNIT_ASSERT( matcher.isMatch(process_match_list, index_match_list) );

    // It works also if the index match list is longer.
    n.distance = 1.93;
    n.x = 0.13;
    n.y = 5.2;
    n.z = 0.34;
    index_match_list.push_back(n);

    // These two are still equal.
    CPPUNIT_ASSERT( matcher.isMatch(process_match_list, index_match_list) );

    // If we add another not matching element to the process_match_list vecctor
    // we get a non-match.

    // Add new values.
    m.match_types[3]  = 1;
    m.update_types[2] = 1;
    m.distance     = 1.93;
    m.coordinate   = Coordinate(0.13, 5.2, 0.34);
    process_match_list.push_back(m);

    // Reset.
    m.match_types[3]  = 0;
    m.update_types[2] = 0;

    // These two are no longer equal.
    CPPUNIT_ASSERT( !matcher.isMatch(process_match_list, index_match_list) );

    // But if they differ in the update index they are still equal.
    m.match_types[8]  = 1;
    m.update_types[3] = 1;
    m.distance     = 1.93;
    m.coordinate   = Coordinate(0.13, 5.2, 0.34);
    process_match_list[3] = m;

    // These two are again equal.
    CPPUNIT_ASSERT( matcher.isMatch(process_match_list, index_match_list) );
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchWildcard()
{
    // Construct.
    Matcher matcher(100, 10);

    // Setup two match lists to check.
    ProcessBucketMatchList process_match_list;
    ProcessBucketMatchListEntry m;

    const int n_types = 9;
    m.match_types = TypeBucket(n_types);
    m.update_types = TypeBucket(n_types);

    m.match_types[3]  = 1;
    m.update_types[2] = 1;
    m.distance     = 1.2;
    m.coordinate   = Coordinate(0.1,2.0,0.34);
    process_match_list.push_back(m);
    // Reset.
    m.match_types[3]  = 0;
    m.update_types[2] = 0;

    // Add a wildcard, 0.
    m.match_types[0]  = 1;
    m.update_types[4] = 1;
    m.distance     = 1.6;
    m.coordinate   = Coordinate(1.1,2.0,0.34);
    process_match_list.push_back(m);
    // Reset.
    m.match_types[0]  = 0;
    m.update_types[4] = 0;

    m.match_types[8]  = 1;
    m.update_types[2] = 1;
    m.distance     = 1.9;
    m.coordinate   = Coordinate(0.1,5.2,0.34);
    process_match_list.push_back(m);
    // Reset.
    m.match_types[8]  = 0;
    m.update_types[2] = 0;

    ConfigBucketMatchList index_match_list;
    ConfigBucketMatchListEntry n;
    n.match_types = TypeBucket(n_types);
    n.match_types[3] = 1;
    n.distance     = 1.2;
    n.x = 0.1;
    n.y = 2.0;
    n.z = 0.34;
    index_match_list.push_back(n);
    // Reset.
    n.match_types[3] = 0;

    // Note that no wildcard here.
    n.match_types[1] = 1;
    n.distance     = 1.6;
    n.x = 1.1;
    n.y = 2.0;
    n.z = 0.34;
    index_match_list.push_back(n);
    // Reset.
    n.match_types[1] = 0;

    n.match_types[8] = 1;
    n.distance     = 1.9;
    n.x = 0.1;
    n.y = 5.2;
    n.z = 0.34;
    index_match_list.push_back(n);
    // Reset.
    n.match_types[8] = 0;

    // These two are equal.
    CPPUNIT_ASSERT( matcher.isMatch(process_match_list, index_match_list) );

}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchIndexListMinimal()
{
    // Construct.
    Matcher m(100, 10);

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
        elements1.push_back(std::vector<std::string>(1,"A"));
        elements1.push_back(std::vector<std::string>(1,"B"));
        std::vector<std::vector<std::string> > elements2;
        elements2.push_back(std::vector<std::string>(1,"C"));
        elements2.push_back(std::vector<std::string>(1,"B"));
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
        CPPUNIT_ASSERT( m.isMatch(process.processMatchList(), config.configMatchList(0)) );

        // This is not a match.
        CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(1)) );

    }

    // Construct another process that should match the second index.
    {
        // Setup the two configurations.
        std::vector<std::vector<std::string> > elements1;
        elements1.push_back(std::vector<std::string>(1,"B"));
        elements1.push_back(std::vector<std::string>(1,"A"));
        std::vector<std::vector<std::string> > elements2;
        elements2.push_back(std::vector<std::string>(1,"C"));
        elements2.push_back(std::vector<std::string>(1,"A"));
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
        CPPUNIT_ASSERT( m.isMatch(process.processMatchList(), config.configMatchList(1)) );

        // This is not a match.
        CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(0)) );
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
        CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(1)) );
        CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(0)) );
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
        CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(1)) );
        CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(0)) );
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
    Matcher m(100, 10);

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
            CPPUNIT_ASSERT(  m.isMatch(process.processMatchList(), config.configMatchList(i)) );
            CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(i+1)) );
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
            CPPUNIT_ASSERT(  m.isMatch(process.processMatchList(), config.configMatchList(i)) );
            CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(i+1)) );
        }
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testIsMatchIndexListComplicatedPeriodic()
{
    // Construct.
    Matcher m(54, 10);

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

        // This process should match all even numbered indices except the first.
        CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(0)) );
        CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(1)) );

        for (int i = 2; i < 27; i += 2)
        {
            CPPUNIT_ASSERT(  m.isMatch(process.processMatchList(), config.configMatchList(i)) );
            CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(i+1)) );
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
        CPPUNIT_ASSERT(  m.isMatch(process.processMatchList(), config.configMatchList(0)) );

        for (int i = 1; i < 27; ++i)
        {
            CPPUNIT_ASSERT( !m.isMatch(process.processMatchList(), config.configMatchList(i)) );
        }
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testCalculateMatchingProcess()
{
    // Construct.
    Matcher m(54, 3);

    // ---------------------------------------------------------------------
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

    // Calculate the match lists.
    config.initMatchLists(lattice_map, 1);

    // ---------------------------------------------------------------------
    // Setup a process that matches all even indexed sites except the first.
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

    // Put the process in a vector.
    std::vector<Process> processes(1, process);

    // Create an interactions object.
    Interactions  interactions(processes, false);

    // Make sure the process' available sites is empty.
    CPPUNIT_ASSERT( interactions.processes()[0]->sites().empty());

    // Match the index and process.
    std::vector<std::pair<int,int> > match_pairs(1,std::pair<int,int>(0,0));

    std::vector<RemoveTask> rt;
    std::vector<RateTask>   ut;
    std::vector<RateTask>   at;

    // Call the matching function.
    m.matchIndicesWithProcesses(match_pairs, interactions, config, rt, ut, at);
    m.updateProcesses(rt, ut, at, interactions);

    // Check that the process' available sites is still empty.
    CPPUNIT_ASSERT( interactions.processes()[0]->sites().empty());

    // Call the matching function again, now with another matching index.
    match_pairs = std::vector<std::pair<int,int> >(1, std::pair<int,int>(2,0));
    m.matchIndicesWithProcesses(match_pairs, interactions, config, rt, ut, at);
    m.updateProcesses(rt, ut, at, interactions);

    // Check that the index was added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(interactions.processes()[0]->sites().size()), 1 );
    CPPUNIT_ASSERT( interactions.processes()[0]->isListed(2) );

    // Match again.
    rt = std::vector<RemoveTask>(0);
    ut = std::vector<RateTask>(0);
    at = std::vector<RateTask>(0);
    m.matchIndicesWithProcesses(match_pairs, interactions, config, rt, ut, at);
    m.updateProcesses(rt, ut, at, interactions);

    // This should not have changed the availability of the index.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(interactions.processes()[0]->sites().size()), 1 );
    CPPUNIT_ASSERT( interactions.processes()[0]->isListed(2) );

    // Match another index.
    rt = std::vector<RemoveTask>(0);
    ut = std::vector<RateTask>(0);
    at = std::vector<RateTask>(0);
    match_pairs = std::vector<std::pair<int,int> >(1,std::pair<int,int>(4,0));
    m.matchIndicesWithProcesses(match_pairs, interactions, config, rt, ut, at);
    m.updateProcesses(rt, ut, at, interactions);

    // This should not have changed the availability of the first index.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(interactions.processes()[0]->sites().size()), 2 );
    CPPUNIT_ASSERT( interactions.processes()[0]->isListed(2) );
    CPPUNIT_ASSERT( interactions.processes()[0]->isListed(4) );

    // Change the configuration and match.
    elements[2] = std::vector<std::string>(1, "D");
    config = Configuration(coords, elements, possible_types);
    config.initMatchLists(lattice_map, 1);

    // Match.
    rt = std::vector<RemoveTask>(0);
    ut = std::vector<RateTask>(0);
    at = std::vector<RateTask>(0);
    match_pairs = std::vector<std::pair<int,int> >(1,std::pair<int,int>(2,0));
    m.matchIndicesWithProcesses(match_pairs, interactions, config, rt, ut, at);
    m.updateProcesses(rt, ut, at, interactions);

    // Now index 2 should be removed but 4 still there.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(interactions.processes()[0]->sites().size()), 1 );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(2) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(4) );

    // Match again - this should not change any thing.
    rt = std::vector<RemoveTask>(0);
    ut = std::vector<RateTask>(0);
    at = std::vector<RateTask>(0);
    match_pairs = std::vector<std::pair<int,int> >(1,std::pair<int,int>(2,0));
    m.matchIndicesWithProcesses(match_pairs, interactions, config, rt, ut, at);
    m.updateProcesses(rt, ut, at, interactions);

    CPPUNIT_ASSERT_EQUAL( static_cast<int>(interactions.processes()[0]->sites().size()), 1 );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(2) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(4) );

    // Match against a changed configuration. This removes index 4.
    elements[4] = std::vector<std::string>(1, "D");
    config = Configuration(coords, elements, possible_types);
    config.initMatchLists(lattice_map, 1);

    // Match.
    rt = std::vector<RemoveTask>(0);
    ut = std::vector<RateTask>(0);
    at = std::vector<RateTask>(0);
    match_pairs = std::vector<std::pair<int,int> >(1,std::pair<int,int>(4,0));
    m.matchIndicesWithProcesses(match_pairs, interactions, config, rt, ut, at);
    m.updateProcesses(rt, ut, at, interactions);

    CPPUNIT_ASSERT( interactions.processes()[0]->sites().empty() );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(4) );

}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testUpdateProcesses()
{
    Matcher m(100, 10);

    // Setup a list of processes and give it to an interactions object.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["C"] = 1;
    possible_types["B"] = 2;
    possible_types["D"] = 3;

    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "D"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    std::vector<std::vector<double> > process_coords(2,std::vector<double>(3,0.0));
    process_coords[1][0] =  0.5;
    process_coords[1][1] =  0.5;
    process_coords[1][2] =  0.5;

    const Configuration config1(process_coords, elements1, possible_types);
    const Configuration config2(process_coords, elements2, possible_types);
    const double rate = 13.7;
    std::vector<int> basis_sites;
    basis_sites.push_back(1);
    basis_sites.push_back(0);

    {
        CustomRateProcess process(config1, config2, rate, basis_sites, 1.0);

        // Set up the interactions.
        std::vector<CustomRateProcess> processes(4, process);
        Interactions interactions(processes, false, RateCalculator());

        // Populate the processes with indices.
        interactions.processes()[0]->addSite(0, 1.1);
        interactions.processes()[0]->addSite(1, 2.2);
        interactions.processes()[0]->addSite(2, 3.3);

        interactions.processes()[1]->addSite(0, 11.0);
        interactions.processes()[1]->addSite(1, 1.0);

        interactions.processes()[2]->addSite(0, 13.7);

        // Setup a couple of valid add, remove and update tasks.
        std::vector<RemoveTask> rt;
        std::vector<RateTask> at;
        std::vector<RateTask> ut;

        RemoveTask rt1;
        rt1.index   = 1;
        rt1.process = 0;
        rt.push_back(rt1);

        RemoveTask rt2;
        rt2.index   = 0;
        rt2.process = 1;
        rt.push_back(rt2);

        RateTask at1;
        at1.index   = 4;
        at1.process = 2;
        at1.rate    = 123.456;
        at.push_back(at1);

        RateTask ut1;
        ut1.index   = 0;
        ut1.process = 2;
        ut1.rate    = 99.0;
        ut.push_back(ut1);

        // Perform the tasks.
        m.updateProcesses(rt, ut, at, interactions);

        // Check the result of the tasks.
        CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(0) );
        CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(1) );
        CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(2) );
        CPPUNIT_ASSERT( !interactions.processes()[1]->isListed(0) );
        CPPUNIT_ASSERT(  interactions.processes()[1]->isListed(1) );
        CPPUNIT_ASSERT(  interactions.processes()[2]->isListed(0) );
        CPPUNIT_ASSERT(  interactions.processes()[2]->isListed(4) );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( interactions.processes()[2]->totalRate(), 123.456 + 99.0, 1.0e-12 );

    }
}


// -------------------------------------------------------------------------- //
//
void Test_Matcher::testCalculateMatchingInteractions()
{
    // Construct.
    Matcher m(54, 2);

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

    // Calculate the match lists.
    config.initMatchLists(lattice_map, 1);

    // ---------------------------------------------------------------------
    // Now the processes.
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
        const Process process(config1, config2, rate, basis_sites);
        processes.push_back(process);

    }

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
        const Process process(config1, config2, rate, basis_sites);
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
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(0) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(3) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(9) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(14) );

    // The second process should match the first index only.
    CPPUNIT_ASSERT(  interactions.processes()[1]->isListed(0) );
    CPPUNIT_ASSERT( !interactions.processes()[1]->isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[1]->isListed(3) );
    CPPUNIT_ASSERT( !interactions.processes()[1]->isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[1]->isListed(5) );

    // Call the matching function again does not change any thing.
    m.calculateMatching(interactions, config, lattice_map, indices);

    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(0) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(3) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(9) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(14) );
    CPPUNIT_ASSERT(  interactions.processes()[1]->isListed(0) );
    CPPUNIT_ASSERT( !interactions.processes()[1]->isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[1]->isListed(3) );
    CPPUNIT_ASSERT( !interactions.processes()[1]->isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[1]->isListed(5) );

    // Change the configuration such that the first proces
    // match with the first (0:th) index, and the second process
    // does not match any.
    elements[0] = std::vector<std::string>(1, "C");
    config = Configuration(coords, elements, possible_types);
    config.initMatchLists(lattice_map, 1);
    m.calculateMatching(interactions, config, lattice_map, indices);

    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(0) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(2) );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(3) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(4) );
    CPPUNIT_ASSERT( !interactions.processes()[0]->isListed(9) );
    CPPUNIT_ASSERT(  interactions.processes()[0]->isListed(14) );
    CPPUNIT_ASSERT( interactions.processes()[1]->sites().empty() );

}

// -------------------------------------------------------------------------- //
// This proxy class is needed for the UpdateRates test below.
class CustRateCalc : public RateCalculator {
public:
    virtual ~CustRateCalc() {}
    virtual double backendRateCallback(const std::vector<double> geometry,
                                       const int len,
                                       const std::vector<std::string> & types_before,
                                       const std::vector<std::string> & types_after,
                                       const double rate_constant,
                                       const int process_number,
                                       const double global_x,
                                       const double global_y,
                                       const double global_z) const
        {
            // Return.
            return std::sqrt(rate_constant);
        }
};

// -------------------------------------------------------------------------- //
//
void Test_Matcher::testUpdateRates()
{
    // Generate a list of tasks to run.
    std::vector<RateTask> tasks;

    seedRandom(3452321, true);
    const double ref_rate1 = randomDouble01();
    const double ref_rate2 = randomDouble01();

    RateTask t1;
    t1.index   = 0;
    t1.process = 0;
    t1.rate    = 0.0;
    tasks.push_back(t1);

    RateTask t2;
    t2.index   = 1;
    t2.process = 3;
    t2.rate    = 0.0;
    tasks.push_back(t2);

    // Here is the matcher to test.
    Matcher m(100, 10);

    // Setup a list of processes and give it to an interactions object.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["C"] = 1;
    possible_types["B"] = 2;
    possible_types["D"] = 3;
    possible_types["A"] = 4;

    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "C"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "D"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    std::vector<std::vector<double> > process_coords(2,std::vector<double>(3,0.0));

    process_coords[1][0] =  0.5;
    process_coords[1][1] =  0.5;
    process_coords[1][2] =  0.5;

    const Configuration config1(process_coords, elements1, possible_types);
    const Configuration config2(process_coords, elements2, possible_types);

    // Here is the process.
    std::vector<int> basis_sites;
    basis_sites.push_back(0);
    CustomRateProcess process1(config1, config2, ref_rate1, basis_sites, 1.0);
    CustomRateProcess process2(config1, config2, ref_rate2, basis_sites, 1.0);

    // Set up the interactions object.
    std::vector<CustomRateProcess> processes(4, process1);
    processes[3] = process2;
    CustRateCalc rate_calculator;
    Interactions interactions(processes, false, rate_calculator );

    // Setup a valid configuration.
    std::vector<std::vector<double> > coords(2, std::vector<double>(3, 0.0));

    // One cell with two atoms.
    coords[0][0] = 0.0;
    coords[0][1] = 0.0;
    coords[0][2] = 0.0;
    coords[1][0] = 0.5;
    coords[1][1] = 0.3;
    coords[1][2] = 0.1;

    // Setup elements.
    std::vector<std::vector<std::string> > elements(2);
    elements[0] = std::vector<std::string>(1, "A");
    elements[1] = std::vector<std::string>(1, "B");

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // Setup a cooresponding lattice map.
    const std::vector<int> repetitions(3, 1);
    const std::vector<bool> periodicity(3, false);
    const int basis = 2;

    LatticeMap lattice_map(basis, repetitions, periodicity);
    config.initMatchLists(lattice_map, 1);

    // Send the interactions object down for update
    // together with the processes and a configuration.
    std::vector<double> rates(tasks.size(), 0.0);
    m.updateRates(rates, tasks, interactions, config);

    // Check that the rates were correctly updated.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( rates[0], std::sqrt(ref_rate1), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( rates[1], std::sqrt(ref_rate2), 1.0e-12 );
}


// -------------------------------------------------------------------------- //
// This proxy class is part of the UpdateSingleRate test below.
class CustomRateCalculator : public RateCalculator {
public:
    CustomRateCalculator() {}
    virtual ~CustomRateCalculator() {}
    virtual double backendRateCallback(const std::vector<double> geometry,
                                       const int len,
                                       const std::vector<std::string> & types_before,
                                       const std::vector<std::string> & types_after,
                                       const double rate_constant,
                                       const int process_number,
                                       const double global_x,
                                       const double global_y,
                                       const double global_z) const
        {
            // Test the geometry.
            CPPUNIT_ASSERT_DOUBLES_EQUAL( geometry[0], 0.0, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( geometry[1], 0.0, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( geometry[2], 0.0, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( geometry[3], 0.5, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( geometry[4], 0.5, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( geometry[5], 0.5, 1.0e-12 );

            // Test the length.
            CPPUNIT_ASSERT_EQUAL(len, 2);

            // Test the types before.
            CPPUNIT_ASSERT_EQUAL(static_cast<int>(types_before.size()), 2);
            CPPUNIT_ASSERT_EQUAL(std::string("B"), types_before[0]);
            CPPUNIT_ASSERT_EQUAL(std::string("A"), types_before[1]);

            // Test the types after.
            CPPUNIT_ASSERT_EQUAL(static_cast<int>(types_after.size()), 2);
            CPPUNIT_ASSERT_EQUAL(std::string("C"), types_after[0]);
            CPPUNIT_ASSERT_EQUAL(std::string("A"), types_after[1]);

            // Test the process number.
            CPPUNIT_ASSERT_EQUAL( process_number, 917 );

            // Return.
            return std::pow(rate_constant,3.14159);
        }
};

// -------------------------------------------------------------------------- //
//
void Test_Matcher::testUpdateSingleRate()
{
    // Setup a valid configuration.
    std::vector<std::vector<double> > coords(2, std::vector<double>(3, 0.0));

    // One cell with two atoms.
    coords[0][0] = 0.0;
    coords[0][1] = 0.0;
    coords[0][2] = 0.0;
    coords[1][0] = -0.5;
    coords[1][1] = -0.5;
    coords[1][2] = -0.5;

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

    // Setup a non periodic cooresponding lattice map.
    const std::vector<int> repetitions(3, 1);
    const std::vector<bool> periodicity(3, false);
    const int basis = 2;
    std::vector<int> basis_sites;
    basis_sites.push_back(1);
    basis_sites.push_back(0);

    LatticeMap lattice_map(basis, repetitions, periodicity);
    config.initMatchLists(lattice_map, 13);

    // Construct a process that should match the second index.

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

    // Construct the process with a random rate.
    seedRandom(19, true);
    const double rate = 13.7*randomDouble01();
    CustomRateProcess process(config1, config2, rate, basis_sites, 12.0, std::vector<int>(0), std::vector<Coordinate>(0), 917);

    // Put the process in a vector.
    std::vector<CustomRateProcess> processes(1, process);

    // Create an interactions object.
    Interactions interactions(processes, false, CustomRateCalculator());

    // Get the matcher to test and a custom rate calculator.
    const Matcher m(100, 10);
    const CustomRateCalculator crc;
    const int index = 1;
    const double ret_rate = m.updateSingleRate(index, *interactions.processes()[0], config, crc);

    // Test against the known reference.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(ret_rate, std::pow(rate, 3.14159), 1.0e-12);

}
