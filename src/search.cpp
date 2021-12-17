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

//maximum approximation factor
double maf=0.0;


unsigned int knnRecursivePrint(KNN *,PQUnique<pair<double,Sequence*> > &,
    priority_queue<pair<double,Sequence*> > &);

int main(int argc, char* argv[]){

    // Default values in case of no input
    unsigned int k_lsh=4, L=5, N=5, M=10, probes=2, k_hcube=14;
    double R=10000.0, delta=0.0;
    
    string inputFileName, queryFileName, outputFileName;
    ifstream inputFileStream, queryFileStream;
    searchMethodType algorithm=S_NONE;
    frechet_type metric=M_NONE;
    KNN *method;
    unsigned int inputSequenceCount=0;
    int inputSequenceLength=-1;
    stringstream sequenceStream;
    string input_sequence, token;
    unsigned int sum_time_exact=0.0, sum_time_approximate=0.0, searchCount=0.0; 


    //check and get arguments
    if (argc%2==0 or argc>21){

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

    //get filenames in not provided

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

    outputFileStream.open(outputFileName,ofstream::trunc);
    inputFileStream.open(inputFileName);
    queryFileStream.open(queryFileName);

    if (not (outputFileStream and inputFileStream and queryFileStream)) {

        outputFileStream << "Error in file arguments." << endl;
        exit(1);
    }


    //calculating count and dimensions of input points

    while (inputFileStream and getline(inputFileStream,input_sequence)){

        //calculating count of input_sequence dimensions
        if (inputSequenceCount==0){

            sequenceStream.str(input_sequence);
            while (getline(sequenceStream,token,'\t')) 
                if (token!="\r")
                    inputSequenceLength++;
            sequenceStream.clear();
        }
        inputSequenceCount++;
    }

    //calculate delta

    if (algorithm==S_FRECHET and !delta) delta = 4.0*2.0*(double)inputSequenceLength/DELTA_RATE;
    
    sequenceStream.clear();
    inputFileStream.clear();
    inputFileStream.seekg(SEEK_SET);


    //set proper method

    if (algorithm==S_LSH)
        method = new LSHvector(k_lsh,L,inputSequenceCount,inputSequenceLength);
    else if (algorithm==S_HCUBE)
        method = new HCUBE(k_hcube,inputSequenceLength,probes,M);
    else if (metric==M_CONTINUOUS)
        method = new ContinuousLSHcurve(inputSequenceCount,inputSequenceLength,delta,L);
    else if (metric==M_DISCRETE)
        method = new DiscreteLSHcurve(inputSequenceCount,inputSequenceLength,delta,L);

    {

        string sequenceID;
        priority_queue<pair<double,Sequence*> > resultPQueueExactKNN;
        set<Sequence*> resultInRange;
        Sequence *sequence;

        PQUnique<pair<double,Sequence*> >resultPQueueApproximateKNN(N);

        while (inputFileStream and getline(inputFileStream,input_sequence)){

            sequenceStream.str(input_sequence);

            //sequenceID holds the id of sequence
            getline(sequenceStream,sequenceID,'\t');

            //insert vector to knn system

            if (algorithm==S_FRECHET) sequence = new Curve(sequenceID,read_curve(sequenceStream));
            else sequence = new Point(sequenceID, read_point(sequenceStream));

            method->insert(sequence);

            sequenceStream.clear();
        }

        do{

            while (queryFileStream and getline(queryFileStream,input_sequence)){

                sequenceStream.str(input_sequence);

                //get Query ID
                getline(sequenceStream,sequenceID,'\t');

                outputFileStream << "Query: " << sequenceID << endl;
                outputFileStream << "Algorithm: " << method->getMethod() << endl;

                //define sequence
                if (algorithm==S_FRECHET) sequence = new Curve(sequenceID,read_curve(sequenceStream));
                else sequence = new Point(sequenceID, read_point(sequenceStream));

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

                //update statistics
                sum_time_approximate += approximateTime.count();
                sum_time_exact += exactTime.count();
                searchCount++;

                sequenceStream.clear();
            }

            outputFileStream << "tApproximateAverage: " << sum_time_approximate/searchCount << " microseconds" << endl;
            outputFileStream << "tTrueAverage: " << sum_time_exact/searchCount << " microseconds" << endl;
            outputFileStream << "MAF: " << maf << endl;
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

    //only print the min-distance element 
    if (index==1){
        outputFileStream << "Approximate Nearest neighbor: " << approximateNeighbour.second->getID() << endl;
        outputFileStream << "True Nearest neighbor: " << exactNeighbour.second->getID() << endl;
        outputFileStream << "distanceApproximate: " << approximateNeighbour.first << endl;
        outputFileStream << "distanceTrue: " << exactNeighbour.first << endl;
        maf = max(maf,approximateNeighbour.first/exactNeighbour.first);
    }

    return index+1;
}