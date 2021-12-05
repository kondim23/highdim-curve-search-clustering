#include "../include/lsh_continuous.h"
#include "../include/utils.h"
#include <cmath>

#define ERROR_TOLERANCE 10.0

extern frechet_type frechet_distance_type;

ContinuousLSHcurve::ContinuousLSHcurve(unsigned int N, unsigned int dimensions, double delta, unsigned int L)
    : LSHcurve(N,dimensions,delta,1){

    frechet_distance_type=M_CONTINUOUS;
}

vector<float> ContinuousLSHcurve::hashFunction(unsigned int hashID, Sequence* sequence){


    //implement filtering snapping minima maxima

    Curve* curve = (Curve*) sequence;
    vector<float> key = curve->getCurve(), pointA ,pointB ,pointC;
    double distanceAB,distanceBC,term;
    int counter=1;


    //filtering

    for (int i=0 ; i<key.size()-2 ; i++){

        pointA = vector<float>(counter++,key.at(i));
        pointB = vector<float>(counter++,key.at(i+1));
        pointC = vector<float>(counter++,key.at(i+2));

        distanceAB = calculate_distance(EUCLIDEAN,pointA,pointB);
        distanceBC = calculate_distance(EUCLIDEAN,pointB,pointC);

        if (distanceAB < ERROR_TOLERANCE and distanceBC < ERROR_TOLERANCE)
            key.erase(key.begin()+i+1);
    }


    //snapping
    //check later
    for (float& item : key){

        term = item > 0 ? this->delta/2.0 : 0.0-this->delta/2.0;
        item = floor((item+term)/this->delta)*this->delta;
    }


    //keep minima maxima

    for (int i=0 ; i<key.size()-2 ; i++){

        if (key.at(i) <= key.at(i+1) and key.at(i+1) <= key.at(i+2))
            key.erase(key.begin()+i+1);
    }


    //padding

    for (int i=key.size() ; i<curve->getCurve().size() ; i++)
        key.push_back(PADDING_VALUE);

    return key;
}