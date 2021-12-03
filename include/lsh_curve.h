#ifndef LSH_H
#define LHS_H

#include "sequence.h"
#include "knn.h"
#include "lsh_vector.h"
#include <vector>
#include <queue>

using std::vector;
using std::priority_queue;

class LSHcurve : public KNN{

    protected:

        virtual pair<unsigned int, int> hashFunction(unsigned int hashID, Sequence* point);

        //lsh method hash tables of count L
        vector<LSHvector> myHashes;

    public:

        LSHcurve (unsigned int N, unsigned int dimensions);
        ~LSHcurve ();

        void insert(Sequence* point);

        void approximateKNN(PQUnique<pair<double, Sequence*> > &neighboursQueue, Sequence* point);

        priority_queue<pair<double, Sequence*> > exactKNN(unsigned int neighbours, Sequence* point);

        set<Sequence*> rangeSearch(double radius, Sequence* point);
};

#endif