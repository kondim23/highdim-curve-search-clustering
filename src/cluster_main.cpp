#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/cluster.h"

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]){

    string inputFileName, configurationsFileName, outputFileName, point, token, pointID, conf;
    ifstream inputFileStream, configurationsFileStream;
    stringstream pointStream;
    ofstream outputFileStream;
    vector<float> pointVector;
    int inputPointDimensions=-1;
    unsigned int k=4, L=1, N=5, inputPointCount=0;
    bool complete=false;
    MethodType method;


    if (argc>9){

        cout << "Error in command line arguments" << endl;
        return 1;
    }

    for (int i=1 ; i<argc ; i++) {

        if(!strcmp(argv[i],"-i")) inputFileName = argv[(i++)+1];
        else if(!strcmp(argv[i],"-c")) configurationsFileName = argv[(i++)+1];
		else if(!strcmp(argv[i],"-o")) outputFileName = argv[(i++)+1];
        else if(!strcmp(argv[i],"-complete")) complete=true;
        else if(!strcmp(argv[i],"-method")) {

            if (!strcmp(argv[i+1],"Classic")) method=_LLOYD;
            else if (!strcmp(argv[i+1],"LSH")) method = _LSH;
            else if (!strcmp(argv[i+1],"Hypercube")) method = _CUBE;
        }
    }

    outputFileStream.open(outputFileName,ofstream::trunc);
    inputFileStream.open(inputFileName);
    configurationsFileStream.open(configurationsFileName);

    if (not (outputFileStream and inputFileStream and configurationsFileStream)) {

        outputFileStream << "Error in file arguments." << endl;
        return 1;
    }

    Confs confs;

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

    Cluster cluster(confs.get_number_of_clusters(),method);

    while (inputFileStream and getline(inputFileStream,point)){

        pointStream.str(point);

        //pointID holds the id of point
        getline(pointStream,pointID,' ');

        //collect all vector components
        while (getline(pointStream,token,' ')) 
            if (token!="\r")
                pointVector.push_back(stof(token));

        Point currentPoint(pointID,pointVector);

        //insert vector to lsh system
        cluster.insertPoint(currentPoint);

        pointVector.clear();
        pointStream.clear();
    }


    auto start = high_resolution_clock::now();
    cluster.startClustering(confs);
    auto stop = high_resolution_clock::now();

    auto executionTime = duration_cast<seconds>(stop - start);

    cout << "Algorithm: ";

    switch (method){

        case _LLOYD:
            cout << "Lloyds" << endl;
            break;
        
        case _LSH:
            cout << "Range Search LSH" << endl;
            break;

        case _CUBE:
            cout << "Range Search HyperCube" << endl;
            break;
    }

    cluster.printCentroids();

    cout << "clustering_time: " << executionTime.count() << endl;

    cluster.silhouette();
    
    if (complete) cluster.printClusters();

    pointVector.clear();
    pointStream.clear();

}