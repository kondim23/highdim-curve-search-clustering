#include "../include/lsh_discrete.h"

extern frechet_type frechet_distance_type;

DiscreteLSHcurve::DiscreteLSHcurve(unsigned int N, unsigned int dimensions, float delta, unsigned int L)
    : LSHcurve(N,dimensions,delta,L){

    srand(time(NULL));
    
    //initialize random float t-parameters
    for (int i=0 ; i<L ; i++)
        this->tParameters.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/delta)));

    frechet_distance_type=CONTINUOUS;
}

vector<float> DiscreteLSHcurve::hashFunction(unsigned int hashID, Sequence* curve){

    //implement 2d snapping padding
    return vector<float>();
}