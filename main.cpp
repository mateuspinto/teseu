#include <iostream>
#include "headers/cgra.hpp"
#include "headers/input.hpp"
#include "headers/heuristic.hpp"

int main(int argc, char const *argv[])
{
    CGRA *cgra = NULL;
    CGRARoutingHeuristic *heuristic = NULL;
    FILE *mappingFile = NULL;
    FILE *routtingFile = NULL;
    unsigned int gridLineSize;

    if (argc == 3)
    {
        mappingFile = fopen(argv[1], "r");
        routtingFile = fopen(argv[2], "r");

        gridLineSize = gridLineSizeRetriever(argv[1]);

        fscanf(mappingFile, "\n");
        cgra = new CGRA(gridLineSize, 1, 2, mappingFile);
        heuristic = new CGRARoutingHeuristic(cgra, routtingFile);

        heuristic->infoXY(cgra);

        fclose(mappingFile);
        fclose(routtingFile);

        return 0;
    }
    else
    {
        std::cout << "You must input the mapping file and the routing file in that order!" << std::endl;
        std::cout << "Error. I give up. <3" << std::endl;
        exit(1);
    }
}