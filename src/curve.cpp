#include "../include/curve.h"
#include "../include/lsh_curve.h"
#include "../include/utils.h"
#include "../include/fred/curve.hpp"
#include "../include/fred/point.hpp"
#include "../include/fred/frechet.hpp"

#define ERROR_TOLERANCE 10.0

extern frechet_type frechet_distance_type;

double discrete_frechet(Curve*,Curve*);
double continuous_frechet(Curve*,Curve*);
void fill_dynamic_array(double **,unsigned int,unsigned int,vector<vector<float> >&,vector<vector<float> >&);

Curve::Curve(string id, vector<vector<float> > pointVector) : Sequence(id,vector<float>()){

    this->curveVector = pointVector;
}

Curve::~Curve(){}

vector<vector<float> > Curve::getCurve(){

    return this->curveVector;
}

Sequence* Curve::get_copy(){

    return new Curve(*this);
}

double Curve::get_distance(Curve* point){

    return  frechet_distance_type==M_CONTINUOUS ? continuous_frechet(this,point) : discrete_frechet(this,point);
}

unsigned int Curve::get_curve_size(){

    return this->curveVector.size();
}

void Curve::filter_until_max_size(unsigned int max_size){

    unsigned int rate=1;

    while (this->curveVector.size() > max_size)
        this->curveVector = filter(this->curveVector,++rate);

    return;
}

vector<vector<float> > filter(vector<vector<float> > curve, float rate){

    vector<float> pointA ,pointB ,pointC;
    double distanceAB,distanceBC;

    for (int i=0 ; i<curve.size()-2 ; i++){

        pointA = vector<float>(curve.at(i));
        pointB = vector<float>(curve.at(i+1));
        pointC = vector<float>(curve.at(i+2));

        distanceAB = calculate_distance(EUCLIDEAN,pointA,pointB);
        distanceBC = calculate_distance(EUCLIDEAN,pointB,pointC);

        if (distanceAB < ERROR_TOLERANCE*rate and distanceBC < ERROR_TOLERANCE*rate)
            curve.erase(curve.begin()+(i--)+1);
    }

    return curve;
}

Curve Curve::mean_curve(Curve* c2){

    vector<vector<float> > curve1 = this->getCurve();
    vector<vector<float> > curve2 = c2->getCurve();
    vector<pair<vector<float>,vector<float> > > optimal_traversal;
    pair<vector<float>,vector<float> > traversal_element;
    vector<vector<float> > mean_c;
    vector<float> tempVec;
    unsigned int m1 = curve1.size(), m2 = curve2.size();
    double minPrev;
    int i,j,min_i,min_j;

    double dynamic[m1][m2];


    //fill dynamic programming array

    fill_dynamic_array((double **) dynamic,m1,m2,curve1,curve2);


    //find optimal traversal

    i = m1-1;
    j = m2-1;
    while(i!=0 and j!=0){

        optimal_traversal.push_back(make_pair(curve1.at(i),curve2.at(j)));
        minPrev = min(dynamic[i-1][j],min(dynamic[i-1][j-1],dynamic[i][j-1]));

        if (dynamic[i-1][j-1] == minPrev){

            i--;
            j--;
        }
        else if (dynamic[i][j-1] == minPrev) j--;
        else if (dynamic[i-1][j] == minPrev) i--;
    }

    while(i!=0) optimal_traversal.push_back(make_pair(curve1.at(i--),curve2.at(j)));
    while(j!=0) optimal_traversal.push_back(make_pair(curve1.at(i),curve2.at(j--)));
    optimal_traversal.push_back(make_pair(curve1.at(0),curve2.at(0)));


    //mean curve

    while (!optimal_traversal.empty()) {

        traversal_element = optimal_traversal.at(optimal_traversal.size()-1);
        optimal_traversal.pop_back();

        tempVec = add_vectors(traversal_element.first,traversal_element.second);
        tempVec = divide_vector(tempVec,2);
        mean_c.push_back(tempVec);
    }

    return Curve("mean",mean_c);
}

double continuous_frechet(Curve* c1,Curve* c2){

    //initialize fred classes

    FredCurve fc1(c1->getCurve().size()), fc2(c2->getCurve().size());

    for (vector<float> point : c1->getCurve()){

        FredPoint fp(c1->getCurve().at(0).size());

        for (int i=0 ; i< point.size() ; i++)
            fp.set(i,point.at(i));

        fc1.push_back(fp);
    }

    for (vector<float> point : c2->getCurve()){

        FredPoint fp(c2->getCurve().at(0).size());

        for (int i=0 ; i< point.size() ; i++)
            fp.set(i,point.at(i));

        fc2.push_back(fp);
    }

    //use fred implementation to get the result
    return Frechet::Continuous::distance(fc1,fc2).value;
}

double discrete_frechet(Curve* c1,Curve* c2){


    vector<vector<float> > curve1 = c1->getCurve();
    vector<vector<float> > curve2 = c2->getCurve();
    unsigned int m1 = curve1.size(), m2 = curve2.size();

    double dynamic[m1][m2];


    //fill dynamic programming array

    fill_dynamic_array((double **) dynamic,m1,m2,curve1,curve2);

    //return discrete frechet distance to (m1,m2)
    return dynamic[m1-1][m2-1];
}

void fill_dynamic_array(double **dynamic, unsigned int m1, unsigned int m2, vector<vector<float> >& c1, vector<vector<float> >& c2){

    double minPrev;

    for (int i=0 ; i<m1 ; i++)
        dynamic[i][0] = calculate_distance(EUCLIDEAN,c1.at(i),c2.at(0));

    for (int i=1 ; i<m2 ; i++)
        dynamic[0][i] = calculate_distance(EUCLIDEAN,c1.at(0),c2.at(i));

    for (int i=1 ; i<m1 ; i++)
        for (int j=1 ; j<m2 ; j++){

            minPrev = min(dynamic[i-1][j],min(dynamic[i-1][j-1],dynamic[i][j-1]));
            dynamic[i][j] = max(minPrev,calculate_distance(EUCLIDEAN,c1.at(i),c2.at(j)));
        }

    return;
}