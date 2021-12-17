#ifndef LSH_VECTOR_H
#define LSH_VECTOR_H

#include "myHashTable.h"
#include "sequence.h"
#include "knn.h"
#include <vector>
#include <queue>

using std::vector;
using std::priority_queue;

class LSHvector : public KNN{

    private:

        //the g hash function - returns pair<index(point),ID(point)>
        pair<unsigned int, int> hashFunction(unsigned int hashID, Sequence* point);

        //r parameters for computing g hash function
        vector<unsigned int> rParameters;

        //lsh method hash tables of count L
        vector<myHashTable> myHashes;

    public:

        LSHvector (unsigned int k, unsigned int L, unsigned int N, unsigned int dimensions);
        ~LSHvector ();

        Sequence* insert(Sequence* point);

        void approximateKNN(PQUnique<pair<double, Sequence*> > &neighboursQueue, Sequence* point);

        priority_queue<pair<double, Sequence*> > exactKNN(unsigned int neighbours, Sequence* point);

        set<Sequence*> rangeSearch(double radius, Sequence* point);
};

#endif