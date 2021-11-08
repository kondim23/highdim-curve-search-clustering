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
    case _LLOYD:
        this->assignCentroids = &Cluster::assignLloyd;
        break;

    case _LSH:
        this->assignCentroids = &Cluster::assignLSH;
        break;

    case _CUBE:
        this->assignCentroids = &Cluster::assignHyperCube;
        break;
    }

    this->method = method;
}


Cluster::~Cluster() {this->FreePoints();}


void Cluster::insertPoint(Point &point){

    //insert a pair(Point,clusterID) to system -- initial clusterID==-1
    this->allPoints.insert(make_pair(point.getID(),new pair<Point,int>(point,-1)));
    return;
}


void Cluster::startClustering(Confs& confs){

    //initialize centroids with k-means++
    this->initializeCentroids();

    //execute centroids-points assignment while clusters change states
    while((this->*assignCentroids)(confs))

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


bool Cluster::assignLloyd(Confs& confs){

    bool stateChanged=false;
    map<string,pair<Point,int>*>::iterator itr;
    unsigned int index;

    //for every point
    for (itr=this->allPoints.begin() ; itr!=this->allPoints.end(); itr++){

        //get index of centroid with minimum distance
        index = this->calculateMinCentroidDistance(itr->second->first).second;

        //if cluster_to_move_ == previous_cluster continue
        if (index==itr->second->second) continue;

        //if point is clustered in the past remove from previous_cluster
        else if (itr->second->second!=-1) this->allClusters.at(itr->second->second).erase(itr->second);

        itr->second->second = index;
        //add in propriate cluster
        this->allClusters.at(index).insert(itr->second);

        //stateChange==true -> at least one state change happened 
        stateChanged=true;
    }
    
    return stateChanged;
}


bool Cluster::assignLSH(Confs& confs){

    set<Point*> clusteredPoints, rangeSet;
    map<string,pair<Point,int>*>::iterator itrCentroidSet;
    set<Point*>::iterator itrRangeSet;
    pair<Point,int>* pointFromRS;
    double radius = 100.0;
    bool stateChanged=false;
    unsigned int times=0;

    static bool initialization=true;
    static LSH* lsh = new LSH(confs.get_number_of_vector_hash_functions(),confs.get_number_of_vector_hash_tables(),
                                this->allPoints.size(),this->allCentroids.at(0).getvector().size());

    if (initialization){

        for (itrCentroidSet=this->allPoints.begin() ; itrCentroidSet!=this->allPoints.end(); itrCentroidSet++){
            Point p(itrCentroidSet->second->first);
            lsh->insertInHashes(p);
        }

        initialization=false;
    }

    while (clusteredPoints.size() < this->allPoints.size() and times++<100){

        for(int i=0 ; i<this->allCentroids.size() ; i++){

            rangeSet = lsh->rangeSearch(radius,this->allCentroids.at(i));

            for (itrRangeSet=rangeSet.begin() ; itrRangeSet!=rangeSet.end() ; itrRangeSet++){

                if (clusteredPoints.find(*itrRangeSet)!=clusteredPoints.end()) continue;
                clusteredPoints.insert(*itrRangeSet);

                pointFromRS = allPoints.at((*itrRangeSet)->getID());

                if (i==pointFromRS->second) continue;
                else if (pointFromRS->second!=-1) 
                    this->allClusters.at(pointFromRS->second).erase(pointFromRS);

                pointFromRS->second = i;
                this->allClusters.at(i).insert(pointFromRS);

                stateChanged=true;
            }
        }

        radius+=100.0;
    }

    return stateChanged;
}


void Cluster::printCentroids(ofstream& out){

    for (int i=0 ; i<this->allClusters.size() ; i++){

        out << "CLUSTER-" << i+1 << " {size: " << this->allClusters.at(i).size() << ", " << "centroid:";
        for (int j=0 ; j<this->allCentroids.at(i).getvector().size() ; j++)
            out << " " << this->allCentroids.at(i).getvector().at(j);
        out << "}" << endl;
    }

    return;
}

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

        for (itr=this->allClusters.at(i).begin() ; itr!=this->allClusters.at(i).end() ; itr++){

            A = this->mean_cluster_distance((*itr)->first,i);
            B = this->mean_cluster_distance((*itr)->first,this->find_closest_centroid(i));

            clusterD+=(B-A)/max(B,A);
            totalD+=(B-A)/max(B,A);
        }

        out << clusterD/this->allClusters.at(i).size() << ",";
    }

    out << totalD/this->allPoints.size() << "]" << endl;

    return;
}

double Cluster::mean_cluster_distance(Point &point, unsigned int index){

    set<pair<Point,int>*>::iterator itr;
    double mean_distance=0;

    for (itr=this->allClusters.at(index).begin() ; itr!=this->allClusters.at(index).end() ; itr++){

        if ((*itr)->first.getID()==point.getID()) continue;

        mean_distance += calculate_distance(EUCLIDEAN,(*itr)->first.getvector(),point.getvector());
    }

    return this->allClusters.at(index).size()-1!=0 ? mean_distance/(this->allClusters.at(index).size()-1) : 0;
}

unsigned int Cluster::find_closest_centroid(unsigned int centroid){

    double distance,minDistance=DBL_MAX;
    unsigned int index;

    Point point = this->allCentroids.at(centroid);

    //for every centroid
    for(int i=0 ; i<this->allCentroids.size() ; i++){

        if (i==centroid) continue;

        //get the minimum distance to Point 
        distance = calculate_distance(EUCLIDEAN,point.getvector(),this->allCentroids.at(i).getvector());
        if (distance<minDistance) {
            minDistance = distance;
            index=i;
        }
    }

    //return distance and index of centroid 
    return index;
}

void Cluster::FreePoints(){

    map<string,pair<Point,int>*>::iterator itr;

    for (itr=this->allPoints.begin() ; itr!=this->allPoints.end(); itr++)
        delete itr->second;
}

bool Cluster::assignHyperCube(Confs&){return true;} //not implemented