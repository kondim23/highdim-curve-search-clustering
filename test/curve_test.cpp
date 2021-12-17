#include "curve_test.h"
#include "../include/lsh_curve.h"
#include "../include/curve.h"

frechet_type frechet_distance_type;

CPPUNIT_TEST_SUITE_REGISTRATION( Curve_test );

void Curve_test::setUp(){

    this->c1 = new Curve("c1",vector<vector<float> >(2,vector<float>(2,1)));
    this->c2 = new Curve("c2",vector<vector<float> >(2,vector<float>(2,2)));
    frechet_distance_type = M_DISCRETE;
}

void Curve_test::tearDown(){

    delete this->c1;
    delete this->c2;
}

void Curve_test::testDistance(){

    CPPUNIT_ASSERT_EQUAL(this->c1->get_distance(this->c2),1.4142135623730951);
}