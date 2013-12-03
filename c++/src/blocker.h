/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  blocker.h
 *  \brief File for the Blocker class definition.
 */

#ifndef __BLOCKER__
#define __BLOCKER__

#include <vector>
#include <utility>

/// Forward declarations.
class Coordinate;

/// Class for handling block averages.
class Blocker {

public:

    /*! \brief Constructor for the blocker class.
     *  \param nbins     : The number of bins in the histograms.
     *  \param blocksize : The number of elements in each block.
     */
    Blocker(const int nbins, const int blocksize);

    /*! \brief Register the step with bin and value.
     *  \param bin   : The bin to add at.
     *  \param value : The value to add.
     */
    void registerStep(const int bin, const Coordinate value);

    /*! \brief Query for blocksize.
     *  \returns : The blocksize.
     */
    double blocksize() const { return blocksize_; }

    /*! \brief Query for the accumulative block histogram values per bin.
     *  \returns : A const reference to the histogram blocks data.
     */
    const std::vector< std::vector<Coordinate> > & hstBlocks() const { return hst_blocks_; }

    /*! \brief Calculate the resulting standard deviation and its error.
     *  \param histogram_bin_counts : The number of samples per bin.
     *  \param histogram_buffer     : The histogram values.
     *  \returns : The standard deviation and its error estimate in x, y and z
     *             for each bin.
     */
    std::vector< std::pair<Coordinate, Coordinate> > values(const std::vector<int> & histogram_bin_counts,
                                                            const std::vector<Coordinate> & histogram_buffer) const;


protected:

private:

    /// The size of each block.
    int blocksize_;

    /// The counts since last block for each bin.
    std::vector<int> counts_since_last_block_;

    /// The accumulative histogram value for the current block.
    std::vector<Coordinate> histogram_block_;

    /// The histogram values for all finnished blocks for each bin.
    std::vector< std::vector<Coordinate> > hst_blocks_;

};


#endif // __BLOCKER__
