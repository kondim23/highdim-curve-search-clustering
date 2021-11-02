#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include "lsh.h"
#include "point.h"
#include "PQUnique.h"

#define WINDOWSIZE 6

using namespace std;

int main(int argc, char* argv[]){

    unsigned int k=4, L=1, N=5, R=10000, inputPointCount=0, inputPointDimensions=-1;
    string inputFileName, queryFileName, outputFileName, point, token, pointID;
    ofstream outputFileStream;
    ifstream inputFileStream, queryFileStream;
    stringstream pointStream;
    vector<float> pointVector;
    priority_queue<pair<double,Point*> > resultPQueueExactKNN;
    PQUnique<pair<double,Point*> >resultPQueueApproximateKNN;
    set<Point*> resultInRange;
    time_t start, stop;
    double approximateTime, exactTime;



    //check and get arguments
    if (argc%2==0 or argc>17){

        cout << "Error in command line arguments" << endl;
        return 1;
    }

    for (int i=1 ; i<argc ; i+=2) {

        if(!strcmp(argv[i],"-i")) inputFileName = argv[i+1];
        else if(!strcmp(argv[i],"-q")) queryFileName = argv[i+1];
        else if(!strcmp(argv[i],"-k")) k = atoi(argv[i+1]);
        else if(!strcmp(argv[i],"-L")) L = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-o")) outputFileName = argv[i+1];
		else if(!strcmp(argv[i],"-N")) N = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-R")) R = atoi(argv[i+1]);
    }



    //opening files
    outputFileStream.open(outputFileName);
    inputFileStream.open(inputFileName);
    queryFileStream.open(queryFileName);

    if (not (outputFileStream and inputFileStream and queryFileStream)) {

        cout << "Error if file arguments." << endl;
        return 1;
    }



    //calculating count of input points 
    while (inputFileStream){

        getline(inputFileStream,point);

        //calculating count of point dimensions
        if (inputPointCount==0){

            pointStream.str(point);
            while (getline(pointStream,token,' ')) inputPointDimensions++;
        }
        inputPointCount++;
    }

    inputFileStream.seekg(SEEK_SET);



    //lsh system initialization
    LSH lsh(k,L,inputPointCount,WINDOWSIZE,inputPointDimensions);



    //insert points in lsh system
    while (inputFileStream){

        getline(inputFileStream,point);
        pointStream.str(point);

        //pointID holds the id of point
        getline(pointStream,pointID,' ');

        //collect all vector components
        while (getline(pointStream,token,' ')) pointVector.push_back(stof(token));

        Point currentPoint(pointID,pointVector);

        //insert vector to lsh system
        lsh.insertInHashes(currentPoint);

        pointVector.clear();
    }


    while (queryFileStream){

        getline(queryFileStream,point);
        pointStream.str(point);

        //get Query ID
        getline(pointStream,pointID,' ');

        cout << "Query: " << pointID << endl;

        //update pointVector with query components
        while (getline(pointStream,token,' ')) pointVector.push_back(stof(token));

        Point currentPoint(pointID,pointVector);

        //call approximate knn and count execution time
        //resultPQueueApproximateKNN has max length N
        start = time(NULL);
        lsh.approximateKNN(resultPQueueApproximateKNN,currentPoint);
        stop = time(NULL);

        approximateTime = difftime(start,stop);

        //call exact knn and count execution time 
        start = time(NULL);
        resultPQueueExactKNN = lsh.exactKNN(N,currentPoint);
        stop = time(NULL);

        exactTime = difftime(start,stop);

        //both priority queues must have same length
        while (resultPQueueApproximateKNN.size() < resultPQueueExactKNN.size())
            resultPQueueExactKNN.pop();

        //print knn results
        knnRecursivePrint(resultPQueueApproximateKNN,resultPQueueExactKNN);

        //print execution times
        cout << "tLSH: " << approximateTime << endl;
        cout << "tTrue: " << exactTime << endl;

        //perform range search and print results
        resultInRange = lsh.rangeSearch(R,currentPoint);

        cout << "R-near neighbours:" << endl;

        for (Point* pointPtr : resultInRange)
            cout << pointPtr->getID() << endl;
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

    cout << "Nearest neighbour-" << index << ": " << approximateNeighbour.second->getID() << endl;
    cout << "distanceLSH: " << approximateNeighbour.first;
    cout << "distanceTrue: " << exactNeighbour.first;

    return index+1;
}