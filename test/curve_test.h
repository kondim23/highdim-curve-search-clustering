#ifndef CURVE_TEST
#define CURVE_TEST

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/curve.h"


class Curve_test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( Curve_test );
    CPPUNIT_TEST(testDistance);
    CPPUNIT_TEST_SUITE_END();

private:

    Curve* c1;
    Curve* c2;

protected:
public:

    Curve_test() : CppUnit::TestFixture(){}
    ~Curve_test(){}

    void setUp();
    void tearDown();
    void testDistance();
};

#endif