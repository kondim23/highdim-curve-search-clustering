#ifndef UTILS_H
#define UTILS_H

#include <vector>
using std::vector;

double calculate_distance(unsigned int normSize, vector<float>&,vector<float>&);
double vector_multiply(vector<float>&,vector<float>&);
vector<float> add_vectors(vector<float>&,vector<float>&);
vector<float> divide_vector(vector<float>&,int);

#endif