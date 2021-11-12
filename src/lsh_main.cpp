#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/lsh.h"
#include "../include/point.h"
#include "../include/PQUnique.h"
#include "../include/PQUnique.t.hpp"
#include "../include/core_utils.h"

using namespace std;

ofstream outputFileStream;

int main(int argc, char* argv[]){

    // Default values in case of no input
    unsigned int k=4, L=1, N=5;
    double R=10000.0;
    
    string inputFileName, queryFileName, outputFileName;
    ifstream inputFileStream, queryFileStream;
    pair<unsigned int,int> inputPointStats;


    //check and get arguments
    if (argc%2==0 or argc>15){

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
		else if(!strcmp(argv[i],"-R")) R = atof(argv[i+1]);
    }

    //opening files
    openCheckFiles(inputFileStream,queryFileStream,inputFileName,outputFileName,queryFileName);


    //calculating count and dimensions of input points
    inputPointStats = getPointCountAndDimensions(inputFileStream);

    //lsh system initialization
    LSH lsh(k,L,inputPointStats.first,inputPointStats.second);

    //execute knn and range search
    knn_core(&lsh,inputFileStream,queryFileStream,N,R);

    //close streams
    inputFileStream.close();
    queryFileStream.close();
    outputFileStream.close();
}