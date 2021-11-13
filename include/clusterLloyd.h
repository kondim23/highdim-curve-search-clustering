#ifndef CLUSTERLLOYD_H
#define CLUSTERLLOYD_H

#include "cluster.h"
#include "confs.h"

class clusterLloyd : public Cluster
{
    private:
        
        //Lloyd's assign - overloads cluster class assignCentroids()
        bool assignCentroids();
        
    public:

        clusterLloyd(Confs&);
        ~clusterLloyd();
};


#endif