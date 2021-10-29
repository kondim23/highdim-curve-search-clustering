#include <iostream>
#include <cmath>
#include "utils.h"

using namespace std;

//normSize-norm  i.e. normSize==2 Euclidean
double calculate_distance(float normSize, vector<float>* vector1,vector<float>* vector2){

    float result=0, factorAbs, term;

    for (int i=0 ; i<vector1->size() ; i++){

        factorAbs = pow(abs(vector1->at(i)-vector2->at(i)),normSize);
        result+=term;
    }

    return pow(result,1.0/normSize);
}

//based on sum of a_i*b_i for i in [0,d]
//needs add of cos(φ)
double vector_multiply(vector<float>* vector1,vector<float>* vector2){

    float result=0;

    for (int i=0 ; i<vector1->size() ; i++) 
        result+=vector1->at(i)*vector2->at(i);

    return result;
}