CFLAGS=-march=native -pipe -fomit-frame-pointer -Wall -g -std=c99 -lm
SRC=ringtophat.c test.c
OBJ=$(subst .c,.o,$(SRC))
EXE=test

$(EXE):	$(OBJ)
	gcc $(CFLAGS) $(OBJ) -o $(EXE)

$(OBJ): $(SRC)
	gcc $(CFLAGS) -c $(SRC)

all:	$(EXE)
	$(EXE).o

clean:
	rm -f $(OBJS) $(EXE) *.o
