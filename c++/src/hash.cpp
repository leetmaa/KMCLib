/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


/*! \file  hash.cpp
 *  \brief File for the hash function implementation code.
 */

#include "hash.h"


#include "process.h"
#include "configuration.h"
#include "matchlist.h"
#include "externals/md5.h"

#include <cstdio>
#include <cstring>


// -------------------------------------------------------------------------- //
//
unsigned long int hash64MD5xor(std::vector<int> & message)
{
    // Get the full hash.
    unsigned long int res[2];
    void *ptr1 = (&message[0]);

    // Call the md5 core.
    md5Core(ptr1, message.size()*sizeof(int), res[0], res[1]);

    // XOR down to 64-bit result.
    return res[0] ^ res[1];
}


// -------------------------------------------------------------------------- //
//
void md5Core(void *message,
             const unsigned long int size,
             unsigned long int & result1,
             unsigned long int & result2)
{
    // Call the external MD5 routine.
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, message, size);
    unsigned char result[16];
    MD5_Final(result, &ctx);

    // Get the digest in the right format and return.
    unsigned long int res[2];
    memcpy(res, result, 16);
    result1 = res[0];
    result2 = res[1];

    // DONE
}


// -------------------------------------------------------------------------- //
//
std::string hashMD5(const std::string & message)
{
    // Get the full hash.
    unsigned long int res[2];
    hashMD5(message, res[0], res[1]);

    // Get the digest in the right format and return.
    char final[33];
    sprintf(final, "%lx%lx", res[0], res[1]);
    return std::string(final);
}


// -------------------------------------------------------------------------- //
//
void hashMD5(const std::string & message,
             unsigned long int & result1,
             unsigned long int & result2)
{
    // Copy the incomming message to avoid an evil const cast.
    std::string msg(message);

    // Call the md5 core.
    void *ptr1 = (&msg[0]);

    // Call the md5 core.
    md5Core(ptr1, message.size(), result1, result2);

    // DONE
}


// -------------------------------------------------------------------------- //
//
unsigned long int hash64MD5xor(const std::string & message)
{
    // Get the full hash.
    unsigned long int res[2];
    hashMD5(message, res[0], res[1]);

    // XOR down to 64-bit result.
    return res[0] ^ res[1];
}


// -------------------------------------------------------------------------- //
//
unsigned long int hashCustomRateInput(const int index,
                                      const Process & process,
                                      const Configuration & configuration)
{
    // This number uniquely identifies the process parameters.
    const int process_number = process.processNumber();

    // Get cutoff distance from the process.
    const double cutoff = process.cutoff();

    // This is the source of configuration information we will need.
    const ConfigBucketMatchList & config_match_list  = configuration.configMatchList(index);

    // This is the data to hash.
    std::vector<int> data_to_hash;
    data_to_hash.push_back(process_number);

    // Add the match types of the config match list to the data to hash.
    ConfigBucketMatchList::const_iterator it1 = config_match_list.begin();
    while ( (*it1).distance <= cutoff && it1 != config_match_list.end())
    {
        for (int i = 0; i < (*it1).match_types.size(); ++i)
        {
            data_to_hash.push_back((*it1).match_types[i]);
        }
        ++it1;
    }

    // Hash and return.
    return hash64MD5xor(data_to_hash);
}
