/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  mpicommons.cpp
 *  \brief File for the implementation code of the common MPI routines.
 */


#include "mpicommons.h"

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

    // Switch for using MPI.
#if RUNMPI == true
    // Dummy args.
    int argc = 0;
    char** argv;

    // Make the init call.
    MPI_Init( &argc, &argv );

    // Set the flag to prevent further calls.
    inited__ = true;
#endif
}


// -----------------------------------------------------------------------------
//
void MPICommons::finalize()
{
    if (finalized__)
    {
        return;
    }

#if RUNMPI == true
        MPI_Finalize();
        finalized__ = true;
#endif
}


// -----------------------------------------------------------------------------
//
int MPICommons::myRank(const MPI_Comm comm)
{
#if RUNMPI == true
    int rank;
    MPI_Comm_rank( comm, &rank );
    return rank;
#else
    return 0;
#endif
}


// -----------------------------------------------------------------------------
//
int MPICommons::size(const MPI_Comm comm)
{
#if RUNMPI == true
    int size;
    MPI_Comm_size( comm, &size );
    return size;
#else
    return 1;
#endif
}


// -----------------------------------------------------------------------------
//
void MPICommons::barrier(const MPI_Comm comm)
{
#if RUNMPI == true
    MPI_Barrier( comm );
#endif
}
