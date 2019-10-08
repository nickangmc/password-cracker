// ------------------------------------------------------------ //
// created for COMP30023 Computer Systems - Project 2, 2019
// by Ang Mink Chen <minka@student.unimelb.edu.au>
// ------------------------------------------------------------ //

#include "sha256_hash_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------ //
/* functions */

// does what it says, get hash id for a specified hash
int get_hash_id(BYTE *hash, int m, BYTE pwd_h[m][SHA256_BLOCK_SIZE]) {
    for (int i=0; i<m; i++) {
        if (!memcmp(hash, pwd_h[i], SHA256_BLOCK_SIZE)) {
            return i+1;
        }
    }
    return -1;
}

// remove the hash at the index specified in the hash array, also move the rest
// array forward to fill the spot
void remove_hash(BYTE *hash_match, int n, int m, BYTE pwd_h[m][SHA256_BLOCK_SIZE]) {

    for (int i=0; i<n; i++) {
        if (!memcmp(hash_match, pwd_h[i], SHA256_BLOCK_SIZE)) {
            for (int j=i; j<n; j++) {
                for (int k=0; k<SHA256_BLOCK_SIZE; k++) {
                    pwd_h[j][k] = pwd_h[j+1][k];
                }
            }
            break;
        }
    }

}

// takes in a word, computes its hash, compares it to all the hashes
// and finds if there's a match
BYTE *hash_compare(char *word, int m, BYTE hash[m][SHA256_BLOCK_SIZE]) {

    BYTE buffer[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    // preprocess word
    BYTE temp[strlen(word)];
    strncpy((char *)temp, word, strlen(word));

    // compute hash
    sha256_init(&ctx);
	sha256_update(&ctx, temp, strlen(word));
	sha256_final(&ctx, buffer);

    // compare hash
    for (int i=0; i<m; i++) {
        if (!memcmp(buffer, hash[i], SHA256_BLOCK_SIZE)) {
            return hash[i];
        }
    }

    return NULL;

}

// read a sha256 hash file with no file name extension, store the hash information
// into the specified array
int read_hash_file(const char *filename, int m, BYTE hash[m][SHA256_BLOCK_SIZE]) {

    FILE *f;
    unsigned int ch;

    // read the hash file
    f=fopen(filename,"rb");
    if (f==NULL) {
        printf("file doesnt exist?!\n");
        return -1;
    }

    // read and store the sha256 hashes 
    int i = 0;
    int j = 0;
    while((ch = fgetc(f)) != EOF) {
        hash[i][j] = ch;
        j++;
        // make sure to reset j
        if(!(j % SHA256_BLOCK_SIZE)) 
        {
            j = 0;
            i++;
        }
    }

    fclose(f);
    return 0;

}
