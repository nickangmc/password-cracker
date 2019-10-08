// ------------------------------------------------------------ //
// created for COMP30023 Computer Systems - Project 2, 2019
// by Ang Mink Chen <minka@student.unimelb.edu.au>
// ------------------------------------------------------------ //

#ifndef SHA256_HASH_HELPER_H
#define SHA256_HASH_HELPER_H

#include <stddef.h>
#include "sha256.h"

// ----------- function prototypes ------------ //

int get_hash_id(BYTE *hash, int m, BYTE pwd_h[m][SHA256_BLOCK_SIZE]);
int read_hash_file(const char *filename, int m, BYTE hash[m][SHA256_BLOCK_SIZE]);
BYTE *hash_compare(char *word, int m, BYTE hash[m][SHA256_BLOCK_SIZE]);
void remove_hash(BYTE *hash_match, int n, int m, BYTE pwd_h[m][SHA256_BLOCK_SIZE]);

#endif
