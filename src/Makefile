CC?=cc
CFLAGS=-Wall -g -std=c99 #-march=native -pipe -fomit-frame-pointer 
LDFLAGS=-lm
SRC=ringtophat.c test_ring.c
OBJ=$(subst .c,.o,$(SRC))
EXE=test_ring

all:	$(OBJ) $(EXE)

$(EXE):	$(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(EXE)

$(OBJ):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

check:
	./test_ring

clean:
	rm -f $(OBJS) $(EXE) *.o *.pgm
