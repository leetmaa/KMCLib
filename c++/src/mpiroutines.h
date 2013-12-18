/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  mpiroutines.h
 *  \brief File for the mpi utlility routines interfaces.
 */


#ifndef __MPIROUTINES__
#define __MPIROUTINES__


#include <vector>
#include "mpih.h"


/*! \brief Calculate the chunks for all processes. Convenient for testing.
 *  \param mpi_size    : This number of processes.
 *  \param vector_size : The length of the vector to split.
 *  \return : The starting position and number of elements to take
 *            from the vector to split for each process.
 */
std::vector< std::pair<int,int> > determineChunks(const int mpi_size,
                                                  const int vector_size);

/*! \brief Distribute and integer from master to all other ranks.
 *  \param data : The data to distrubite from master to all others.
 *  \param comm : The communicator to use.
 */
void distributeToAll(int & data,
                     const MPI_Comm & comm=MPI_COMM_WORLD);


/*! \brief Sum the data over all processors.
 *  \param data : The data to sum.
 *  \param comm : The communicator to use.
 */
void sumOverProcesses(int & data,
                      const MPI_Comm & comm=MPI_COMM_WORLD);


/*! \brief Sum the data over all processors.
 *  \param data : The data to sum.
 *  \param comm : The communicator to use.
 */
void sumOverProcesses(std::vector<int> & data,
                      const MPI_Comm & comm=MPI_COMM_WORLD);


/*! \brief Sum the data over all processors.
 *  \param data : The data to sum.
 *  \param comm : The communicator to use.
 */
void sumOverProcesses(std::vector<double> & data,
                      const MPI_Comm & comm=MPI_COMM_WORLD);


/*! \brief Split the global vector over the processes.
 *  \param global : The data vector to split.
 *  \param comm   : The communicator to use.
 *  \return       : A part of the vector for each process.
 */
template <class T_vector>
T_vector splitOverProcesses(const T_vector & global,
                            const MPI_Comm & comm=MPI_COMM_WORLD);

/*! \brief Join the local vectors to form a global.
 *  \param local  : The data vector to join.
 *  \param comm   : The communicator to use.
 *  \return       : The global vector.
 */
template <class T_vector>
T_vector joinOverProcesses(const T_vector & local,
                           const MPI_Comm & comm=MPI_COMM_WORLD);



// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
//
// TEMPLATE IMPLEMENTATION CODE FOLLOW
//
// -------------------------------------------------------------------------- //
//
template <class T_vector>
T_vector splitOverProcesses(const T_vector & global,
                            const MPI_Comm & comm)
{
    // Get the dimensions.
#if RUNMPI == true
    int rank, size;
    MPI_Comm_rank( comm, &rank );
    MPI_Comm_size( comm, &size );
#else
    int rank = 0;
    int size = 1;
#endif

    const int global_len = global.size();


    // Calculate everyones chunk sizes.
    std::vector< std::pair<int,int> > chunks = determineChunks(size,
                                                               global_len);

    // Determine which elements to work on base on my rank.
    const int start  = chunks[rank].first;
    const int n_take = chunks[rank].second;

    // Copy the values over.
    T_vector my_copy(n_take);
    for (int i = 0; i < n_take; ++i)
    {
        my_copy[i] = global[start+i];
    }

    // Return.
    return my_copy;
}


// -------------------------------------------------------------------------- //
//
template <class T_vector>
T_vector joinOverProcesses(const T_vector & local,
                           const MPI_Comm & comm)
{
    // PERFORMME: Prototyping. Chunks does not need to be this involved.

    // Get the dimensions.
#if RUNMPI == true
    int rank, size;
    MPI_Comm_rank( comm, &rank );
    MPI_Comm_size( comm, &size );
#else
    int rank = 0;
    int size = 1;
#endif

    const int local_len = local.size();

    // Get the total length of the vector.
    int global_len = local_len;
    sumOverProcesses(global_len, comm);

    // Setup the return data.
    T_vector global_data(global_len);

    // Calculate everyones chunk sizes.
    std::vector< std::pair<int,int> > chunks = determineChunks(size,
                                                               global_len);

    // Determine which elements to take base on my rank.
    const int start  = chunks[rank].first;
    const int n_take = chunks[rank].second;

    // Copy the values over.
    for (int i = 0; i < n_take; ++i)
    {
        global_data[start+i] = local[i];
    }

    // Now the global data holds the local data in the correct
    // positions for each processor, but must be communicated.
    sumOverProcesses(global_data, comm);

    // Return.
    return global_data;
}


#endif // __MPIROUTINES__
