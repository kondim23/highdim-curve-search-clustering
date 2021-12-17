#ifndef LSH_DISCRETE_TEST
#define LSH_DISCRETE_TEST

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/knn.h"
#include "../include/curve.h"


class Lsh_disrete_test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( Lsh_disrete_test );
    CPPUNIT_TEST(test_insert);
    CPPUNIT_TEST_SUITE_END();

private:

    KNN* method;
    Curve* curve;

protected:
public:

    Lsh_disrete_test() : CppUnit::TestFixture(){}
    ~Lsh_disrete_test(){}

    void setUp();
    void tearDown();
    void test_insert();
};


#endif