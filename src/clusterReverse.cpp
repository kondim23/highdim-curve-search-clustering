#include "../include/clusterReverse.h"
#include "../include/lsh.h"
#include "../include/hcube.h"


clusterReverse::clusterReverse(Confs& confs, MethodType mType, pair<unsigned int, int> pointStats) 
                : Cluster(confs){

    // set function for centroid assignment and method
    switch (mType)
    {
    case _LSH:
        this->method = new LSH(confs.get_number_of_vector_hash_functions(),confs.get_number_of_vector_hash_tables(),
                                pointStats.first,pointStats.second);
        this->methodName = "Range Search LSH";
        break;

    case _CUBE:
        this->method = new HCUBE(confs.get_number_of_hypercube_dimensions(), pointStats.second, 
                                confs.get_number_of_probes(),confs.get_max_number_M_hypercube());
        this->methodName = "Range Search Hypercube";
        break;
    }
}


clusterReverse::~clusterReverse() { delete this->method; }


bool clusterReverse::assignCentroids(){

    
    map<string,pair<Point,int>*>::iterator itrCentroidSet;
    set<Point*>::iterator itrRangeSet;
    pair<Point,int>* pointFromRS;
    set<pair<Point,int>*> clusteredPoints;
    double radius = initializeRadius()/2.0;
    bool totalStateChanged=false;
    bool iterationStateChange=true;
    unsigned int index, times=0;
    set<Point*> rangeSet;

    static bool initialization=true;

    //initialize lsh system with all Points
    if (initialization){

        for (itrCentroidSet=this->allPoints.begin() ; itrCentroidSet!=this->allPoints.end(); itrCentroidSet++){
            Point p(itrCentroidSet->second->first);
            this->method->insert(p);
        }

        initialization=false;
    }


    //perform ANN
    while (clusteredPoints.size() < this->allPoints.size() and (iterationStateChange or times++<3)){

        iterationStateChange=false;

        for(int i=0 ; i<this->allCentroids.size() ; i++){

            //perform RS for every centroid as query
            rangeSet = this->method->rangeSearch(radius,this->allCentroids.at(i));

            //for every point returned
            for (itrRangeSet=rangeSet.begin() ; itrRangeSet!=rangeSet.end() ; itrRangeSet++){

                pointFromRS = allPoints.at((*itrRangeSet)->getID());

                //if point in set of already clustered points continue
                if (clusteredPoints.find(pointFromRS)!=clusteredPoints.end()) continue;
                clusteredPoints.insert(pointFromRS);

                //if cluster_to_move_ == previous_cluster continue
                if (i==pointFromRS->second) continue;

                //if point is clustered in the past remove from previous_cluster
                else if (pointFromRS->second!=-1) 
                    this->allClusters.at(pointFromRS->second).erase(pointFromRS);

                //add in propriate cluster
                pointFromRS->second = i;
                this->allClusters.at(i).insert(pointFromRS);

                //stateChange==true -> at least one state change happened 
                totalStateChanged=iterationStateChange=true;
            }
        }

        radius*=2.0;
    }

    //perform brute force for the rest of the points
    for (itrCentroidSet=this->allPoints.begin() ; itrCentroidSet!=this->allPoints.end(); itrCentroidSet++){

        if (clusteredPoints.find(itrCentroidSet->second)!=clusteredPoints.end()) 
            continue;

        //get index of centroid with minimum distance
        index = this->calculateMinCentroidDistance(itrCentroidSet->second->first).second;

        //if cluster_to_move_ == previous_cluster continue
        if (index==itrCentroidSet->second->second) continue;

        //if point is clustered in the past remove from previous_cluster
        else if (itrCentroidSet->second->second!=-1) 
            this->allClusters.at(itrCentroidSet->second->second).erase(itrCentroidSet->second);

        //add in propriate cluster
        itrCentroidSet->second->second = index;
        this->allClusters.at(index).insert(itrCentroidSet->second);

    }

    return totalStateChanged;
}
