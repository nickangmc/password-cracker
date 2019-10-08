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

The following diagram shows how the protocol works:

![Diffie-Hellman Protocol](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a9/Diffie-Hellman_Key_Exchange.png/451px-Diffie-Hellman_Key_Exchange.png)

###### 	Dav-FL-IN-AZ-id, accessed October 2019 \<<https://commons.wikimedia.org/wiki/File:Diffie-Hellman_Key_Exchange.png>\>. Free Art License

---

### File structure in this repo
