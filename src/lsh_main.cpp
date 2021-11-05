#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/lsh.h"
#include "../include/point.h"
#include "../include/PQUnique.h"
#include "../include/PQUnique.t.hpp"

using namespace std;
using namespace chrono;

unsigned int knnRecursivePrint(PQUnique<pair<double,Point*> > &approximateQueue,
                                priority_queue<pair<double,Point*> > &exactQueue);

ofstream outputFileStream;

int main(int argc, char* argv[]){

    unsigned int k=4, L=1, N=5, inputPointCount=0;
    int inputPointDimensions=-1;
    string inputFileName, queryFileName, outputFileName, point, token, pointID;
    ifstream inputFileStream, queryFileStream;
    stringstream pointStream;
    vector<float> pointVector;
    priority_queue<pair<double,Point*> > resultPQueueExactKNN;
    set<Point*> resultInRange;
    double R=10000.0;


    //check and get arguments
    if (argc%2==0 or argc>17){

        outputFileStream << "Error in command line arguments" << endl;
        return 1;
    }

    for (int i=1 ; i<argc ; i+=2) {

        if(!strcmp(argv[i],"-i")) inputFileName = argv[i+1];
        else if(!strcmp(argv[i],"-q")) queryFileName = argv[i+1];
        else if(!strcmp(argv[i],"-k")) k = atoi(argv[i+1]);
        else if(!strcmp(argv[i],"-L")) L = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-o")) outputFileName = argv[i+1];
		else if(!strcmp(argv[i],"-N")) N = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-R")) R = atof(argv[i+1]);
    }

    PQUnique<pair<double,Point*> >resultPQueueApproximateKNN(N);

    //opening files
    outputFileStream.open(outputFileName,ofstream::trunc);
    inputFileStream.open(inputFileName);
    queryFileStream.open(queryFileName);

    if (not (outputFileStream and inputFileStream and queryFileStream)) {

        outputFileStream << "Error in file arguments." << endl;
        return 1;
    }


    //calculating count of input points 
    while (getline(inputFileStream,point)){

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

    //lsh system initialization
    LSH lsh(k,L,inputPointCount,inputPointDimensions);

    //insert points in lsh system
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
        lsh.insertInHashes(currentPoint);

        pointVector.clear();
        pointStream.clear();
    }


    while (queryFileStream and getline(queryFileStream,point)){

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
        lsh.approximateKNN(resultPQueueApproximateKNN,currentPoint);
        auto stop = high_resolution_clock::now();

        auto approximateTime = duration_cast<microseconds>(stop - start);

        //call exact knn and count execution time 
        start = high_resolution_clock::now();
        resultPQueueExactKNN = lsh.exactKNN(N,currentPoint);
        stop = high_resolution_clock::now();

        auto exactTime = duration_cast<microseconds>(stop - start);

        //both priority queues must have same length
        while (resultPQueueApproximateKNN.size() < resultPQueueExactKNN.size())
            resultPQueueExactKNN.pop();

        //print knn results
        knnRecursivePrint(resultPQueueApproximateKNN,resultPQueueExactKNN);

        //print execution times
        outputFileStream << "tLSH: " << approximateTime.count() << " microseconds" << endl;
        outputFileStream << "tTrue: " << exactTime.count() << " microseconds" << endl;

        //perform range search and print results
        resultInRange = lsh.rangeSearch(R,currentPoint);

        outputFileStream << "R-near neighbours:" << endl;

        for (Point* pointPtr : resultInRange)
            outputFileStream << pointPtr->getID() << endl;

        pointVector.clear();
        pointStream.clear();
    }
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
    outputFileStream << "distanceLSH: " << approximateNeighbour.first << endl;
    outputFileStream << "distanceTrue: " << exactNeighbour.first << endl;

    return index+1;
}