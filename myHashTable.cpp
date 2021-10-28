#include <iostream>
#include <ctime>
#include <random>
#include "myHashTable.h"

using namespace std;

static unsigned int (*calculate_distance) (vector<float>*,vector<float>*);

myHashTable::myHashTable(unsigned int k, unsigned int N, unsigned int w, unsigned int dimensions){

    //initialize a hash of N buckets
    this->myHash = new set<pair<vector<float>*,unsigned int> > [N];
    this->myHashSize = N;

    default_random_engine generator(time(NULL));
    normal_distribution<double> distribution(0.0,1.0);
    vector<float> tempV;

    for (int i=0 ; i<k ; i++){

        //t is a random float in [0,w]
        this->tParameters.push_back(static_cast <float> (generator()) / (static_cast <float> (RAND_MAX/w)));

        //v is a d-dimension vector of random floats according to N(0,1)
        for (int j=0 ; j<dimensions ; j++) tempV.push_back(distribution(generator));
        this->vParameters.push_back(tempV);

        tempV.clear();
    }
}

myHashTable::~myHashTable(){

    delete(this->myHash);
}

void myHashTable::storeInHash(unsigned int index, vector<float>* point, unsigned int pointID){

    //store a pair <pointPtr,ID(point)> in Hash[index]
    this->myHash[index].insert(pair<vector<float>*,unsigned int>(point,pointID));
    return;
}

pair<vector<float>,float> myHashTable::getVTParameters(unsigned int index){

    //return a pair <v,t> for calculation of h[index] function where index in [0,k]
    return pair<vector<float>,float> (this->vParameters.at(index),this->tParameters.at(index));
}

void myHashTable::deleteAllAllocatedPoints(){

    set<pair<vector<float>*,unsigned int> >::iterator itr;

    //in every bucket
    for(int i=0 ; i<this->myHashSize ; i++)
        //for every point
        for (itr = this->myHash[i].begin(); itr != this->myHash[i].end(); itr++)
            //delete allocated pointer to point
            delete((*itr).first);

    return;
}

priority_queue<pair<unsigned int, vector<float>*> > myHashTable::approximateKNN(unsigned int neighboursCount, 
    unsigned int index, vector<float>* point, unsigned int pointID){

    set<pair<vector<float>*,unsigned int> >::iterator itr;

    //the queue to be returned 
    priority_queue<pair<unsigned int, vector<float>*> > neighboursQueue;
    unsigned int currentDistance;

    //for every point in bucket myHash[index]
    for(itr = this->myHash[index].begin(); itr != this->myHash[index].end(); itr++) {

        if ((*itr).second==pointID){

            currentDistance = calculate_distance((*itr).first,point);

            //queue not full
            if (neighboursQueue.size()<neighboursCount)
                neighboursQueue.push(make_pair(currentDistance,(*itr).first));

            //found new k-th nearest neighbour
            else if (currentDistance<neighboursQueue.top().first){

                neighboursQueue.push(make_pair(currentDistance,(*itr).first));
                neighboursQueue.pop();
            }
        }
    }

    //TODO: #1 Must return k neighbours.

    return neighboursQueue;
}

set<vector<float>*> myHashTable::rangeSearch(unsigned int radius, unsigned int index, vector<float>* point, unsigned int pointID){
    
    set<pair<vector<float>*,unsigned int> >::iterator itr;
    set<vector<float>*> setToReturn;
    unsigned int currentDistance;

    //for every point in bucket myHash[index]
    for(itr = this->myHash[index].begin(); itr != this->myHash[index].end(); itr++) {

        if ((*itr).second==pointID){

            currentDistance = calculate_distance((*itr).first,point);
            
            //insert in set if point in radius
            if (calculate_distance((*itr).first,point)<=radius) 
                setToReturn.insert((*itr).first);
        }
    }

    return setToReturn;
}

priority_queue<pair<unsigned int, vector<float>*> > myHashTable::exactKNN(unsigned int neighboursCount, vector<float>* point){

    set<pair<vector<float>*,unsigned int> >::iterator itr;
    priority_queue<pair<unsigned int, vector<float>*> > neighboursQueue;
    unsigned int currentDistance;

    //for every bucket
    for(int i=0 ; i<this->myHashSize ; i++)

        //for every point in bucket myHash[index]
        for (itr = this->myHash[i].begin(); itr != this->myHash[i].end(); itr++){

            currentDistance = calculate_distance((*itr).first,point);

            //queue not full
            if (neighboursQueue.size()<neighboursCount)
                neighboursQueue.push(make_pair(currentDistance,(*itr).first));

            //found new k-th nearest neighbour
            else if (currentDistance<neighboursQueue.top().first){

                neighboursQueue.push(make_pair(currentDistance,(*itr).first));
                neighboursQueue.pop();
            }
        }

    return neighboursQueue;
}