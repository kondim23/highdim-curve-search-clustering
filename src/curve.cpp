#include "../include/curve.h"
#include "../include/lsh_curve.h"
#include "../include/utils.h"

extern frechet_type frechet_distance_type;

double discrete_frechet(Curve*,Curve*);
double continuous_frechet(Curve*,Curve*);

Curve::Curve(string id, vector<float> &pointVector) : Sequence(id,pointVector){

    this->curveVector = pointVector;
}

vector<float> Curve::getCurve(){

    return this->curveVector;
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


    vector<float> curve1 = c1->getCurve(), point1;
    vector<float> curve2 = c2->getCurve(), point2;
    unsigned int m1,m2;
    double minPrev;

    for (m1=0 ; m1 < curve1.size() and curve1.at(m1) != PADDING_VALUE ; m1++){}
    for (m2=0 ; m2 < curve2.size() and curve1.at(m2) != PADDING_VALUE ; m2++){}

    double dynamic[m1][m2];

    for (int i=0 ; i<m1 ; i++){

        point1 = vector<float>(i+1,curve1.at(i));
        point2 = vector<float>(1,curve2.at(0));
        
        dynamic[i][0] = calculate_distance(EUCLIDEAN,point1,point2);
    }

    for (int i=1 ; i<m2 ; i++){

        point2 = vector<float>(i+1,curve2.at(i));
        point1 = vector<float>(1,curve1.at(0));
        
        dynamic[0][i] = calculate_distance(EUCLIDEAN,point1,point2);
    }

    for (int i=1 ; i<m1 ; i++){
        for (int j=1 ; j<m2 ; j++){

            point1 = vector<float>(i+1,curve1.at(i));
            point2 = vector<float>(i+1,curve2.at(j));

            minPrev = min(dynamic[i-1][j],min(dynamic[i-1][j-1],dynamic[i][j-1]));

            dynamic[i][j] = max(minPrev,calculate_distance(EUCLIDEAN,point1,point2));
        }
    }

    return dynamic[m1-1][m2-1];
}

