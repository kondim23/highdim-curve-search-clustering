#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>
#include "sequence.h"

using namespace std;

class Point : public Sequence
{
public:

    Point(string id, vector<float> pointVector);
    ~Point();
    double get_distance(Sequence*);
    Sequence* get_copy();
    void printSequence(ofstream&);
};

#endif