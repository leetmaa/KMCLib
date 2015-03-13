/*
  Copyright (c)  2012-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  configuration.h
 *  \brief File for the Configuration class definition.
 */

#ifndef __CONFIGURATION__
#define __CONFIGURATION__


#include <vector>
#include <string>
#include <map>
#include "matchlist.h"
#include "coordinate.h"
#include "typebucket.h"

// Forward declarations.
class LatticeMap;
class Process;

/*! \brief Class for defining the configuration used in a KMC simulation to
 *         use for communicating elements and positions to and from python.
 */
class Configuration {

public:

    /*! \brief Constructor for setting up the configuration.
     *  \param coordinates   : The coordinates of the configuration.
     *  \param elements      : The elements of the configuration.
     *  \param possible_types: A global mapping from type string to number.
     */
    Configuration(const std::vector< std::vector<double> > & coordinates,
                  const std::vector< std::vector<std::string> > & elements,
                  const std::map<std::string,int> & possible_types);

    /*! \brief Initiate the calculation of the match lists.
     *  \param lattice_map : The lattice map needed to get coordinates wrapped.
     *  \param range       : The number of shells to include.
     */
    void initMatchLists(const LatticeMap & lattice_map, const int range);

    /*! \brief Const query for the coordinates.
     *  \return : The coordinates of the configuration.
     */
    const std::vector<Coordinate> & coordinates() const { return coordinates_; }

    /*! \brief Const query for the atom id coordinates.
     *  \return : The atom id coordinates of the configuration.
     */
    const std::vector<Coordinate> & atomIDCoordinates() const { return atom_id_coordinates_; }

    /*! \brief Const query for the elements.
     *  \return : The elements of the configuration.
     */
    const std::vector<std::vector<std::string> > & elements() const { return elements_; }

    /*! \brief Const query for the atom id types.
     *  \return : The atom id types of the configuration.
     */
    const std::vector<std::string> & atomIDElements() const { return atom_id_elements_; }

    /*! \brief Const query for the types.
     *  \return : The types of the configuration.
     */
    const std::vector<TypeBucket> & types() const { return types_; }

    /*! \brief Const query for the moved atom ids.
     *  \return : A copy of the moved atom ids, resized to correct length.
     */
    inline
    std::vector<int> movedAtomIDs() const;

    /*! \brief Const query for the process number of the latest process performed.
     *  \return : The number of the latest process performed.
     */
    int latestEventProcess() const { return latest_event_process_; }

    /*! \brief Const query for the site index for the latest performed event.
     *  \return : The latest site where a process was performed.
     */
    int latestEventSite() const { return latest_event_site_; }

    /*! \brief Const query for the moved atoms move vectors, in the same order as the id's.
     *  \return : A copy of the recent move vectors, resized to correct length.
     */
    inline
    std::vector<Coordinate> recentMoveVectors() const;

    /*! \brief Construct and return the match list for the given list of
     *         indices.
     *  \param origin_index : The index to treat as the origin.
     *  \param indices      : The indices to get the match list for.
     *  \param lattice_map  : The lattice map needed for calculating distances
     *                        using correct boundaries.
     *  \return : The match list.
     */
    const ConfigBucketMatchList & configMatchList(const int origin_index,
                                                  const std::vector<int> & indices,
                                                  const LatticeMap & lattice_map) const;


    /*! \brief Update the cached match list for the given index.
     *  \param index : The index to update the match list for.
     */
    void updateMatchList(const int index);

    /*! \brief Return the cached match list without update.
     *  \param index : The index to get the match list for.
     *  \return : The match list.
     */
    const ConfigBucketMatchList & configMatchList(const int index) const { return match_lists_[index]; }

    /*! \brief Perform the given process.
     *  \param process : The process to perform, which will be updated with the affected
     *                   indices.
     *  \param site_index : The index of the site where the process should be performed.
     *  \param lattice_map : The lattice map needed for proper move vector indexing.
     */
    void performBucketProcess(Process & process,
                              const int site_index,
                              const LatticeMap & lattice_map);

    /*! \brief Query for the type name.
     *  \param type: The type integer to get the name for.
     *  \return : The string representation of the type integer.
     */
    const std::string & typeName(const int type) const { return type_names_[type]; }

    // ML: Needs testing.
    /*! \brief Query for the type names list.
     *  \return : A handle to the type names list.
     */
    const std::vector<std::string> & typeNames() const { return type_names_; }

    /*! \brief Get the atom id coordinates.
     *  \return : The list of atom id coordinates.
     */
    const std::vector<Coordinate> & atomIdCoordinates() const { return atom_id_coordinates_; }

    /*! \brief Get the atom id at each lattice site.
     *  \retrurn : The list of atom ids for the lattice sites.
     */
    const std::vector<int> & atomID() const { return atom_id_; }

    /*! \brief Set the update info on the configuration. This is used
     *         in connection with setting up processes of bucket type.
     *  \param update_info: The update info to set on the class.
     */
    void setUpdateInfo(const std::vector<std::map<std::string, int> > & update_info) { update_info_ = update_info; }

    /*! \brief Query for the update info.
     *  \return : The update info stored on the class.
     */
    const std::vector<std::map<std::string, int> > & updateInfo() const { return update_info_; }

    /*! \brief Query for the mapping between string and int
               type representations.
     *  \return : A const handle to the mapping.
     */
    const std::map<std::string,int> & possibleTypes() const { return possible_types_; }

    /*! \brief Query for the number of particles per type.
     *  \return : A vector holding the number of particles per type.
     */
    std::vector<int> particlesPerType() const;

protected:

private:

    /// Counter for the number of moved atom ids the last move.
    int n_moved_;

    /// The lattice coordinates.
    std::vector<Coordinate> coordinates_;

    /// The coordinates for each atom id.
    std::vector<Coordinate> atom_id_coordinates_;

    /// The lattice elements.
    std::vector<std::vector<std::string> > elements_;

    /// The elements per atom id.
    std::vector<std::string> atom_id_elements_;

    /// The the lattice elements in integer representation.
    std::vector<TypeBucket> types_;

    /// The atom id for each lattice point.
    std::vector<int> atom_id_;

    /// The first n_moved_ elements hold the moved atom ids.
    std::vector<int> moved_atom_ids_;

    /// The first n_moved_ elements hold the moved atoms move vectors (listed on id).
    std::vector<Coordinate> recent_move_vectors_;

    /// The mapping from type integers to names.
    std::vector<std::string> type_names_;

    /// The match lists for all indices.
    std::vector< ConfigBucketMatchList > match_lists_;

    /// The update info.
    std::vector< std::map<std::string, int> > update_info_;

    /// Mapping from string to int representation of types.
    std::map<std::string,int> possible_types_;

    /// The process number of the latest event that took place.
    int latest_event_process_;

    /// The site index of the latest event that took place.
    int latest_event_site_;

};


// -----------------------------------------------------------------------------
// Inlined function definitions follow.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
std::vector<int> Configuration::movedAtomIDs() const
{
    // PERFORMME
    std::vector<int> moved(moved_atom_ids_);
    moved.resize(n_moved_);
    return moved;
}


// -----------------------------------------------------------------------------
//
std::vector<Coordinate> Configuration::recentMoveVectors() const
{
    // PERFORMME
    std::vector<Coordinate> move_vectors(recent_move_vectors_);
    move_vectors.resize(n_moved_);
    return move_vectors;
}




#endif // __CONFIGURATION__

