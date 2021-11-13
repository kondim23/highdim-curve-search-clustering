#ifndef SYSTEM_H
#define SYSTEM_H

#include "knn.h"


//the core of KNN and range search - used in lsh_main and hcube_main
void knn_core(KNN *method, ifstream &inputFileStream, ifstream &queryFileStream, unsigned int N, float R);

//returns the count of points and count of dimensions of each point in input
pair<unsigned int, int> getPointCountAndDimensions(ifstream &inputFileStream);

//open files indicated by filenames and check for errors
void openCheckFiles(ifstream &inputFileStream, ifstream &queryFileStream, string inputFileName,
                    string outputFileName, string queryFileName);

//recursively print results of knn from given priority queue
unsigned int knnRecursivePrint(KNN *method,PQUnique<pair<double,Point*> > &approximateQueue,
                                priority_queue<pair<double,Point*> > &exactQueue);

#endif