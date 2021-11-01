#ifndef LSH_H
#define LHS_H

#include "myHashTable.h"
#include "point.h"
#include <vector>
#include <queue>

using std::vector;
using std::priority_queue;

class LSH {

    private:

        //the g hash function - returns pair<index(point),ID(point)>
        pair<unsigned int, int> hashFunction(unsigned int hashID, Point& point);

        //r parameters for computing g hash function
        vector<unsigned int> rParameters;

        //lsh method hash tables of count L
        vector<myHashTable> myHashes;

    public:

        LSH (unsigned int k, unsigned int L, unsigned int N, unsigned int w, unsigned int dimensions);
        ~LSH ();

        void insertInHashes(Point& point);

        priority_queue<pair<double, Point*> > approximateKNN(unsigned int neighbours, 
            Point& point);

        priority_queue<pair<double, Point*> > exactKNN(unsigned int neighbours, 
            Point& point);

        set<Point*> rangeSearch(unsigned int radius, Point& point);
};

#endif