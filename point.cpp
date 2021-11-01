#include "point.h"

Point::Point(string id, vector<float> &pointVector){

    this->id=id;
    this->pointVector=pointVector;
}

Point::~Point(){}

string Point::getID(){return this->id;}

vector<float>& Point::getvector(){return this->pointVector;}