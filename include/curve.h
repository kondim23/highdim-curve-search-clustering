#ifndef CURVE_H
#define CURVE_H

#include <iostream>
#include <vector>
#include "sequence.h"

using namespace std;

class Curve : public Sequence
{
private:

    vector<vector<float> > curveVector;

public:

    Curve(string id, vector<vector<float> > pointVector);
    ~Curve();
    double get_distance(Curve*);
    vector<vector<float> > getCurve();
    void setVector(vector<float>);
    Sequence* get_copy();
    Curve mean_curve(Curve*);
    unsigned int get_curve_size();
    void filter_until_max_size(unsigned int max_size);
};

vector<vector<float> > filter(vector<vector<float> >, float rate=0.1);

#endif