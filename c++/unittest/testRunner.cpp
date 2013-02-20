/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#include <cppunit/ui/text/TestRunner.h>
#include "testRunner.h"
#include <mpi.h>

int main (int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    CppUnit::Test *test = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    CppUnit::TextTestRunner runner;
    runner.addTest(test);

    runner.run();

    MPI_Finalize();

    return 0;
}

