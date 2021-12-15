#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/cluster.h"
#include "../include/clusterLloyd.h"
#include "../include/clusterReverse.h"
#include "../include/utils.h"
#include "../include/point.h"
#include "../include/curve.h"

using namespace std;
using namespace chrono;

ofstream outputFileStream;

int main(int argc, char* argv[]){

    string inputFileName, configurationsFileName, outputFileName, point, token, pointID, conf;
    ifstream inputFileStream, configurationsFileStream;
    stringstream pointStream;
    bool complete=false,silhouette=false;
    MethodType method=CL_NONE;
    MeanType meanMethod=U_NONE;
    Cluster *clusterMethod;
    Sequence* sequence;

    //check-get arguments
    if (argc>12){

        cout << "Error in command line arguments" << endl;
        return 1;
    }

    for (int i=1 ; i<argc ; i++) {

        if(!strcmp(argv[i],"-i")) inputFileName = argv[(i++)+1];
        else if(!strcmp(argv[i],"-c")) configurationsFileName = argv[(i++)+1];
		else if(!strcmp(argv[i],"-o")) outputFileName = argv[(i++)+1];
        else if(!strcmp(argv[i],"-complete")) complete=true;
        else if(!strcmp(argv[i],"-silhouette")) silhouette=true;
        else if(!strcmp(argv[i],"-assignment")) {

            if (!strcmp(argv[i+1],"Classic")) method=CL_LLOYD;
            else if (!strcmp(argv[i+1],"LSH")) method = CL_LSH;
            else if (!strcmp(argv[i+1],"Hypercube")) method = CL_CUBE;
            else if (!strcmp(argv[i+1],"LSH_Frechet")) method = CL_LSH_CURVE;
        }
        else if(!strcmp(argv[i],"-update")) {

            if (!strcmp(argv[i+1],"Frechet")) meanMethod=U_FRECHET;
            else if (!strcmp(argv[i+1],"Vector")) meanMethod = U_VECTOR;
        }
    }


    if (method==CL_NONE or meanMethod==U_NONE){
        
        cout << "Error: Undefined assignment or update method." << endl;
        return 1;
    }

    if(inputFileName.empty()){

        cout << "Enter input file:" << endl;
        getline(cin,inputFileName);
    }

    if(outputFileName.empty()){

        cout << "Enter output file:" << endl;
        getline(cin,outputFileName);
    }

    if(configurationsFileName.empty()){

        cout << "Enter conf file:" << endl;
        getline(cin,configurationsFileName);
    }

    if (method!=CL_LLOYD and ((method==CL_LSH_CURVE) != (meanMethod==U_FRECHET))){

        cout << "Error: Bad match of assignment and update functions." << endl;
        return 1;
    }

    //open-check filestreams
    outputFileStream.open(outputFileName,ofstream::trunc);
    inputFileStream.open(inputFileName);
    configurationsFileStream.open(configurationsFileName);

    if (not (outputFileStream and inputFileStream and configurationsFileStream)) {

        outputFileStream << "Error in file arguments." << endl;
        return 1;
    }

    //configurations obj holds data from cluster.conf
    Confs confs;

    //set confs
    while (configurationsFileStream and getline(configurationsFileStream,conf)){

        pointStream.str(conf);
        while (getline(pointStream,token,' ')) {

            if (token=="\r") continue;
            else if(token=="number_of_clusters:") {
                
                getline(pointStream,token,' ');
                confs.set_number_of_clusters(stoi(token));
            }
            else if(token=="number_of_vector_hash_tables:") {
                
                getline(pointStream,token,' ');
                confs.set_number_of_vector_hash_tables(stoi(token));
            }
            else if(token=="number_of_vector_hash_functions:") {
                
                getline(pointStream,token,' ');
                confs.set_number_of_vector_hash_functions(stoi(token));
            }
            else if(token=="max_number_M_hypercube:") {
                
                getline(pointStream,token,' ');
                confs.set_max_number_M_hypercube(stoi(token));
            }
            else if(token=="number_of_hypercube_dimensions:") {
                
                getline(pointStream,token,' ');
                confs.set_number_of_hypercube_dimensions(stoi(token));
            }
            else if(token=="number_of_probes:") {
                
                getline(pointStream,token,' ');
                confs.set_number_of_probes(stoi(token));
            }
        }
        pointStream.clear();
    }

    configurationsFileStream.close();


    //initialize cluster system

    if (method==CL_LLOYD) 
        clusterMethod = new clusterLloyd(confs,meanMethod==U_FRECHET);
    else 
        clusterMethod = new clusterReverse(confs,method,getPointCountAndDimensions(inputFileStream));


    //get all Points from input
    while (inputFileStream and getline(inputFileStream,point)){

        pointStream.str(point);

        //pointID holds the id of point
        getline(pointStream,pointID,'\t');

        //insert sequence to cluster system
        if (meanMethod==U_FRECHET) sequence = new Curve(pointID,read_curve(pointStream));
        else sequence = new Point(pointID, read_point(pointStream));

        clusterMethod->insertPoint(sequence);

        pointStream.clear();
    }

    inputFileStream.close();


    //clustering
    auto start = high_resolution_clock::now();
    clusterMethod->startClustering();
    auto stop = high_resolution_clock::now();

    auto executionTime = duration_cast<seconds>(stop - start);


    //print results
    outputFileStream << "Algorithm: " << clusterMethod->getMethod() << endl;

    clusterMethod->printCentroids(outputFileStream);

    outputFileStream << "clustering_time: " << executionTime.count() << endl;

    //compute and print silhouette
    if (silhouette) clusterMethod->silhouette(outputFileStream);
    
    if (complete) clusterMethod->printClusters(outputFileStream);

    outputFileStream.close();

    delete clusterMethod;
}