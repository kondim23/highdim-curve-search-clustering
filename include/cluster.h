#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <set>
#include <map>
#include <fstream>
#include "sequence.h"
#include "confs.h"

using namespace std;

typedef enum{_LLOYD,_LSH,_CUBE} MethodType;

class Cluster
{
protected:
    
    //a map holding pointers to all Points -- pair(Point,clusterID)* -- mapped by PointID
    map<string,pair<Sequence*,int>*> allPoints;

    //a vector of set-clusters holding pointers to clustered Points -- pair(Point,clusterID)*
    vector<set<pair<Sequence*,int>*> > allClusters;

    //a vector of Points representing the centroids
    vector<Sequence*> allCentroids;

    //k-means++
    void initializeCentroids();

    //compute the new centroid of each cluster
    void updateCentroids();

    //assign points to clusters - overloaded based on method (Lloyd's - Reverse RS)
    virtual bool assignCentroids();
    
    //find closest centroid to point
    pair<double,unsigned int> calculateMinCentroidDistance(Sequence*);

    //calculate the mean distance of given point on points in given cluster 
    double mean_cluster_distance(Sequence *, unsigned int );

    //find closect centroid to given centroid 
    pair<double,unsigned int> find_closest_centroid(unsigned int);

    //delete all allocated points
    void FreePoints();

    //initialize radius based on smallest distance of two centroids divided by 2 
    double initializeRadius();

    //the name of the method used (Lloyds - LSH - Hypercube)
    string methodName;

public:

    Cluster(Confs&);
    ~Cluster();
    void insertPoint(Sequence *point);
    void startClustering();
    void printCentroids(ofstream&);

    //calculate and print silhouette
    void silhouette(ofstream&);
    
    void printClusters(ofstream&);
    string getMethod();
};

#endif