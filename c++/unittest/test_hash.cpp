/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_hash.h"

// Include the files to test.
#include "hash.h"


// -------------------------------------------------------------------------- //
//
void Test_Hash::testMD5String()
{
    std::string hash = hashMD5("");
    // The reference in little and big endian.
    //    d41d8cd9 8f00b204 e9800998 ecf8427e
    //    d98c1dd4  4b2008f 980980e9 7e42f8ec
    const std::string ref0("d98c1dd4 4b2008f980980e97e42f8ec");
    CPPUNIT_ASSERT( hash == ref0 );

    hash = hashMD5("The quick brown fox jumps over the lazy dog");
    // The reference in little and big endian.
    //    9d7d109e 82b62b37 351dd86b d619a442
    //    9e107d9d 372bb682 6bd81d35 42a419d6
    const std::string ref1("9d7d109e82b62b37351dd86bd619a442");
    CPPUNIT_ASSERT( hash == ref1 );

    hash = hashMD5("The quick brown fox jumps over the lazy dog.");
    // The reference in little and big endian.
    //    c209d9e4 1cfbd090 adff68a0 d0cb22df
    //    e4d909c2 90d0fb1c a068ffad df22cbd0
    const std::string ref2("c209d9e41cfbd090adff68a0d0cb22df");
    CPPUNIT_ASSERT( hash == ref2 );
}

