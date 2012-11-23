/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_latticemodel.h"

// Include the files to test.
#include "../src/latticemodel.h"


// -------------------------------------------------------------------------- //
//
void Test_LatticeModel::testConstruction()
{
    // Construct.
    LatticeModel model;

    // Call the single step function.
    model.singleStep();

}

