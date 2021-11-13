#include <iostream>
#include <random>
#include <ctime>
#include <map>
#include <string>
#include <float.h>
#include <fstream>
#include "../include/cluster.h"
#include "../include/utils.h"
#include "../include/lsh.h"
#include "../include/hcube.h"
#include "../include/confs.h"

using namespace std;

typedef enum{ZERO,HAMMING,EUCLIDEAN} normType;


Cluster::Cluster(Confs &confs){

    //wrong arguments
    if (!confs.get_number_of_clusters()){

        cout << "Error in cluster.conf: number_of_clusters <= 0>" << endl;
        exit(1);
    }

    //initialize allClusters with number_of_clusters sets
    for (int i=0 ; i<confs.get_number_of_clusters() ; i++)
        this->allClusters.push_back(set<pair<Point,int>*>());

}


Cluster::~Cluster() {this->FreePoints();}


void Cluster::insertPoint(Point &point){

    //insert a pair(Point,clusterID) to system -- initial clusterID==-1
    this->allPoints.insert(make_pair(point.getID(),new pair<Point,int>(point,-1)));
    return;
}


void Cluster::startClustering(){

    //initialize centroids with k-means++
    this->initializeCentroids();

    //execute centroids-points assignment while clusters change states
    while(this->assignCentroids())

        //and update centroids
        this->updateCentroids();

    return;
}


//k-means++
void Cluster::initializeCentroids(){

    map<double,Point*> nonCentroids, prevNonCentroids;
    map<string,pair<Point,int>*>::iterator itr;
    map<double,Point*>::iterator itrMap;
    double itemToRemove;
    double distanceSum, distance;
    pair<map<double,Point*>::iterator,map<double,Point*>::iterator> pairItrMap;

    srand(time(NULL));

    //initialize a map of pair(distance,Point*) -- all Points mapped by sequence i=(i-1)+1, i in [0,N]
    for (itr=this->allPoints.begin() ; itr!=this->allPoints.end(); itr++)
        nonCentroids.insert(make_pair(static_cast<double>(nonCentroids.size()),&(itr->second->first)));

    //choose a random initial point, erase it from nonCentroids n and insert it in allCentroids t
    itemToRemove = static_cast<double>(abs(rand())%nonCentroids.size());
    this->allCentroids.push_back(*(nonCentroids.find(itemToRemove)->second));
    nonCentroids.erase(itemToRemove);
    
    prevNonCentroids = nonCentroids;

    //while not enough centroids
    while(this->allCentroids.size() < this->allClusters.size()){

        distanceSum = 0;

        for (itrMap=prevNonCentroids.begin() ; itrMap!=prevNonCentroids.end() ; itrMap++){

            //compute accumulated probability = sum of D(i)^2   --   D(i) min distance from some centroid
            distance = calculateMinCentroidDistance(*(itrMap->second)).first;
            distanceSum += pow(distance,2);

            //insert pair(sum of D(i)^2,Point*) to nonCentroid
            nonCentroids.insert(make_pair(distanceSum,itrMap->second));
        }

        prevNonCentroids.clear();

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
        if (distance<minDistance){
            minDistance=distance;
            index=i;
        }
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


double Cluster::initializeRadius(){

    double minDistance=DBL_MAX;

    //find and return min distance across all centroids
    for (int i=0 ; i<this->allCentroids.size() ; i++)
        minDistance = min(minDistance,find_closest_centroid(i).first);

    return minDistance;
}   


//print centroids for every cluster
void Cluster::printCentroids(ofstream& out){

    for (int i=0 ; i<this->allClusters.size() ; i++){

        out << "CLUSTER-" << i+1 << " {size: " << this->allClusters.at(i).size() << ", " << "centroid:";
        for (int j=0 ; j<this->allCentroids.at(i).getvector().size() ; j++)
            out << " " << this->allCentroids.at(i).getvector().at(j);
        out << "}" << endl;
    }

    return;
}


//print centroid and pointIDs of every cluster
void Cluster::printClusters(ofstream& out){

    set<pair<Point,int>*>::iterator itr;

    for (int i=0 ; i<this->allClusters.size() ; i++){

        out << "CLUSTER-" << i+1 << " {";
        for (int j=0 ; j<this->allCentroids.at(i).getvector().size() ; j++)
            out << " " << this->allCentroids.at(i).getvector().at(j);

        for (itr=this->allClusters.at(i).begin() ; itr!=this->allClusters.at(i).end() ; itr++)
            out << ", " << (*itr)->first.getID();
        out << "}" << endl;
    }

    return;
}



void Cluster::silhouette(ofstream& out){

    set<pair<Point,int>*>::iterator itr;
    double A,B,totalD=0,clusterD;

    out << "Silhouette: [";

    for (int i=0 ; i<this->allClusters.size() ; i++){

        clusterD=0;

        //for every point in cluster compute S(i)  
        for (itr=this->allClusters.at(i).begin() ; itr!=this->allClusters.at(i).end() ; itr++){

            A = this->mean_cluster_distance((*itr)->first,i);
            B = this->mean_cluster_distance((*itr)->first,this->find_closest_centroid(i).second);

            clusterD+=(B-A)/max(B,A);
            totalD+=(B-A)/max(B,A);
        }

        out << clusterD/this->allClusters.at(i).size() << ",";
    }

    out << totalD/this->allPoints.size() << "]" << endl;

    return;
}


//compute mean distance of point point from points in cluster[index]
double Cluster::mean_cluster_distance(Point &point, unsigned int index){

    set<pair<Point,int>*>::iterator itr;
    double mean_distance=0;

    //iterate between all points in cluster[index]
    for (itr=this->allClusters.at(index).begin() ; itr!=this->allClusters.at(index).end() ; itr++){

        if ((*itr)->first.getID()==point.getID()) continue;

        mean_distance += calculate_distance(EUCLIDEAN,(*itr)->first.getvector(),point.getvector());
    }

    //return mean distance
    return this->allClusters.at(index).size()-1!=0 ? mean_distance/(this->allClusters.at(index).size()-1) : 0;
}


pair<double,unsigned int> Cluster::find_closest_centroid(unsigned int centroid){

    double distance,minDistance=DBL_MAX;
    unsigned int index;

    Point point = this->allCentroids.at(centroid);

    //for every centroid
    for(int i=0 ; i<this->allCentroids.size() ; i++){

        if (i==centroid) continue;

        //get the minimum distance to given centroid 
        distance = calculate_distance(EUCLIDEAN,point.getvector(),this->allCentroids.at(i).getvector());
        if (distance<minDistance) {
            minDistance = distance;
            index=i;
        }
    }

    //return dindex of closest centroid 
    return make_pair(minDistance,index);
}


//free all allocated points
void Cluster::FreePoints(){

    map<string,pair<Point,int>*>::iterator itr;

    for (itr=this->allPoints.begin() ; itr!=this->allPoints.end(); itr++)
        delete itr->second;
}

bool Cluster::assignCentroids(){}

string Cluster::getMethod() {return this->methodName;}