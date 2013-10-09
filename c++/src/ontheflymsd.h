/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  ontheflymsd.h
 *  \brief File for the Process class definition.
 */

#ifndef __ONTHEFLYMSD__
#define __ONTHEFLYMSD__

#include <string>
#include <vector>
#include <utility>

#include "coordinate.h"


// Forward declarations.
class Configuration;


/*! \brief Class for performing on-the-fly mean square displacement analysis.
 */
class OnTheFlyMSD {

public:

    /*! \brief Constructor for setting up all internal buffers.
     *  \param configuration : The configuration of the simulation.
     *  \param history_steps : The number of steps to save in the history buffer.
     *  \param n_bins        : The number of bins in the histogram.
     *  \param t_max         : The starting time value of the last bin.
     *  \param t0            : The starting time of the simulation.
     *  \param track_type    : The atomic type to track.
     */
    OnTheFlyMSD(const Configuration & configuration,
                const int history_steps,
                const int n_bins,
                const double t_max,
                const double t0,
                const std::string track_type);

    /*! \brief Register a step.
     *  \param time          : The time of the configuration snapshot.
     *  \param configuration : The configuration to extract move-info from.
     */
    void registerStep(const double time,
                      const Configuration & configuration );

    /*! \brief Histogram buffer query.
     *  \return: The histogram buffer.
     */
    const std::vector<Coordinate> & histogramBuffer() const
    { return histogram_buffer_; }

    /*! \brief Histogram buffer squared query.
     *  \return: The histogram buffer of the squared values.
     */
    const std::vector<Coordinate> & histogramBufferSqr() const
    { return histogram_buffer_sqr_; }

    /*! \brief Query for the histogram bin counters.
     *  \return: The histogram bin counters vector.
     */
    const std::vector<int> & histogramBinCounts() const
    { return histogram_bin_counts_; }

    /*! \brief Query for the histogram bin counters per history step.
     *  \return: The histogram bin counters per history step vector.
     */
    const std::vector< std::vector<int> > & historyStepsHistogramBinCounts() const
    { return history_steps_bin_counts_; }

    /*! \brief Query for the history buffer, to facilitate testing.
     *  \return: The history buffer.
     */
    const std::vector< std::vector< std::pair<Coordinate, double> > > & historyBuffer() const
    { return history_buffer_; }

protected:

private:

    /// The history buffer.
    std::vector< std::vector< std::pair<Coordinate, double> > > history_buffer_;

    /// The histogram buffer.
    std::vector<Coordinate> histogram_buffer_;

    /// The histogram buffer with squared values.
    std::vector<Coordinate> histogram_buffer_sqr_;

    /// The histogram bin counts.
    std::vector<int> histogram_bin_counts_;

    /// The tracking type.
    std::string track_type_;

    /// The max time for binning.
    double t_max_;

    /// The bin size.
    double bin_size_;

    /// The number of history steps.
    size_t history_steps_;

    /// The bin counts per history step.
    std::vector< std::vector<int> > history_steps_bin_counts_;

};


/*! \brief Function for calculating and binning the MSD values from a
 *         history buffer vector.
 *  \param history (in)               : History buffer with coordiantes and times.
 *  \param binsize (in)               : The bin size of the histogram.
 *  \param histogram (in/out)         : The histogram to store the result in.
 *  \param bin_counters (in/out)      : The counters collecting the
 *                                      total number of values added to each bin.
 *  \param hsteps_bin_counts (in/out) : The histogram bin counts per history step.
 *
 */
void calculateAndBinMSD(const std::vector< std::pair<Coordinate, double> > & history,
                        const double binsize,
                        std::vector<Coordinate> & histogram,
                        std::vector<Coordinate> & histogram_sqr,
                        std::vector<int> & bin_counters,
                        std::vector< std::vector<int> > & hsteps_bin_counts);


#endif // __ONTHEFLYMSD__
