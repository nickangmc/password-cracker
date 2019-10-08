# Password Cracker
Program written and developed in the language C as the final project for the subject COMP30023: Computer Systems at the University of Melbourne - Semester 1, 2019.

### Goal:

There are two parts to this project: to learn and implement Diffie-Hellman Key Exchange protocol; to crack the passwords of a simple system that has four and six-character passwords. The passwords can contain any ASCII character from 32 (space) to 126 (∼), but the passwords are not arbitrary strings, they are generated from a combination of english words. To use the knowledge of common human frailties to identify the passwords used.

#### Note:

The Diffie-Hellman Key Exchange program in this project does not work, as the server it communicates with is no longer running. However, the passwork-cracking C program has been modularized and is still working as a standalone program.

---

### Tasks:
- To implement Diffie-Hellman Key Exchange protocol to download SHA256 encrypted hashes.
- To develop a C program that runs in 3 modes:
  - _No arguments mode_ - Generate guesses that test them against the SHA256 hashes.
  - _1-integer arguments mode_ - The argument specifies how many guesses it should produce.
  - _2 arguments mode_ - Takes in first argument as the filename of a list of passwords (one per line), and the second as the filename of a list of SHA256 hashes. It then tests each of the passwords against each of the hashes, and product output that matches.
  
---

### Diffie-Hellman Key Exchange Protocol

The following diagram shows how the protocol works in a simplified way:

![Diffie-Hellman Protocol](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a9/Diffie-Hellman_Key_Exchange.png/451px-Diffie-Hellman_Key_Exchange.png)

###### 	Dav-FL-IN-AZ-id, accessed October 2019 \<<https://commons.wikimedia.org/wiki/File:Diffie-Hellman_Key_Exchange.png>\>. Free Art License

---

### File structure in this repo
- Main password-cracking program: 
  - _crack.c_
- Diffie-Hellman Key Exchange program: 
  - _dh.c_
- 4 and 6-character SHA256 hashes respectively:
  - _pwd4sha256_
  - _pwd6sha256_
- Common password lists from free online resources: 
  - _common_6_pwds.txt_
  - _common_passwords.txt_
- Cracked/Found passwords for both 4 and 6-character SHA256 hashes:
  - _found_pwds.txt_
- SHA256 functions (provided):
  - _sha256.c_
  - _sha256.h_
- Program and SHA256 Helper functions:
  - _guess_strategies.c_
  - _guess_strategies.h_
  - _sha256_hash_helper.c_
  - _sha256_hash_helper.h_
- Program to calculate the statistical distribution of letters and words amongst the common passwords:
  - _char_freq_distribution.ipynb_
  - _sequential-character-frequency-generator.ipynb_
- Text file (used by main program) containing the info on the statistical distribution calculated
  - _char_freq_distribution.txt_
  - _sequential-character-freq.txt_
  
---

### Project Outcome

All the tasks and challenges within the scope of this project were completed.
