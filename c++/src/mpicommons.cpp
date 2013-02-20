/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  mpicommons.cpp
 *  \brief File for the implementation code of the common MPI routines.
 */


#include "mpicommons.h"

#include <mpi.h>

bool inited__    = false;
bool finalized__ = false;

// -----------------------------------------------------------------------------
//
void MPICommons::init()
{
    if (inited__)
    {
        return;
    }

    // Allocate dummy arg memory.
    int argc;
    char** argv = new char*[24];
    for (int i = 0; i < 24; ++i)
    {
        argv[i] = new char[1];
    }

    // Make the init call.
    MPI_Init( &argc, &argv );

    // Free what we allocated.
    for (int i = 0; i < 24; ++i)
    {
        delete [] argv[i];
    }
    delete [] argv;

    // Set the flag to prevent further calls.
    inited__ = true;
}


// -----------------------------------------------------------------------------
//
void MPICommons::finalize()
{
    if (finalized__)
    {
        return;
    }

    MPI_Finalize();

    finalized__ = true;
}


// -----------------------------------------------------------------------------
//
int MPICommons::myRank(const MPI_Comm comm)
{
    int rank;
    MPI_Comm_rank( comm, &rank );
    return rank;
}


// -----------------------------------------------------------------------------
//
int MPICommons::size(const MPI_Comm comm)
{
    int size;
    MPI_Comm_size( comm, &size );
    return size;
}


// -----------------------------------------------------------------------------
//
void MPICommons::barrier(const MPI_Comm comm)
{
    MPI_Barrier( comm );
}
