#include <iostream>
#include <ctime>
#include <random>
#include "../include/myHashTable.h"
#include "../include/utils.h"
#include "../include/PQUnique.h"
#include "../include/PQUnique.t.hpp"

using namespace std;

typedef enum{ZERO,HAMMING,EUCLIDEAN} normType;

myHashTable::myHashTable(unsigned int k, unsigned int N, unsigned int w, unsigned int dimensions){

    srand(time(NULL));

    //initialize a hash of N buckets
    for (int i=0 ; i<N ; i++) this->myHash.push_back(set<pair<Point*,int> >());
    this->myHashSize = N;

    default_random_engine generator(time(NULL));
    normal_distribution<double> distribution(0.0,1.0);
    vector<float> tempV;

    for (int i=0 ; i<k ; i++){

        //t is a random float in [0,w]
        this->tParameters.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/w)));

        //v is a d-dimension vector of random floats according to N(0,1)
        for (int j=0 ; j<dimensions ; j++) tempV.push_back(distribution(generator));
        this->vParameters.push_back(tempV);

        tempV.clear();
    }
}

myHashTable::~myHashTable() {}

void myHashTable::storeInHash(unsigned int index, Point* point, int pointID){

    //store a pair <pointPtr,ID(point)> in Hash[index]
    this->myHash[index].insert(make_pair(point,pointID));
    return;
}

pair<vector<float>,float> myHashTable::getVTParameters(unsigned int index){

    //return a pair <v,t> for calculation of h[index] function where index in [0,k]
    return make_pair(this->vParameters.at(index),this->tParameters.at(index));
}

void myHashTable::deleteAllAllocatedPoints(){

    set<pair<Point*,int> >::iterator itr;

    //in every bucket
    for(int i=0 ; i<this->myHashSize ; i++)
        //for every point
        for (itr = this->myHash[i].begin(); itr != this->myHash[i].end(); itr++)
            //delete allocated pointer to point
            delete((*itr).first);

    return;
}

void myHashTable::approximateKNN(PQUnique<pair<double, Point*> > &pqUnique,
    unsigned int index, Point& point, int pointID){

    set<pair<Point*,int> >::iterator itr;

    //the queue to be returned 
    priority_queue<pair<double, Point*> > neighboursQueue;
    double currentDistance;

    //for every point in bucket myHash[index]
    for(itr = this->myHash[index].begin(); itr != this->myHash[index].end(); itr++) {

        if (itr->second==pointID){

            currentDistance = calculate_distance(EUCLIDEAN,itr->first->getvector(),point.getvector());

            //insert in size-limited PQUnique list
            pqUnique.insert(make_pair(currentDistance,itr->first));
        }
    }

    return;
}

set<Point*> myHashTable::rangeSearch(double radius, unsigned int index, Point& point, int pointID){
    
    set<pair<Point*,int> >::iterator itr;
    set<Point*> setToReturn;
    double currentDistance;

    //for every point in bucket myHash[index]
    for(itr = this->myHash[index].begin(); itr != this->myHash[index].end(); itr++) {

            //insert in set if point in radius
        if (itr->second==pointID and 
            calculate_distance(EUCLIDEAN,itr->first->getvector(),point.getvector())<=radius) 
                setToReturn.insert(itr->first);
    }

    return setToReturn;
}

priority_queue<pair<double, Point*> > myHashTable::exactKNN(unsigned int neighboursCount, Point& point){

    set<pair<Point*,int> >::iterator itr;
    priority_queue<pair<double, Point*> > neighboursQueue;
    double currentDistance;

    //for every bucket
    for(int i=0 ; i<this->myHashSize ; i++)

        //for every point in bucket myHash[index]
        for (itr = this->myHash[i].begin(); itr != this->myHash[i].end(); itr++){

            currentDistance = calculate_distance(EUCLIDEAN,itr->first->getvector(),point.getvector());

            //queue not full
            if (neighboursQueue.size()<neighboursCount)
                neighboursQueue.push(make_pair(currentDistance,itr->first));

            //found new k-th nearest neighbour
            else if (currentDistance<neighboursQueue.top().first){

                neighboursQueue.push(make_pair(currentDistance,itr->first));
                neighboursQueue.pop();
            }
        }

    return neighboursQueue;
}

unsigned int myHashTable::getBucketPointCount(unsigned int index){

    return this->myHash.at(index).size();
}