#ifndef SYSTEM_H
#define SYSTEM_H

#include "knn.h"

void knn_core(KNN *method, ifstream &inputFileStream, ifstream &queryFileStream, unsigned int N, float R);
pair<unsigned int, int> getPointCountAndDimensions(ifstream &inputFileStream);
void openCheckFiles(ifstream &inputFileStream, ifstream &queryFileStream, string inputFileName,
                    string outputFileName, string queryFileName);
unsigned int knnRecursivePrint(KNN *method,PQUnique<pair<double,Point*> > &approximateQueue,
                                priority_queue<pair<double,Point*> > &exactQueue);

#endif