#ifndef VOCABULARY_H
#define VOCABULARY_H

#define NUM_DIR 50     // assuming we are given maximum 50 directories
#define TABLE_SIZE 1000000   // assuming we have maximum 1,000,000 unique words in all of corpus

// a particular word in the vocabulary
struct vocabulary_node{
    char* word;
    int term_frequencies[NUM_DIR];  // number of occurences in each file in corpus(0 index indicates the target file and then corpus from 1 onwards)
    int no_of_documents;    // no of documents where the word occurs
};

struct vocabulary{
    struct vocabulary_node* all_words[TABLE_SIZE];  // a hash table of words , direct hashing is follwed where if index is filled , then we check index + 1 and so on
    int filled_indices[TABLE_SIZE]; // an array containing all the indices of hash table wich have been filled
    int no_of_words;
};

// check for word in vocabulary , if present , then just increase its term frequencies , else add word to vocabulary
void add_word_to_vocabulary(struct vocabulary* corpus_vocabulary , char* word , int file_index);

#endif