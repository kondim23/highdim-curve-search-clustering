#ifndef LSH_DISCRETE
#define LSH_DISCRETE

#include "lsh_curve.h"
#include "curve.h"

class DiscreteLSHcurve : public LSHcurve
{
private:

    //all t parameters that shift the grids of discrete lsh algorithm 
    vector<float> tParameters;

    //discrete lsh hashing
    vector<float> hashFunction(unsigned int hashID, Sequence* curve);

public:

    DiscreteLSHcurve(unsigned int N, unsigned int dimensions, double delta, unsigned int L=5, unsigned int point_dimensions=2);
    ~DiscreteLSHcurve();
};

#endif