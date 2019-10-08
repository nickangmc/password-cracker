// ------------------------------------------------------------ //
// created for COMP30023 Computer Systems - Project 2, 2019
// by Ang Mink Chen <minka@student.unimelb.edu.au>
// ------------------------------------------------------------ //

#include <stdio.h>
#include <stdlib.h>

#include "guess_strategies.h"

// ----------- main function ------------ //

int main(int argc, char *argv[]) {

    // no argument mode
    if (argc == 1) {
        int remaining = 1;
        // first gets all 4-letters password
        exhaustive_search(4, 0, 0);
        // cracks 6-letters password
        remaining = dict_attack("common_6_pwds.txt", "pwd6sha256", 10);
        
        if (remaining > 0) {
            // find 6-letters exhaustively
            exhaustive_search(6, 0, 0);
        }
    }

    // 1 argument mode
    else if (argc == 2) {
        // reads in most common 6-letters passwords, 
        // randomly permutate each word (up to n characters) based on characters 
        // distribution calculated in "common_passwords.txt" file. 
        int guessed_num = random_attack(10, atoi(argv[1]));

        // the number of word generated by random attack does not reach the number
        // of required guesses
        if (guessed_num < atoi(argv[1])) {
            exhaustive_search(6, 1, atoi(argv[1])-guessed_num);
        }
    }

    // 2 arguments mode
    else if (argc == 3) {
        // read both passwords file and hash file provided
        // and use them for cracking passwords in dictionary attack way
        dict_attack(argv[1], argv[2], 0);
    }

    else {
        // error
        printf("Please give the correct number of arguments.\n");
    }

    return 0;

}

