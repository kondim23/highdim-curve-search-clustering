#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <iostream>
#include <vector>

using namespace std;

//represents a sequence as read from input
//Point and Curve extend this class
class Sequence
{
protected:

    //id of sequence 
    string id;

    //vector-key of sequence
    vector<float> pointVector;

public:

    Sequence(string id, vector<float> pointVector);
    ~Sequence();

    string getID();

    //get vector-key
    vector<float>& getvector();

    virtual double get_distance(Sequence*);
    
    void setVector(vector<float> key);

    virtual Sequence* get_copy();
    
    virtual void printSequence(ofstream&);
};

#endif