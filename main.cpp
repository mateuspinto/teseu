#include <iostream>
#include "headers/cgra.hpp"
#include "headers/input.hpp"
#include "headers/heuristic.hpp"

int main(int argc, char const *argv[])
{
    FILE *mappingFile = fopen("benchmarks/sbcci/grid/verilog_chebyshev.out", "r");
    FILE *routtingFile = fopen("benchmarks/sbcci/edge_list/chebyshev/chebyshev_0.in", "r");

    unsigned int gridLineSize = gridLineSizeRetriever("benchmarks/sbcci/grid/verilog_chebyshev.out");

    fscanf(mappingFile, "\n");

    CGRA slash = CGRA(gridLineSize, 1, 2, mappingFile);
    CGRARoutingHeuristic heuristic = CGRARoutingHeuristic(&slash, routtingFile);

    fclose(mappingFile);
    fclose(routtingFile);

    return 0;
}