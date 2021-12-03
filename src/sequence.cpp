#include <iostream>
#include "../include/sequence.h"

Sequence::Sequence(string id, vector<float> &pointVector){

    this->id=id;
    this->pointVector=pointVector;
}

Sequence::~Sequence(){}

string Sequence::getID(){return this->id;}

vector<float>& Sequence::getvector(){return this->pointVector;}