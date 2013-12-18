/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_mpiroutines.h"

// Include the files to test.
#include "mpiroutines.h"

#include <cmath>

// -------------------------------------------------------------------------- //
//
void Test_MPIRoutines::testDetermineChunks()
{
    {
        // 9 elements and 1 processes makes 9 elments per process.
        std::vector< std::pair<int,int> > chunks = determineChunks(1,9);
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(chunks.size()), 1);

        CPPUNIT_ASSERT_EQUAL( chunks[0].first, 0 );
        CPPUNIT_ASSERT_EQUAL( chunks[0].second, 9 );
    }
    {
        // 15 elements and 3 processes makes 5 elments per process.
        std::vector< std::pair<int,int> > chunks = determineChunks(3,15);
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(chunks.size()), 3);

        CPPUNIT_ASSERT_EQUAL( chunks[0].first, 0 );
        CPPUNIT_ASSERT_EQUAL( chunks[0].second, 5 );

        CPPUNIT_ASSERT_EQUAL( chunks[1].first, 5 );
        CPPUNIT_ASSERT_EQUAL( chunks[1].second, 5 );

        CPPUNIT_ASSERT_EQUAL( chunks[2].first, 10 );
        CPPUNIT_ASSERT_EQUAL( chunks[2].second, 5 );
    }
    {
        // 15 elements and 4 processes makes 4 elements for the first
        // 3 processes, and 3 elemenst for the last.
        std::vector< std::pair<int,int> > chunks = determineChunks(4,15);
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(chunks.size()), 4);

        CPPUNIT_ASSERT_EQUAL( chunks[0].first, 0 );
        CPPUNIT_ASSERT_EQUAL( chunks[0].second, 4 );

        CPPUNIT_ASSERT_EQUAL( chunks[1].first, 4 );
        CPPUNIT_ASSERT_EQUAL( chunks[1].second, 4 );

        CPPUNIT_ASSERT_EQUAL( chunks[2].first, 8 );
        CPPUNIT_ASSERT_EQUAL( chunks[2].second, 4 );

        CPPUNIT_ASSERT_EQUAL( chunks[3].first, 12 );
        CPPUNIT_ASSERT_EQUAL( chunks[3].second, 3 );
    }
    {
        // 3 elements and 5 processes makes 1 elements for the first
        // 3 processes, and 0 elemenst for the last two.
        std::vector< std::pair<int,int> > chunks = determineChunks(5,3);
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(chunks.size()), 5);

        CPPUNIT_ASSERT_EQUAL( chunks[0].first, 0 );
        CPPUNIT_ASSERT_EQUAL( chunks[0].second, 1 );

        CPPUNIT_ASSERT_EQUAL( chunks[1].first, 1 );
        CPPUNIT_ASSERT_EQUAL( chunks[1].second, 1 );

        CPPUNIT_ASSERT_EQUAL( chunks[2].first, 2 );
        CPPUNIT_ASSERT_EQUAL( chunks[2].second, 1 );

        CPPUNIT_ASSERT_EQUAL( chunks[3].first, 3 );
        CPPUNIT_ASSERT_EQUAL( chunks[3].second, 0 );

        CPPUNIT_ASSERT_EQUAL( chunks[4].first, 3 );
        CPPUNIT_ASSERT_EQUAL( chunks[4].second, 0 );
    }

    {
        // 13 elements and 2 processes makes 7 elements for the first
        // processes and 6 elemenst for the second.
        std::vector< std::pair<int,int> > chunks = determineChunks(2,13);
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(chunks.size()), 2);

        CPPUNIT_ASSERT_EQUAL( chunks[0].first, 0 );
        CPPUNIT_ASSERT_EQUAL( chunks[0].second, 7 );

        CPPUNIT_ASSERT_EQUAL( chunks[1].first, 7 );
        CPPUNIT_ASSERT_EQUAL( chunks[1].second, 6 );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MPIRoutines::testDistributeToAll()
{
#if RUNMPI == true
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
#else
    const int rank = 0;
#endif

    // Set the data to zero on all processes.
    int data = 0;

    // Set the data on master to the reference.
    const int reference = 23456765;
    if (rank == 0)
    {
        data = reference;
    }

    // Send to all other nodes.
    distributeToAll(data, MPI_COMM_WORLD);

    // Check.
    CPPUNIT_ASSERT_EQUAL( data, reference );

}


// -------------------------------------------------------------------------- //
//
void Test_MPIRoutines::testSumOverProcessesInt()
{
#if RUNMPI == true
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
#else
    const int rank = 0;
    const int size = 1;
#endif

    int sum_value = rank*rank + 3;

    // Sum.
    sumOverProcesses(sum_value, MPI_COMM_WORLD);

    // Calculate the reference.
    int ref_sum = 0;
    for (int i = 0; i < size; ++i)
    {
        ref_sum += i*i + 3;
    }

    // Check.
    CPPUNIT_ASSERT_EQUAL( ref_sum, sum_value );

}


// -------------------------------------------------------------------------- //
//
void Test_MPIRoutines::testSumOverProcessesVectorInt()
{
#if RUNMPI == true
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
#else
    const int rank = 0;
    const int size = 1;
#endif

    std::vector<int> data(size, rank*rank+17);
    data[rank] = 0;

    // Sum.
    sumOverProcesses(data, MPI_COMM_WORLD);

    // Calculate the reference.
    std::vector<int> ref_data(size, 0);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (i != j)
            {
                ref_data[i] += j*j + 17;
            }
        }
    }

    // Check.
    for (int i = 0; i < size; ++i)
    {
        CPPUNIT_ASSERT_EQUAL( ref_data[i], data[i] );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MPIRoutines::testSumOverProcessesVectorDouble()
{
#if RUNMPI == true
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
#else
    const int rank = 0;
    const int size = 1;
#endif

    std::vector<double> data(size, rank*rank+std::sqrt(2.0));
    data[rank] = 0.0;

    // Sum.
    sumOverProcesses(data, MPI_COMM_WORLD);

    // Calculate the reference.
    std::vector<double> ref_data(size, 0.0);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (i != j)
            {
                ref_data[i] += j*j + std::sqrt(2.0);
            }
        }
    }

    // Check.
    for (int i = 0; i < size; ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( ref_data[i], data[i], 1.0e-12 );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MPIRoutines::testSplitOverProcesses()
{
#if RUNMPI == true
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
#else
    const int rank = 0;
    const int size = 1;
#endif

    // Setup the global data such that each process gets
    // three elements except the last process which gets two.
    std::vector<int> global_data((size*3)-1);
    for (size_t i = 0; i < global_data.size(); ++i)
    {
        global_data[i] = i;
    }

    // Split.
    std::vector<int> local_data = splitOverProcesses(global_data, MPI_COMM_WORLD);

    // Check.
    if (rank == size - 1)
    {
        CPPUNIT_ASSERT_EQUAL( static_cast<int>(local_data.size()), 2 );
        const size_t len = global_data.size();
        CPPUNIT_ASSERT_EQUAL( local_data[0], global_data[len-2] );
        CPPUNIT_ASSERT_EQUAL( local_data[1], global_data[len-1] );
    }
    else
    {
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(local_data.size()), 3);

        int my_start = rank * 3;
        CPPUNIT_ASSERT_EQUAL( local_data[0], global_data[my_start + 0] );
        CPPUNIT_ASSERT_EQUAL( local_data[1], global_data[my_start + 1] );
        CPPUNIT_ASSERT_EQUAL( local_data[2], global_data[my_start + 2] );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_MPIRoutines::testJoinOverProcesses()
{
#if RUNMPI == true
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
#else
    const int rank = 0;
    const int size = 1;
#endif

    // Setup global data such that each process gets
    // three elements except the last process which gets two.
    std::vector<int> global_data((size*3)-1);
    for (size_t i = 0; i < global_data.size(); ++i)
    {
        global_data[i] = i;
    }

    // Split.
    std::vector<int> local_data = splitOverProcesses(global_data, MPI_COMM_WORLD);

    // Multiply the local data with the rank.
    for (size_t i = 0; i < local_data.size(); ++i)
    {
        local_data[i] *= rank;
    }

    // Join.
    std::vector<int> new_global = joinOverProcesses(local_data, MPI_COMM_WORLD);

    // Create a reference new global.
    std::vector<int> new_global_ref(global_data);

    int counter = 0;
    for (int i = 0; i < size; ++i)
    {
        if (i != size-1)
        {
            new_global_ref[counter + 0] *= i;
            new_global_ref[counter + 1] *= i;
            new_global_ref[counter + 2] *= i;
            counter += 3;
        }
        else
        {
            new_global_ref[counter + 0] *= i;
            new_global_ref[counter + 1] *= i;
        }
    }

    // Check.
    for (size_t i = 0; i < new_global.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( new_global_ref[i], new_global[i], 1.0e-12 );
    }
}

