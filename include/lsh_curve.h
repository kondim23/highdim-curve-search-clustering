#ifndef LSH_CURVE_H
#define LSH_CURVE_H

#include "sequence.h"
#include "knn.h"
#include "lsh_vector.h"
#include <vector>
#include <queue>

#define PADDING_VALUE 1000.0

using std::vector;
using std::priority_queue;

typedef enum{M_NONE,M_CONTINUOUS,M_DISCRETE} frechet_type;

class LSHcurve : public KNN{

    protected:

        virtual vector<float> hashFunction(unsigned int hashID, Sequence* curve);

        //lsh method hash tables of count L
        vector<LSHvector> myHashes;

        double delta;

    public:

        LSHcurve (unsigned int N, unsigned int dimensions, double delta, unsigned int L=5);
        ~LSHcurve ();

        Sequence* insert(Sequence* curve);

        void approximateKNN(PQUnique<pair<double, Sequence*> > &neighboursQueue, Sequence* curve);

        priority_queue<pair<double, Sequence*> > exactKNN(unsigned int neighbours, Sequence* curve);

        set<Sequence*> rangeSearch(double radius, Sequence* curve);
};

#endif