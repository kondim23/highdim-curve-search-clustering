#include <iostream>
#include "../include/confs.h"

Confs::Confs(unsigned int K, unsigned int L, unsigned int k, 
        unsigned int M, unsigned int kc, unsigned int pr){

    this->number_of_clusters = K;
    this->number_of_vector_hash_tables = L;
    this->number_of_vector_hash_functions = k;
    this->max_number_M_hypercube = M;
    this->number_of_hypercube_dimensions = kc;
    this->number_of_probes = pr;
}

Confs::~Confs(){}

unsigned int Confs::get_number_of_clusters() {return this->number_of_clusters;}
unsigned int Confs::get_number_of_vector_hash_tables() {return this->number_of_vector_hash_tables;}
unsigned int Confs::get_number_of_vector_hash_functions() {return this->number_of_vector_hash_functions;}
unsigned int Confs::get_max_number_M_hypercube() {return this->max_number_M_hypercube;}
unsigned int Confs::get_number_of_hypercube_dimensions() {return this->number_of_hypercube_dimensions;}
unsigned int Confs::get_number_of_probes() {return this->number_of_probes;}
void Confs::set_number_of_clusters(unsigned int v) {this->number_of_clusters=v;}
void Confs::set_number_of_vector_hash_tables(unsigned int v) {this->number_of_vector_hash_tables=v;}
void Confs::set_number_of_vector_hash_functions(unsigned int v) {this->number_of_vector_hash_functions=v;}
void Confs::set_max_number_M_hypercube(unsigned int v) {this->max_number_M_hypercube=v;}
void Confs::set_number_of_hypercube_dimensions(unsigned int v) {this->number_of_hypercube_dimensions=v;}
void Confs::set_number_of_probes(unsigned int v) {this->number_of_probes=v;}