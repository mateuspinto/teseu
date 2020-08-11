#ifndef CGRA_HPP
#define CGRA_HPP

#include <iostream>
#include <stdlib.h>
#include <math.h>

#define EMPTY_PE 255

unsigned int square(unsigned int x)
{
    return x * x;
}

class PE
{
private:
    bool *outputs;
    unsigned int used;

public:
    PE(unsigned int usedBypasses, unsigned int n_hop);
    ~PE();
    void info();
    void infoOutputs(unsigned int nHop);
};

PE::PE(unsigned int usedBypasses, unsigned int nHop)
{
    if ((this->outputs = (bool *)malloc((1 + nHop) * 4 * sizeof(bool))) == NULL)
    {
        std::cout << "FATAL FAILURE: Memory Allocation Error on PE" << std::endl;
        exit(1);
    }

    for (size_t i = 0; i < (1 * nHop) * 4; i++)
    {
        this->outputs[i] = false;
    }

    used = usedBypasses;
}

PE::~PE()
{
    free((void *)this->outputs);
}

void PE::info()
{
    std::cout << "Number of used bypasses: " << this->used << std::endl;
}

void PE::infoOutputs(unsigned int nHop)
{
    for (size_t i = 0; i < (1 + nHop) * 4; i++)
    {
        if (this->outputs[i])
        {
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }

        std::cout << " ";
    }

    std::cout << std::endl;
}

class CGRA
{
private:
    PE *grid;
    unsigned int gridLineSize;
    unsigned int nHop;
    unsigned int maxBypassesAllowed;

    unsigned int *mapping;

public:
    CGRA(unsigned int gridLineSize, unsigned int nHop, unsigned int maxBypassesAllowed, FILE *mappingFile);
    ~CGRA();
    void info();

    unsigned int getGridLineSize();

    unsigned int mappingPosition(unsigned int x);
    unsigned int xPosition(unsigned int x);
    unsigned int yPosition(unsigned int x);
    unsigned int xRelativePosition(unsigned int x);
    unsigned int yRelativePosition(unsigned int x);
    unsigned int fromXYPosition(unsigned int x, unsigned int y);
};

CGRA::CGRA(unsigned int gridLineSize, unsigned int nHop, unsigned int maxBypassesAllowed, FILE *mappingFile)
{
    unsigned int gridSize = square(gridLineSize);
    unsigned int mappingSwap;

    if ((this->mapping = (unsigned int *)malloc(gridSize * sizeof(unsigned int))) == NULL)
    {
        std::cout << "FATAL FAILURE: Memory Allocation Error on CGRA Mapping Vector" << std::endl;
        exit(1);
    }

    if ((this->grid = (PE *)malloc(gridSize * sizeof(PE))) == NULL)
    {
        std::cout << "FATAL FAILURE: Memory Allocation Error on CGRA Vector Grid" << std::endl;
        exit(1);
    }

    for (size_t i = 0; i < gridSize; i++)
    {
        fscanf(mappingFile, "%u ", &mappingSwap);
        if (mappingSwap != EMPTY_PE)
        {
            this->mapping[mappingSwap] = i;
            this->grid[i] = PE(1, nHop);
        }
        else
        {
            this->grid[i] = PE(0, nHop);
        }
    }

    this->gridLineSize = gridLineSize;
    this->nHop = nHop;
    this->maxBypassesAllowed = maxBypassesAllowed;
}

CGRA::~CGRA()
{
    free((void *)this->grid);
    free((void *)this->mapping);
}

unsigned int CGRA::mappingPosition(unsigned int x)
{
    return mapping[x];
}

unsigned int CGRA::xRelativePosition(unsigned int x)
{
    return x % this->gridLineSize;
}

unsigned int CGRA::yRelativePosition(unsigned int x)
{
    return x / this->gridLineSize;
}

void CGRA::info()
{
    unsigned int gridSize = square(gridLineSize);

    std::cout << "Grid Line Size = " << this->gridLineSize << std::endl;
    std::cout << "N-Hop = " << this->nHop << std::endl;
    std::cout << "Max Bypasses Allowed = " << this->maxBypassesAllowed << std::endl;
    std::cout << std::endl;

    std::cout << "[Grid]" << std::endl;
    for (size_t i = 0; i < gridSize; i++)
    {
        this->grid[i].info();
        this->grid[i].infoOutputs(this->nHop);
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "[Mapping Vector]" << std::endl;
    for (size_t i = 0; i < gridSize; i++)
    {
        std::cout << this->mapping[i] << " ";
    }
    std::cout << std::endl;
}

unsigned int CGRA::getGridLineSize()
{
    return this->gridLineSize;
}

unsigned int CGRA::fromXYPosition(unsigned int x, unsigned int y)
{
    return y * this->gridLineSize + x;
}

#endif // CGRA_HPP