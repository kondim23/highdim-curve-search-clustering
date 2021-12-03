#include "../include/lsh_curve.h"
#include "../include/curve.h"

#define LSH_VECTOR_L 1
#define LSH_VECTOR_K 5

frechet_type frechet_distance_type;

LSHcurve::LSHcurve (unsigned int N, unsigned int dimensions, float delta, unsigned int L){

    //initialize lsh hash tables
    for (int i=0 ; i<L ; i++)
        this->myHashes.push_back(LSHvector(LSH_VECTOR_K,LSH_VECTOR_L,N,dimensions));

    this->delta = delta;
}

LSHcurve::~LSHcurve(){}

vector<float> LSHcurve::hashFunction(unsigned int hashID, Sequence* curve){}

void LSHcurve::insert(Sequence* curve){

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(curve), ID(curve) and store curve.
        curve->setVector(this->hashFunction(i,curve));
        this->myHashes.at(i).insert(curve);
    }

    return;
}


void LSHcurve::approximateKNN(PQUnique<pair<double, Sequence*> > &neighboursQueue, Sequence* curve){

    for (int i=0 ; i<this->myHashes.size() ; i++){

        curve->setVector(this->hashFunction(i,curve));

        //get knn for hash with hashID==i
        this->myHashes.at(i).approximateKNN(neighboursQueue,curve);        
    }

    return;
}


set<Sequence*> LSHcurve::rangeSearch(double radius, Sequence* curve){

    set<Sequence*> pointsInRange, tempSet;

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(curve) and ID(curve)
        curve->setVector(this->hashFunction(i,curve));

        //get set of points in range for hash with hashID==i
        tempSet = this->myHashes.at(i).rangeSearch(radius,curve);

        //insert elements in result set excluding duplicates
        for(Sequence* item : tempSet)
            if(pointsInRange.find(item) == pointsInRange.end())
                pointsInRange.insert(item);
    }

    return pointsInRange;
}


priority_queue<pair<double, Sequence*> > LSHcurve::exactKNN(unsigned int neighbours, Sequence* curve){

    return this->myHashes.at(0).exactKNN(neighbours,curve);
}