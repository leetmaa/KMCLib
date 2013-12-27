/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


#include <cppunit/TextOutputter.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFailure.h>

#include "testRunner.h"
#include "mpih.h"
#include "mpicommons.h"


/// Outputter for custom test results message.
class CustomOutputter : public CppUnit::TextOutputter {

public:

    CustomOutputter( CppUnit::TestResultCollector *result,
                     CppUnit::OStream &stream ) : CppUnit::TextOutputter(result, stream) {}

    /// Overrides the print header with a nicer message.
    void printHeader()
    {
        // This is no guarantee, but it can help to give sorted output.
        MPICommons::barrier();

        for (int i = 0; i < MPICommons::size(); ++i)
        {
            if (MPICommons::myRank() == i)
            {

#if RUNMPI == true
                m_stream << std::endl << "Test results from MPI rank " << i << std::endl;
#endif

                if ( m_result->wasSuccessful() )
                {
                    m_stream << std::endl << "     ALL (" << m_result->runTests () << " tests) OK" << std::endl ;
                    m_stream << std::endl;
                }
                else
                {
                    printFailureWarning();
                    printStatistics();
                }
                m_stream.flush();
            }
        }
    }
};


/// Progress listener for custom printouts.
class CustomProgressListener : public CppUnit::TestListener {

public:

    /// Print the name of the test running.
    void startTest( CppUnit::Test *test )
    {
        if (MPICommons::isMaster())
        {
            CppUnit::stdCOut() << "[ " << test->getName() << " ]";
            CppUnit::stdCOut().flush();
        }
    }

    /// New line, and make sure to flush.
    void endTest( CppUnit::Test *test )
    {
        MPICommons::barrier();
        if (MPICommons::isMaster())
        {
            CppUnit::stdCOut() << std::endl;
            CppUnit::stdCOut().flush();
        }
    }

    /// Clear FAILURE and ERROR markers.
    void addFailure( const CppUnit::TestFailure &failure )
    {
#if RUNMPI == true
        CppUnit::stdCOut() << std::endl << "!!! "
                           << failure.failedTest()->getName()
                           << ( failure.isError() ? "- ERROR" : "- FAIL" )
                           << " on MPI rank " << MPICommons::myRank()
                           << std::endl;
        CppUnit::stdCOut().flush();
#else
        const int padding_length = 60 - failure.failedTest()->getName().length();
        const std::string padding(padding_length, '-');
        CppUnit::stdCOut() << "   <" << padding << ( failure.isError() ? "- ERROR" : "- FAIL" );
        CppUnit::stdCOut().flush();
#endif
    }

    /// Flush.
    void endTestRun( CppUnit::Test *test,
                     CppUnit::TestResult *eventManager )
    {
        MPICommons::barrier();
        if (MPICommons::isMaster())
        {
            CppUnit::stdCOut()  <<  std::endl;
            CppUnit::stdCOut().flush();
        }
    }
};



int main (int argc, char *argv[])
{

    // Start MPI if this is a parallel build.
#if RUNMPI == true
    MPI_Init(&argc, &argv);
#endif

    // Print a header message.
#if RUNMPI == true
    if (MPICommons::isMaster())
    {
        CppUnit::stdCOut() << std::endl << "Running KMCLib C++ unit tests on " << MPICommons::size() << " MPI processes." << std::endl;
    }
#else
    CppUnit::stdCOut() << std::endl << "Running KMCLib C++ unit tests." << std::endl;

#endif

    // Setup the tests.
    CppUnit::Test *test = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    CppUnit::TextTestRunner runner;
    runner.addTest(test);

    // Add our custom outputter.
    runner.setOutputter( new CustomOutputter(&runner.result(),
                                             std::cout) );

    // Add our custom progress listener.
    CppUnit::TestResult & event_manager = runner.eventManager();
    CustomProgressListener progress;
    event_manager.addListener( &progress );

    // Run the tests.
    runner.run(std::string(""), false, true, false);

    // Remove the progress listener.
    event_manager.removeListener( &progress );

    // Finalize if MPI.
#if RUNMPI == true
    MPI_Finalize();
#endif

    // DONE
    return 0;
}
