/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_configuration.h"

// Include the files to test.
#include "../src/configuration.h"

#include "../src/latticemap.h"
#include "../src/process.h"

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
    std::vector<std::string> elements(5);
    elements[0] = "A";
    elements[1] = "B";
    elements[2] = "D";
    elements[3] = "H";
    elements[4] = "J";


    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
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
    std::vector<std::string> const & ret_elements = config.elements();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(ret_coords.size()),
                         static_cast<int>(ret_elements.size()));

    for (size_t i = 0; i < elements.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(ret_elements[i], elements[i]);
    }

    // DONE

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

    elements[0]    = "V";
    elements[216]  = "V";   // These affects process 0,1 and 3
    elements[1434] = "V";
    elements[2101] = "V";   // This affects process 0,1 and 2

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

    // Init the match lists.
    configuration.initMatchLists(lattice_map);

    // Get a process that finds a V between two B and turns one of
    // the Bs into an A.
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

    // Now, add index 1434 to the process.
    // We know by construction that these match.
    p.addSite(1434);

    // For site 1434
    // 350 changes from 1 to 0
    // 1434 changes from 2 to 1
    // All other must remain unchanged.
    CPPUNIT_ASSERT_EQUAL( configuration.types()[1434], 2 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[350],  1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[1433], 1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[349],  1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[351],  0 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[2517], 0 );

    // Peform the process.
    configuration.performProcess(p, 1434);

    // Check that the types were correctly updated.
    CPPUNIT_ASSERT_EQUAL( configuration.types()[1434], 1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[350],  0 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[1433], 1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[349],  1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[351],  0 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[2517], 0 );

    // Check that the correct indices were added to the list of affected.
    const std::vector<int> affected = p.affectedIndices();
    CPPUNIT_ASSERT_EQUAL( affected[0], 1434 );
    CPPUNIT_ASSERT_EQUAL( affected[1], 350  );

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

    elements[0]    = "V";
    elements[216]  = "V";
    elements[1434] = "V";
    elements[2101] = "V";

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

    // Try to access the match lists before initialization. They should be
    // empty.
    CPPUNIT_ASSERT( configuration.minimalMatchList(10).empty()   );
    CPPUNIT_ASSERT( configuration.minimalMatchList(2101).empty() );
    CPPUNIT_ASSERT( configuration.minimalMatchList(1434).empty() );

    // Init the match lists.
    configuration.initMatchLists(lattice_map);

    // This did something.
    CPPUNIT_ASSERT( !configuration.minimalMatchList(10).empty()   );
    CPPUNIT_ASSERT( !configuration.minimalMatchList(2101).empty() );
    CPPUNIT_ASSERT( !configuration.minimalMatchList(1434).empty() );

    // Get the match list the hard way.
    const std::vector<MinimalMatchListEntry> ref_1434 = \
        configuration.minimalMatchList( 1434,
                                        lattice_map.neighbourIndices(1434),
                                        lattice_map);
    // Check the size.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ref_1434.size()),
                          static_cast<int>(configuration.minimalMatchList(1434).size()) );

    // Check the values.
    for (size_t i = 0; i < ref_1434.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL( ref_1434[i].match_type,
                              configuration.minimalMatchList(1434)[i].match_type );
        CPPUNIT_ASSERT_EQUAL( ref_1434[i].update_type,
                              configuration.minimalMatchList(1434)[i].update_type );
        CPPUNIT_ASSERT_EQUAL( ref_1434[i].index,
                              configuration.minimalMatchList(1434)[i].index );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_1434[i].distance,
                                      configuration.minimalMatchList(1434)[i].distance,
                                      1.0e-14 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_1434[i].coordinate.x(),
                                      configuration.minimalMatchList(1434)[i].coordinate.x(),
                                      1.0e-14 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_1434[i].coordinate.y(),
                                      configuration.minimalMatchList(1434)[i].coordinate.y(),
                                      1.0e-14 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_1434[i].coordinate.z(),
                                      configuration.minimalMatchList(1434)[i].coordinate.z(),
                                      1.0e-14 );

    }

    // Setup a process that changes V to B.

    // Get a process that finds a V between two B and turns one of
    // the Bs into an A.
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

    // Now, add index 1434 to the process.
    // We know by construction that these match.
    p.addSite(1434);

    // For site 1434
    // 350 changes from 1 to 0
    // 1434 changes from 2 to 1
    // All other must remain unchanged.
    CPPUNIT_ASSERT_EQUAL( configuration.types()[1434], 2 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[350],  1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[1433], 1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[349],  1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[351],  0 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[2517], 0 );

    // Peform the process.
    configuration.performProcess(p, 1434);

    // Check that the types were correctly updated.
    CPPUNIT_ASSERT_EQUAL( configuration.types()[1434], 1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[350],  0 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[1433], 1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[349],  1 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[351],  0 );
    CPPUNIT_ASSERT_EQUAL( configuration.types()[2517], 0 );

    // Check that the match cached match list was correctly updated.
    const std::vector<MinimalMatchListEntry> ref2_1434 =        \
        configuration.minimalMatchList( 1434,
                                        lattice_map.neighbourIndices(1434),
                                        lattice_map);
    // Check the size.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ref2_1434.size()),
                          static_cast<int>(configuration.minimalMatchList(1434).size()) );

    // Check the values.
    for (size_t i = 0; i < ref2_1434.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL( ref2_1434[i].match_type,
                              configuration.minimalMatchList(1434)[i].match_type );
        CPPUNIT_ASSERT_EQUAL( ref2_1434[i].update_type,
                              configuration.minimalMatchList(1434)[i].update_type );
        CPPUNIT_ASSERT_EQUAL( ref2_1434[i].index,
                              configuration.minimalMatchList(1434)[i].index );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref2_1434[i].distance,
                                      configuration.minimalMatchList(1434)[i].distance,
                                      1.0e-14 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref2_1434[i].coordinate.x(),
                                      configuration.minimalMatchList(1434)[i].coordinate.x(),
                                      1.0e-14 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref2_1434[i].coordinate.y(),
                                      configuration.minimalMatchList(1434)[i].coordinate.y(),
                                      1.0e-14 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref2_1434[i].coordinate.z(),
                                      configuration.minimalMatchList(1434)[i].coordinate.z(),
                                      1.0e-14 );

    }
}

