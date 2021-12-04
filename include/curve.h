#ifndef CURVE_H
#define CURVE_H

#include <iostream>
#include <vector>
#include "sequence.h"

using namespace std;

class Curve : public Sequence
{
private:

    vector<float> curveVector;

public:

    Curve(string id, vector<float> &pointVector);
    ~Curve();
    double get_distance(Curve*);
    void setVector(vector<float>);
};

#endif