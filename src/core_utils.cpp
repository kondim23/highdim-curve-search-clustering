#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/core_utils.h"
#include "../include/sequence.h"
#include "../include/point.h"
#include "../include/PQUnique.h"
#include "../include/PQUnique.t.hpp"

using namespace std;
using namespace chrono;

extern ofstream outputFileStream;

void knn_core(KNN *method, ifstream &inputFileStream, ifstream &queryFileStream, unsigned int N, float R){

    string point, token, pointID, queryFileName;
    stringstream pointStream;
    vector<float> pointVector;
    priority_queue<pair<double,Sequence*> > resultPQueueExactKNN;
    set<Sequence*> resultInRange;

    PQUnique<pair<double,Sequence*> >resultPQueueApproximateKNN(N);

    while (inputFileStream and getline(inputFileStream,point)){

        pointStream.str(point);

        //pointID holds the id of point
        getline(pointStream,pointID,' ');

        //collect all vector components
        while (getline(pointStream,token,' ')) 
            if (token!="\r")
                pointVector.push_back(stof(token));

        //insert vector to knn system
        method->insert(new Point(pointID,pointVector));

        pointVector.clear();
        pointStream.clear();
    }

    do{

        while (queryFileStream and getline(queryFileStream,point)){

            pointStream.str(point);

            //get Query ID
            getline(pointStream,pointID,' ');

            outputFileStream << "Query: " << pointID << endl;

            //update pointVector with query components
            while (getline(pointStream,token,' ')) 
                if (token!="\r")
                    pointVector.push_back(stof(token));

            Sequence* currentSequence = new Point(pointID,pointVector);

            //call approximate knn and count execution time
            //resultPQueueApproximateKNN has max length N
            auto start = high_resolution_clock::now();
            method->approximateKNN(resultPQueueApproximateKNN,currentSequence);
            auto stop = high_resolution_clock::now();

            auto approximateTime = duration_cast<microseconds>(stop - start);

            //call exact knn and count execution time 
            start = high_resolution_clock::now();
            resultPQueueExactKNN = method->exactKNN(N,currentSequence);
            stop = high_resolution_clock::now();

            auto exactTime = duration_cast<microseconds>(stop - start);

            //both priority queues must have same length
            while (resultPQueueApproximateKNN.size() < resultPQueueExactKNN.size())
                resultPQueueExactKNN.pop();

            //print knn results
            knnRecursivePrint(method,resultPQueueApproximateKNN,resultPQueueExactKNN);

            //print execution times
            outputFileStream << "t" << method->getMethod() << ": " << approximateTime.count() << " microseconds" << endl;
            outputFileStream << "tTrue: " << exactTime.count() << " microseconds" << endl;

            //perform range search and print results
            resultInRange = method->rangeSearch(R,currentSequence);

            outputFileStream << "R-near neighbours:" << endl;

            for (Sequence* pointPtr : resultInRange)
                outputFileStream << pointPtr->getID() << endl;

            pointVector.clear();
            pointStream.clear();
        }

        cout << "Enter a new query-file filename or type \"exit\" to exit." << endl;
        getline(cin,queryFileName);

        if (queryFileName!="exit") {
            
            queryFileStream.close();
            queryFileStream.open(queryFileName);
        }

    }while(queryFileName!="exit");
}

//recursive print of elements in priority queues of KNN
unsigned int knnRecursivePrint(KNN *method,PQUnique<pair<double,Sequence*> > &approximateQueue,
                    priority_queue<pair<double,Sequence*> > &exactQueue){

    if (approximateQueue.empty() or exactQueue.empty()) return 1;

    pair<double,Sequence*> approximateNeighbour = approximateQueue.pop();

    pair<double,Sequence*> exactNeighbour = exactQueue.top();
    exactQueue.pop();

    unsigned int index = knnRecursivePrint(method,approximateQueue,exactQueue);

    outputFileStream << "Nearest neighbour-" << index << ": " << approximateNeighbour.second->getID() << endl;
    outputFileStream << "distance" << method->getMethod() << ": " << approximateNeighbour.first << endl;
    outputFileStream << "distanceTrue: " << exactNeighbour.first << endl;

    return index+1;
}


//open files and check filestreams
void openCheckFiles(ifstream &inputFileStream, ifstream &queryFileStream, string inputFileName, string outputFileName,
                    string queryFileName){

    outputFileStream.open(outputFileName,ofstream::trunc);
    inputFileStream.open(inputFileName);
    queryFileStream.open(queryFileName);

    if (not (outputFileStream and inputFileStream and queryFileStream)) {

        outputFileStream << "Error in file arguments." << endl;
        exit(1);
    }

    return;
}

//get count of points and dimensions of points from input
pair<unsigned int,int> getPointCountAndDimensions(ifstream &inputFileStream){

    string point, token;
    unsigned int inputPointCount=0;
    int inputPointDimensions=-1;
    stringstream pointStream;

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

    return make_pair(inputPointCount,inputPointDimensions);
}