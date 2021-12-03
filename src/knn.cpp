#include "../include/knn.h"

//All virtual functions will be overloaded

unsigned int KNN::hashFunction(Sequence* point){}
KNN::KNN(){}
KNN::~KNN(){}
void KNN::insert(Sequence* point){}
void KNN::approximateKNN(PQUnique <pair<double, Sequence*> > &neighborsQueue,Sequence* point){}
priority_queue<pair<double, Sequence*> > KNN::exactKNN(unsigned int neighbours, Sequence* point){}
set<Sequence*> KNN::rangeSearch(double radius, Sequence* point){}
string KNN::getMethod() {return this->method;}