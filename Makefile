CC     = gcc
CFLAGS = -c -Wall -Wpedantic


TARGET = crack
SRC	   = crack.c
OBJ    = crack.o sha256.o guess_strategies.o sha256_hash_helper.o

DH_TARGET = dh
DH_SRC = dh.c
DH_OBJ = dh.o

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

dh: $(DH_OBJ)
	$(CC) $(DH_OBJ) -o $(DH_TARGET) -lm

# other dependencies
crack.o: crack.c sha256.h guess_strategies.h sha256_hash_helper.h
	$(CC) $(CFLAGS) crack.c -o crack.o

sha256.o: sha256.c sha256.h
	$(CC) $(CFLAGS) sha256.c -o sha256.o

guess_strategies.o: guess_strategies.c guess_strategies.h sha256.h
	$(CC) $(CFLAGS) guess_strategies.c -o guess_strategies.o

sha256_hash_helper.o: sha256_hash_helper.c sha256_hash_helper.h sha256.h
	$(CC) $(CFLAGS) sha256_hash_helper.c -o sha256_hash_helper.o

dh.o: $(DH_SRC)
	$(CC) $(CFLAGS) $(DH_SRC) -o $(DH_OBJ)

# `make clean` to remove all object files
clean:
	rm -f $(DH_TARGET) $(TARGET) *.o

