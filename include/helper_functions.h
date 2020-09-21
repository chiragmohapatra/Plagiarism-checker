#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "vocabulary.h"

// assuming f1 and f2 to be float vectors with size n , returns the cosine of the angle between the two vectors
float cosine_similarity(float* f1 , float* f2 , int n);

// calculate similarity between files using cosine similarity and tf-idf
void calculate_similarity(struct vocabulary* corpus_vocabulary , int file_index , int original_size , int* corpus_file_sizes , char** file_names);



#endif