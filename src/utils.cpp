#include <iostream>
#include <cmath>
#include "../include/utils.h"

using namespace std;

//normSize-norm  i.e. normSize==2 Euclidean
double calculate_distance(unsigned int normSize, vector<float>& vector1,vector<float>& vector2){

    double result=0, term;

    for (int i=0 ; i<vector1.size() ; i++){

        term = pow(abs(vector1.at(i)-vector2.at(i)),normSize);
        result+=term;
    }

    return pow(result,1.0/static_cast<double> (normSize));
}

//based on sum of a_i*b_i for i in [0,d]
//needs add of cos(φ)
double vector_multiply(vector<float>& vector1,vector<float>& vector2){

    double result=0;

    for (int i=0 ; i<vector1.size() ; i++) 
        result+=vector1.at(i)*vector2.at(i);

    return result;
}