#include "../include/lsh_continuous.h"

extern frechet_type frechet_distance_type;

ContinuousLSHcurve::ContinuousLSHcurve(unsigned int N, unsigned int dimensions, unsigned int L)
    : LSHcurve(N,dimensions,1){

    frechet_distance_type=CONTINUOUS;
}

vector<float> ContinuousLSHcurve::hashFunction(unsigned int hashID, Sequence* curve){

    //implement filtering snapping minima maxima
    return vector<float>();
}