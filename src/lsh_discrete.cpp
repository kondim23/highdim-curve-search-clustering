#include <cmath>
#include "../include/lsh_discrete.h"

extern frechet_type frechet_distance_type;

DiscreteLSHcurve::DiscreteLSHcurve(unsigned int N, unsigned int dimensions, double delta, unsigned int L, unsigned int point_dimensions)
    : LSHcurve(N,dimensions,delta,L){

    srand(time(NULL));
    
    //initialize random float t-parameters
    for (int i=0 ; i<point_dimensions*L ; i++)
        this->tParameters.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/delta)));

    frechet_distance_type=M_DISCRETE;
    this->dimensions = point_dimensions;
}

vector<float> DiscreteLSHcurve::hashFunction(unsigned int hashID, Sequence* sequence){

    //implement 2d snapping padding

    Curve* curve = (Curve*)sequence;
    vector<float> key/*, curve = ((Curve*)sequence)->getCurve()*/;
    float t, snapped_p, term, counter=1.0;
    bool duplicate=true;

    //2d snapping

    // for (float item : curve->getCurve()){
    // for (int i=0 ; i<curve.size() ; i+=this->dimensions){

    //     // t = this->tParameters.at(hashID);
    //     // // snapped_p = (counter++)-t;
    //     // snapped_p = curve.at(i)-t;
    //     // term = snapped_p>0 ? this->delta/2.0 : 0.0-this->delta/2.0;
    //     // key.push_back((floor((snapped_p+term)/this->delta)*this->delta)+t);

    //     // t = this->tParameters.at(hashID+1);
    //     // snapped_p = curve.at(i+1)-t;
    //     // term = snapped_p>0 ? this->delta/2.0 : 0.0-this->delta/2.0;
    //     // key.push_back((floor((snapped_p+term)/this->delta)*this->delta)+t);

    //     // if (key.at(key.size()-1)==key.at(key.size()-3) and key.at(key.size()-2)==key.at(key.size()-4)){

    //     //     key.pop_back();
    //     //     key.pop_back();
    //     // }

    //     for (int j=0 ; j<this->dimensions ; j++){

    //         t = this->tParameters.at(hashID+j%this->dimensions);
    //         snapped_p = curve.at(i+j)-t;
    //         term = snapped_p>0 ? this->delta/2.0 : 0.0-this->delta/2.0;
    //         key.push_back((floor((snapped_p+term)/this->delta)*this->delta)+t);
    //     }

    //     duplicate=true;

    //     for (int j=0 ; j<this->dimensions ; j++)
    //         duplicate = duplicate and key.at(key.size()-j-1)==key.at(key.size()-1-j-this->dimensions);

    //     if (duplicate)
    //         for (int j=0 ; j<this->dimensions ; j++)
    //             key.pop_back();
    // }



    //2d snapping

    for (vector<float> point : curve->getCurve()){
    
        for (int i=0 ; i<point.size() ; i++){

            t = this->tParameters.at(hashID*point.size()+i);
            snapped_p = point.at(i)-t;
            term = snapped_p>0 ? this->delta/2.0 : 0.0-this->delta/2.0;
            key.push_back((floor((snapped_p+term)/this->delta)*this->delta)+t);
        }

        if(key.size()>point.size()){

            duplicate=true;

            for (int j=0 ; j<point.size() ; j++)
                duplicate = duplicate and key.at(key.size()-j-1)==key.at(key.size()-1-j-point.size());

            if (duplicate)
                for (int j=0 ; j<point.size() ; j++)
                    key.pop_back();
        }
    }


    //padding
    for (int i=key.size() ; i<this->tParameters.size() ; i++)
        key.push_back(PADDING_VALUE);

    return key;
}