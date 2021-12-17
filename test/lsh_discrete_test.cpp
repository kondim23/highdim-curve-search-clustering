#include "lsh_discrete_test.h"
#include "../include/lsh_discrete.h"
#include "../include/curve.h"
#include <vector>


CPPUNIT_TEST_SUITE_REGISTRATION( Lsh_disrete_test );

void Lsh_disrete_test::setUp(){

    this->method = new DiscreteLSHcurve(10,2,0.4);
    this->curve = new Curve("p1",vector<vector<float> >(2,vector<float>(2,1)));
}

void Lsh_disrete_test::tearDown(){

    delete this->method;
    delete this->curve;

}

void Lsh_disrete_test::test_insert(){

    CPPUNIT_ASSERT(this->method->insert(this->curve) == this->curve);
}