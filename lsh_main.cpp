#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include "lsh.h"

#define WINDOWSIZE 6

using namespace std;

int main(int argc, char* argv[]){

    unsigned int k=4, L=1, N=5, R=10000, inputPointCount=0, inputPointDimensions=-1;
    string inputFileName, queryFileName, outputFileName, point, token; 
    ofstream outputFileStream;
    ifstream inputFileStream, queryFileStream;
    stringstream pointStream;
    vector<float> *pointVector;
    priority_queue<pair<double,vector<float>*> > resultPQueueApproximateKNN, resultPQueueExactKNN;

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


    //lsh initialization
    LSH lsh(k,L,inputPointCount,WINDOWSIZE,inputPointDimensions);


    //insert points in lsh system
    while (inputFileStream){

        getline(inputFileStream,point);
        pointStream.str(point);

        //a inside-lsh allocation on an adt is a better idea
        pointVector = new vector<float>;
        while (getline(pointStream,token,' ')) pointVector->push_back(stof(token));

        lsh.insertInHashes(pointVector);
    }
}