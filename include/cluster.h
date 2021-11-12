#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <set>
#include <map>
#include <fstream>
#include "point.h"
#include "confs.h"

using namespace std;

typedef enum{_LLOYD,_LSH,_CUBE} MethodType;

class Cluster
{
protected:
    
    //a set holding pointers to all Points -- pair(Point,clusterID)*
    map<string,pair<Point,int>*> allPoints;

    //a vector of set-clusters holding pointers to clustered Points -- pair(Point,clusterID)*
    vector<set<pair<Point,int>*> > allClusters;

    //a vector of Points representing the centroids
    vector<Point> allCentroids;

    void initializeCentroids();
    void updateCentroids();
    virtual bool assignCentroids();
    
    pair<double,unsigned int> calculateMinCentroidDistance(Point&);
    double mean_cluster_distance(Point &, unsigned int );
    pair<double,unsigned int> find_closest_centroid(unsigned int);
    void FreePoints();
    double initializeRadius();
    string methodName;

public:

    Cluster(Confs&);
    ~Cluster();
    void insertPoint(Point &point);
    void startClustering();
    void printCentroids(ofstream&);
    void silhouette(ofstream&);
    void printClusters(ofstream&);
    string getMethod();
};

#endif