#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>

using namespace std;

class Sequence
{
private:

    //id of point 
    string id;

    //vector of point
    vector<float> pointVector;

public:

    Sequence(string id, vector<float> &pointVector);
    ~Sequence();
    string getID();
    vector<float>& getvector();

};

#endif