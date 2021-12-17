#include <iostream>
#include "../include/sequence.h"

Sequence::Sequence(string id, vector<float> pointVector){

    this->id=id;
    this->pointVector=pointVector;
}

Sequence::~Sequence(){}

string Sequence::getID(){return this->id;}

vector<float>& Sequence::getvector(){return this->pointVector;}

//set the key
void Sequence::setVector(vector<float> key){

    this->pointVector=key;
    return;
}

//will be overloaded
double Sequence::get_distance(Sequence*){}
Sequence* Sequence::get_copy(){}
void Sequence::printSequence(ofstream&){}