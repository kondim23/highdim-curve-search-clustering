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

    //the g hash function - returns pair<index(point),ID(point)>
    unsigned int hashFunction(Point& point);

    myHashTable* myHash;

    vector<map<int,int> > fMappings;

    bool hammingDistance(unsigned int number, int bit, int distance, Point& point, 
                        bool (HCUBE::*nextProbe)(void*,void*,Point&,unsigned int),void* p1,void* p2);

    // bool nextProbe(PQUnique<pair<double, Point*> > &pqUnique , Point& point,int number);
    bool nextProbe_KNN(void* p1, void* p2 , Point& point,unsigned int number);
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