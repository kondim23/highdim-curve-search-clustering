#include <iostream>
#include <vector>
#include "lsh.h"
#include "myHashTable.h"
#include "utils.h"

#define BUCKET_MEAN_CAPACITY 8

using namespace std;

static unsigned int w, tableSize;

LSH::LSH (unsigned int k, unsigned int L, unsigned int N, unsigned int givenW, unsigned int dimensions){

    srand(time(NULL));

    //global static variables
    w = givenW;
    tableSize = N/BUCKET_MEAN_CAPACITY;

    //initialize lsh hash tables
    for (int i=0 ; i<L ; i++) 
        this->myHashes.push_back(myHashTable(k,tableSize,givenW,dimensions));
    
    //initialize random r-parameters for g function calculation
    for (int i=0 ; i<k ; i++)
        this->rParameters.push_back((abs(rand()))%INT32_MAX);

}

LSH::~LSH(){}

pair<unsigned int, int> LSH::hashFunction(unsigned int hashID, vector<float>* point){

    long long int result=0;

    //holds parameters v and t for calculation of h
    pair<vector<float>,float> currentVTParameters;

    for (int i=0 ; i<this->rParameters.size() ; i++){

        //get v,t
        currentVTParameters = this->myHashes.at(hashID).getVTParameters(i);

        //calculating sum of r_i*h_i ;  That is  r_i*((v*p)+t)/w 
        result+= this->rParameters.at(i) * ((vector_multiply(point,&(currentVTParameters.first))+currentVTParameters.second)/w);
    }

    //(r_i*h_i)%2^32
    result%=INT32_MAX;

    //return pair<index(point),ID(point)>
    return make_pair(result>0 ? result%tableSize : tableSize+result%tableSize,result);
}


void LSH::insertInHashes(vector<float>* point){

    pair<unsigned int, int> hashFunctionResults;

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(point), ID(point) and store point.
        hashFunctionResults = this->hashFunction(i,point);
        this->myHashes.at(i).storeInHash(hashFunctionResults.first,point,hashFunctionResults.second);
    }

    return;
}


priority_queue<pair<double, vector<float>*> > LSH::approximateKNN(unsigned int neighboursCount, vector<float>* point){

    pair<unsigned int, int> hashFunctionResults;
    priority_queue<pair<double, vector<float>*> > neighboursQueue, tempQueue;


    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(point) and ID(point)
        hashFunctionResults = this->hashFunction(i,point);

        //get knn for hash with hashID==i
        tempQueue = this->myHashes.at(i).approximateKNN(neighboursCount,hashFunctionResults.first,point,hashFunctionResults.second);

        //insert all items in result priority queue
        while(!tempQueue.empty()){

            neighboursQueue.push(tempQueue.top());
            tempQueue.pop();
        }        
    }

    //keep only the knn
    while(neighboursQueue.size()>neighboursCount) 
        neighboursQueue.pop();

    return neighboursQueue;
}


set<vector<float>*> LSH::rangeSearch(unsigned int radius, vector<float>* point){

    set<vector<float>*> pointsInRange, tempSet;
    pair<unsigned int, int> hashFunctionResults;

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(point) and ID(point)
        hashFunctionResults = this->hashFunction(i,point);

        //get set of points in range for hash with hashID==i
        tempSet = this->myHashes.at(i).rangeSearch(radius,hashFunctionResults.first,point,hashFunctionResults.second);

        //insert elements in result set excluding duplicates
        for(vector<float>* item : tempSet)
            if(pointsInRange.find(item) != pointsInRange.end())
                pointsInRange.insert(item);
    }

    return pointsInRange;
}


priority_queue<pair<double, vector<float>*> > LSH::exactKNN(unsigned int neighbours, vector<float>* point){

    return this->myHashes.at(0).exactKNN(neighbours,point);
}