#ifndef LSH_CONTINUOUS
#define LSH_CONTINUOUS

#include "lsh_curve.h"
#include "curve.h"

class ContinuousLSHcurve : public LSHcurve
{
private:

    vector<float> hashFunction(unsigned int hashID, Sequence* curve);

public:

    ContinuousLSHcurve(unsigned int N, unsigned int dimensions, double delta, unsigned int L=5);
    ~ContinuousLSHcurve();
};

#endif