/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file  latticemodel.h
 *  \brief File for the LatticeModel class definition.
 */



#ifndef __LATTICEMODEL__
#define __LATTICEMODEL__


/// Class for defining and running a lattice KMC model.
class LatticeModel {

public:

    /*! \brief Constructor for setting up the model.
     */
    LatticeModel();

    /*! \brief Function for taking one time step in the KMC lattice model.
     */
    void singleStep();

protected:

private:

};


#endif // __LATTICEMODEL__

