#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>
#include "sequence.h"

using namespace std;

//represents a Point (Vector)
class Point : public Sequence
{
public:

    Point(string id, vector<float> pointVector);
    ~Point();

    //return l-norm distance between points
    double get_distance(Sequence*);

    //return a newly allocated copy
    Sequence* get_copy();

    //print point-vector
    void printSequence(ofstream&);
};

#endif