#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include "../include/myHashTable.h"
#include "../include/utils.h"
#include "../include/hcube.h"

#define BUCKET_MEAN_CAPACITY 8
#define WINDOW_SIZE 4.0

using namespace std;

static unsigned int tableSize;


//used for not exceding max probes and points in search
static int hd_probes,hd_M,temp_probes,temp_M;


HCUBE::HCUBE (unsigned int k, unsigned int dimensions,int probes,int M) {

    tableSize=pow(2,k);

    hd_probes = probes;
    hd_M = M;
    

    this->myHash = new myHashTable(k,tableSize,WINDOW_SIZE,dimensions);

    // initialize the vector of map for the h-f pairs 
    for (int i= 0 ; i < k ; i++) this->fMappings.push_back(map<int, int>());

    this->method = "Hypercube";
}

HCUBE::~HCUBE() {
    
    this->myHash->deleteAllAllocatedPoints();
    delete this->myHash;
}


void HCUBE::insert(Point &point) {


    Point* pointPtr = new Point(point);
    
    //insert point in hash
    this->myHash->storeInHash(this->hashFunction(point),pointPtr,0);

    return ;
}


unsigned int HCUBE::hashFunction(Point& point) {

    //holds parameters v and t for calculation of h
    pair<vector<float>, float> currentVTParameters; // vector for v , standard value for t
    int hi_keeper; //store temporary the hi(p) result
    int result;
    int fi_keeper; //store temporary the fi result
    map<int, int>::iterator itr;
    int sum = 0 ; // sum of the d' bits

    // Providing a seed value
    srand((unsigned) time(NULL));

    for (int i =0 ; i<this->fMappings.size() ; i++) {
        
        //get v and t
        currentVTParameters = myHash->getVTParameters(i); 

        //comput h_i
        hi_keeper = static_cast<int>((vector_multiply(point.getvector(),currentVTParameters.first)+currentVTParameters.second)/WINDOW_SIZE);

        //search h_i
        itr = fMappings[i].find(hi_keeper);

        if (itr == fMappings[i].end() ) {

            //set a new f and update map
            fi_keeper = abs(rand()) % 2;
            fMappings[i].insert(make_pair(hi_keeper,fi_keeper));
        }
        // get the already set bit from previous inputPoint with the same hi(p) result
        else fi_keeper = itr->second;

        if (fi_keeper) sum += (1 << (this->fMappings.size()-1-i));
    }

    return sum;
}


void HCUBE::approximateKNN(PQUnique <pair<double, Point*> > &neighborsQueue,Point& point) {

    temp_M = hd_M;
    temp_probes = hd_probes;
        
    //for every sequence of nums in hamming distance [1,k] call nextProbeKNN() until max probes or points reached
    for (int i = 1 ; i <= fMappings.size() ; i++ )
        if(hammingDistance(this->hashFunction(point),this->fMappings.size(),i,point,&HCUBE::nextProbe_KNN,&neighborsQueue,NULL))
            break;


    return ;
}

set<Point*> HCUBE::rangeSearch(double radius, Point& point) {

    temp_M = hd_M;
    temp_probes = hd_probes;

    set<Point*> setToReturn;
        
    //for every sequence of nums in hamming distance [1,k] call nextProbeRS() until max probes or points reached
    for (int i = 1 ; i <= fMappings.size() ; i++ )
        if(hammingDistance(this->hashFunction(point),this->fMappings.size(),i,point,&HCUBE::nextProbe_RS,&setToReturn,&radius))
            break;

    return setToReturn;    
}

//recursively calculate numbers of hamming distance 'distance' and call nextProbe
bool HCUBE::hammingDistance(unsigned  int number, int bit, int distance, Point& point, 
                            bool (HCUBE::*nextProbe)(void*,void*,Point&,unsigned int),void* p1,void* p2) {

    if (!distance) 
        return (this->*nextProbe)(p1,p2,point,number);
    else if (bit-1<0) 
        return false;

    //hamming distance with current bit
    if (hammingDistance(number,bit-1,distance,point,nextProbe,p1,p2))
        return true;

    //change current bit
    number = number ^ (1<<(bit-1));

    //hamming distance with new bit
    if (hammingDistance(number,bit-1,distance-1,point,nextProbe,p1,p2))
        return true;

    return false;
}


bool HCUBE::nextProbe_KNN(void* p1, void* p2 , Point& point,unsigned int number) {

    PQUnique<pair<double, Point*> >* pqUnique = (PQUnique<pair<double, Point*> >*) p1;

    if ( !temp_M or !temp_probes ) return true;

    //call knn and update pqUnique with results
    this->myHash->approximateKNN(*pqUnique,number,point,0);

    temp_probes--;
    temp_M -= this->myHash->getBucketPointCount(number);

    if ( temp_M <= 0 || !temp_probes ) return true;

    return false;
}

bool HCUBE::nextProbe_RS(void* p1, void* p2 , Point& point,unsigned int number) {

    set<Point*> tempSet;
    set<Point*>* pointsInRange = (set<Point*>*) p1;
    double radius = *(double*)p2;

    if ( !temp_M or !temp_probes ) return true;

    //call RS and get tempSet with results
    tempSet = this->myHash->rangeSearch(radius,number,point,0);

    //insert elements in result set excluding duplicates
    for(Point* item : tempSet)
        if(pointsInRange->find(item) == pointsInRange->end())
            pointsInRange->insert(item);

    temp_probes--;
    temp_M -= this->myHash->getBucketPointCount(number);

    if ( temp_M <= 0 || !temp_probes ) return true;

    return false;
}


priority_queue<pair<double, Point*> > HCUBE::exactKNN(unsigned int neighbours, Point& point){

    return this->myHash->exactKNN(neighbours,point);
}