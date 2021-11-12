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

        //the g hash function - returns pair<index(point),ID(point)>
        virtual unsigned int hashFunction(Point& point);

    protected:

        string method;

    public:

        KNN();
        ~KNN();

        virtual void insert(Point& point);

        virtual void approximateKNN(PQUnique <pair<double, Point*> > &neighborsQueue,Point& point);

        virtual priority_queue<pair<double, Point*> > exactKNN(unsigned int neighbours, Point& point);

        virtual set<Point*> rangeSearch(double radius, Point& point);

        string getMethod();
};

#endif