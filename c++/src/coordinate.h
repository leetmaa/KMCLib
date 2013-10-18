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

#include <cmath>
#include <vector>

/*! \brief Class for representing a coordinate.
 */
class Coordinate {

public:

    /*! \brief Default constructor for SWIG wrapping in std::vector.
     */
    Coordinate();

    /*! \brief Constructor.
     *  \param x : The x coordinate.
     *  \param y : The y coordinate.
     *  \param z : The z coordinate.
     */
    Coordinate(const double x, const double y, const double z);

    /*! \brief Get the norm of the coordinate vector.
     *  \return : The norm of the coordinate vector.
     */
    inline
    double norm() const;

    /*! \brief Calculate the diagonal of the outer product of this
     *         coordinate with the other.
     *  \param other : The other coordinate.
     *  \return : The diagonal of the outer product of this and the other
                  coordinate.
     */
    inline
    Coordinate outerProdDiag(const Coordinate & other) const;

    /*! \brief Calculate the dot product of this
     *         coordinate with the other.
     *  \param other : The other coordinate.
     *  \return : The dot product of this and the other
                  coordinate.
     */
    inline
    double dot(const Coordinate & other) const;

    /*! \brief 'less than' for sorting.
     *  \param other : The Coordinate to compare.
     *  \return : (this < other)
     */
    inline
    bool operator<(const Coordinate & other) const;

    /*! \brief 'equals' operator.
     *  \return : this == other
     */
    inline
    bool operator==(const Coordinate & other) const;

    /*! \brief 'not equals' operator.
     *  \return : this != other
     */
    inline
    bool operator!=(const Coordinate & other) const;

    /*! \brief 'minus' operator.
     *  \param other : The Coordinate to subtract.
     *  \return : this - other
     */
    inline
    Coordinate operator-(const Coordinate & other) const;

    /*! \brief 'plus' operator.
     *  \param other : The Coordinate to add.
     *  \return : this + other
     */
    inline
    Coordinate operator+(const Coordinate & other) const;

    /*! \brief 'add into' operator.
     *  \param other : The Coordinate to add into this.
     */
    inline
    void operator+=(const Coordinate & other);

    /*! \brief 'times' operator.
     *  \param scalar : The scalar to multiply with.
     *  \return : this times the scalar.
     */
    inline
    Coordinate operator*(const double scalar) const;

    /*! \brief 'access' operator.
     *  \param i : The index to get out (0,1 or 2) corresponding to x,y,z
     *  \return : The queried component.
     */
    inline
    double operator[](const int i) const;

    /*! \brief 'access' operator.
     *  \param i : The index to get out (0,1 or 2) corresponding to x,y,z
     *  \return : The queried component.
     */
    inline
    double & operator[](const int i);

    /*! \brief Query for the x component.
     *  \return : The x component.
     */
    inline
    const double & x() const { return x_; }

    /*! \brief Query for the y component.
     *  \return : The y component.
     */
    inline
    const double & y() const { return y_; }

    /*! \brief Query for the z component.
     *  \return : The z component.
     */
    inline
    const double & z() const { return z_; }

    /*! \brief Query for the data as a std::vector.
     *  \return : The data as a vector.
     */
    inline
    std::vector<double>data() const;

    /*! \brief Calculate the distance between this and another coordinate.
     *  \return : The distance.
     */
    inline
    double distance(const Coordinate & other) const;

    /*! \brief Calculate the distance between this and another coordinate.
     *  \return : The distance.
     */
    inline
    double distanceToOrigin() const;

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

// -----------------------------------------------------------------------------
// INLINE FUNCTION DEFINITIONS FOLLOW
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
double Coordinate::norm() const
{
    return std::sqrt(x_*x_ + y_*y_ + z_*z_);
}


// -----------------------------------------------------------------------------
//
Coordinate Coordinate::outerProdDiag(const Coordinate & other) const
{
    return Coordinate(x_*other.x_, y_*other.y_, z_*other.z_);
}


// -----------------------------------------------------------------------------
//
double Coordinate::dot(const Coordinate & other) const
{
    return x_*other.x_ + y_*other.y_ + z_*other.z_;
}


// -----------------------------------------------------------------------------
//
bool Coordinate::operator<(const Coordinate & other) const
{
    if ( x_ < other.x_)
    {
        return true;
    }
    else if (x_ > other.x_)
    {
        return false;
    }
    else
    {
        if ( y_ < other.y_ )
        {
            return true;
        }
        else if ( y_ > other.y_ )
        {
            return false;
        }
        else
        {
            return ( z_ < other.z_ );
        }
    }
}


// -----------------------------------------------------------------------------
//
bool Coordinate::operator==(const Coordinate & other) const
{
    return (!(*this < other) && !(other < *this));
}


// -----------------------------------------------------------------------------
//
bool Coordinate::operator!=(const Coordinate & other) const
{
    return !(*this == other);
}


// -----------------------------------------------------------------------------
//
Coordinate Coordinate::operator-(const Coordinate & other) const
{
    return Coordinate(x_ - other.x_,
                      y_ - other.y_,
                      z_ - other.z_);
}


// -----------------------------------------------------------------------------
//
Coordinate Coordinate::operator+(const Coordinate & other) const
{
    return Coordinate(x_ + other.x_,
                      y_ + other.y_,
                      z_ + other.z_);
}


// -----------------------------------------------------------------------------
//
void Coordinate::operator+=(const Coordinate & other)
{
    x_ += other.x_;
    y_ += other.y_;
    z_ += other.z_;
}


// -----------------------------------------------------------------------------
//
Coordinate Coordinate::operator*(const double scalar) const
{
    return Coordinate(x_ * scalar,
                      y_ * scalar,
                      z_ * scalar);
}


// -----------------------------------------------------------------------------
//
double Coordinate::operator[](const int index) const
{
    if (index < 1)
    {
        return x_;
    }
    else if (index < 2)
    {
        return y_;
    }
    else
    {
        return z_;
    }
}


// -----------------------------------------------------------------------------
//
double & Coordinate::operator[](const int index)
{
    if (index < 1)
    {
        return x_;
    }
    else if (index < 2)
    {
        return y_;
    }
    else
    {
        return z_;
    }
}


// -----------------------------------------------------------------------------
//
double Coordinate::distance(const Coordinate & other) const
{
    // PERFOMME:
    // Working in squared distances saves only a few percent in the time
    // it takes to setup the configuration match lists.
    const double tmp_value = (std::pow(x_ - other.x_, 2) +
                              std::pow(y_ - other.y_, 2) +
                              std::pow(z_ - other.z_, 2));
    //return tmp_value;
    return std::sqrt( tmp_value );
}


// -----------------------------------------------------------------------------
//
double Coordinate::distanceToOrigin() const
{
    // PERFOMME:
    // Working in squared distances saves only a few percent in the time
    // it takes to setup the configuration match lists.
    const double tmp_value = (std::pow(x_, 2) +
                              std::pow(y_, 2) +
                              std::pow(z_, 2));
    //return tmp_value;
    return std::sqrt( tmp_value );
}


// -----------------------------------------------------------------------------
//
std::vector<double> Coordinate::data() const
{
    std::vector<double> tmp(3);
    tmp[0] = x_;
    tmp[1] = y_;
    tmp[2] = z_;
    return tmp;
}


#endif // __COORDINATE__



