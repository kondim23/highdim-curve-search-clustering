#ifndef MYHASHTABLE_H
#define MYHASHTABLE_H

#include <vector>
#include <set>
#include <queue>
#include <utility>
#include "point.h"
#include "PQUnique.h"

using std::vector;
using std::set;
using std::priority_queue;
using std::pair;

class myHashTable {

    private:

        //v parameters for computing h function
        vector<vector<float> > vParameters;

        //t parameters for computing h function
        vector<float> tParameters;

        //a hash table of sets
        set< pair< Point*,int > >* myHash;

        //count of buckets
        unsigned int myHashSize;

    public:

        myHashTable(unsigned int k, unsigned int N, unsigned int w, unsigned int dimensions);
        ~myHashTable();

        void storeInHash(unsigned int index, Point* point, int pointID);

        pair<vector<float>,float> getVTParameters(unsigned int index);

        void approximateKNN(PQUnique<pair<double, Point*> > &pqUnique,
            unsigned int index, Point& point, int pointID);

        priority_queue<pair<double, Point*> > exactKNN(unsigned int neighbours, Point& point);
            
        set<Point*> rangeSearch(double radius, unsigned int index, Point& point, int pointID);

        void deleteAllAllocatedPoints();
};

#endif