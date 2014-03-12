/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  hash.cpp
 *  \brief File for the hash function implementation code.
 */

#include "hash.h"

#include "externals/md5.h"

#include <cstdio>
#include <cstring>


// -------------------------------------------------------------------------- //
//
std::string hashMD5(const std::string & message)
{
    // Copy the incomming message to avoid an evil const cast.
    std::string msg(message);

    // Call the external MD5 routine.
    MD5_CTX ctx;
    MD5_Init(&ctx);
    void *ptr1 = (&msg[0]);
    unsigned long size = static_cast<unsigned long>(message.size());
    MD5_Update(&ctx, ptr1, size);
    unsigned char result[16];
    MD5_Final(result, &ctx);

    // Get the digest in the right format and return.
    unsigned int res[4];
    memcpy(res, result, 16);
    char final[33];
    sprintf(final, "%8x%8x%8x%8x", res[0], res[1], res[2], res[3]);

    return std::string(final);
}

