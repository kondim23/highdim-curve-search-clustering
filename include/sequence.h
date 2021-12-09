#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <iostream>
#include <vector>

using namespace std;

class Sequence
{
protected:

    //id of point 
    string id;

    //vector of point
    vector<float> pointVector;

public:

    Sequence(string id, vector<float> pointVector);
    ~Sequence();
    string getID();
    vector<float>& getvector();
    virtual double get_distance(Sequence*);
    void setVector(vector<float> key);
    virtual Sequence* get_copy();
};

#endif