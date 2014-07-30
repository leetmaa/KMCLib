/*
  Copyright (c)  2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_ontheflymsd.h"

// Include the files to test.
#include "ontheflymsd.h"

#include "configuration.h"
#include "latticemap.h"
#include "process.h"


// -------------------------------------------------------------------------- //
//
void Test_OnTheFlyMSD::testConstruction()
{
    // Setup a configuration.
    std::vector<std::vector<double> > coordinates(2, std::vector<double>(3, 0.0));
    coordinates[1][0] = 1.0;
    coordinates[1][1] = 2.0;
    coordinates[1][2] = 3.0;
    const std::vector<std::vector<std::string> > elements(2, std::vector<std::string>(1, "AA"));
    std::map<std::string,int>  possible_types;
    possible_types["*"]  = 0;
    possible_types["AA"] = 1;
    possible_types["B"]  = 2;
    Configuration configuration(coordinates, elements, possible_types);

    // Set the other input parameteres.
    const int history_steps = 100;
    const int n_bins = 200;
    const double t_max = 300.0;
    const double t0 = 1.23;
    const std::string track_type("AA");

    std::vector<Coordinate> abc_to_xyz;
    abc_to_xyz.push_back(Coordinate(1.0, 0.0, 0.0));
    abc_to_xyz.push_back(Coordinate(0.5, 1.0, 0.0));
    abc_to_xyz.push_back(Coordinate(0.0, 0.0, 1.0));

    // Construct.
    OnTheFlyMSD msd(configuration,
                    history_steps,
                    n_bins,
                    t_max,
                    t0,
                    track_type,
                    abc_to_xyz);

    // Check that the setup was completed correctly.
    const std::vector< std::vector< std::pair<Coordinate, double> > > & history_buffer = msd.historyBuffer();

    for (size_t i = 0; i < history_buffer.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[i][0].first.x(), coordinates[i][0], 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[i][0].first.y(), coordinates[i][1], 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[i][0].first.z(), coordinates[i][2], 1.0e-12);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[i][0].second, t0, 1.0e-12);
    }
}


// -------------------------------------------------------------------------- //
//
void Test_OnTheFlyMSD::testStepX()
{
    // Setup a minimal realistic system.
    std::vector< std::vector<double> > basis(1, std::vector<double>(3,0.0));

    // Make a 6x1x1x1 structure.
    const int nI = 6;
    const int nJ = 1;
    const int nK = 1;
    const int nB = 1;

    // Coordinates and elements.
    std::vector<std::vector<double> > coordinates;
    std::vector<std::vector<std::string> > elements;

    for (int i = 0; i < nI; ++i)
    {
        std::vector<double> c(3, 0.0);
        c[0] = static_cast<double>(i);
        coordinates.push_back(c);
        elements.push_back(std::vector<std::string>(1, "A"));
    }
    elements[0] = std::vector<std::string>(1, "V");
    elements[2] = std::vector<std::string>(1, "V");
    elements[4] = std::vector<std::string>(1, "V");

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    // Setup the configuration.
    Configuration configuration(coordinates, elements, possible_types);

    // Setup the lattice map.
    std::vector<int> repetitions(3);
    repetitions[0] = nI;
    repetitions[1] = nJ;
    repetitions[2] = nK;
    std::vector<bool> periodicity(3, false);
    periodicity[0] = true;

    LatticeMap lattice_map(nB, repetitions, periodicity);

    // Init the match lists.
    configuration.initMatchLists(lattice_map, 1);

    // Setup a process that moves a V to the left.
    std::vector<std::vector<std::string> > process_elements1(3);
    process_elements1[0] = std::vector<std::string>(1, "V");
    process_elements1[1] = std::vector<std::string>(1, "A");
    process_elements1[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<std::string> > process_elements2(3);
    process_elements2[0] = std::vector<std::string>(1, "A");
    process_elements2[1] = std::vector<std::string>(1, "V");
    process_elements2[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<double> > process_coordinates(3, std::vector<double>(3, 0.0));
    process_coordinates[1][0] = -1.0;
    process_coordinates[1][1] =  0.0;
    process_coordinates[1][2] =  0.0;
    process_coordinates[2][0] =  1.0;
    process_coordinates[2][1] =  0.0;
    process_coordinates[2][2] =  0.0;

    std::vector<Coordinate> move_vectors;
    move_vectors.push_back( Coordinate(-1.0,  0.0,  0.0) );
    move_vectors.push_back( Coordinate( 1.0,  0.0,  0.0) );

    std::vector<int> move_origins;
    move_origins.push_back(0);
    move_origins.push_back(1);

    const double rate = 13.7;
    Configuration c1(process_coordinates, process_elements1, possible_types);
    Configuration c2(process_coordinates, process_elements2, possible_types);
    std::vector<int> basis_sites(1,0);
    Process p1(c1, c2, rate, basis_sites, move_origins, move_vectors);

    // Now, add index 2 to the process.
    // We know by construction that these match.
    p1.addSite(2);

    // Construct the MSD analyser.
    const int history_steps = 5;
    const int n_bins = 200;
    const double t_max = 10.0;
    const double t0 = 1.2;
    const std::string track_type("V");

    std::vector<Coordinate> abc_to_xyz;
    abc_to_xyz.push_back(Coordinate(1.1234, 0.0, 0.0));
    abc_to_xyz.push_back(Coordinate(0.9987, 1.0, 0.0));
    abc_to_xyz.push_back(Coordinate(0.0123, 0.0, 1.0));

    OnTheFlyMSD msd(configuration,
                    history_steps,
                    n_bins,
                    t_max,
                    t0,
                    track_type,
                    abc_to_xyz);
    const std::vector<int> & atom_id = configuration.atomID();

    // Peform the process.
    configuration.performBucketProcess(p1, 2, lattice_map);

    // Check that we moved the correct indices.
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[0], 2);
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[1], 1);

    // Check the atom ids.
    CPPUNIT_ASSERT_EQUAL(atom_id[0], 0);
    CPPUNIT_ASSERT_EQUAL(atom_id[1], 2);
    CPPUNIT_ASSERT_EQUAL(atom_id[2], 1);
    CPPUNIT_ASSERT_EQUAL(atom_id[3], 3);
    CPPUNIT_ASSERT_EQUAL(atom_id[4], 4);
    CPPUNIT_ASSERT_EQUAL(atom_id[5], 5);

    // Check the atom id elements.
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[0]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[1]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[2]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[3]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[4]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[5]], std::string("A"));

    // Register the step on the msd.
    double time = 4.3;
    msd.registerStep(time, configuration);

    // Check that the data was stored correctly in the history buffer.
    const std::vector< std::vector< std::pair<Coordinate, double> > > & history_buffer = \
        msd.historyBuffer();

    // Check the size of the history buffer for the moved element.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[2].size()), 2 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][0].first.x(), 1.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][1].first.x(), 2.0, 1.0e-12 );

    // Check that the time difference is correct.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[2][0].second, time, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[2][1].second,   t0, 1.0e-12);

    // Check that the histogram was correctly updated.
    const double delta_t = time - t0;
    const double binsize = t_max / n_bins;
    const size_t bin = static_cast<size_t>(delta_t / binsize);

    const std::vector<Coordinate> & histogram = msd.histogramBuffer();
    for (size_t i = 0; i < histogram.size(); ++i)
    {
        if (bin != i)
        {
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].x(), 0.0, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].y(), 0.0, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].z(), 0.0, 1.0e-12 );
        }
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin].x(), 1.1234*1.1234, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin].y(), 0.9987*0.9987, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin].z(), 0.0123*0.0123, 1.0e-12 );

    // ---------------------------------------------------------------------

    // Apply the reverse process.
    process_elements1[0] = std::vector<std::string>(1, "A");
    process_elements1[1] = std::vector<std::string>(1, "V");
    process_elements1[2] = std::vector<std::string>(1, "A");

    process_elements2[0] = std::vector<std::string>(1, "V");
    process_elements2[1] = std::vector<std::string>(1, "A");
    process_elements2[2] = std::vector<std::string>(1, "A");

    Configuration c3(process_coordinates, process_elements1, possible_types);
    Configuration c4(process_coordinates, process_elements2, possible_types);
    Process p2(c3, c4, rate, basis_sites, move_origins, move_vectors);

    p2.addSite(2, 0.0);
    configuration.updateMatchList(2);
    configuration.performBucketProcess(p2, 2, lattice_map);
    time = 5.4;
    msd.registerStep(time, configuration);

    // Check that we moved the atoms we believe we moved.
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[0], 1);
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[1], 2);

    // Check the atom ids.
    CPPUNIT_ASSERT_EQUAL(atom_id[0], 0);
    CPPUNIT_ASSERT_EQUAL(atom_id[1], 1);
    CPPUNIT_ASSERT_EQUAL(atom_id[2], 2);
    CPPUNIT_ASSERT_EQUAL(atom_id[3], 3);
    CPPUNIT_ASSERT_EQUAL(atom_id[4], 4);
    CPPUNIT_ASSERT_EQUAL(atom_id[5], 5);

    // Check the atom id elements.
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[0]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[1]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[2]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[3]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[4]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[5]], std::string("A"));

    // Check the results.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[0].size()), 1 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[1].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[2].size()), 3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[3].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[4].size()), 1 );

    // ---------------------------------------------------------------------

    // Apply the processes to the zeroth index.
    configuration.updateMatchList(0);
    configuration.performBucketProcess(p1, 0, lattice_map);
    time += 5.2;
    msd.registerStep(time, configuration);
    configuration.updateMatchList(0);
    configuration.performBucketProcess(p2, 0, lattice_map);
    time += 1.3;
    msd.registerStep(time, configuration);

    // ---------------------------------------------------------------------

    // Apply each process again, 4 times.
    for (int i = 0; i < 4; ++i)
    {
        configuration.updateMatchList(2);
        configuration.performBucketProcess(p1, 2, lattice_map);
        time += 5.2;
        msd.registerStep(time, configuration);
        configuration.updateMatchList(2);
        configuration.performBucketProcess(p2, 2, lattice_map);
        time += 1.3;
        msd.registerStep(time, configuration);
    }

    // We should be back to the initial configuration.
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[0]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[1]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[2]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[3]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[4]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[5]], std::string("A"));

    // Check the results.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[0].size()), 3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[1].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[2].size()), 5 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[3].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[4].size()), 1 );

    // The full time series we have taken is:
    // t0   t1   t2   t3   t4   t5   t6   t7   t8   t9   t10  t11  t12
    // 1.2, 4.3, 5.4, 10.6,11.9,17.1,18.4,23.6,24.9,30.1,31.4,36.6,37.9

    // The last four of these should be in the history buffer for id 2.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][3].second, 30.1, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][2].second, 31.4, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][1].second, 36.6, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][0].second, 37.9, 1.0e-12 );

}


// -------------------------------------------------------------------------- //
//
void Test_OnTheFlyMSD::testStepY()
{
    // Setup a minimal realistic system.
    std::vector< std::vector<double> > basis(1, std::vector<double>(3,0.0));

    // Make a 13x11 structure.
    const int nI = 1;
    const int nJ = 6;
    const int nK = 1;
    const int nB = 1;

    // Coordinates and elements.
    std::vector<std::vector<double> > coordinates;
    std::vector<std::vector<std::string> > elements;

    for (int i = 0; i < nJ; ++i)
    {
        std::vector<double> c(3, 0.0);
        c[1] = static_cast<double>(i);
        coordinates.push_back(c);
        elements.push_back(std::vector<std::string>(1, "A"));
    }
    elements[0] = std::vector<std::string>(1, "V");
    elements[2] = std::vector<std::string>(1, "V");
    elements[4] = std::vector<std::string>(1, "V");

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    // Setup the configuration.
    Configuration configuration(coordinates, elements, possible_types);

    // Setup the lattice map.
    std::vector<int> repetitions(3);
    repetitions[0] = nI;
    repetitions[1] = nJ;
    repetitions[2] = nK;
    std::vector<bool> periodicity(3, false);
    periodicity[1] = true;

    LatticeMap lattice_map(nB, repetitions, periodicity);

    // Init the match lists.
    configuration.initMatchLists(lattice_map, 1);

    // Setup a process that moves a V to the left.
    std::vector<std::vector<std::string> > process_elements1(3);
    process_elements1[0] = std::vector<std::string>(1, "V");
    process_elements1[1] = std::vector<std::string>(1, "A");
    process_elements1[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<std::string> > process_elements2(3);
    process_elements2[0] = std::vector<std::string>(1, "A");
    process_elements2[1] = std::vector<std::string>(1, "V");
    process_elements2[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<double> > process_coordinates(3, std::vector<double>(3, 0.0));
    process_coordinates[1][0] =  0.0;
    process_coordinates[1][1] = -1.0;
    process_coordinates[1][2] =  0.0;
    process_coordinates[2][0] =  0.0;
    process_coordinates[2][1] =  1.0;
    process_coordinates[2][2] =  0.0;

    std::vector<Coordinate> move_vectors;
    move_vectors.push_back( Coordinate( 0.0, -1.0,  0.0) );
    move_vectors.push_back( Coordinate( 0.0,  1.0,  0.0) );

    std::vector<int> move_origins;
    move_origins.push_back(0);
    move_origins.push_back(1);

    const double rate = 13.7;
    Configuration c1(process_coordinates, process_elements1, possible_types);
    Configuration c2(process_coordinates, process_elements2, possible_types);
    std::vector<int> basis_sites(1,0);
    Process p1(c1, c2, rate, basis_sites, move_origins, move_vectors);

    // Now, add index 2 to the process.
    // We know by construction that these match.
    p1.addSite(2);

    // Construct the MSD analyser.
    const int history_steps = 5;
    const int n_bins = 200;
    const double t_max = 10.0;
    const double t0 = 1.2;
    const std::string track_type("V");

    std::vector<Coordinate> abc_to_xyz;
    abc_to_xyz.push_back(Coordinate(1.0, 0.11, 0.0));
    abc_to_xyz.push_back(Coordinate(0.0, 11.1, 0.0));
    abc_to_xyz.push_back(Coordinate(0.0, 1.1, 1.0));

    OnTheFlyMSD msd(configuration,
                    history_steps,
                    n_bins,
                    t_max,
                    t0,
                    track_type,
                    abc_to_xyz);
    const std::vector<int> & atom_id = configuration.atomID();

    // Peform the process.
    configuration.performBucketProcess(p1, 2, lattice_map);

    // Check that we moved the correct indices.
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[0], 2);
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[1], 1);

    // Check the atom ids.
    CPPUNIT_ASSERT_EQUAL(atom_id[0], 0);
    CPPUNIT_ASSERT_EQUAL(atom_id[1], 2);
    CPPUNIT_ASSERT_EQUAL(atom_id[2], 1);
    CPPUNIT_ASSERT_EQUAL(atom_id[3], 3);
    CPPUNIT_ASSERT_EQUAL(atom_id[4], 4);
    CPPUNIT_ASSERT_EQUAL(atom_id[5], 5);

    // Check the atom id elements.
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[0]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[1]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[2]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[3]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[4]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[5]], std::string("A"));

    // Register the step on the msd.
    double time = 4.3;
    msd.registerStep(time, configuration);

    // Check that the data was stored correctly in the history buffer.
    const std::vector< std::vector< std::pair<Coordinate, double> > > & history_buffer = \
        msd.historyBuffer();

    // Check the size of the history buffer for the moved element.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[2].size()), 2 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][0].first.y(), 1.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][1].first.y(), 2.0, 1.0e-12 );

    // Check that the time difference is correct.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[2][0].second, time, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[2][1].second,   t0, 1.0e-12);

    // Check that the histogram was correctly updated.
    const double delta_t = time - t0;
    const double binsize = t_max / n_bins;
    const size_t bin = static_cast<size_t>(delta_t / binsize);

    const std::vector<Coordinate> & histogram = msd.histogramBuffer();
    for (size_t i = 0; i < histogram.size(); ++i)
    {
        if (bin != i)
        {
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].x(), 0.0, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].y(), 0.0, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].z(), 0.0, 1.0e-12 );
        }
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin].x(), 0.11*0.11, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin].y(), 11.1*11.1, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin].z(), 1.1*1.1,   1.0e-12 );

    // ---------------------------------------------------------------------

    // Apply the reverse process.
    process_elements1[0] = std::vector<std::string>(1, "A");
    process_elements1[1] = std::vector<std::string>(1, "V");
    process_elements1[2] = std::vector<std::string>(1, "A");

    process_elements2[0] = std::vector<std::string>(1, "V");
    process_elements2[1] = std::vector<std::string>(1, "A");
    process_elements2[2] = std::vector<std::string>(1, "A");

    Configuration c3(process_coordinates, process_elements1, possible_types);
    Configuration c4(process_coordinates, process_elements2, possible_types);
    Process p2(c3, c4, rate, basis_sites, move_origins, move_vectors);

    p2.addSite(2, 0.0);
    configuration.updateMatchList(2);
    configuration.performBucketProcess(p2, 2, lattice_map);
    time = 5.4;
    msd.registerStep(time, configuration);

    // Check that we moved the atoms we believe we moved.
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[0], 1);
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[1], 2);

    // Check the atom ids.
    CPPUNIT_ASSERT_EQUAL(atom_id[0], 0);
    CPPUNIT_ASSERT_EQUAL(atom_id[1], 1);
    CPPUNIT_ASSERT_EQUAL(atom_id[2], 2);
    CPPUNIT_ASSERT_EQUAL(atom_id[3], 3);
    CPPUNIT_ASSERT_EQUAL(atom_id[4], 4);
    CPPUNIT_ASSERT_EQUAL(atom_id[5], 5);

    // Check the atom id elements.
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[0]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[1]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[2]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[3]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[4]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[5]], std::string("A"));

    // Check the results.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[0].size()), 1 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[1].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[2].size()), 3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[3].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[4].size()), 1 );

    // ---------------------------------------------------------------------

    // Apply the processes to the zeroth index.
    configuration.updateMatchList(0);
    configuration.performBucketProcess(p1, 0, lattice_map);
    time += 5.2;
    msd.registerStep(time, configuration);
    configuration.updateMatchList(0);
    configuration.performBucketProcess(p2, 0, lattice_map);
    time += 1.3;
    msd.registerStep(time, configuration);

    // ---------------------------------------------------------------------

    // Apply each process again, 4 times.
    for (int i = 0; i < 4; ++i)
    {
        configuration.updateMatchList(2);
        configuration.performBucketProcess(p1, 2, lattice_map);
        time += 5.2;
        msd.registerStep(time, configuration);
        configuration.updateMatchList(2);
        configuration.performBucketProcess(p2, 2, lattice_map);
        time += 1.3;
        msd.registerStep(time, configuration);
    }

    // We should be back to the initial configuration.
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[0]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[1]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[2]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[3]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[4]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[5]], std::string("A"));

    // Check the results.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[0].size()), 3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[1].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[2].size()), 5 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[3].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[4].size()), 1 );

    // The full time series we have taken is:
    // t0   t1   t2   t3   t4   t5   t6   t7   t8   t9   t10  t11  t12
    // 1.2, 4.3, 5.4, 10.6,11.9,17.1,18.4,23.6,24.9,30.1,31.4,36.6,37.9

    // The last four of these should be in the history buffer for id 2.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][3].second, 30.1, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][2].second, 31.4, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][1].second, 36.6, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][0].second, 37.9, 1.0e-12 );

}

// -------------------------------------------------------------------------- //
//
void Test_OnTheFlyMSD::testStepZ()
{
    // Setup a minimal realistic system.
    std::vector< std::vector<double> > basis(1, std::vector<double>(3,0.0));

    // Make a 13x11 structure.
    const int nI = 1;
    const int nJ = 1;
    const int nK = 6;
    const int nB = 1;

    // Coordinates and elements.
    std::vector<std::vector<double> > coordinates;
    std::vector<std::vector<std::string> > elements;

    for (int i = 0; i < nK; ++i)
    {
        std::vector<double> c(3, 0.0);
        c[2] = static_cast<double>(i);
        coordinates.push_back(c);
        elements.push_back(std::vector<std::string>(1, "A"));
    }
    elements[0] = std::vector<std::string>(1, "V");
    elements[2] = std::vector<std::string>(1, "V");
    elements[4] = std::vector<std::string>(1, "V");

    // Possible types.
    std::map<std::string, int> possible_types;
    possible_types["*"] = 0;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["V"] = 3;

    // Setup the configuration.
    Configuration configuration(coordinates, elements, possible_types);

    // Setup the lattice map.
    std::vector<int> repetitions(3);
    repetitions[0] = nI;
    repetitions[1] = nJ;
    repetitions[2] = nK;
    std::vector<bool> periodicity(3, false);
    periodicity[2] = true;

    LatticeMap lattice_map(nB, repetitions, periodicity);

    // Init the match lists.
    configuration.initMatchLists(lattice_map, 1);

    // Setup a process that moves a V to the left.
    std::vector<std::vector<std::string> > process_elements1(3);
    process_elements1[0] = std::vector<std::string>(1, "V");
    process_elements1[1] = std::vector<std::string>(1, "A");
    process_elements1[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<std::string> > process_elements2(3);
    process_elements2[0] = std::vector<std::string>(1, "A");
    process_elements2[1] = std::vector<std::string>(1, "V");
    process_elements2[2] = std::vector<std::string>(1, "A");

    std::vector<std::vector<double> > process_coordinates(3, std::vector<double>(3, 0.0));
    process_coordinates[1][0] =  0.0;
    process_coordinates[1][1] =  0.0;
    process_coordinates[1][2] = -1.0;
    process_coordinates[2][0] =  0.0;
    process_coordinates[2][1] =  0.0;
    process_coordinates[2][2] =  1.0;

    std::vector<Coordinate> move_vectors;
    move_vectors.push_back( Coordinate( 0.0,  0.0, -1.0) );
    move_vectors.push_back( Coordinate( 0.0,  0.0,  1.0) );

    std::vector<int> move_origins;
    move_origins.push_back(0);
    move_origins.push_back(1);

    const double rate = 13.7;
    Configuration c1(process_coordinates, process_elements1, possible_types);
    Configuration c2(process_coordinates, process_elements2, possible_types);
    std::vector<int> basis_sites(1,0);
    Process p1(c1, c2, rate, basis_sites, move_origins, move_vectors);

    // Now, add index 2 to the process.
    // We know by construction that these match.
    p1.addSite(2);

    // Construct the MSD analyser.
    const int history_steps = 5;
    const int n_bins = 200;
    const double t_max = 10.0;
    const double t0 = 1.2;
    const std::string track_type("V");

    std::vector<Coordinate> abc_to_xyz;
    abc_to_xyz.push_back(Coordinate(1.0, 0.0,12.0));
    abc_to_xyz.push_back(Coordinate(0.0, 1.0, 3.0));
    abc_to_xyz.push_back(Coordinate(0.0, 0.0, 1.3));

    OnTheFlyMSD msd(configuration,
                    history_steps,
                    n_bins,
                    t_max,
                    t0,
                    track_type,
                    abc_to_xyz);
    const std::vector<int> & atom_id = configuration.atomID();

    // Peform the process.
    configuration.performBucketProcess(p1, 2, lattice_map);

    // Check that we moved the correct indices.
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[0], 2);
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[1], 1);

    // Check the atom ids.
    CPPUNIT_ASSERT_EQUAL(atom_id[0], 0);
    CPPUNIT_ASSERT_EQUAL(atom_id[1], 2);
    CPPUNIT_ASSERT_EQUAL(atom_id[2], 1);
    CPPUNIT_ASSERT_EQUAL(atom_id[3], 3);
    CPPUNIT_ASSERT_EQUAL(atom_id[4], 4);
    CPPUNIT_ASSERT_EQUAL(atom_id[5], 5);

    // Check the atom id elements.
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[0]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[1]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[2]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[3]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[4]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[5]], std::string("A"));

    // Register the step on the msd.
    double time = 4.3;
    msd.registerStep(time, configuration);

    // Check that the data was stored correctly in the history buffer.
    const std::vector< std::vector< std::pair<Coordinate, double> > > & history_buffer = \
        msd.historyBuffer();

    // Check the size of the history buffer for the moved element.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[2].size()), 2 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][0].first.z(), 1.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][1].first.z(), 2.0, 1.0e-12 );

    // Check that the time difference is correct.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[2][0].second, time, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(history_buffer[2][1].second,   t0, 1.0e-12);

    // Check that the histogram was correctly updated.
    const double delta_t = time - t0;
    const double binsize = t_max / n_bins;
    const size_t bin = static_cast<size_t>(delta_t / binsize);

    const std::vector<Coordinate> & histogram = msd.histogramBuffer();
    for (size_t i = 0; i < histogram.size(); ++i)
    {
        if (bin != i)
        {
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].x(), 0.0, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].y(), 0.0, 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].z(), 0.0, 1.0e-12 );
        }
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin].x(), 12.0*12.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin].y(), 3.0*3.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin].z(), 1.3*1.3, 1.0e-12 );

    // ---------------------------------------------------------------------

    // Apply the reverse process.
    process_elements1[0] = std::vector<std::string>(1, "A");
    process_elements1[1] = std::vector<std::string>(1, "V");
    process_elements1[2] = std::vector<std::string>(1, "A");

    process_elements2[0] = std::vector<std::string>(1, "V");
    process_elements2[1] = std::vector<std::string>(1, "A");
    process_elements2[2] = std::vector<std::string>(1, "A");

    Configuration c3(process_coordinates, process_elements1, possible_types);
    Configuration c4(process_coordinates, process_elements2, possible_types);
    Process p2(c3, c4, rate, basis_sites, move_origins, move_vectors);

    p2.addSite(2, 0.0);
    configuration.updateMatchList(2);
    configuration.performBucketProcess(p2, 2, lattice_map);
    time = 5.4;
    msd.registerStep(time, configuration);

    // Check that we moved the atoms we believe we moved.
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[0], 1);
    CPPUNIT_ASSERT_EQUAL(configuration.movedAtomIDs()[1], 2);

    // Check the atom ids.
    CPPUNIT_ASSERT_EQUAL(atom_id[0], 0);
    CPPUNIT_ASSERT_EQUAL(atom_id[1], 1);
    CPPUNIT_ASSERT_EQUAL(atom_id[2], 2);
    CPPUNIT_ASSERT_EQUAL(atom_id[3], 3);
    CPPUNIT_ASSERT_EQUAL(atom_id[4], 4);
    CPPUNIT_ASSERT_EQUAL(atom_id[5], 5);

    // Check the atom id elements.
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[0]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[1]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[2]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[3]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[4]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[5]], std::string("A"));

    // Check the results.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[0].size()), 1 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[1].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[2].size()), 3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[3].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[4].size()), 1 );

    // ---------------------------------------------------------------------

    // Apply the processes to the zeroth index.
    configuration.updateMatchList(0);
    configuration.performBucketProcess(p1, 0, lattice_map);
    time += 5.2;
    msd.registerStep(time, configuration);
    configuration.updateMatchList(0);
    configuration.performBucketProcess(p2, 0, lattice_map);
    time += 1.3;
    msd.registerStep(time, configuration);

    // ---------------------------------------------------------------------

    // Apply each process again, 4 times.
    for (int i = 0; i < 4; ++i)
    {
        configuration.updateMatchList(2);
        configuration.performBucketProcess(p1, 2, lattice_map);
        time += 5.2;
        msd.registerStep(time, configuration);
        configuration.updateMatchList(2);
        configuration.performBucketProcess(p2, 2, lattice_map);
        time += 1.3;
        msd.registerStep(time, configuration);
    }

    // We should be back to the initial configuration.
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[0]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[1]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[2]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[3]], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[4]], std::string("V"));
    CPPUNIT_ASSERT_EQUAL(configuration.atomIDElements()[atom_id[5]], std::string("A"));

    // Check the results.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[0].size()), 3 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[1].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[2].size()), 5 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[3].size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(history_buffer[4].size()), 1 );

    // The full time series we have taken is:
    // t0   t1   t2   t3   t4   t5   t6   t7   t8   t9   t10  t11  t12
    // 1.2, 4.3, 5.4, 10.6,11.9,17.1,18.4,23.6,24.9,30.1,31.4,36.6,37.9

    // The last four of these should be in the history buffer for id 2.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][3].second, 30.1, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][2].second, 31.4, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][1].second, 36.6, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( history_buffer[2][0].second, 37.9, 1.0e-12 );

}


// -------------------------------------------------------------------------- //
//
void Test_OnTheFlyMSD::testCalculateAndBinMSD()
{
    // Setup input.
    std::vector< std::pair<Coordinate, double> > history;
    history.push_back(std::pair<Coordinate, double>(Coordinate( 1.1, 1.3, 7.8), 7.3));
    history.push_back(std::pair<Coordinate, double>(Coordinate( 1.3, 1.9, 7.6), 6.3));
    history.push_back(std::pair<Coordinate, double>(Coordinate(-0.2, 1.0,-4.6), 2.1));
    history.push_back(std::pair<Coordinate, double>(Coordinate( 1.0,-1.8, 3.5), 1.3));
    history.push_back(std::pair<Coordinate, double>(Coordinate( 2.1, 1.3,-5.5), 0.3));

    // The histogram and histogram squared, filled with some initial values.
    const int h_size = 6;

    const Coordinate h_start(2.2, 3.1, 1.4);
    std::vector<Coordinate> histogram(h_size, h_start);

    const Coordinate h_sqr_start(4.48, 9.61, 1.96);
    std::vector<Coordinate> histogram_sqr(h_size, h_sqr_start);

    std::vector<int> bin_counters(h_size, 9);
    std::vector< std::vector<int> > hsteps_bin_counters(history.size()-1, std::vector<int>(h_size, 8));
    std::vector<int> hstep_counts(history.size()-1, 2);

    // A blocker with zero block size and the correct number of bins.
    Blocker blocker(h_size, 2);

    // The bin size.
    const double binsize = 1.0;

    // The unit transformation matrix.
    std::vector<Coordinate> transformation(3);
    transformation[0] = Coordinate(1.0, 0.0, 0.0);
    transformation[1] = Coordinate(0.0, 1.0, 0.0);
    transformation[2] = Coordinate(0.0, 0.0, 1.0);

    // Call the binning function with this data and a unit transformation matrix.
    calculateAndBinMSD(history,
                       transformation,
                       binsize,
                       histogram,
                       histogram_sqr,
                       bin_counters,
                       hsteps_bin_counters,
                       hstep_counts,
                       blocker);

    // Calculate the results by hand.
    Coordinate diff = history[0].first - history[1].first;
    const Coordinate diff_0(diff.x()*diff.x(),
                            diff.y()*diff.y(),
                            diff.z()*diff.z());
    const Coordinate diff_0_sqr(diff_0.x()*diff_0.x(),
                                diff_0.y()*diff_0.y(),
                                diff_0.z()*diff_0.z());
    const double dt_0 = history[0].second - history[1].second;
    const int bin_0 = static_cast<size_t>(dt_0 / binsize);

    // This should result in bin 1.
    CPPUNIT_ASSERT_EQUAL( bin_0, 1 );

    diff = history[0].first - history[2].first;
    const Coordinate diff_1(diff.x()*diff.x(),
                            diff.y()*diff.y(),
                            diff.z()*diff.z());
    const Coordinate diff_1_sqr(diff_1.x()*diff_1.x(),
                                diff_1.y()*diff_1.y(),
                                diff_1.z()*diff_1.z());
    const double dt_1 = history[0].second - history[2].second;
    const int bin_1 = static_cast<size_t>(dt_1 / binsize);

    // This should result in bin 5.
    CPPUNIT_ASSERT_EQUAL( bin_1, 5 );

    const double dt_2 = history[0].second - history[3].second;
    const int bin_2 = static_cast<size_t>(dt_2 / binsize);

    // This should result in bin 6.
    CPPUNIT_ASSERT_EQUAL( bin_2, 6 );

    const double dt_3 = history[0].second - history[4].second;
    const int bin_3 = static_cast<size_t>(dt_3 / binsize);

    // This should result in bin 7.
    CPPUNIT_ASSERT_EQUAL( bin_3, 7 );

    // Check the results.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_0].x(), h_start.x() + diff_0.x(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_0].y(), h_start.y() + diff_0.y(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_0].z(), h_start.z() + diff_0.z(), 1.0e-12 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_0].x(), h_sqr_start.x() + diff_0_sqr.x(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_0].y(), h_sqr_start.y() + diff_0_sqr.y(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_0].z(), h_sqr_start.z() + diff_0_sqr.z(), 1.0e-12 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_1].x(), h_start.x() + diff_1.x(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_1].y(), h_start.y() + diff_1.y(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_1].z(), h_start.z() + diff_1.z(), 1.0e-12 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_1].x(), h_sqr_start.x() + diff_1_sqr.x(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_1].y(), h_sqr_start.y() + diff_1_sqr.y(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_1].z(), h_sqr_start.z() + diff_1_sqr.z(), 1.0e-12 );

    for (int i = 0; i < h_size; ++i)
    {
        if (i != bin_0 && i != bin_1)
        {
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].x(), h_start.x(), 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].y(), h_start.y(), 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].z(), h_start.z(), 1.0e-12 );

            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[i].x(), h_sqr_start.x(), 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[i].y(), h_sqr_start.y(), 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[i].z(), h_sqr_start.z(), 1.0e-12 );
        }
    }

    // Check the bin counts.
    CPPUNIT_ASSERT_EQUAL( bin_counters[bin_0], 10 );
    CPPUNIT_ASSERT_EQUAL( bin_counters[bin_1], 10 );

    // Check the bin counts splitted up on history steps.
    CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[0][bin_0], 9 );
    CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[1][bin_1], 9 );

    for (int i = 0; i < h_size; ++i)
    {
        if (i != bin_0)
        {
            CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[0][i], 8 );
        }

        if (i != bin_1)
        {
            CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[1][i], 8 );
        }

        CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[2][i], 8 );
        CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[3][i], 8 );
    }

    // Check the h-step counts.
    CPPUNIT_ASSERT_EQUAL( hstep_counts[0], 3 );
    CPPUNIT_ASSERT_EQUAL( hstep_counts[1], 3 );
    CPPUNIT_ASSERT_EQUAL( hstep_counts[2], 3 );
    CPPUNIT_ASSERT_EQUAL( hstep_counts[3], 3 );

    // Check the content of the blocker.
    std::vector< std::vector<Coordinate> > hst_blocks = blocker.hstBlocks();

    // Check the size.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(hst_blocks.size()), 6);
    for (size_t i = 0; i < hst_blocks.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(hst_blocks[i].size()), 0);
    }

    // Call the calc and bin again.
    calculateAndBinMSD(history,
                       transformation,
                       binsize,
                       histogram,
                       histogram_sqr,
                       bin_counters,
                       hsteps_bin_counters,
                       hstep_counts,
                       blocker);

    // Check the content. We should now have steps registred.
    hst_blocks = blocker.hstBlocks();

    for (int i = 0; i < static_cast<int>(hst_blocks.size()); ++i)
    {
        if (i != bin_0 && i != bin_1)
        {
            CPPUNIT_ASSERT_EQUAL(static_cast<int>(hst_blocks[i].size()), 0);
        }
    }

    CPPUNIT_ASSERT_EQUAL(static_cast<int>(hst_blocks[bin_0].size()), 1);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(hst_blocks[bin_1].size()), 1);
}


// -------------------------------------------------------------------------- //
//
void Test_OnTheFlyMSD::testCalculateAndBinMSDTransformation()
{
    // Setup input.
    std::vector< std::pair<Coordinate, double> > history;
    history.push_back(std::pair<Coordinate, double>(Coordinate( 1.1, 1.3, 7.8), 7.3));
    history.push_back(std::pair<Coordinate, double>(Coordinate( 1.3, 1.9, 7.6), 6.3));
    history.push_back(std::pair<Coordinate, double>(Coordinate(-0.2, 1.0,-4.6), 2.1));
    history.push_back(std::pair<Coordinate, double>(Coordinate( 1.0,-1.8, 3.5), 1.3));
    history.push_back(std::pair<Coordinate, double>(Coordinate( 2.1, 1.3,-5.5), 0.3));

    // The histogram and histogram squared, filled with some initial values.
    const int h_size = 6;

    const Coordinate h_start(2.2, 3.1, 1.4);
    std::vector<Coordinate> histogram(h_size, h_start);

    const Coordinate h_sqr_start(4.48, 9.61, 1.96);
    std::vector<Coordinate> histogram_sqr(h_size, h_sqr_start);

    std::vector<int> bin_counters(h_size, 9);
    std::vector< std::vector<int> > hsteps_bin_counters(history.size()-1, std::vector<int>(h_size, 8));
    std::vector<int> hstep_counts(history.size()-1, 2);

    // A blocker with zero block size and the correct number of bins.
    Blocker blocker(h_size, 0);

    // The bin size.
    const double binsize = 1.0;

    // The non-trivial transformation matrix.
    std::vector<Coordinate> transformation(3);
    transformation[0] = Coordinate(13.4,   1.13,  0.9 );
    transformation[1] = Coordinate( 0.6,   14.2,  0.01);
    transformation[2] = Coordinate( 0.1,   0.02,  11.0 );

    // Call the binning function.
    calculateAndBinMSD(history,
                       transformation,
                       binsize,
                       histogram,
                       histogram_sqr,
                       bin_counters,
                       hsteps_bin_counters,
                       hstep_counts,
                       blocker);

    // Calculate the results by hand including the transformation.
    Coordinate diff_abc = history[0].first - history[1].first;

    // Transform.
    Coordinate diff(diff_abc.x() * transformation[0].x() +
                    diff_abc.y() * transformation[0].y() +
                    diff_abc.z() * transformation[0].z(),
                    diff_abc.x() * transformation[1].x() +
                    diff_abc.y() * transformation[1].y() +
                    diff_abc.z() * transformation[1].z(),
                    diff_abc.x() * transformation[2].x() +
                    diff_abc.y() * transformation[2].y() +
                    diff_abc.z() * transformation[2].z());

    const Coordinate diff_0(diff.x()*diff.x(),
                            diff.y()*diff.y(),
                            diff.z()*diff.z());
    const Coordinate diff_0_sqr(diff_0.x()*diff_0.x(),
                                diff_0.y()*diff_0.y(),
                                diff_0.z()*diff_0.z());
    const double dt_0 = history[0].second - history[1].second;
    const int bin_0 = static_cast<size_t>(dt_0 / binsize);

    // This should result in bin 1.
    CPPUNIT_ASSERT_EQUAL( bin_0, 1 );

    diff_abc = history[0].first - history[2].first;

    // Transform.
    diff = Coordinate(diff_abc.x() * transformation[0].x() +
                      diff_abc.y() * transformation[0].y() +
                      diff_abc.z() * transformation[0].z(),
                      diff_abc.x() * transformation[1].x() +
                      diff_abc.y() * transformation[1].y() +
                      diff_abc.z() * transformation[1].z(),
                      diff_abc.x() * transformation[2].x() +
                      diff_abc.y() * transformation[2].y() +
                      diff_abc.z() * transformation[2].z());

    const Coordinate diff_1(diff.x()*diff.x(),
                            diff.y()*diff.y(),
                            diff.z()*diff.z());
    const Coordinate diff_1_sqr(diff_1.x()*diff_1.x(),
                                diff_1.y()*diff_1.y(),
                                diff_1.z()*diff_1.z());
    const double dt_1 = history[0].second - history[2].second;
    const int bin_1 = static_cast<size_t>(dt_1 / binsize);

    // This should result in bin 5.
    CPPUNIT_ASSERT_EQUAL( bin_1, 5 );

    const double dt_2 = history[0].second - history[3].second;
    const int bin_2 = static_cast<size_t>(dt_2 / binsize);

    // This should result in bin 6.
    CPPUNIT_ASSERT_EQUAL( bin_2, 6 );

    const double dt_3 = history[0].second - history[4].second;
    const int bin_3 = static_cast<size_t>(dt_3 / binsize);

    // This should result in bin 7.
    CPPUNIT_ASSERT_EQUAL( bin_3, 7 );

    // Check the results.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_0].x(), h_start.x() + diff_0.x(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_0].y(), h_start.y() + diff_0.y(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_0].z(), h_start.z() + diff_0.z(), 1.0e-12 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_0].x(), h_sqr_start.x() + diff_0_sqr.x(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_0].y(), h_sqr_start.y() + diff_0_sqr.y(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_0].z(), h_sqr_start.z() + diff_0_sqr.z(), 1.0e-12 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_1].x(), h_start.x() + diff_1.x(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_1].y(), h_start.y() + diff_1.y(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[bin_1].z(), h_start.z() + diff_1.z(), 1.0e-12 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_1].x(), h_sqr_start.x() + diff_1_sqr.x(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_1].y(), h_sqr_start.y() + diff_1_sqr.y(), 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[bin_1].z(), h_sqr_start.z() + diff_1_sqr.z(), 1.0e-12 );

    for (int i = 0; i < h_size; ++i)
    {
        if (i != bin_0 && i != bin_1)
        {
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].x(), h_start.x(), 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].y(), h_start.y(), 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[i].z(), h_start.z(), 1.0e-12 );

            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[i].x(), h_sqr_start.x(), 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[i].y(), h_sqr_start.y(), 1.0e-12 );
            CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram_sqr[i].z(), h_sqr_start.z(), 1.0e-12 );
        }
    }

    // Check the bin counts.
    CPPUNIT_ASSERT_EQUAL( bin_counters[bin_0], 10 );
    CPPUNIT_ASSERT_EQUAL( bin_counters[bin_1], 10 );

    // Check the bin counts splitted up on history steps.
    CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[0][bin_0], 9 );
    CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[1][bin_1], 9 );

    for (int i = 0; i < h_size; ++i)
    {

        if (i != bin_0)
        {
            CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[0][i], 8 );
        }

        if (i != bin_1)
        {
            CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[1][i], 8 );
        }

        CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[2][i], 8 );
        CPPUNIT_ASSERT_EQUAL( hsteps_bin_counters[3][i], 8 );
    }
}
