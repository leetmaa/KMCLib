/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#include <cppunit/ui/text/TestRunner.h>
#include "testRunner.h"
#include "mpih.h"

int main (int argc, char *argv[])
{
#if RUNMPI == true
    MPI_Init(&argc, &argv);
#endif

    CppUnit::Test *test = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    CppUnit::TextTestRunner runner;
    runner.addTest(test);

    runner.run();

#if RUNMPI == true
    MPI_Finalize();
#endif

    return 0;
}

