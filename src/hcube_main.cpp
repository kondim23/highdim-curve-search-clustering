#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/point.h"
#include "../include/hcube.h"

using namespace std;
using namespace chrono;

unsigned int knnRecursivePrint(PQUnique<pair<double,Point*> > &approximateQueue,
                                priority_queue<pair<double,Point*> > &exactQueue);

ofstream outputFileStream;

int main(int argc, char* argv[]){

    // Default values in case of no input

    unsigned int k=14, N=1, M=10, probes=2 , inputPointCount=0;
    double R=10000.0;


    int inputPointDimensions=-1; // -1 cause the first one is the pointId
    string inputFileName, queryFileName, outputFileName, point, token, pointID;
    ifstream inputFileStream, queryFileStream;
    stringstream pointStream;
    vector<float> pointVector;
    priority_queue<pair<double,Point*> > resultPQueueExactKNN;
    set<Point*> resultInRange;
    

    //check and get arguments
    if (argc%2==0 or argc>17){

        cout << "Error in command line arguments" << endl;
        return 1;
    }

    for (int i=1 ; i<argc ; i+=2) {

        if(!strcmp(argv[i],"-i")) inputFileName = argv[i+1];
        else if(!strcmp(argv[i],"-q")) queryFileName = argv[i+1];
        else if(!strcmp(argv[i],"-k")) k = atoi(argv[i+1]);
        else if(!strcmp(argv[i],"-M")) M = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-probes")) probes = atoi(argv[i+1]);
        else if(!strcmp(argv[i],"-o")) outputFileName = argv[i+1];
		else if(!strcmp(argv[i],"-N")) N = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-R")) R = atof(argv[i+1]);
    }

    PQUnique<pair<double,Point*> >resultPQueueApproximateKNN(N); // 

    //opening files
    outputFileStream.open(outputFileName,ofstream::trunc);
    inputFileStream.open(inputFileName);
    queryFileStream.open(queryFileName);

    if (not (outputFileStream and inputFileStream and queryFileStream)) {

        outputFileStream << "Error in file arguments." << endl;
        return 1;
    }

    //calculating count of input points 
    while (inputFileStream and getline(inputFileStream,point)){

        //calculating count of point dimensions
        if (inputPointCount==0){

            pointStream.str(point);
            while (getline(pointStream,token,' ')) 
                if (token!="\r")
                    inputPointDimensions++;
            pointStream.clear();
        }
        inputPointCount++;
    }

    pointStream.clear();
    inputFileStream.clear();
    inputFileStream.seekg(SEEK_SET);

    //HCUBE system initialization
    HCUBE hcube(k,inputPointCount,inputPointDimensions,probes,M);

    //insert points in hcube system
    while (inputFileStream and getline(inputFileStream,point)){

        pointStream.str(point);

        //pointID holds the id of point
        getline(pointStream,pointID,' ');

        //collect all vector components
        while (getline(pointStream,token,' ')) 
            if (token!="\r")
                pointVector.push_back(stof(token));

        Point currentPoint(pointID,pointVector);

        //insert vector to hcube system
        hcube.insertInHashe(currentPoint);

        pointVector.clear();
        pointStream.clear();
    }

    // Starting the implementation of the searches.

    do{

        while (queryFileStream and getline(queryFileStream,point)) {

            pointStream.str(point);

            //get Query ID
            getline(pointStream,pointID,' ');

            outputFileStream << "Query: " << pointID << endl;

            //update pointVector with query components
            while (getline(pointStream,token,' ')) 
                if (token!="\r")
                    pointVector.push_back(stof(token));

            Point currentPoint(pointID,pointVector);

            //call approximate knn and count execution time
            //resultPQueueApproximateKNN has max length N
            auto start = high_resolution_clock::now();
            hcube.approximateKNN(resultPQueueApproximateKNN,currentPoint);
            auto stop = high_resolution_clock::now();

            auto approximateTime = duration_cast<microseconds>(stop - start);

            //call exact knn and count execution time 
            start = high_resolution_clock::now();
            resultPQueueExactKNN = hcube.exactKNN(N,currentPoint);
            stop = high_resolution_clock::now();

            auto exactTime = duration_cast<microseconds>(stop - start);

            //both priority queues must have same length
            while (resultPQueueApproximateKNN.size() < resultPQueueExactKNN.size())
                resultPQueueExactKNN.pop();

            //print knn results
            knnRecursivePrint(resultPQueueApproximateKNN,resultPQueueExactKNN);

            //print execution times
            outputFileStream << "tHypercube: " << approximateTime.count() << " microseconds" << endl;
            outputFileStream << "tTrue: " << exactTime.count() << " microseconds" << endl;

            //perform range search and print results
            resultInRange = hcube.rangeSearch(R,currentPoint);

            outputFileStream << "R-near neighbours:" << endl;

            for (Point* pointPtr : resultInRange)
                outputFileStream << pointPtr->getID() << endl;

            pointVector.clear();
            pointStream.clear();
        }

        queryFileStream.close();

        cout << "Enter a new query-file filename or type \"exit\" to exit." << endl;
        getline(cin,queryFileName);

        if (queryFileName!="exit") queryFileStream.open(queryFileName);

    }while(queryFileName!="exit");
    
    outputFileStream.close();
}

//recursive print of elements in priority queues of KNN
unsigned int knnRecursivePrint(PQUnique<pair<double,Point*> > &approximateQueue,
                    priority_queue<pair<double,Point*> > &exactQueue){

    if (approximateQueue.empty() or exactQueue.empty()) return 1;

    pair<double,Point*> approximateNeighbour = approximateQueue.pop();

    pair<double,Point*> exactNeighbour = exactQueue.top();
    exactQueue.pop();

    unsigned int index = knnRecursivePrint(approximateQueue,exactQueue);

    outputFileStream << "Nearest neighbour-" << index << ": " << approximateNeighbour.second->getID() << endl;
    outputFileStream << "distanceHypercube: " << approximateNeighbour.first << endl;
    outputFileStream << "distanceTrue: " << exactNeighbour.first << endl;

    return index+1;
}