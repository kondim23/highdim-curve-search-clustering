#ifndef KNN_H
#define KNN_H

#include <vector>
#include <map>
#include "point.h"
#include "PQUnique.h"
#include "PQUnique.t.hpp"

using std::priority_queue;

class KNN {

    private:

        //the g hash function - will be overloaded based on method (LSH - Hypercube)
        virtual unsigned int hashFunction(Point& point);

    protected:

        //the method used (LSH - Hypercube)
        string method;

    public:

        KNN();
        ~KNN();

        //Every member below will be overloaded based on method (LSH - Hypercube)
        
        virtual void insert(Point& point);

        virtual void approximateKNN(PQUnique <pair<double, Point*> > &neighborsQueue,Point& point);

        virtual priority_queue<pair<double, Point*> > exactKNN(unsigned int neighbours, Point& point);

        virtual set<Point*> rangeSearch(double radius, Point& point);

        string getMethod();
};

#endif