#ifndef CLUSTER_TEST
#define CLUSTER_TEST

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/curve.h"
#include "../include/cluster.h"


class Cluster_test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( Cluster_test );
    CPPUNIT_TEST(test_clustering);
    CPPUNIT_TEST_SUITE_END();

private:

    Curve* curve;
    Cluster* method;

protected:
public:

    Cluster_test() : CppUnit::TestFixture(){}
    ~Cluster_test(){}

    void setUp();
    void tearDown();
    void test_clustering();
};

#endif