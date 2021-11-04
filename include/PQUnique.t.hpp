#include <iostream>
#include "PQUnique.h"

using namespace std;

template <typename T>
PQUnique<T>::PQUnique(unsigned int size){

    this->sizeQueue=size;
}

template <typename T>
PQUnique<T>::~PQUnique(){}

template <typename T>
void PQUnique<T>::insert(T item){

    if (this->allItems.find(item) == this->allItems.end()) return;

    this->priorityQueue.push(item);
    this->allItems.insert(item);

    if (this->priorityQueue.size() > this->sizeQueue) {

        T popedItem = this->priorityQueue.top();
        this->allItems.erase(popedItem);
        this->priorityQueue.pop();
    }

    return;
}

template <typename T>
T PQUnique<T>::pop(){

    T popedItem = this->priorityQueue.top();
    this->allItems.erase(popedItem);
    this->priorityQueue.pop();

    return popedItem;
}

template <typename T>
bool PQUnique<T>::empty(){

    return this->allItems.empty();
}

template <typename T>
unsigned int PQUnique<T>::size(){

    return this->allItems.size();
}