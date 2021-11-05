#include <iostream>
#include <vector>
#include "../include/lsh.h"
#include "../include/myHashTable.h"
#include "../include/utils.h"
#include "../include/PQUnique.h"
#include "../include/PQUnique.t.hpp"

#define BUCKET_MEAN_CAPACITY 8
#define WINDOW_SIZE 4.0

using namespace std;

static unsigned int tableSize;

LSH::LSH (unsigned int k, unsigned int L, unsigned int N, unsigned int dimensions){

    srand(time(NULL));

    //global static variables
    tableSize = N/BUCKET_MEAN_CAPACITY+1;

    //initialize lsh hash tables
    for (int i=0 ; i<L ; i++)
        this->myHashes.push_back(myHashTable(k,tableSize,WINDOW_SIZE,dimensions));
    
    //initialize random r-parameters for g function calculation
    for (int i=0 ; i<k ; i++)
        this->rParameters.push_back((abs(rand()))%4);

}

LSH::~LSH(){}

pair<unsigned int, int> LSH::hashFunction(unsigned int hashID, Point& point){

    long long int result=0;

    //holds parameters v and t for calculation of h
    pair<vector<float>,float> currentVTParameters;

    for (int i=0 ; i<this->rParameters.size() ; i++){

        //get v,t
        currentVTParameters = this->myHashes.at(hashID).getVTParameters(i);

        //calculating sum of r_i*h_i ;  That is  r_i*((v*p)+t)/w 
        result+= this->rParameters.at(i) * static_cast<int>((vector_multiply(point.getvector(),currentVTParameters.first)+currentVTParameters.second)/WINDOW_SIZE);
    }

    //(r_i*h_i)%2^32
    result%=INT32_MAX;

    //return pair<index(point),ID(point)>
    return make_pair(result>=0 ? result%tableSize : tableSize+result%tableSize,result);
}


void LSH::insertInHashes(Point& point){

    pair<unsigned int, int> hashFunctionResults;

    Point* pointPtr = new Point(point);

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(point), ID(point) and store point.
        hashFunctionResults = this->hashFunction(i,point);
        this->myHashes.at(i).storeInHash(hashFunctionResults.first,pointPtr,hashFunctionResults.second);
    }

    return;
}


void LSH::approximateKNN(PQUnique<pair<double, Point*> > &neighboursQueue, Point& point){

    pair<unsigned int, int> hashFunctionResults;

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(point) and ID(point)
        hashFunctionResults = this->hashFunction(i,point);

        //get knn for hash with hashID==i
        this->myHashes.at(i).approximateKNN(neighboursQueue,hashFunctionResults.first,point,hashFunctionResults.second);        
    }

    return;
}


set<Point*> LSH::rangeSearch(double radius, Point& point){

    set<Point*> pointsInRange, tempSet;
    pair<unsigned int, int> hashFunctionResults;

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(point) and ID(point)
        hashFunctionResults = this->hashFunction(i,point);

        //get set of points in range for hash with hashID==i
        tempSet = this->myHashes.at(i).rangeSearch(radius,hashFunctionResults.first,point,hashFunctionResults.second);

        //insert elements in result set excluding duplicates
        for(Point* item : tempSet)
            if(pointsInRange.find(item) == pointsInRange.end())
                pointsInRange.insert(item);
    }

    return pointsInRange;
}


priority_queue<pair<double, Point*> > LSH::exactKNN(unsigned int neighbours, Point& point){

    return this->myHashes.at(0).exactKNN(neighbours,point);
}