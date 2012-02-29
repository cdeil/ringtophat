CC?=cc
CFLAGS=-Wall -g -std=c99 #-march=native -pipe -fomit-frame-pointer 
LDFLAGS=-lm
SRC=ringtophat.c test.c
OBJ=$(subst .c,.o,$(SRC))
EXE=test

all:	$(OBJ) $(EXE)

$(EXE):	$(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(EXE)

$(OBJ):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -f $(OBJS) $(EXE) *.o *.pgm
