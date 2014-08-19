/*
  Copyright (c)  2012-2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_customrateprocess.h"

// Include the files to test.
#include "customrateprocess.h"

#include "configuration.h"
#include "random.h"

#include <cmath>

// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testConstruction()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    std::vector<int> basis_sites(3,0);
    basis_sites[1] = 23;
    basis_sites[2] = 11;
    const double rate = 13.7;
    CustomRateProcess process(config1, config2, rate, basis_sites, 1.0);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Check that the basis sites can be returned correctly.
    const std::vector<int> ret_basis_sites = process.basisSites();
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[0], basis_sites[0] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[1], basis_sites[1] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[2], basis_sites[2] );

}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testMatchListLong()
{
    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["C"] = 2;
    possible_types["D"] = 3;
    possible_types["E"] = 4;
    possible_types["F"] = 5;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    elements1.push_back(std::vector<std::string>(1, "B"));
    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > process_coords(9,std::vector<double>(3,0.0));
    process_coords[1][0] =  0.5;
    process_coords[1][1] =  0.5;
    process_coords[1][2] =  0.5;

    process_coords[2][0] = -0.5;
    process_coords[2][1] =  0.5;
    process_coords[2][2] =  0.5;

    process_coords[3][0] =  0.5;
    process_coords[3][1] = -0.5;
    process_coords[3][2] =  0.5;

    process_coords[4][0] =  0.5;
    process_coords[4][1] =  0.5;
    process_coords[4][2] = -0.5;

    process_coords[5][0] = -0.5;
    process_coords[5][1] = -0.5;
    process_coords[5][2] =  0.5;

    process_coords[6][0] = -0.5;
    process_coords[6][1] =  0.5;
    process_coords[6][2] = -0.5;

    process_coords[7][0] =  0.5;
    process_coords[7][1] = -0.5;
    process_coords[7][2] = -0.5;

    process_coords[8][0] = -0.5;
    process_coords[8][1] = -0.5;
    process_coords[8][2] = -0.5;

    // The configurations.
    const Configuration config1(process_coords, elements1, possible_types);
    const Configuration config2(process_coords, elements2, possible_types);


    // Check the length of the coordinates from the configuration.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(config1.coordinates().size()),
                          static_cast<int>(process_coords.size()));

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    CustomRateProcess process(config1, config2, rate, basis_sites, 1.0);

    // Get the match list out.
    const ProcessBucketMatchList match_list = process.processMatchList();

    // Check the size of the match list.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(process_coords.size()),
                          static_cast<int>(match_list.size()));

    // Check the coordinates.
    const Coordinate c0 = match_list[0].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c0.x(), 0.0, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c0.y(), 0.0, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c0.z(), 0.0, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[0].distance, 0.0, 1.0e-14 );

    const Coordinate c1 = match_list[1].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[1].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c2 = match_list[2].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c2.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c2.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c2.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[2].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c3 = match_list[3].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c3.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c3.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c3.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[3].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c4 = match_list[4].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c4.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c4.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c4.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[4].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c5 = match_list[5].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c5.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c5.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c5.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[5].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c6 = match_list[6].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c6.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c6.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c6.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[6].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c7 = match_list[7].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c7.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c7.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c7.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[7].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c8 = match_list[8].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c8.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c8.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c8.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[8].distance, std::sqrt(0.25*3.0), 1.0e-14 );

}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testTotalRate()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    CustomRateProcess process(config1, config2, rate, basis_sites, 1.0);

    // Get the total rate.
    const double total_rate_0 = process.totalRate();

    // This total rate should be zero since there are no sites added.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( total_rate_0, 0.0, 1.0e-12 );

    // If we add two sites the total rate should be the sum.
    process.addSite(123, 1.2, 1.0);
    process.addSite(456, 3.1, 1.0);
    const double total_rate_1 = process.totalRate();
    CPPUNIT_ASSERT_DOUBLES_EQUAL( total_rate_1, 4.3, 1.0e-12 );

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testTotalRateMultiplicity()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    CustomRateProcess process(config1, config2, rate, basis_sites, 1.0);

    // Get the total rate.
    const double total_rate_0 = process.totalRate();

    // This total rate should be zero since there are no sites added.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( total_rate_0, 0.0, 1.0e-12 );

    // If we add two sites the total rate should be the sum of the
    // rate times the multiplicity.
    process.addSite(123, 1.2, 13.0);
    process.addSite(456, 3.1, 7.0);
    const double total_rate_1 = process.totalRate();
    const double ref_total_rate_1 = (1.2 * 13.0) + (3.1 * 7.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( total_rate_1, ref_total_rate_1, 1.0e-12 );

    // Remove one. This should update the total rate accordingly.
    process.removeSite(123);
    const double total_rate_2 = process.totalRate();
    const double ref_total_rate_2 = 3.1 * 7.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( total_rate_2, ref_total_rate_2, 1.0e-12 );

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testAddAndRemoveSite()
{
    // Test that the individual sites rates are added and removed correctly.

    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    CustomRateProcess process(config1, config2, rate, basis_sites, 1.0);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Check the rate constant.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(process.rateConstant(), rate, 1.0e-12);

    // Chek the total rate.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(process.totalRate(), 0.0, 1.0e-12);

    // Add a few indices.
    seedRandom(true, 1);
    const double r0 = randomDouble01()*10.0;
    const double r1 = randomDouble01()*10.0;
    const double r2 = randomDouble01()*10.0;
    const double r3 = randomDouble01()*10.0;

    process.addSite(1234, r0, 1.0);
    process.addSite(3,    r1, 1.0);
    process.addSite(11,   r2, 1.0);
    process.addSite(-123, r3, 1.0);

    // Check the total rate again.
    const double rtot0 = r0 + r1 + r2 + r3;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(process.totalRate(), rtot0, 1.0e-12);


    // Check that these indices are now there.
    CPPUNIT_ASSERT( process.isListed(1234) );
    CPPUNIT_ASSERT( process.isListed(3)    );
    CPPUNIT_ASSERT( process.isListed(11)   );
    CPPUNIT_ASSERT( process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 4);

    // Remove two of the indices.
    process.removeSite(3);
    process.removeSite(11);

    // Check again.
    CPPUNIT_ASSERT( process.isListed(1234) );
    CPPUNIT_ASSERT( !process.isListed(3)   );
    CPPUNIT_ASSERT( !process.isListed(11)  );
    CPPUNIT_ASSERT( process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 2);

    const double rtot1 = r0 + r3;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(process.totalRate(), rtot1, 1.0e-12);

    // Remove the remaining two indices.
    process.removeSite(-123);
    process.removeSite(1234);

    // Check.
    CPPUNIT_ASSERT( !process.isListed(1234) );
    CPPUNIT_ASSERT( !process.isListed(3)    );
    CPPUNIT_ASSERT( !process.isListed(11)   );
    CPPUNIT_ASSERT( !process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    const double rtot2 = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(process.totalRate(), rtot2, 1.0e-12);

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testPickSite()
{
    // Default construct a process.
    CustomRateProcess process;

    // Add sites.
    process.addSite(199, 2.00, 1.0);
    process.addSite(12,  5.00, 1.0);
    process.addSite(19,  3.00, 1.0);
    process.updateRateTable();

    // Get the cite.
    int counter12  = 0;
    int counter19  = 0;
    int counter199 = 0;

    seedRandom(false, 97);
    const int n_loop = 1000000;

    for (int i = 0; i < n_loop; ++i)
    {
        const int site = process.pickSite();
        CPPUNIT_ASSERT( ! (site != 12 && site != 199 && site != 19) );

        // Count how often each gets selected.
        if (site == 12)
        {
            ++counter12;
        }

        if (site == 199)
        {
            ++counter199;
        }

        if (site == 19)
        {
            ++counter19;
        }
    }

    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0/10.0, 1.0 * counter199 / n_loop,  1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/2.0 , 1.0 * counter12  / n_loop,  1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0/10.0, 1.0 * counter19  / n_loop,  1.0e-2);

}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testPickSiteMultiplicity()
{
    // Default construct a process.
    CustomRateProcess process;

    // Add sites.
    process.addSite(199, 2.00, 3.0);
    process.addSite(12,  5.00, 5.0);
    process.addSite(19,  3.00, 7.0);
    process.updateRateTable();

    // Get the cite.
    int counter12  = 0;
    int counter19  = 0;
    int counter199 = 0;

    seedRandom(false, 97);
    const int n_loop = 1000000;

    for (int i = 0; i < n_loop; ++i)
    {
        const int site = process.pickSite();
        CPPUNIT_ASSERT( ! (site != 12 && site != 199 && site != 19) );

        // Count how often each gets selected.
        if (site == 12)
        {
            ++counter12;
        }

        if (site == 199)
        {
            ++counter199;
        }

        if (site == 19)
        {
            ++counter19;
        }
    }

    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.0/52.0, 1.0 * counter199 / n_loop,  1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(25.0/52.0, 1.0 * counter12  / n_loop,  1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(21.0/52.0, 1.0 * counter19  / n_loop,  1.0e-2);

}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testAffectedIndices()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    CustomRateProcess process(config1, config2, rate, basis_sites, 1.0);

    // Check that the size of the affected is correct.
    size_t one = 1;
    CPPUNIT_ASSERT_EQUAL( process.affectedIndices().size(), one );

    // Make sure we can add at some position.
    process.affectedIndices()[1] = 123;

    // And access.
    CPPUNIT_ASSERT_EQUAL( process.affectedIndices()[1], 123 );

}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testCutoffAndRange()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    CustomRateProcess process(config1, config2, rate, basis_sites, 1.0);

    // Test that the cutoff is set by the base class to the longest distance.
    const double d = std::sqrt(std::pow(1.0,2) + std::pow(1.3,2) + std::pow(-4.4,2));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( process.cutoff(), d, 1.0e-12 );
    CPPUNIT_ASSERT_EQUAL( process.range(), static_cast<int>(process.cutoff()) );

    // Get a new process with longer cutoff.
    CustomRateProcess process1(config1, config2, rate, basis_sites, 124.0);

    // Check that the cutoff was set accordingly.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( process1.cutoff(), 124.0, 1.0e-12 );
    CPPUNIT_ASSERT_EQUAL( process.range(), static_cast<int>(process.cutoff()) );

}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testProcessNumber()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Setup an empty move vector.
    const std::vector<int> move_origins(0);
    const std::vector<Coordinate> move_vectors(0);

    // Give the process its number.
    const int p_number = 198;

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    CustomRateProcess process(config1,
                              config2,
                              rate,
                              basis_sites,
                              2.0,
                              move_origins,
                              move_vectors,
                              p_number);

    // Check that the number that is returned is the same.
    CPPUNIT_ASSERT_EQUAL( process.processNumber(), p_number );

    // Construct another process without the process number.
    CustomRateProcess process2(config1,
                               config2,
                               rate,
                               basis_sites,
                               2.0,
                               move_origins,
                               move_vectors);

    // Check that the process number is -1 as default.
    CPPUNIT_ASSERT_EQUAL( process2.processNumber(), -1 );

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_CustomRateProcess::testCacheRate()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::vector<std::string> > elements1;
    elements1.push_back(std::vector<std::string>(1, "A"));
    elements1.push_back(std::vector<std::string>(1, "B"));

    std::vector<std::vector<std::string> > elements2;
    elements2.push_back(std::vector<std::string>(1, "C"));
    elements2.push_back(std::vector<std::string>(1, "B"));

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Setup an empty move vector.
    const std::vector<int> move_origins(0);
    const std::vector<Coordinate> move_vectors(0);

    // Give the process its number.
    const int p_number = 198;

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    const CustomRateProcess p0(config1,
                               config2,
                               rate,
                               basis_sites,
                               2.0,
                               move_origins,
                               move_vectors,
                               p_number);

    // Check that the default behavior is no caching.
    CPPUNIT_ASSERT( !p0.cacheRate() );

    // Construct with explicit values and check again.
    const CustomRateProcess p1(config1,
                               config2,
                               rate,
                               basis_sites,
                               2.0,
                               move_origins,
                               move_vectors,
                               p_number,
                               false);
    CPPUNIT_ASSERT( !p1.cacheRate() );

    const CustomRateProcess p2(config1,
                               config2,
                               rate,
                               basis_sites,
                               2.0,
                               move_origins,
                               move_vectors,
                               p_number,
                               true);
    CPPUNIT_ASSERT( p2.cacheRate() );

    // DONE
}
