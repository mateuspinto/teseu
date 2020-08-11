#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

unsigned int floorSqrt(unsigned int x)
{
    if (x == 0 || x == 1)
        return x;

    unsigned int i = 1, result = 1;
    while (result <= x)
    {
        i++;
        result = i * i;
    }
    return i - 1;
}

unsigned int min(unsigned int a, unsigned int b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

unsigned int max(unsigned int a, unsigned int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

unsigned int gridLineSizeRetriever(const char *mappingFileName)
{
    FILE *mappingFile = fopen(mappingFileName, "r");
    char swap;
    unsigned int gridSize = 0;

    fscanf(mappingFile, "\n");
    do
    {
        fscanf(mappingFile, "%c", &swap);
        if (swap == ' ')
        {
            gridSize++;
        }

    } while (swap != '\n');

    fclose(mappingFile);

    return floorSqrt(gridSize);
}

#endif // INPUT_HPP