#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <sstream>
#include <fstream>
using std::vector;

using namespace std;

typedef enum{ZERO,HAMMING,EUCLIDEAN} normType;

//calculate distance of 2 vectors
double calculate_distance(unsigned int normSize, vector<float>&,vector<float>&);

//multiply 2 vectors
double vector_multiply(vector<float>&,vector<float>&);

//add 2 vectors
vector<float> add_vectors(vector<float>&,vector<float>&);

//divide a vector by a number
vector<float> divide_vector(vector<float>&,int);

vector<vector<float> > read_curve(stringstream&);

vector<float> read_point(stringstream&);

//returns the count of points and count of dimensions of each point in input
pair<unsigned int, int> getPointCountAndDimensions(ifstream &);

#endif