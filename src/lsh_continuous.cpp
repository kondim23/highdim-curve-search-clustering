#include "../include/lsh_continuous.h"
#include "../include/utils.h"
#include <cmath>

extern frechet_type frechet_distance_type;

ContinuousLSHcurve::ContinuousLSHcurve(unsigned int N, unsigned int dimensions, double delta, unsigned int L)
    : LSHcurve(N,dimensions,delta,1){

    frechet_distance_type=M_CONTINUOUS;
}

//implement filtering, snapping, keep minima maxima
vector<float> ContinuousLSHcurve::hashFunction(unsigned int hashID, Sequence* sequence){

    // Curve* curve = (Curve*) sequence;
    vector<vector<float> > curve = ((Curve*) sequence)->getCurve();
    vector<float> pointA ,pointB ,pointC, key;
    double distanceAB,distanceBC,term;
    unsigned int keysize = curve.size();


    //filtering
    curve = filter(((Curve*) sequence)->getCurve());

    //reduce dimension to 1
    for(vector<float> point : curve) key.push_back(point.at(1));


    //snapping

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

    for (int i=key.size() ; i<keysize ; i++)
        key.push_back(PADDING_VALUE);

    return key;
}