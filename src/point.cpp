#include "../include/point.h"
#include "../include/utils.h"

Point::Point(string id, vector<float> pointVector) : Sequence(id,pointVector){}

Point::~Point(){}

Sequence* Point::get_copy(){

    return new Point(*this);
}

//get vector - euclidean distance
double Point::get_distance(Sequence* point){

    return calculate_distance(EUCLIDEAN,this->getvector(),point->getvector());
}

void Point::printSequence(ofstream& out){

    for (int j=0 ; j<this->getvector().size() ; j++)
        out << " " << this->getvector().at(j);
}