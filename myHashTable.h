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
        set<vector<float>*>* myHash;

    public:

        myHashTable(unsigned int k, unsigned int N, unsigned int w);
        ~myHashTable();

        void storeInHash(unsigned int index, vector<float>* point);

        pair<vector<float>,float> getVTParameters(unsigned int index);

        priority_queue<vector<float>*> approximateKNN(unsigned int neighbours,
            unsigned int index, vector<float>* point);

        priority_queue<vector<float>*> exactKNN(unsigned int neighbours, 
            vector<float>* point);
            
        set<vector<float>*> rangeSearch(unsigned int radius, unsigned int index, 
            vector<float>* point);
};

#endif