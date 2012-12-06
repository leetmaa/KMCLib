/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_configuration.h"

// Include the files to test.
#include "../src/configuration.h"


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

    for (size_t i=0; i < coords.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ret_coords[i][0], coords[i][0], 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ret_coords[i][1], coords[i][1], 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ret_coords[i][2], coords[i][2], 1.0e-12);
    }

    std::vector<std::string> const & ret_elements = config.elements();
    for (size_t i=0; i < elements.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(ret_elements[i], elements[i]);
    }

    // DONE

}

