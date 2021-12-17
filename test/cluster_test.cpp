#include "cluster_test.h"
#include "../include/cluster.h"
#include "../include/clusterLloyd.h"
#include "../include/curve.h"
#include <vector>


CPPUNIT_TEST_SUITE_REGISTRATION( Cluster_test );

void Cluster_test::setUp(){

    this->curve = new Curve("p1",vector<vector<float> >(2,vector<float>(2,2)));

    Confs conf;
    this->method = new clusterLloyd(conf,true);
}

void Cluster_test::tearDown(){
    
    delete this->method;
}

void Cluster_test::test_clustering(){

    CPPUNIT_ASSERT(this->method->insertPoint(this->curve) == this->curve);
}