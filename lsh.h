#ifndef LSH_H
#define LHS_H

#include "myHashTable.h"
#include <vector>
#include <queue>

using std::vector;
using std::priority_queue;

class LSH {

    private:

        //the g hash function
        unsigned int hashFunction(vector<float>* point);

        //r parameters for computing g hash function
        vector<unsigned int> rParameters;

        //lsh method hash tables of count L
        myHashTable myHashes[];

    public:

        LSH (unsigned int k, unsigned int L, unsigned int N);
        ~LSH ();

        void insertInHashes(vector<float>* point);

        priority_queue<vector<float>*> approximateKNN(unsigned int neighbours, 
            vector<float>* point);

        priority_queue<vector<float>*> exactKNN(unsigned int neighbours, 
            vector<float>* point);

        set<vector<float>*> rangeSearch(unsigned int radius, vector<float>* point);
};

#endif