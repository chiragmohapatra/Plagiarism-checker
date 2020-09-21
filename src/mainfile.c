#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

#include "../include/vocabulary.h"
#include "../include/helper_functions.h"

int main(int argc , char** argv){
    DIR *folder;
    struct dirent *entry;
    FILE* fptr;
    char buffer [BUFSIZ];
    char* pch;
    int original_size = 0;
    int file_index = 0;

    struct vocabulary* corpus_vocabulary = (struct vocabulary*)malloc(sizeof(struct vocabulary));
    corpus_vocabulary->no_of_words = 0;
    for(int i = 0 ; i < TABLE_SIZE ; i++){
        corpus_vocabulary->all_words[i] = NULL;
    }

    int corpus_file_sizes[NUM_DIR];
    char* file_names[NUM_DIR];

    fptr = fopen(argv[1] , "r");

    while(fgets(buffer , BUFSIZ , fptr)){
        pch = strtok (buffer," ()-.;,:?!\r\n");
        while (pch != NULL){
            if(strlen(pch) != 0 && pch[0] != '\r') {
                char* temp = malloc((strlen(pch)+1)*sizeof(char));
                int i;
                for(i = 0 ; pch[i] != '\0' ; i++){
                    if(isalpha(pch[i])){
                        temp[i] = tolower(pch[i]);
                    }
                    else
                        temp[i] = pch[i];
                }
                temp[i] = '\0';
                add_word_to_vocabulary(corpus_vocabulary , temp , file_index);
                original_size++;
            }
            pch = strtok (NULL, " ()-.;,:?!\r\n");
        }
    }
        
    fclose(fptr);
    file_index++;

    folder = opendir(argv[2]);
    if(folder == NULL){
        perror("Unable to read directory");
        return(1);
    }

    while( (entry=readdir(folder))){
        if(strcmp(entry->d_name , ".") == 0 || strcmp(entry->d_name , "..") == 0)
            continue;

        // to dismiss non .txt files
        char* temp = ".txt";
        if(strcmp ((entry -> d_name + strlen(entry -> d_name) - 4), temp))
            continue;

        file_names[file_index - 1] = (char*)malloc(strlen(entry->d_name)*sizeof(char));
        strcpy(file_names[file_index - 1] , entry->d_name);

        char* filename = malloc(sizeof(char) * (20+ strlen(entry-> d_name)));
        filename[0] = '\0';
        strcat(filename, argv[2]);
        strcat(filename, "/");
        strcat(filename, entry->d_name);

        FILE* fp = fopen(filename , "r");
        int plagiarized_size = 0;
        while(fgets(buffer , BUFSIZ , fp)){
            pch = strtok (buffer," ()-.;,:?!\r\n");
            while (pch != NULL)
            {
                if(strlen(pch) != 0 && pch[0] != '\r') {
                    char* temp = malloc((strlen(pch) + 1)*sizeof(char));
                    int i;
                    for(i = 0 ; pch[i] != '\0' ; i++){
                        if(isalpha(pch[i])){
                            temp[i] = tolower(pch[i]);
                        }
                        else
                            temp[i] = pch[i];
                    }
                    temp[i] = '\0';
                    add_word_to_vocabulary(corpus_vocabulary , temp , file_index);
                    plagiarized_size++;
                }
                pch = strtok (NULL, " ()-.;,:?!\r\n");
            }
        }
        fclose(fp);
        corpus_file_sizes[file_index - 1] = plagiarized_size;
        file_index++;

        free(filename);
    }

    closedir(folder);

    calculate_similarity(corpus_vocabulary , file_index , original_size , corpus_file_sizes , file_names);

    for(int i = 0 ; i < file_index - 1 ; i++){
        free(file_names[i]);
    }

    for(int i = 0 ; i < corpus_vocabulary->no_of_words ; i++){
        free(corpus_vocabulary->all_words[corpus_vocabulary->filled_indices[i]]);
    }

    free(corpus_vocabulary);

    return(0);
}