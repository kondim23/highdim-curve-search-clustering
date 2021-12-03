#ifndef MYHASHTABLE_H
#define MYHASHTABLE_H

#include <vector>
#include <set>
#include <queue>
#include <utility>
#include "sequence.h"
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

        //a hash table of sets of points
        vector<set< pair< Sequence*,int > > > myHash;

        //count of buckets
        unsigned int myHashSize;

    public:

        myHashTable(unsigned int k, unsigned int N, unsigned int w, unsigned int dimensions);
        ~myHashTable();

        //store a point with id pointID in bucket index
        void storeInHash(unsigned int index, Sequence* point, int pointID);

        //get v and t for computing h
        pair<vector<float>,float> getVTParameters(unsigned int index);

        void approximateKNN(PQUnique<pair<double, Sequence*> > &pqUnique,
            unsigned int index, Sequence* point, int pointID);

        priority_queue<pair<double, Sequence*> > exactKNN(unsigned int neighbours, Sequence* point);
            
        set<Sequence*> rangeSearch(double radius, unsigned int index, Sequence* point, int pointID);

        void deleteAllAllocatedPoints();

        //returns the count of points in given bucket 
        unsigned int getBucketPointCount(unsigned int);
};

#endif