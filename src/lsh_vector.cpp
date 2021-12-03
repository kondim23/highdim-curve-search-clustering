#include <iostream>
#include <vector>
#include "../include/lsh_vector.h"
#include "../include/myHashTable.h"
#include "../include/utils.h"
#include "../include/PQUnique.h"
#include "../include/PQUnique.t.hpp"

#define BUCKET_MEAN_CAPACITY 8
#define WINDOW_SIZE 4.0

using namespace std;

static unsigned int tableSize;

LSHvector::LSHvector (unsigned int k, unsigned int L, unsigned int N, unsigned int dimensions){

    srand(time(NULL));

    //global static variables
    tableSize = N/BUCKET_MEAN_CAPACITY+1;

    //initialize lsh hash tables
    for (int i=0 ; i<L ; i++)
        this->myHashes.push_back(myHashTable(k,tableSize,WINDOW_SIZE,dimensions));
    
    //initialize random r-parameters for g function calculation
    for (int i=0 ; i<k ; i++)
        this->rParameters.push_back((abs(rand()))%10);

    this->method = "LSH";
}

LSHvector::~LSHvector(){this->myHashes.at(0).deleteAllAllocatedPoints();}

pair<unsigned int, int> LSHvector::hashFunction(unsigned int hashID, Sequence* sequence){

    long long int result=0;

    //holds parameters v and t for calculation of h
    pair<vector<float>,float> currentVTParameters;

    for (int i=0 ; i<this->rParameters.size() ; i++){

        //get v,t
        currentVTParameters = this->myHashes.at(hashID).getVTParameters(i);

        //calculating sum of r_i*h_i ;  That is  r_i*((v*p)+t)/w 
        // result+= static_cast<int>(this->rParameters.at(i)) * static_cast<int>((vector_multiply(sequence.getvector(),currentVTParameters.first)+currentVTParameters.second)/WINDOW_SIZE);
        double vm = vector_multiply(sequence->getvector(),currentVTParameters.first);
        vm+=currentVTParameters.second;
        vm/=WINDOW_SIZE;
        int in=static_cast<int>(vm);
        int r = (static_cast<int>(this->rParameters.at(i)));
        result+=r*in;
        result%=INT32_MAX;
    }

    //(r_i*h_i)%2^32
    result%=INT32_MAX;

    //return pair<index(sequence),ID(sequence)>
    return make_pair(result>=0 ? result%tableSize : tableSize-(abs(result)%tableSize+1),result);
}


void LSHvector::insert(Sequence* sequence){

    pair<unsigned int, int> hashFunctionResults;

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(sequence), ID(sequence) and store sequence.
        hashFunctionResults = this->hashFunction(i,sequence);
        this->myHashes.at(i).storeInHash(hashFunctionResults.first,sequence,hashFunctionResults.second);
    }

    return;
}


void LSHvector::approximateKNN(PQUnique<pair<double, Sequence*> > &neighboursQueue, Sequence* sequence){

    pair<unsigned int, int> hashFunctionResults;

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(sequence) and ID(sequence)
        hashFunctionResults = this->hashFunction(i,sequence);

        //get knn for hash with hashID==i
        this->myHashes.at(i).approximateKNN(neighboursQueue,hashFunctionResults.first,sequence,hashFunctionResults.second);        
    }

    return;
}


set<Sequence*> LSHvector::rangeSearch(double radius, Sequence* sequence){

    set<Sequence*> pointsInRange, tempSet;
    pair<unsigned int, int> hashFunctionResults;

    for (int i=0 ; i<this->myHashes.size() ; i++){

        //get index(sequence) and ID(sequence)
        hashFunctionResults = this->hashFunction(i,sequence);

        //get set of points in range for hash with hashID==i
        tempSet = this->myHashes.at(i).rangeSearch(radius,hashFunctionResults.first,sequence,hashFunctionResults.second);

        //insert elements in result set excluding duplicates
        for(Sequence* item : tempSet)
            if(pointsInRange.find(item) == pointsInRange.end())
                pointsInRange.insert(item);
    }

    return pointsInRange;
}


priority_queue<pair<double, Sequence*> > LSHvector::exactKNN(unsigned int neighbours, Sequence* sequence){

    return this->myHashes.at(0).exactKNN(neighbours,sequence);
}