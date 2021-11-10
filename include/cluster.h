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
private:
    
    //the method used for clustering {LLOYD,LSH,CUBE}
    MethodType method;

    //a set holding pointers to all Points -- pair(Point,clusterID)*
    map<string,pair<Point,int>*> allPoints;

    //a vector of set-clusters holding pointers to clustered Points -- pair(Point,clusterID)*
    vector<set<pair<Point,int>*> > allClusters;

    //a vector of Points representing the centroids
    vector<Point> allCentroids;

    void initializeCentroids();
    void updateCentroids();

    //a pointer to the function used for centroid assignment {assignLloyd,assignLSH,assignHyperCube}
    bool (Cluster::*assignCentroids)(Confs&);

    //possible assignment to assignCentroids
    bool assignLloyd(Confs&);

    //possible assignment to assignCentroids
    bool assignLSH(Confs&);

    //possible assignment to assignCentroids
    bool assignHyperCube(Confs&);
    
    pair<double,unsigned int> calculateMinCentroidDistance(Point&);
    double mean_cluster_distance(Point &, unsigned int );
    pair<double,unsigned int> find_closest_centroid(unsigned int);
    void FreePoints();
    double initializeRadius();

public:

    Cluster(unsigned int, MethodType);
    ~Cluster();
    void insertPoint(Point &point);
    void startClustering(Confs&);
    void printCentroids(ofstream&);
    void silhouette(ofstream&);
    void printClusters(ofstream&);

};

#endif