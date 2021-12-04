#ifndef CONFS_H
#define CONFS_H

class Confs
{
private:

    //all members of cluster.conf
    unsigned int number_of_clusters;
    unsigned int number_of_vector_hash_tables;
    unsigned int number_of_vector_hash_functions;
    unsigned int max_number_M_hypercube;
    unsigned int number_of_hypercube_dimensions;
    unsigned int number_of_probes;

public:

    //initialize with default values
    Confs(unsigned int K=5, unsigned int L=3, unsigned int k=4, 
        unsigned int M=10, unsigned int kc=3, unsigned int pr=2);
    ~Confs();

    //getters and setters
    unsigned int get_number_of_clusters();
    unsigned int get_number_of_vector_hash_tables();
    unsigned int get_number_of_vector_hash_functions();
    unsigned int get_max_number_M_hypercube();
    unsigned int get_number_of_hypercube_dimensions();
    unsigned int get_number_of_probes();
    void set_number_of_clusters(unsigned int);
    void set_number_of_vector_hash_tables(unsigned int);
    void set_number_of_vector_hash_functions(unsigned int);
    void set_max_number_M_hypercube(unsigned int);
    void set_number_of_hypercube_dimensions(unsigned int);
    void set_number_of_probes(unsigned int);
};

#endif