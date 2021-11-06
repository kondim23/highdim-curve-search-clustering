#include <iostream>
#include <random>
#include <ctime>
#include <map>
#include <string>
#include <float.h>
#include "../include/cluster.h"
#include "../include/utils.h"

using namespace std;

typedef enum{ZERO,HAMMING,EUCLIDEAN} normType;


Cluster::Cluster(unsigned int mediansCount, MethodType method){

    //wrong arguments
    if (!mediansCount){

        cout << "Error in cluster.conf: number_of_clusters <= 0>" << endl;
        exit(1);
    }

    //initialize allClusters with mediansCount sets
    for (int i=0 ; i<mediansCount ; i++)
        this->allClusters.push_back(set<pair<Point,int>*>());

    //set function for centroid assignment and method
    switch (method)
    {
    case LLOYD:
        this->assignCentroids = &(Cluster::assignLloyd);
        break;

    case LSH:
        this->assignCentroids = &(Cluster::assignLSH);
        break;

    case CUBE:
        this->assignCentroids = &(Cluster::assignHyperCube);
        break;
    }

    this->method = method;
}


Cluster::~Cluster(){}


void Cluster::insertPoint(Point &point){

    //insert a pair(Point,clusterID) to system -- initial clusterID==-1
    this->allPoints.insert(new pair<Point,int>(point,-1));
    return;
}


void Cluster::startClustering(){

    //initialize centroids with k-means++
    this->initializeCentroids();

    //execute centroids-points assignment while clusters change states
    while((this->*assignCentroids)())

        //and update centroids
        this->updateCentroids();

    return;
}


//k-means++
void Cluster::initializeCentroids(){

    map<unsigned int,Point*> nonCentroids, prevNonCentroids;
    set<pair<Point,int>*>::iterator itr;
    map<unsigned int,Point*>::iterator itrMap;
    unsigned int itemToRemove;
    double distanceSum, distance;
    pair<map<unsigned int,Point*>::iterator,map<unsigned int,Point*>::iterator> pairItrMap;

    srand(time(NULL));

    //initialize a map of pair(distance,Point*) -- all Points mapped by sequence i=(i-1)+1, i in [0,N]
    for (itr=this->allPoints.begin() ; itr!=this->allPoints.end(); itr++)
        nonCentroids.insert(make_pair(nonCentroids.size(),&((*itr)->first)));

    //choose a random initial point, erase it from nonCentroids n and insert it in allCentroids t
    itemToRemove = abs(rand())%nonCentroids.size();
    this->allCentroids.push_back(*(nonCentroids.find(itemToRemove)->second));
    nonCentroids.erase(itemToRemove);
    
    prevNonCentroids = nonCentroids;

    //while not enough centroids
    while(this->allCentroids.size() < this->allClusters.size()){

        distanceSum = 0;

        for (itrMap=prevNonCentroids.begin() ; itrMap!=prevNonCentroids.end() ; itrMap++){

            //compute accumulated probability = sum of D(i)^2   --   D(i) min distance from some centroid
            distance = calculateMinCentroidDistance(*(itrMap->second)).first;
            distance *= distance;
            distanceSum += distance;

            //insert pair(sum of D(i)^2,Point*) to nonCentroid
            nonCentroids.insert(make_pair(distanceSum,itrMap->second));
            prevNonCentroids.erase(itrMap);
        }

        //pairs.first in nonCentroid  are increasing in [0,distanceSum]
        //indicate a new Point for centroid, based on the closest upper_bound in probabilities of a random float
        pairItrMap = nonCentroids.equal_range(static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/distanceSum)));

        //remove from nonCentroids - add to allCentroids
        this->allCentroids.push_back(*(pairItrMap.first->second));
        nonCentroids.erase(pairItrMap.first);
        
        prevNonCentroids = nonCentroids;
        nonCentroids.clear();
    }

    return;
}


pair<double,unsigned int> Cluster::calculateMinCentroidDistance(Point &point){

    double distance,minDistance=DBL_MAX;
    unsigned int index;

    //for every centroid
    for(int i=0 ; i<this->allCentroids.size() ; i++){

        //get the minimum distance to Point 
        distance = calculate_distance(EUCLIDEAN,point.getvector(),this->allCentroids.at(i).getvector());
        if (distance<minDistance) minDistance,index=distance,i;
    }

    //return distance and index of centroid 
    return make_pair(minDistance,index);
}


void Cluster::updateCentroids(){

    set<pair<Point,int>*>::iterator itr;
    vector<float> currentPoint;

    for(int i=0 ; i<this->allClusters.size() ; i++){

        //initialize a zero-vector
        for(int j=0 ; j<this->allCentroids.at(0).getvector().size() ; j++) 
            currentPoint.push_back(0);

        //add all vectors of cluster
        for(itr=this->allClusters.at(i).begin() ; itr!=this->allClusters.at(i).end() ; itr++)
            currentPoint = add_vectors(currentPoint,(*itr)->first.getvector());

        //divide result by count of vector in bucket
        currentPoint = divide_vector(currentPoint,this->allClusters.at(i).size());

        //save new centroid
        this->allCentroids.at(i) = Point(to_string(i),currentPoint);

        currentPoint.clear();
    }

    return;    
}


bool Cluster::assignLloyd(){

    bool stateChanged=false;
    set<pair<Point,int>*>::iterator itr;
    unsigned int index;

    //for every point
    for (itr=this->allPoints.begin() ; itr!=this->allPoints.end(); itr++){

        //get index of centroid with minimum distance
        index = this->calculateMinCentroidDistance((*itr)->first).second;

        //if cluster_to_move_ == previous_cluster continue
        if (index==(*itr)->second) continue;

        //if point is clustered in the past remove from previous_cluster
        else if ((*itr)->second!=-1) this->allClusters.at((*itr)->second).erase(*itr);

        //add in propriate cluster
        this->allClusters.at(index).insert(*itr);

        //stateChange==true -> at least one state change happened 
        stateChanged=true;
    }
    
    return stateChanged;
}