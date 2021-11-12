#ifndef CLUSTERREVERSE_H
#define CLUSTERREVERSE_H

#include "cluster.h"
#include "confs.h"
#include "knn.h"

class clusterReverse : public Cluster
{
    private:
        
        bool assignCentroids();
        KNN *method;
        
    public:

        clusterReverse(Confs&, MethodType, pair<unsigned int, int>);
        ~clusterReverse();
};


#endif