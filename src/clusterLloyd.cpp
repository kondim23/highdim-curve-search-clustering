#include "../include/clusterLloyd.h"
#include "../include/confs.h"



clusterLloyd::clusterLloyd(Confs& confs,bool frechet) : Cluster(confs){

    this->methodName = "Lloyds";
    if (frechet) this->updateCentroids = &clusterLloyd::updateCentroidsCurve;
}
    


clusterLloyd::~clusterLloyd(){}



bool clusterLloyd::assignCentroids(){

    bool stateChanged=false;
    map<string,pair<Sequence*,int>*>::iterator itr;
    unsigned int index;

    //for every point
    for (itr=this->allPoints.begin() ; itr!=this->allPoints.end(); itr++){

        //get index of centroid with minimum distance
        index = this->calculateMinCentroidDistance(itr->second->first).second;

        //if cluster_to_move_ == previous_cluster continue
        if (index==itr->second->second) continue;

        //if point is clustered in the past remove from previous_cluster
        else if (itr->second->second!=-1) this->allClusters.at(itr->second->second).erase(itr->second);

        //add in propriate cluster
        itr->second->second = index;
        this->allClusters.at(index).insert(itr->second);

        //stateChange==true -> at least one state change happened 
        stateChanged=true;
    }
    
    return stateChanged;
}
