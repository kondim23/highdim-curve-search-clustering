#include "../include/point.h"
#include "../include/utils.h"

Point::Point(string id, vector<float> &pointVector) : Sequence(id,pointVector){}

double Point::get_distance(Sequence* point){

    return calculate_distance(EUCLIDEAN,this->getvector(),point->getvector());
}