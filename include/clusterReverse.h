#ifndef CLUSTERREVERSE_H
#define CLUSTERREVERSE_H

#include "cluster.h"
#include "confs.h"
#include "knn.h"

class clusterReverse : public Cluster
{
    private:
        
        //Reverse assign - overloads cluster class assignCentroids()
        bool assignCentroids();

        //a pointer to the method used for reverse clustering (LSH - Hypercube)
        KNN *method;
        
    public:

        clusterReverse(Confs&, MethodType, pair<unsigned int, int>);
        ~clusterReverse();
};


#endif