/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/



/*! \file hash.h
 *  \brief File for the hash functions.
 */


#ifndef __HASH__
#define __HASH__

#include <string>


/*! \brief Function for generating the MD5 hash of string message.
 *  \param message : The message to hash.
 *  \returns : The MD5 digest of the message.
 */
std::string hashMD5(const std::string & message);


#endif // __HASH__

