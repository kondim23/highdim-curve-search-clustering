#include "../include/lsh_continuous.h"

extern frechet_type frechet_distance_type;

ContinuousLSHcurve::ContinuousLSHcurve(unsigned int N, unsigned int dimensions, double delta, unsigned int L)
    : LSHcurve(N,dimensions,delta,1){

    frechet_distance_type=M_CONTINUOUS;
}

vector<float> ContinuousLSHcurve::hashFunction(unsigned int hashID, Sequence* curve){

    //implement filtering snapping minima maxima
    return vector<float>();
}