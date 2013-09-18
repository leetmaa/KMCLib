/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_mpicommons.h"

// Include the files to test.
#include "mpicommons.h"

#include <ctime>
#include <unistd.h>

// -------------------------------------------------------------------------- //
//
void Test_MPICommons::testSize()
{
    // Get the reference size.
#if RUNMPI == true
    int ref_size;
    MPI_Comm_size( MPI_COMM_WORLD, &ref_size );
#else
    const int ref_size = 1;
#endif

    // Call the common interface.
    const int size = MPICommons::size();

    // Make sure the result is the same.
    CPPUNIT_ASSERT_EQUAL( size, ref_size );
}


// -------------------------------------------------------------------------- //
//
void Test_MPICommons::testRank()
{
    // Get the reference rank.
#if RUNMPI == true
    int ref_rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &ref_rank );
#else
    const int ref_rank = 0;
#endif

    // Call the common interface.
    const int rank = MPICommons::myRank();

    // Make sure the result is the same.
    CPPUNIT_ASSERT_EQUAL( rank, ref_rank );
}


// -------------------------------------------------------------------------- //
//
void Test_MPICommons::testIsMaster()
{
    // Get the reference rank.
#if RUNMPI == true
    int ref_rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &ref_rank );
#else
    const int ref_rank = 0;
#endif

    if (ref_rank == 0)
    {
        CPPUNIT_ASSERT( MPICommons::isMaster() );
    }
    else
    {
        CPPUNIT_ASSERT( !MPICommons::isMaster() );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MPICommons::testBarrier()
{
    // Call the barrier function.
    MPICommons::barrier();

// Only if run in parallel.
#if RUNMPI == true
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    time_t seconds;

    std::vector<int> time_before_sleep(size);
    std::vector<int> time_after_sleep(size);
    std::vector<int> time_after_barrier(size);

    // Call the barrier function to test.
    MPICommons::barrier();

    // Get the time.
    time(&seconds);
    time_before_sleep[rank] = seconds;

    // Sleep.
    sleep(rank*2);

    // Get the time again.
    time(&seconds);
    time_after_sleep[rank] = seconds;

    // Synchronize.
    MPICommons::barrier();

    // Get the time.
    time(&seconds);
    time_after_barrier[rank] = seconds;

    // Communicate the timing results.
    std::vector<int> send1(time_before_sleep);
    MPI_Allreduce(&send1[0],
                  &time_before_sleep[0],
                  size,
                  MPI_INT,
                  MPI_SUM,
                  MPI_COMM_WORLD);

    std::vector<int> send2(time_after_sleep);
    MPI_Allreduce(&send2[0],
                  &time_after_sleep[0],
                  size,
                  MPI_INT,
                  MPI_SUM,
                  MPI_COMM_WORLD);

    std::vector<int> send3(time_after_barrier);
    MPI_Allreduce(&send3[0],
                  &time_after_barrier[0],
                  size,
                  MPI_INT,
                  MPI_SUM,
                  MPI_COMM_WORLD);

    // Check that the results.

    // Time to sleep - different depending on rank.
    int max = 0;
    for (int i = 0; i < size; ++i)
    {
        const int time_to_sleep = time_after_sleep[i] - time_before_sleep[i];
        CPPUNIT_ASSERT_EQUAL(time_to_sleep, i*2);
        max = std::max(time_to_sleep, max);
    }

    // Time total - the same for everyone.
    for (int i = 0; i < size; ++i)
    {
        const int time_total = time_after_barrier[i] - time_before_sleep[i];
        CPPUNIT_ASSERT_EQUAL(time_total, max);
    }

#endif // if RUNMPI == true
}

