// ------------------------------------------------------------ //
// created for COMP30023 Computer Systems - Project 2, 2019
// by Ang Mink Chen <minka@student.unimelb.edu.au>
// ------------------------------------------------------------ //

#include "guess_strategies.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <ctype.h>

// ------------------------------------------------------------ //
/* strategy functions */

// read a dictionary, use the words within it to crack passwords
// parameters: dictionary (text file), password hash file, the number to offset the hash ID to be printed
int dict_attack(char *dictionary, char *hash_file, int offset) {

     // define the number of hashes
    int hash_num = get_hash_num(hash_file);
    int remaining_hash_num = hash_num;

    // error 
    if (hash_num < 0) return 1;

    // arrays to store hashes
    BYTE  pwd_h[hash_num][SHA256_BLOCK_SIZE];
    BYTE temp_h[hash_num][SHA256_BLOCK_SIZE];

    // read hash file into arrays and handle error
    if (read_hash_file(hash_file, hash_num,  pwd_h) < 0) return 1;
    if (read_hash_file(hash_file, hash_num,  temp_h) < 0) return 1;

    // read dictionary
    FILE *f=fopen(dictionary, "r");
    if (f==NULL) {
        printf("file doesnt exist?!\n");
        return remaining_hash_num;
    }

    /* crack passwords */
    char *word = NULL;
    size_t len = 0;
    size_t line = 0;

    BYTE *hash_match = NULL;

    // compute hash for each word and compare them
    while ((line = getline(&word, &len, f)) != -1) {
        // remove the trailing newline character
        word[strlen(word)-1] = 0;

        hash_match = hash_compare(word, remaining_hash_num, temp_h);

        if (hash_match) {
            // print the password found
            printf("%s %d\n", word, get_hash_id(hash_match, hash_num, pwd_h)+offset);
            
            remaining_hash_num--;
            // remove the matching hash from temp array
            remove_hash(hash_match, remaining_hash_num, hash_num, temp_h);
        }
    }

    return remaining_hash_num;

}

// exhaustively search for all passwords, works in different ways based on 
// required passwords length (either 4 or 6)
// parameter: guess_len - number of characters required in a password, mode -
// to determine whether to show password generated only or that passwords cracking 
// is also required (0 for cracking passwords, any other integers for showing passwords only),
// guesses_num - number of guesses required by user
void exhaustive_search(int guess_len, int mode, int guesses_num) {

    if (guess_len == 4) {
        four_exhaustive();

    } else if (guess_len == 6) {
        six_exhaustive(mode, guesses_num);

    } else {
        printf("Error: Invalid length requested. Please check your input parameter\n");
    }
    
}

// generate randomly-modified passwords from a dictionary based on 
// characters distribution calculated from "common_passwords.txt"
// parameters: random_degree - the number of permutations for a word, 
// guess_num - the number of guesses required by the user
int random_attack(int random_degree, int guess_num) {

    // read and store the character distribution information
    // note: cfd stands for character frequency distribution
    int cfd_char[CHAR_FREQ_DIS_NUM];
    int cfd_count[CHAR_FREQ_DIS_NUM];
    int total_count = 0;
    
    extract_cfd(cfd_char, cfd_count, &total_count, CHAR_FREQ_DIS_NUM);
    
    // read dictionary
    FILE *f=fopen("common_6_pwds.txt", "r");
    if (f==NULL) {
        printf("file doesnt exist?!\n");
        return -1;
    }

    /* crack passwords */
    char *word = NULL;
    size_t len = 0;
    size_t line = 0;

    // set seed to randomize the attack
    srand(time(0));

    int ctr = 0;
    char temp[MAX_WORD_LENGTH];
    bzero(temp, MAX_WORD_LENGTH);

    // compute hash for each word and compare them
    while ((line = getline(&word, &len, f)) != -1 && ctr < guess_num) {

        // remove the trailing newline character
        word[strlen(word)-1] = 0;
        
        // use the base word as one of the guess as well
        bzero(temp, MAX_WORD_LENGTH);
        strncpy(temp, word, strlen(word));
        temp[strlen(temp)] = 0;
        
        printf("%s\n", temp);

        // count number of guesses generated
        ctr++;
        
        // permutate word for random_degree times
        for (int i=0; i<random_degree; i++) {

            if (ctr >= guess_num) break;

            char rand_char = 0;

            // reset to original word
            bzero(temp, MAX_WORD_LENGTH);
            strncpy(temp, word, strlen(word));
            temp[strlen(temp)] = 0;

            // gets a random value which will be used for selecting a 
            // character
            int r = uniform_rand(total_count);
            
            // randomly get a character based on character distribution range
            for (int j=0; j<CHAR_FREQ_DIS_NUM; j++) {
                if ((j == CHAR_FREQ_DIS_NUM-1) || (r > cfd_count[j] && r < cfd_count[j+1])) {
                    rand_char = cfd_char[j];
                    break;
                } 
            }

            // randomly picks a position to assign to the random character
            temp[uniform_rand(strlen(temp))] = rand_char;

            printf("%s\n", temp);

            ctr++;

            if (ctr >= guess_num) break;

            // randomly selects a character then capitalizes it if applicable
            char ch = temp[uniform_rand(strlen(temp))];
            if (ch>='a' && ch<='z') {
                ch = toupper(ch);
                temp[uniform_rand(strlen(temp))] = ch;
                printf("%s\n", temp);
                ctr++;
            }
        }
    }
    return ctr;
}

// partly-exhaustively finds all 4-letters passwords, only uses characters found
// in 'common_passwords.txt', as to retain/mimics the character distribution, 
// at the same time reduces the search space
// it also uses next-character frequency information to generate passwords
// basically, it finds the character that is the most likely to come after 
// the previous character (distribution calculated from 'common_passwords.txt' in python)
void four_exhaustive() {

    int hash_num = PWD4_HASH_NO;
    int remaining_hash_num = hash_num;

    // Initialize 4-letters password hashes array
    BYTE pwd4_h[hash_num][SHA256_BLOCK_SIZE];
    BYTE temp_h[hash_num][SHA256_BLOCK_SIZE];

    // store the next-character frequency information
    // note: i called this character sequential frequency initially (hence char_seq_freq), weird i know
    // first_letters stores the first letter distribution (the char frequency of the first letter)
    // char_seq_freq stores the next-character frequency information
    char first_letters[CHAR_SEQ_FREQ_NUM+1];
    char char_seq_freq[CHAR_SEQ_FREQ_NUM+1][CHAR_SEQ_FREQ_NUM*2+1];

    bzero(first_letters, CHAR_SEQ_FREQ_NUM+1);
    for (int i=0; i<CHAR_SEQ_FREQ_NUM+1; i++) {
        bzero(char_seq_freq[i], CHAR_SEQ_FREQ_NUM*2+1);
    } 

    // read the analysed information txt file that was prepared in python
    extract_char_seq_freq(first_letters, char_seq_freq);

    // write hashes into array
    read_hash_file("pwd4sha256", hash_num, pwd4_h);
    read_hash_file("pwd4sha256", hash_num, temp_h);

    BYTE *hash_match = NULL;
    
    // loops begin!
    int first_i = 0, second_i = 0, third_i = 0;

    // start by looping through first letters (mimics common passwords distribution, 
    // that is to go in order of most common first letter)
    for (int i=0; i<strlen(first_letters); i++) {
        // use the character to find the index to the next char in the array
        first_i = get_char_index(first_letters[i], first_letters);

        // gets and loops through the most likely next-char character (based on distribution calculated)
        for (int j=0; j<strlen(char_seq_freq[first_i]); j++) {
            second_i = get_char_index(char_seq_freq[first_i][j], first_letters);

             for (int k=0; k<strlen(char_seq_freq[second_i]); k++) {
                third_i = get_char_index(char_seq_freq[second_i][k], first_letters);

                for (int l=0; l<strlen(char_seq_freq[third_i]); l++) {
                    
                    // try both lower and upper case for the first letter
                    char f_letters[3] = {first_letters[i], toupper(first_letters[i]), '\0'};

                    // construct word
                    for (int m=0; m<strlen(f_letters); m++) {
                        char word[5] = {f_letters[m], 
                                        char_seq_freq[first_i][j], 
                                        char_seq_freq[second_i][k], 
                                        char_seq_freq[third_i][l], 
                                        '\0'};

                        hash_match = hash_compare(word, remaining_hash_num, temp_h);
                        
                        if (hash_match) {
                            // print the password found
                            printf("%s %d\n", word, get_hash_id(hash_match, hash_num, pwd4_h));
                            
                            remaining_hash_num--;
                            // remove the matching hash from temp array
                            remove_hash(hash_match, remaining_hash_num, hash_num, temp_h);
                        }
                        if (!remaining_hash_num) {
                            goto hell;
                        }
                    }
                }
            }
        }
    }
    hell:
    return;
}

// exhaustively search for all 6-letters passwords
// only use as last resort, it's a dumb algorithm tbh :D
void six_exhaustive(int mode, int guesses_num) {

    int hash_num = PWD6_HASH_NO;
    int remaining_hash_num = hash_num;

    // Initialize 4-letters password hashes array
    BYTE pwd4_h[hash_num][SHA256_BLOCK_SIZE];
    BYTE temp_h[hash_num][SHA256_BLOCK_SIZE];

    // write hashes into array
    read_hash_file("pwd6sha256", hash_num, pwd4_h);
    read_hash_file("pwd6sha256", hash_num, temp_h);

    BYTE *hash_match = NULL;

    int ctr = 0;

    // runs through all characters (from ascii value of 32 to 126)
    for (int i=PWD_CHAR_LOWER_BOUND; i<PWD_CHAR_UPPER_BOUND; i++) {
        for (int j=PWD_CHAR_LOWER_BOUND; j<PWD_CHAR_UPPER_BOUND; j++) {
            for (int k=PWD_CHAR_LOWER_BOUND; k<PWD_CHAR_UPPER_BOUND; k++) {
                for (int l=PWD_CHAR_LOWER_BOUND; l<PWD_CHAR_UPPER_BOUND; l++) {
                    for (int m=PWD_CHAR_LOWER_BOUND; m<PWD_CHAR_UPPER_BOUND; m++) {
                        for (int n=PWD_CHAR_LOWER_BOUND; n<PWD_CHAR_UPPER_BOUND; n++) {
                            
                            char word[7] = {i, j, k, l, m, n, '\0'};

                            printf("%s\n", word);

                            ctr++; 

                            if (mode != 0) {
                              if (ctr >= guesses_num) goto deeper_hell;
                              continue;
                            } 

                            hash_match = hash_compare(word, remaining_hash_num, temp_h);
                            
                            if (hash_match) {
                                // print the password found
                                printf("%s %d\n", word, get_hash_id(hash_match, hash_num, pwd4_h));
                                
                                remaining_hash_num--;
                                // remove the matching hash from temp array
                                remove_hash(hash_match, remaining_hash_num, hash_num, temp_h);
                            }
                            if (!remaining_hash_num) {
                                goto deeper_hell;
                            }

                        }
                    }
                }
            }
        }
    }        
    deeper_hell:
    return;
}

// ------------------------------------------------------------ //
/* strategy helper functions */

// use to extract the character distribution (a.k.a. character frequency distribution, cfd)
// from the text file that was prepared in python
void extract_cfd(int *cfd_char, int *cfd_count, int *total_count, int count_num) {

    FILE *cfd_f=fopen("char_freq_distribution.txt", "r");
    if (cfd_f==NULL) {
        printf("file doesnt exist?!\n");
        return ;
    }
    
    /* read char distribution information */
    char *word = NULL;
    size_t len = 0;
    size_t line = 0;
    int ctr = 0;
    
    // read total count first
    if ((line = getline(&word, &len, cfd_f)) != -1) {
        word[strlen(word)-1] = 0;
        *total_count = atoi(word);
    }
    // read each bit of information into arrays
    while ((line = getline(&word, &len, cfd_f)) != -1) {
        cfd_char[ctr] = word[0];

        if ((line = getline(&word, &len, cfd_f)) != -1) {
            word[strlen(word)-1] = 0;
            cfd_count[ctr] = atoi(word);
        }
        ctr++;
    }

}

// count the number of hashes there are in the hash file
int get_hash_num(char *filename) {

    FILE *f=fopen(filename, "r");
    if (f==NULL) {
        printf("file doesnt exist?!\n");
        return -1;
    }
    
    int count_blocks = 0, i=0;
    unsigned int ch;
    while((ch = fgetc(f)) != EOF) {
        i++;
        if (!(i % SHA256_BLOCK_SIZE)) {
            count_blocks += 1;
            i = 0;
        }
    }
    fclose(f); //close file.
    return count_blocks;

}

// modified from ref: https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
// used to generate a random integer with uniform distribution
int uniform_rand(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    } else {
        long limit = (RAND_MAX / n)*n; 
        int r; 
        while ((r = rand()) >= limit);
        return r % n; 
    }
}

// read the next-character distribution information and store it into arrays
void extract_char_seq_freq(char first_letters[], char char_seq_freq[CHAR_SEQ_FREQ_NUM+1][CHAR_SEQ_FREQ_NUM*2+1]) {

    FILE *fr=fopen("sequential_char_freq.txt",  "r");

    char *word = NULL;
    size_t len = 0;
    size_t line;

    int i=0;

    while ((line = getline(&word, &len, fr)) != -1) {
        strncat(first_letters, word, 1);
        if ((line = getline(&word, &len, fr)) != -1) {
            strncpy(char_seq_freq[i], word, strlen(word)-1);
            char_seq_freq[i][strlen(char_seq_freq[i])] = 0;
        } else {
            break;
        }
        i++;
    }
    first_letters[strlen(first_letters)] = 0;

}

// get the character index in the array that stores the next-character distribution
// information
int get_char_index(char ch, char *chars) {
    int i = 0;
    while (i < strlen(chars)) {
        if (tolower(ch) == tolower(chars[i])) {
            return i;
        }
        i++;
    }
    return -1;
}
