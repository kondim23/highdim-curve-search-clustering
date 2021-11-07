#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <set>
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
    double Cluster::mean_cluster_distance(Point &, unsigned int );
    unsigned int Cluster::find_closest_centroid(unsigned int);
    void Cluster::FreePoints();

public:

    Cluster(unsigned int, MethodType);
    ~Cluster();
    void insertPoint(Point &point);
    void startClustering(Confs&);
    void printCentroids();
    void silhouette();
    void printClusters();

};

#endif