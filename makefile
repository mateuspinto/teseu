BIN =teseu
CPP =clang++
SRC =main.cpp
CFLAGS =-O2 -march=native
LFLAGS =

all:
	$(CPP) $(CFLAGS) $(LFLAGS) $(SRC) -o $(BIN)

run:
	./$(BIN)
