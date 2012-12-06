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

    /*! \brief 'minus' operator.
     *  \param other : The Coordinate to subtract.
     *  \return : this - other
     */
    Coordinate operator-(const Coordinate & other) const;

    /*! \brief 'access' operator.
     *  \param i : The index to get out (0,1 or 2) corresponding to x,y,z
     *  \return : The queried component.
     */
    double operator[](const int i) const;

    /*! \brief 'access' operator.
     *  \param i : The index to get out (0,1 or 2) corresponding to x,y,z
     *  \return : The queried component.
     */
    double & operator[](const int i);

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

    /*! \brief Calculate the distance between this and another coordinate.
     *  \return : The distance.
     */
    double distance(const Coordinate & other) const;

    /* \brief Print the coordinate to stdout. Useful for debugging.
     */
    void print() const;

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

