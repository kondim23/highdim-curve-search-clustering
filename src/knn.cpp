#include "../include/knn.h"

//All virtual functions will be overloaded

unsigned int KNN::hashFunction(Point& point){}
KNN::KNN(){}
KNN::~KNN(){}
void KNN::insert(Point& point){}
void KNN::approximateKNN(PQUnique <pair<double, Point*> > &neighborsQueue,Point& point){}
priority_queue<pair<double, Point*> > KNN::exactKNN(unsigned int neighbours, Point& point){}
set<Point*> KNN::rangeSearch(double radius, Point& point){}
string KNN::getMethod() {return this->method;}