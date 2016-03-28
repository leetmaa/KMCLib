/*
  Copyright (c)  2014  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_typebucket.h"

// Include the files to test.
#include "typebucket.h"


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testDefaultConstruction()
{
    TypeBucket tb;
    CPPUNIT_ASSERT_EQUAL(tb.size(), 0);
}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testConstruction()
{
    TypeBucket tb(4);

    CPPUNIT_ASSERT_EQUAL(tb.size(), 4);

    CPPUNIT_ASSERT_EQUAL(tb[0], 0);
    CPPUNIT_ASSERT_EQUAL(tb[1], 0);
    CPPUNIT_ASSERT_EQUAL(tb[2], 0);
    CPPUNIT_ASSERT_EQUAL(tb[3], 0);

}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testCopyConstruction()
{
    TypeBucket t1(4);

    t1[3] = 1;
    t1[0] = 9;

    const TypeBucket t2(t1);

    CPPUNIT_ASSERT_EQUAL(t2.size(), 4);
    CPPUNIT_ASSERT_EQUAL(t2[0], 9);
    CPPUNIT_ASSERT_EQUAL(t2[1], 0);
    CPPUNIT_ASSERT_EQUAL(t2[2], 0);
    CPPUNIT_ASSERT_EQUAL(t2[3], 1);

}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testAccessOperator()
{
    // Construct and fill with values.
    TypeBucket t1(3);
    t1[0] = 4;
    t1[1] = 3;
    t1[2] = 5;

    // Check.
    CPPUNIT_ASSERT_EQUAL(t1[0], 4);
    CPPUNIT_ASSERT_EQUAL(t1[1], 3);
    CPPUNIT_ASSERT_EQUAL(t1[2], 5);

    // Access through const version.
    const TypeBucket t2(t1);

    CPPUNIT_ASSERT_EQUAL(t2[0], 4);
    CPPUNIT_ASSERT_EQUAL(t2[1], 3);
    CPPUNIT_ASSERT_EQUAL(t2[2], 5);
}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testIdentical()
{
    TypeBucket t1(2);
    t1[0] = 4;
    t1[1] = 3;

    TypeBucket t2(2);
    t2[0] = 4;
    t2[1] = 3;

    TypeBucket t3(2);
    t3[0] = 4;
    t3[1] = 0;

    // [ 0 0 0 1 0 0 0  ]    [ 0 1 0 0 0 0 0  ]


    CPPUNIT_ASSERT(  t1.identical(t2) );
    CPPUNIT_ASSERT( !t1.identical(t3) );

    CPPUNIT_ASSERT(  t2.identical(t1) );
    CPPUNIT_ASSERT( !t2.identical(t3) );

    CPPUNIT_ASSERT( !t3.identical(t1) );
    CPPUNIT_ASSERT( !t3.identical(t2) );

    // With two empty.
    const TypeBucket t4(0);
    const TypeBucket t5(0);
    CPPUNIT_ASSERT( t4.identical(t5) );
}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testEqualOperator()
{
    TypeBucket t1(2);
    t1[0] = 4;
    t1[1] = 3;

    TypeBucket t2(2);
    t2[0] = 4;
    t2[1] = 3;

    TypeBucket t3(2);
    t3[0] = 4;
    t3[1] = 0;

    CPPUNIT_ASSERT(  (t1 == t2) );
    CPPUNIT_ASSERT( !(t1 == t3) );

    CPPUNIT_ASSERT(  (t2 == t1) );
    CPPUNIT_ASSERT( !(t2 == t3) );

    CPPUNIT_ASSERT( !(t3 == t1) );
    CPPUNIT_ASSERT( !(t3 == t2) );

    // With two empty.
    const TypeBucket t4(0);
    const TypeBucket t5(0);
    CPPUNIT_ASSERT( (t4 == t5) );
}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testNotEqualOperator()
{
    TypeBucket t1(2);
    t1[0] = 4;
    t1[1] = 3;

    TypeBucket t2(2);
    t2[0] = 4;
    t2[1] = 3;

    TypeBucket t3(2);
    t3[0] = 4;
    t3[1] = 0;

    CPPUNIT_ASSERT( !(t1 != t2) );
    CPPUNIT_ASSERT(  (t1 != t3) );

    CPPUNIT_ASSERT( !(t2 != t1) );
    CPPUNIT_ASSERT(  (t2 != t3) );

    CPPUNIT_ASSERT(  (t3 != t1) );
    CPPUNIT_ASSERT(  (t3 != t2) );

    // With two empty.
    const TypeBucket t4(0);
    const TypeBucket t5(0);
    CPPUNIT_ASSERT( !(t4 != t5) );
}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testGreaterOrEqual()
{
    TypeBucket t1(2);
    t1[0] = 4;
    t1[1] = 3;

    TypeBucket t2(2);
    t2[0] = 4;
    t2[1] = 3;

    TypeBucket t3(2);
    t3[0] = 4;
    t3[1] = 0;

    TypeBucket t4(2);
    t4[0] = 5;
    t4[1] = 4;

    TypeBucket t5(2);
    t5[0] = 3;
    t5[1] = 4;

    // t1 and t2 are equal.
    CPPUNIT_ASSERT(  t1.greaterOrEqual(t2) );
    CPPUNIT_ASSERT(  t2.greaterOrEqual(t1) );

    // t1 is greater than t3.
    CPPUNIT_ASSERT(  t1.greaterOrEqual(t3) );
    CPPUNIT_ASSERT( !t3.greaterOrEqual(t1) );

    // t4 is greater than t1.
    CPPUNIT_ASSERT(  t4.greaterOrEqual(t1) );
    CPPUNIT_ASSERT( !t1.greaterOrEqual(t4) );

    // t5 is smaller than t1.
    CPPUNIT_ASSERT(  t1.greaterOrEqual(t5) );
    CPPUNIT_ASSERT( !t5.greaterOrEqual(t1) );

    // For different lengths we throw an error.
    TypeBucket t6(3);
    t6[0] = 4;
    t6[1] = 3;
    t6[2] = 3;

    CPPUNIT_ASSERT_THROW( t6.greaterOrEqual(t1), std::runtime_error );

}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testLessThan()
{
    TypeBucket t1(2);
    t1[0] = 4;
    t1[1] = 3;

    TypeBucket t2(2);
    t2[0] = 4;
    t2[1] = 3;

    TypeBucket t3(2);
    t3[0] = 4;
    t3[1] = 0;

    TypeBucket t4(2);
    t4[0] = 5;
    t4[1] = 4;

    TypeBucket t5(2);
    t5[0] = 3;
    t5[1] = 4;

    // t1 and t2 are equal.
    CPPUNIT_ASSERT( !t1.lessThan(t2) );
    CPPUNIT_ASSERT( !t2.lessThan(t1) );

    // t1 is greater than t3.
    CPPUNIT_ASSERT( !t1.lessThan(t3) );
    CPPUNIT_ASSERT(  t3.lessThan(t1) );

    // t4 is greater than t1.
    CPPUNIT_ASSERT( !t4.lessThan(t1) );
    CPPUNIT_ASSERT(  t1.lessThan(t4) );

    // t5 is less than t1.
    CPPUNIT_ASSERT( !t1.lessThan(t5) );
    CPPUNIT_ASSERT(  t5.lessThan(t1) );

    // For different lengths we throw an error.
    TypeBucket t6(3);
    t6[0] = 4;
    t6[1] = 3;
    t6[2] = 3;

    CPPUNIT_ASSERT_THROW( t6.lessThan(t1), std::runtime_error );


    // This should be less than this.
    // [ 0 1 0 0 0 0 0  ] [ 0 0 0 1 0 0 0  ]
    TypeBucket t7(7);
    TypeBucket t8(7);
    t7[1] = 1;
    t8[3] = 1;
    CPPUNIT_ASSERT(  t8.lessThan(t7) );



}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testComparisonOperator()
{
    TypeBucket t1(3);
    t1[0] = 7;
    t1[1] = 8;
    t1[2] = 1;

    // Check false.
    CPPUNIT_ASSERT( ! (t1 == 1) );

    // Check true.
    t1[0] = 0;
    t1[1] = 1;
    t1[2] = 0;
    CPPUNIT_ASSERT( t1 == 1 );

    // Check throw.
    CPPUNIT_ASSERT_THROW( t1 == 7, std::runtime_error );

}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testMatch()
{
    // These should not match.
    // [ 0 1 0 0 0 0 0  ] [ 0 0 0 1 0 0 0  ]
    TypeBucket t1(7);
    TypeBucket t2(7);
    t1[1] = 1;
    t2[3] = 1;
    CPPUNIT_ASSERT(  !t1.match(t2) );
    CPPUNIT_ASSERT(  !t2.match(t1) );

    // These match one way but not the other.
    // [ 0 4 0 0 0 0 0  ] [ 0 3 0 0 0 0 0  ]
    TypeBucket t3(7);
    TypeBucket t4(7);
    t3[1] = 4;
    t4[1] = 3;
    CPPUNIT_ASSERT(  t4.match(t3) );
    CPPUNIT_ASSERT( !t3.match(t4) );

    // These do also match one way.
    t3[5] = 7;
    t4[5] = 7;
    CPPUNIT_ASSERT(  t4.match(t3) );
    CPPUNIT_ASSERT( !t3.match(t4) );

    // But now they don't.
    t4[6] = 1;

    CPPUNIT_ASSERT( !t4.match(t3) );
    CPPUNIT_ASSERT( !t3.match(t4) );

}


// -------------------------------------------------------------------------- //
//
void Test_TypeBucket::testAdd()
{
    TypeBucket t1(3);
    t1[0] =  12;
    t1[1] =   0;
    t1[2] = -12;

    TypeBucket t2(3);
    t2[0] =  7;
    t2[1] = -8;
    t2[2] = 13;

    // Add.
    TypeBucket t3(3);
    t3 = t1.add(t2);

    // Check.
    CPPUNIT_ASSERT_EQUAL(t3[0], t1[0] + t2[0]);
    CPPUNIT_ASSERT_EQUAL(t3[1], t1[1] + t2[1]);
    CPPUNIT_ASSERT_EQUAL(t3[2], t1[2] + t2[2]);

}
