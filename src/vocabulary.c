#include "../include/vocabulary.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// compute a hash value for s
unsigned long long int compute_hash(const char* s) {
    const int p = 31;
    const int m = TABLE_SIZE;
    unsigned long long int hash_value = 0;
    unsigned long long int p_pow = 1;
    int n = strlen(s);
    for (int i = 0 ; i < n ; i++) {
        hash_value = (hash_value + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

// search for word in vocabulary , return index where it is found , not found then return -1
int search(struct vocabulary* corpus_vocabulary, char* x , int index){
    int ctr = 0;    // if ctr becomes TABLE_SIZE , it means the table is full , but we did not find our word
    while(corpus_vocabulary->all_words[index] != NULL && ctr < TABLE_SIZE){
        if(strcmp(corpus_vocabulary->all_words[index]->word , x) == 0)
            return index;

        index++;
        index = index % TABLE_SIZE;
        ctr++;
    }
    return -1;
} 

// add word to hash table
void insert(struct vocabulary* corpus_vocabulary , char* word , int file_index , int index){
    while(corpus_vocabulary->all_words[index] != NULL){
        index++;
        index = index % TABLE_SIZE;
    }

    corpus_vocabulary->all_words[index] = (struct vocabulary_node*)(malloc(sizeof(struct vocabulary_node)));
    corpus_vocabulary->all_words[index]->word = word;
    corpus_vocabulary->all_words[index]->no_of_documents = 1;
    for(int i = 0 ; i < NUM_DIR ; i++)
        corpus_vocabulary->all_words[index]->term_frequencies[i] = 0;

    corpus_vocabulary->all_words[index]->term_frequencies[file_index] = 1;
    corpus_vocabulary->filled_indices[corpus_vocabulary->no_of_words] = index;
    corpus_vocabulary->no_of_words++;
} 

// check for word in vocabulary , if present , then just increase its term frequencies , else add word to vocabulary
void add_word_to_vocabulary(struct vocabulary* corpus_vocabulary , char* word , int file_index){
    int index = (compute_hash(word))%TABLE_SIZE;
    int i1 = search(corpus_vocabulary , word , index);

    if(i1 == -1){
        insert(corpus_vocabulary , word , file_index , index);
    }

    else{
        if(corpus_vocabulary->all_words[i1]->term_frequencies[file_index] == 0)
            corpus_vocabulary->all_words[i1]->no_of_documents++;

        corpus_vocabulary->all_words[i1]->term_frequencies[file_index]++;
        free(word);
    }

}