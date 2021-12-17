#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include "../include/utils.h"

using namespace std;

//normSize-norm  i.e. normSize==2 Euclidean
double calculate_distance(unsigned int normSize, vector<float>& vector1,vector<float>& vector2){

    double result=0, term;

    for (int i=0 ; i<vector1.size() and i<vector2.size() ; i++){

        term = pow(abs(vector1.at(i)-vector2.at(i)),normSize);
        result+=term;
    }

    return pow(result,1.0/static_cast<double> (normSize));
}

//based on sum of a_i*b_i for i in [0,d]
double vector_multiply(vector<float>& vector1,vector<float>& vector2){

    double result=0;

    for (int i=0 ; i<vector1.size() and i<vector2.size() ; i++) 
        result+=vector1.at(i)*vector2.at(i);

    return result;
}

//based on a_i =b_i + c_i
vector<float> add_vectors(vector<float>& v1,vector<float>& v2){

    vector<float> vecToReturn;

    for(int i=0 ; i<v1.size() and i<v2.size() ; i++) 
        vecToReturn.push_back(v1.at(i)+v2.at(i));

    return vecToReturn;
}

//based on a_i = b_i / mul
vector<float> divide_vector(vector<float>& vec,int mul){

    vector<float> vecToReturn;

    for(int i=0 ; i<vec.size() ; i++) 
        vecToReturn.push_back(vec.at(i)/static_cast<float>(mul));

    return vecToReturn;
}


//read input curve and add time dimension
vector<vector<float> > read_curve(stringstream& curveStream){

    string token;
    vector<vector<float> > curve;
    float counter=1.0;

    while (getline(curveStream,token,'\t')) 
            if (token!="\r")
                curve.push_back(vector<float>{counter++,stof(token)});

    return curve;
}


//read input point 
vector<float> read_point(stringstream& pointStream){

    string token;
    vector<float> pointVector;

    while (getline(pointStream,token,'\t')) 
            if (token!="\r")
                pointVector.push_back(stof(token));

    return pointVector;
}

//get count of sequences and dimensions of sequences from input
pair<unsigned int,int> getPointCountAndDimensions(ifstream &inputFileStream){

    string point, token;
    unsigned int inputPointCount=0;
    int inputPointDimensions=-1;
    stringstream pointStream;

    while (inputFileStream and getline(inputFileStream,point)){

        //calculating count of sequence dimensions
        if (inputPointCount==0){

            pointStream.str(point);
            while (getline(pointStream,token,'\t')) 
                if (token!="\r")
                    inputPointDimensions++;
            pointStream.clear();
        }
        inputPointCount++;
    }
    
    pointStream.clear();
    inputFileStream.clear();
    inputFileStream.seekg(SEEK_SET);

    return make_pair(inputPointCount,inputPointDimensions);
}