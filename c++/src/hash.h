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
#include <vector>

// Forward declarations.
class Process;
class Configuration;


/*! \brief Function for generating the MD5 hash of string message.
 *  \param message : The message to hash.
 *  \returns : The MD5 digest of the message.
 */
std::string hashMD5(const std::string & message);


/*! \brief Function for generating the MD5 hash as an xor-ed
 *         64-bit integer of the two parts of the MD5 hash.
 *  \param message : The message to hash.
 *  \returns : The 64-bit 'MD5' digest of the message.
 */
unsigned long int hash64MD5xor(const std::string & message);


/*! \brief Function for generating the MD5 hash as an xor-ed
 *         64-bit integer of the two parts of the MD5 hash.
 *  \param message : The message to hash.
 *  \returns : The 64-bit 'MD5' digest of the message.
 */
unsigned long int hash64MD5xor(std::vector<int> & message);


/*! \brief Function for generating the MD5 hash as two 64-bit integers.
 *  \param message (in)  : The message to hash.
 *  \param result1 (out) : The first 64-bits of the hash.
 *  \param result2 (out) : The last 64-bits of the hash.
 */
void hashMD5(const std::string & message,
             unsigned long int & result1,
             unsigned long int & result2);


/*! \brief Core md5 work horse. The only place we call the external
 *         md5 routine from.
 *  \param message (in)  : The message to digest.
 *  \param size    (in)  : The size of the message in bytes.
 *  \param result1 (out) : The first 64-bits of the digest.
 *  \param result2 (out) : The last 64-bits of the digest.
 */
void md5Core(void *message,
             const unsigned long int size,
             unsigned long int & result1,
             unsigned long int & result2);


/*! \brief Function for generating a 64-bit hash from the input to the
 *         custom rate calculator input.
 *  \param index         : The global site index.
 *  \param process       : The process that takes place.
 *  \param configuration : The global configuration of the system.
 *  \returns: 64-bit hash value.
 */
unsigned long int hashCustomRateInput(const int index,
                                      const Process & process,
                                      const Configuration & configurartion);


#endif // __HASH__

