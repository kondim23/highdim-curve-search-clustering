#ifndef KNN_H
#define KNN_H

#include <vector>
#include <map>
#include "sequence.h"
#include "PQUnique.h"
#include "PQUnique.t.hpp"

using std::priority_queue;

class KNN {

    private:

        //the g hash function - will be overloaded based on method (LSH - Hypercube)
        virtual unsigned int hashFunction(Sequence* point);

    protected:

        //the method used (LSH - Hypercube)
        string method;

    public:

        KNN();
        ~KNN();

        //Every member below will be overloaded based on method (LSH - Hypercube)
        
        virtual void insert(Sequence* point);

        virtual void approximateKNN(PQUnique <pair<double, Sequence*> > &neighborsQueue,Sequence* point);

        virtual priority_queue<pair<double, Sequence*> > exactKNN(unsigned int neighbours, Sequence* point);

        virtual set<Sequence*> rangeSearch(double radius, Sequence* point);

        string getMethod();
};

#endif