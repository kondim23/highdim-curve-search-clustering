#ifndef HCUBE_H
#define HCUBE_H

#include <vector>
#include <map>
#include "myHashTable.h"
#include "point.h"
#include "PQUnique.h"
#include "PQUnique.t.hpp"
#include "../include/knn.h"

using std::vector;
using std::priority_queue;

class HCUBE : public KNN{

    private:

    //the g hash function - returns index(point)
    unsigned int hashFunction(Point& point);

    //the hash table of hcube
    myHashTable* myHash;

    //a vector holding a map h->f for every h_i
    vector<map<int,int> > fMappings;

    //recursively computes all numbers with hamming distance 'distance' of number 'number' having 'bit' bits
    //for each number generated calls nextProbe_KNN() or nextProbe_RS()
    bool hammingDistance(unsigned int number, int bit, int distance, Point& point, 
                        bool (HCUBE::*nextProbe)(void*,void*,Point&,unsigned int),void* p1,void* p2);

    //checks next probe updating a PQUnique for KNN
    bool nextProbe_KNN(void* p1, void* p2 , Point& point,unsigned int number);

    //checks next probe updating a set for Range Search
    bool nextProbe_RS(void* p1, void* p2 , Point& point,unsigned int number);

    public:

        HCUBE(unsigned int k,unsigned int dimensions,int probes,int M);
        ~HCUBE();

        void insert(Point& point);

        void approximateKNN(PQUnique <pair<double, Point*> > &neighborsQueue,Point& point);

        priority_queue<pair<double, Point*> > exactKNN(unsigned int neighbours, Point& point);

        set<Point*> rangeSearch(double radius, Point& point);
};

#endif