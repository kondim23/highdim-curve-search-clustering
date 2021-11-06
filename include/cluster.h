#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <set>
#include "point.h"

using namespace std;

typedef enum{LLOYD,LSH,CUBE} MethodType;

class Cluster
{
private:
    
    //the method used for clustering {LLOYD,LSH,CUBE}
    MethodType method;

    //a set holding pointers to all Points -- pair(Point,clusterID)*
    set<pair<Point,int>*> allPoints;

    //a vector of set-clusters holding pointers to clustered Points -- pair(Point,clusterID)*
    vector<set<pair<Point,int>*> > allClusters;

    //a vector of Points representing the centroids
    vector<Point> allCentroids;

    void initializeCentroids();
    void updateCentroids();

    //a pointer to the function used for centroid assignment {assignLloyd,assignLSH,assignHyperCube}
    bool (Cluster::*assignCentroids)();

    //possible assignment to assignCentroids
    bool assignLloyd();

    //possible assignment to assignCentroids
    bool assignLSH();

    //possible assignment to assignCentroids
    bool assignHyperCube();
    
    pair<double,unsigned int> calculateMinCentroidDistance(Point&);

public:

    Cluster(unsigned int, MethodType);
    ~Cluster();
    void insertPoint(Point &point);
    void startClustering();
    void printCentroids();
    void silhouette();
    void printClusters();

};

#endif