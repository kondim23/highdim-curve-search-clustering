#include "../include/curve.h"
#include "../include/lsh_curve.h"
#include "../include/utils.h"

extern frechet_type frechet_distance_type;

double discrete_frechet(Curve*,Curve*);
double continuous_frechet(Curve*,Curve*);

Curve::Curve(string id, vector<vector<float> > pointVector) : Sequence(id,vector<float>()){

    this->curveVector = pointVector;
}

vector<vector<float> > Curve::getCurve(){

    return this->curveVector;
}

Sequence* Curve::get_copy(){

    return new Curve(*this);
}

double Curve::get_distance(Curve* point){

    return  frechet_distance_type==M_CONTINUOUS ? continuous_frechet(this,point) : discrete_frechet(this,point);
}

//non implemented
double continuous_frechet(Curve*,Curve*){

    return 1.0;
}

//non implemented
//TODO #4 euclidean or 1d?
double discrete_frechet(Curve* c1,Curve* c2){


    vector<vector<float> > curve1 = c1->getCurve();
    vector<vector<float> > curve2 = c2->getCurve();
    unsigned int m1 = curve1.size(), m2 = curve2.size();
    double minPrev;

    double dynamic[m1][m2];

    for (int i=0 ; i<m1 ; i++)
        dynamic[i][0] = calculate_distance(EUCLIDEAN,curve1.at(i),curve2.at(0));

    for (int i=1 ; i<m2 ; i++)
        dynamic[0][i] = calculate_distance(EUCLIDEAN,curve1.at(0),curve2.at(i));

    for (int i=1 ; i<m1 ; i++)
        for (int j=1 ; j<m2 ; j++){

            minPrev = min(dynamic[i-1][j],min(dynamic[i-1][j-1],dynamic[i][j-1]));
            dynamic[i][j] = max(minPrev,calculate_distance(EUCLIDEAN,curve1.at(i),curve2.at(j)));
        }

    return dynamic[m1-1][m2-1];
}

