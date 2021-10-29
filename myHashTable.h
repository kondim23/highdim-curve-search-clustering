#ifndef MYHASHTABLE_H
#define MYHASHTABLE_H

#include <vector>
#include <set>
#include <queue>
#include <utility>

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
        set< pair< vector<float>*,int > >* myHash;

        //count of buckets
        unsigned int myHashSize;

    public:

        myHashTable(unsigned int k, unsigned int N, unsigned int w, unsigned int dimensions);
        ~myHashTable();

        void storeInHash(unsigned int index, vector<float>* point, int pointID);

        pair<vector<float>,float> getVTParameters(unsigned int index);

        priority_queue<pair<double, vector<float>*> > approximateKNN(unsigned int neighbours,
            unsigned int index, vector<float>* point, int pointID);

        priority_queue<pair<double, vector<float>*> > exactKNN(unsigned int neighbours, 
            vector<float>* point);
            
        set<vector<float>*> rangeSearch(double radius, unsigned int index, 
            vector<float>* point, int pointID);

        void deleteAllAllocatedPoints();
};

#endif