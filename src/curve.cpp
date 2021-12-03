#include "../include/curve.h"
#include "../include/lsh_curve.h"
#include "../include/utils.h"

extern frechet_type frechet_distance_type;

double discrete_frechet(Curve*,Curve*);
double continuous_frechet(Curve*,Curve*);

Curve::Curve(string id, vector<float> &pointVector) : Sequence(id,pointVector){

    this->curveVector = pointVector;
}

double Curve::get_distance(Curve* point){

    return  frechet_distance_type==CONTINUOUS ? continuous_frechet(this,point) : discrete_frechet(this,point);
}

//non implemented
double continuous_frechet(Curve*,Curve*){

    return 1.0;
}

//non implemented
double discrete_frechet(Curve*,Curve*){

    return 1.0;
}