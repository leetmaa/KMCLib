/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  coordinate.h
 *  \brief File for the coordinate class definition.
 */

#ifndef __COORDINATE__
#define __COORDINATE__


/*! \brief Class for representing a coordinate.
 */
class Coordinate {

public:

    /*! \brief Constructor.
     *  \param x : The x coordinate.
     *  \param y : The y coordinate.
     *  \param z : The z coordinate.
     */
    Coordinate(const double x, const double y, const double z);

    /*! \brief 'less than' for sorting.
     *  \param other : The Coordinate to compare.
     *  \return : (this < other)
     */
    bool operator<(const Coordinate & other) const;

    /*! \brief Query for the x component.
     *  \return : The x component.
     */
    const double & x() const { return x_; }

    /*! \brief Query for the y component.
     *  \return : The y component.
     */
    const double & y() const { return y_; }

    /*! \brief Query for the z component.
     *  \return : The z component.
     */
    const double & z() const { return z_; }

protected:

private:

    /// The x coordinate.
    double x_;

    /// The y coordinate.
    double y_;

    /// The z coordinate.
    double z_;

};


#endif // __COORDINATE__

