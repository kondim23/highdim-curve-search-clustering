#ifndef CURVE_H
#define CURVE_H

#include <iostream>
#include <vector>
#include "sequence.h"

using namespace std;

class Curve : public Sequence
{
private:

    //the curve: a vector of points, where a point is a vector of floats 
    vector<vector<float> > curveVector;

public:

    Curve(string id, vector<vector<float> > pointVector);
    ~Curve();

    //discrete - continuous frechet (overloads method of lsh_curve)
    double get_distance(Sequence*);

    vector<vector<float> > getCurve();

    //set the key
    void setVector(vector<float>);

    //return a new allocated copy
    Sequence* get_copy();

    //compute mean curve
    Curve mean_curve(Curve*);

    //return count of points
    unsigned int get_curve_size();

    //filter this->curvevector until it reaches complexity max_size
    void filter_until_max_size(unsigned int max_size);

    //print the curve
    void printSequence(ofstream&);
};

//filter given curve
vector<vector<float> > filter(vector<vector<float> >, float rate=0.1);

#endif