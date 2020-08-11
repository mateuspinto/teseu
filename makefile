BIN =teseu
CPP =clang++
SRC =main.cpp
CFLAGS =-O2 -march=native
LFLAGS =
MAPPING_FILE =benchmarks/sbcci/grid/verilog_chebyshev.out
ROUTING_FILE =benchmarks/sbcci/edge_list/chebyshev/chebyshev_0.in

all:
	$(CPP) $(CFLAGS) $(LFLAGS) $(SRC) -o $(BIN)

run:
	./$(BIN) $(MAPPING_FILE) $(ROUTING_FILE)
