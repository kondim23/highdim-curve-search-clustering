#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/point.h"
#include "../include/hcube.h"
#include "../include/core_utils.h"

using namespace std;

ofstream outputFileStream;

int main(int argc, char* argv[]){

    // Default values in case of no input
    unsigned int k=14, N=1, M=10, probes=2;
    double R=10000.0;

    string inputFileName, queryFileName, outputFileName;
    ifstream inputFileStream, queryFileStream;
    int inputPointDimensions;
    

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

    //opening files
    openCheckFiles(inputFileStream,queryFileStream,inputFileName,outputFileName,queryFileName);

    //calculating count and dimensions of input points
    inputPointDimensions = getPointCountAndDimensions(inputFileStream).second;

    //HCUBE system initialization
    HCUBE hcube(k,inputPointDimensions,probes,M);

    //execute knn and range search
    knn_core(&hcube,inputFileStream,queryFileStream,N);

    //close streams
    inputFileStream.close();
    queryFileStream.close();
    outputFileStream.close();
}