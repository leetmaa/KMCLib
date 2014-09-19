/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_hash.h"

// Include the files to test.
#include "hash.h"

#include "configuration.h"
#include "customrateprocess.h"
#include "latticemap.h"
#include "random.h"


// For profiling on Linux.
#include <sys/time.h>
double cpu_time() { return (double)clock() / CLOCKS_PER_SEC; }


// -------------------------------------------------------------------------- //
//
void Test_Hash::testMD5String()
{
    std::string hash = hashMD5("");
    // The reference in 32-bit big endian.
    // d41d8cd9 8f00b204 e9800998 ecf8427e
    const std::string ref0("4b2008fd98c1dd47e42f8ec980980e9");
    CPPUNIT_ASSERT( hash == ref0 );

    hash = hashMD5("The quick brown fox jumps over the lazy dog");
    // The reference in 32-bit big endian.
    // 9e107d9d 372bb682 6bd81d35 42a419d6
    const std::string ref1("82b62b379d7d109ed619a442351dd86b");
    CPPUNIT_ASSERT( hash == ref1 );

    hash = hashMD5("The quick brown fox jumps over the lazy dog.");
    // The reference in 32-bit big endian.
    // e4d909c2 90d0fb1c a068ffad df22cbd0
    const std::string ref2("1cfbd090c209d9e4d0cb22dfadff68a0");
    CPPUNIT_ASSERT( hash == ref2 );
}


// -------------------------------------------------------------------------- //
//
void Test_Hash::test64MD5String()
{
    {
        // Get the 64-bit xor-ed hash.
        size_t hash = hash64MD5xor("");

        // Get the full hash.
        size_t hash1, hash2;
        hashMD5("", hash1, hash2);

        // Check that this is the xor-ed parts of the full md5 hash.
        CPPUNIT_ASSERT_EQUAL(hash, (hash1 ^ hash2));
    }


    {
        // Get the 64-bit xor-ed hash.
        size_t hash = hash64MD5xor("This is another string.");

        // Get the full hash.
        size_t hash1, hash2;
        hashMD5("This is another string.", hash1, hash2);

        // Check that this is the xor-ed parts of the full md5 hash.
        CPPUNIT_ASSERT_EQUAL(hash, (hash1 ^ hash2));
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Hash::testHashCustomRateInput()
{
    // Setup a configuration and a process.

    // -----------------------------------------------------------------------
    // Setup a valid configuration.
    std::vector<std::vector<double> > coords(2, std::vector<double>(3, 0.0));

    // One cell with two atoms.
    coords[0][0] = 0.0;
    coords[0][1] = 0.0;
    coords[0][2] = 0.0;
    coords[1][0] = 0.5;
    coords[1][1] = 0.3;
    coords[1][2] = 0.1;

    // Setup elements.
    std::vector<std::vector<std::string> > elements(2);
    elements[0] = std::vector<std::string>(1,"A");
    elements[1] = std::vector<std::string>(1,"B");

    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 3;
    possible_types["D"] = 4;
    possible_types["E"] = 5;
    possible_types["F"] = 6;

    // Construct the configuration.
    Configuration config(coords, elements, possible_types);

    // Setup a non periodic cooresponding lattice map.
    const std::vector<int> repetitions(3, 1);
    const std::vector<bool> periodicity(3, false);
    const int basis = 2;
    std::vector<int> basis_sites;
    basis_sites.push_back(1);
    basis_sites.push_back(0);

    LatticeMap lattice_map(basis, repetitions, periodicity);
    config.initMatchLists(lattice_map, 13);

    // Construct a process that should match the second index.

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1,"B"));
    elements1.push_back(std::vector<std::string>(1,"A"));
    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1,"C"));
    elements2.push_back(std::vector<std::string>(1,"A"));
    // Setup coordinates.
    std::vector<std::vector<double> > process_coords(2,std::vector<double>(3,0.0));
    process_coords[1][0] =  -0.5;
    process_coords[1][1] =  -0.5;
    process_coords[1][2] =  -0.5;
    // The configurations.
    const Configuration config1(process_coords, elements1, possible_types);
    const Configuration config2(process_coords, elements2, possible_types);

    // Construct the process with a random rate.
    seedRandom(19, true);
    const double rate = 13.7*randomDouble01();
    const CustomRateProcess process1(config1, config2, rate, basis_sites, 12.0, std::vector<int>(0), std::vector<Coordinate>(0), 917);
    const CustomRateProcess process2(config1, config2, rate, basis_sites, 12.0, std::vector<int>(0), std::vector<Coordinate>(0), 916);
    int index;
    // -----------------------------------------------------------------------

    {
        // Get the hash.
        index = 1;
        const unsigned long int hash1 = hashCustomRateInput(index, process1, config);
        const unsigned long int ref1 = 18009609292013583759u;
        CPPUNIT_ASSERT_EQUAL(hash1, ref1);

        // Get the hash.
        index = 0;
        const unsigned long int hash0 = hashCustomRateInput(index, process1, config);
        const unsigned long int ref0 = 4224368175550234772u;
        CPPUNIT_ASSERT_EQUAL(hash0, ref0);
    }

    {
        // Check against another process that differs in the process number.
        index = 1;
        const unsigned long int hash1 = hashCustomRateInput(index, process2, config);
        const unsigned long int ref1 = 4824710481459367137u;
        CPPUNIT_ASSERT_EQUAL(hash1, ref1);

        index = 0;
        const unsigned long int hash0 = hashCustomRateInput(index, process2, config);
        const unsigned long int ref0 = 17780468236463825071u;
        CPPUNIT_ASSERT_EQUAL(hash0, ref0);
    }

    // Check performance.
    if (false)
    {
        double t1 = cpu_time();
        unsigned long int hash_loop;
        for (int i = 0; i < 10000000; ++i)
        {
            hash_loop = hashCustomRateInput(index, process2, config);
        }
        double t2 = cpu_time();

        // Printout to avoid optimization.
        printf("hash0 %lx\n %e", hash_loop, (t2-t1)/10000000);
    }
}
