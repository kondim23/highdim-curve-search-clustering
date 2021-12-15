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

    vector<vector<float> > curve = ((Curve*)sequence)->getCurve();
    vector<float> key/*, curve = ((Curve*)sequence)->getCurve()*/;
    float t, snapped_p, term, counter=1.0, delta_division_result;
    bool duplicate=true;
    int count=0;


    //2d snapping

    for (vector<float> point : curve){
    
        for (int i=0 ; i<point.size() ; i++){
            count++;

            t = this->tParameters.at(hashID*point.size()+i);
            snapped_p = point.at(i)-t;
            if (snapped_p>0){

                term = this->delta/2.0;
                delta_division_result = floor((snapped_p+term)/this->delta);
            }
            else{

                term = 0.0-this->delta/2.0;
                delta_division_result = ceil((snapped_p+term)/this->delta);
            }
            key.push_back(delta_division_result*this->delta+t);
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
    for (int i=key.size() ; i<curve.size()*curve.at(0).size() ; i++)
        key.push_back(PADDING_VALUE);

    return key;
}