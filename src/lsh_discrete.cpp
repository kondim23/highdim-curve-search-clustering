#include <cmath>
#include "../include/lsh_discrete.h"

extern frechet_type frechet_distance_type;

DiscreteLSHcurve::DiscreteLSHcurve(unsigned int N, unsigned int dimensions, double delta, unsigned int L)
    : LSHcurve(N,dimensions,delta,L){

    srand(time(NULL));
    
    //initialize random float t-parameters
    for (int i=0 ; i<2*L ; i++)
        this->tParameters.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/delta)));

    frechet_distance_type=M_DISCRETE;
}

vector<float> DiscreteLSHcurve::hashFunction(unsigned int hashID, Sequence* sequence){

    //implement 2d snapping padding

    Curve* curve = (Curve*)sequence;
    vector<float> key;
    float t, snapped_p, term, counter=1.0;


    //2d snapping

    for (float item : curve->getCurve()){

        t = this->tParameters.at(hashID);
        snapped_p = (counter++)-t;
        term = snapped_p>0 ? this->delta/2.0 : 0.0-this->delta/2.0;
        key.push_back(floor((snapped_p+term)/this->delta)*this->delta);

        t = this->tParameters.at(hashID+1);
        snapped_p = item-t;
        term = snapped_p>0 ? this->delta/2.0 : 0.0-this->delta/2.0;
        key.push_back(floor((snapped_p+term)/this->delta)*this->delta);

        if (key.at(key.size()-1)==key.at(key.size()-3) and key.at(key.size()-2)==key.at(key.size()-4)){

            key.pop_back();
            key.pop_back();
        }
    }


    //padding
    for (int i=key.size() ; i<this->tParameters.size() ; i++)
        key.push_back(PADDING_VALUE);

    return key;
}