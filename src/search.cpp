#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/lsh_vector.h"
#include "../include/lsh_continuous.h"
#include "../include/lsh_discrete.h"
#include "../include/hcube.h"
#include "../include/knn.h"
#include "../include/PQUnique.h"
#include "../include/PQUnique.t.hpp"
#include "../include/point.h"
#include "../include/curve.h"
#include "../include/utils.h"

#define DELTA_RATE 10000.0

using namespace std;
using namespace chrono;


ofstream outputFileStream;


unsigned int knnRecursivePrint(KNN *,PQUnique<pair<double,Sequence*> > &,
    priority_queue<pair<double,Sequence*> > &);

int main(int argc, char* argv[]){

    // Default values in case of no input
    unsigned int k_lsh=4, L=1, N=5, M=10, probes=2, k_hcube=14;
    double R=10000.0, delta=0.0;
    
    string inputFileName, queryFileName, outputFileName;
    ifstream inputFileStream, queryFileStream;
    searchMethodType algorithm=S_NONE;
    frechet_type metric=M_NONE;
    KNN *method;
    unsigned int inputPointCount=0;
    int inputPointDimensions=-1;
    stringstream pointStream;
    string point, token;


    //check and get arguments
    if (argc%2==0 or argc>15){

        cout << "Error in command line arguments" << endl;
        return 1;
    }

    for (int i=1 ; i<argc ; i+=2) {

        if(!strcmp(argv[i],"-i")) inputFileName = argv[i+1];
        else if(!strcmp(argv[i],"-q")) queryFileName = argv[i+1];
        else if(!strcmp(argv[i],"-k")) k_lsh = k_hcube = atoi(argv[i+1]);
        else if(!strcmp(argv[i],"-L")) L = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-o")) outputFileName = argv[i+1];
		else if(!strcmp(argv[i],"-algorithm")){

            if (!strcmp(argv[i+1],"Frechet")) algorithm = S_FRECHET;
            else if (!strcmp(argv[i+1],"LSH")) algorithm = S_LSH;
            else if (!strcmp(argv[i+1],"Hypercube")) algorithm = S_HCUBE;
        }
		else if(!strcmp(argv[i],"-metric")){

            if (!strcmp(argv[i+1],"continuous")) metric = M_CONTINUOUS;
            else if (!strcmp(argv[i+1],"discrete")) metric = M_DISCRETE;
        }
		else if(!strcmp(argv[i],"-N")) N = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-R")) R = atof(argv[i+1]);
        else if(!strcmp(argv[i],"-M")) M = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-probes")) probes = atoi(argv[i+1]);
		else if(!strcmp(argv[i],"-delta")) delta = atoi(argv[i+1]);
    }

    if (algorithm==S_NONE){
        
        cout << "Error: Undefined algorithm." << endl;
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

    if(queryFileName.empty()){

        cout << "Enter query file:" << endl;
        getline(cin,queryFileName);
    }

    if ((algorithm==S_FRECHET) != (metric!=M_NONE))
        cout << "Metric is only declared on Frechet algorithm - will be ignored." << endl;

    //opening files
    // openCheckFiles(inputFileStream,queryFileStream,inputFileName,outputFileName,queryFileName);

    outputFileStream.open(outputFileName,ofstream::trunc);
    inputFileStream.open(inputFileName);
    queryFileStream.open(queryFileName);

    if (not (outputFileStream and inputFileStream and queryFileStream)) {

        outputFileStream << "Error in file arguments." << endl;
        exit(1);
    }


    //calculating count and dimensions of input points
    // inputPointStats = getPointCountAndDimensions(inputFileStream);


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

    //TODO #2 calculate delta

    if (algorithm==S_FRECHET and !delta) delta = 4.0*2.0*(double)inputPointDimensions/DELTA_RATE;
    
    pointStream.clear();
    inputFileStream.clear();
    inputFileStream.seekg(SEEK_SET);

    if (algorithm==S_LSH)
        method = new LSHvector(k_lsh,L,inputPointCount,inputPointDimensions);
    else if (algorithm==S_HCUBE)
        method = new HCUBE(k_hcube,inputPointDimensions,probes,M);
    else if (metric==M_CONTINUOUS)
        method = new ContinuousLSHcurve(inputPointCount,inputPointDimensions,delta,L);
    else if (metric==M_DISCRETE)
        method = new DiscreteLSHcurve(inputPointCount,inputPointDimensions,delta,L);

    //lsh system initialization
    // LSHvector lsh(k_lsh,L,inputPointStats.first,inputPointStats.second);

    //execute knn and range search
    // knn_core(&lsh,inputFileStream,queryFileStream,N,R);
    // knn_core(method,inputFileStream,queryFileStream,N,R);

    {

        string pointID;
        priority_queue<pair<double,Sequence*> > resultPQueueExactKNN;
        set<Sequence*> resultInRange;
        Sequence *sequence;

        PQUnique<pair<double,Sequence*> >resultPQueueApproximateKNN(N);

        while (inputFileStream and getline(inputFileStream,point)){

            pointStream.str(point);

            //pointID holds the id of point
            getline(pointStream,pointID,' ');

            //insert vector to knn system
            // method->insert(new Point(pointID,pointVector));

            if (algorithm==S_FRECHET) sequence = new Curve(pointID,read_curve(pointStream));
            else sequence = new Point(pointID, read_point(pointStream));

            method->insert(sequence);

            pointStream.clear();
        }

        do{

            while (queryFileStream and getline(queryFileStream,point)){

                pointStream.str(point);

                //get Query ID
                getline(pointStream,pointID,' ');

                outputFileStream << "Query: " << pointID << endl;

                // sequence = new Point(pointID,pointVector);

                if (algorithm==S_FRECHET) sequence = new Curve(pointID,read_curve(pointStream));
                else sequence = new Point(pointID, read_point(pointStream));

                //call approximate knn and count execution time
                //resultPQueueApproximateKNN has max length N
                auto start = high_resolution_clock::now();
                method->approximateKNN(resultPQueueApproximateKNN,sequence);
                auto stop = high_resolution_clock::now();

                auto approximateTime = duration_cast<microseconds>(stop - start);

                //call exact knn and count execution time 
                start = high_resolution_clock::now();
                resultPQueueExactKNN = method->exactKNN(N,sequence);
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
                resultInRange = method->rangeSearch(R,sequence);

                outputFileStream << "R-near neighbours:" << endl;

                for (Sequence* pointPtr : resultInRange)
                    outputFileStream << pointPtr->getID() << endl;

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

    //close streams
    inputFileStream.close();
    queryFileStream.close();
    outputFileStream.close();

    delete method;

    return 0;
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