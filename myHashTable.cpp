#include <iostream>
#include <cstdlib>
#include <ctime>
#include "myHashTable.h"

using namespace std;

myHashTable::myHashTable(unsigned int k, unsigned int N, unsigned int w){

    //initialize a hash of N buckets
    this->myHash = new set<vector<float>*> [N];

    srand(time(NULL));

    for (int i=0 ; i<k ; i++){

        //t is a random float in [0,w]
        this->tParameters.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/w)));

        //bug: a random device generator for normal distribution on vector v requires a newer version of g++ and c++11
    }
}