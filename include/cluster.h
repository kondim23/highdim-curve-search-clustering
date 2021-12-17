#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <set>
#include <map>
#include <fstream>
#include "sequence.h"
#include "confs.h"

using namespace std;

typedef enum{CL_NONE,CL_LLOYD,CL_LSH,CL_CUBE,CL_LSH_CURVE} MethodType;
typedef enum{U_NONE,U_FRECHET,U_VECTOR} MeanType;

class Cluster
{
protected:
    
    //a map holding pointers to all Sequences -- pair(Sequence*,clusterID)* -- mapped by SequenceID
    map<string,pair<Sequence*,int>*> allPoints;

    //a vector of set-clusters holding pointers to clustered Sequences -- pair(Sequence,clusterID)*
    vector<set<pair<Sequence*,int>*> > allClusters;

    //a vector of Sequence* representing the centroids
    vector<Sequence*> allCentroids;

    //k-means++
    void initializeCentroids();

    //compute the new Point-centroid of each cluster
    void updateCentroidsPoint();

    //compute the new Curve-centroid of each cluster
    void updateCentroidsCurve();

    //a pointer to function indicating updateCentroidsPoint or updateCentroidsCurve
    void (Cluster::*updateCentroids)();

    //assign Sequences to clusters - overloaded based on method (Lloyd's - Reverse RS)
    virtual bool assignCentroids();
    
    //find closest centroid to Sequence
    pair<double,unsigned int> calculateMinCentroidDistance(Sequence*);

    //calculate the mean distance of given Sequence on Sequences in given cluster 
    double mean_cluster_distance(Sequence *, unsigned int );

    //find closect centroid to given centroid 
    pair<double,unsigned int> find_closest_centroid(unsigned int);

    //delete all allocated Sequences
    void FreePoints();

    //initialize radius based on smallest distance of two centroids divided by 2 
    double initializeRadius();

    //the name of the method used (Lloyds - LSH - Hypercube - LSH_Frechet)
    string methodName;

public:

    Cluster(Confs&);
    ~Cluster();

    Sequence* insertPoint(Sequence *point);

    void startClustering();

    //print all centroids
    void printCentroids(ofstream&);

    //calculate and print silhouette
    void silhouette(ofstream&);
    
    //print the centroid and SequenceId's of all sequences in cluster
    void printClusters(ofstream&);
    
    string getMethod();
};

#endif