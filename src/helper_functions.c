#include "../include/helper_functions.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// assuming f1 and f2 to be float vectors with size n , returns the cosine of the angle between the two vectors
float cosine_similarity(float* f1 , float* f2 , int n){
    float numerator = 0 , modf1 = 0 , modf2 = 0;

    for(int i = 0 ; i < n ; i++){
        numerator+=(f1[i]*f2[i]);
        modf1+=(f1[i]*f1[i]);
        modf2+=(f2[i]*f2[i]);
    }

    modf1 = sqrtf(modf1);
    modf2 = sqrtf(modf2);

    return numerator/(modf1*modf2);
}

// calculate similarity between files using cosine similarity and tf-idf , file index represents total number of files including original and corpus
void calculate_similarity(struct vocabulary* corpus_vocabulary , int file_index , int original_size , int* corpus_file_sizes , char** file_names){
    float* original_text_tf_idf = (float*)(malloc((corpus_vocabulary->no_of_words)*sizeof(float)));

    for(int i = 0 ; i < corpus_vocabulary->no_of_words ; i++){
        float tf = (float)(corpus_vocabulary->all_words[corpus_vocabulary->filled_indices[i]]->term_frequencies[0])/(float)(original_size);
        original_text_tf_idf[i] = tf*(log(file_index/corpus_vocabulary->all_words[corpus_vocabulary->filled_indices[i]]->no_of_documents));
    }

    for(int i = 0 ; i < file_index - 1 ; i++){
        float* plagiarized_text_tf_idf = (float*)(malloc((corpus_vocabulary->no_of_words)*sizeof(float)));

        for(int j = 0 ; j < corpus_vocabulary->no_of_words ; j++){
            float tf = (float)(corpus_vocabulary->all_words[corpus_vocabulary->filled_indices[j]]->term_frequencies[i+1])/(float)(corpus_file_sizes[i]);
            plagiarized_text_tf_idf[j] = tf*(log(file_index/corpus_vocabulary->all_words[corpus_vocabulary->filled_indices[j]]->no_of_documents));
        }

        printf("%s  %f\n" , file_names[i] , 100*cosine_similarity(original_text_tf_idf , plagiarized_text_tf_idf , corpus_vocabulary->no_of_words));

        free(plagiarized_text_tf_idf);
    }

    free(original_text_tf_idf);
}