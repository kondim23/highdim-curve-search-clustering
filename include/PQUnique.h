#ifndef PQUNIQUE_H
#define PQUNIQUE_H

#include <queue>
#include <set>

using namespace std;

//a priority queue holding unique elements with max size

template <typename T>
class PQUnique
{
private:
    
    //all items sorted
    priority_queue<T> priorityQueue;

    //all items (for set::find and set::size use)
    set<T> allItems;

    //max length of priority queue
    unsigned int sizeQueue;

public:

    PQUnique(unsigned int size=INT32_MAX);
    ~PQUnique();
    void insert(T item);
    T pop();
    bool empty();
    unsigned int size();
};


#endif