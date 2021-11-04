#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>

using namespace std;

class Point
{
private:

    string id;
    vector<float> pointVector;

public:

    Point(string id, vector<float> &pointVector);
    ~Point();
    string getID();
    vector<float>& getvector();

};

#endif