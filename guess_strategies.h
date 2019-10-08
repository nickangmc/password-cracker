// ------------------------------------------------------------ //
// created for COMP30023 Computer Systems - Project 2, 2019
// by Ang Mink Chen <minka@student.unimelb.edu.au>
// ------------------------------------------------------------ //

#ifndef GUESS_STRATEGIES_H
#define GUESS_STRATEGIES_H

#include <stddef.h>
#include "sha256.h"
#include "sha256_hash_helper.h"

// constants
#define MAX_WORD_LENGTH 1024
#define PWD4_HASH_NO 10
#define PWD6_HASH_NO 20
#define CHAR_SEQ_FREQ_NUM 26 
#define CHAR_FREQ_DIS_NUM 46
#define PWD_CHAR_LOWER_BOUND 32
#define PWD_CHAR_UPPER_BOUND 126

// ----------- function prototypes ------------ //

int uniform_rand(int n);
int get_hash_num(char *filename);
int get_char_index(char ch, char *chars);
int random_attack(int random_degree, int guess_num);
int dict_attack(char *dictionary, char *hash_file, int offset);
void four_exhaustive();
void six_exhaustive(int mode, int guesses_num);
void exhaustive_search(int guess_len, int mode, int guesses_num);
void extract_cfd(int *cfd_char, int *cfd_count, int *total_count, int count_num);
void extract_char_seq_freq(char first_letters[], char char_seq_freq[CHAR_SEQ_FREQ_NUM+1][CHAR_SEQ_FREQ_NUM*2+1]);

#endif
