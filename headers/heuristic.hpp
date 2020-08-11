#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <iostream>
#include "cgra.hpp"

class GraphEdge
{
private:
    unsigned int source;
    unsigned int destination;
    unsigned int routedOrder;
    bool trivial;

public:
    GraphEdge(unsigned int source, unsigned int destination, bool trivial);
    ~GraphEdge();

    void info();
    void infoXY(unsigned int gridLineSize);
    unsigned int getSource();
    unsigned int getDestination();
};

GraphEdge::GraphEdge(unsigned int source, unsigned int destination, bool trivial)
{
    this->source = source;
    this->destination = destination;
    this->trivial = trivial;
    this->routedOrder = 0;
}

GraphEdge::~GraphEdge()
{
}

void GraphEdge::info()
{
    if (this->trivial)
    {
        std::cout << "(Trivial=1 R=" << this->routedOrder << ") " << this->source << " -> " << this->destination << std::endl;
    }
    else
    {
        std::cout << "(Trivial=0 R=" << this->routedOrder << ") " << this->source << " -> " << this->destination << std::endl;
    }
}

void GraphEdge::infoXY(unsigned int gridLineSize)
{
    if (this->trivial)
    {
        std::cout << "(Trivial=1 R=" << this->routedOrder << ") " << this->source % gridLineSize << "," << this->source / gridLineSize << " -> " << this->destination % gridLineSize << "," << this->destination / gridLineSize << std::endl;
    }
    else
    {
        std::cout << "(Trivial=0 R=" << this->routedOrder << ") " << this->source % gridLineSize << "," << this->source / gridLineSize << " -> " << this->destination % gridLineSize << "," << this->destination / gridLineSize << std::endl;
    }
}

unsigned int GraphEdge::getSource()
{
    return this->source;
}

unsigned int GraphEdge::getDestination()
{
    return this->destination;
}

class CGRARoutingHeuristic
{
private:
    unsigned int *grid;
    unsigned int gridLineSize;

    GraphEdge *inputs;
    unsigned int inputsQuantity;

public:
    CGRARoutingHeuristic(CGRA *cgra, FILE *routingFile);
    ~CGRARoutingHeuristic();

    void info();
    void infoXY(CGRA *cgra);
};

CGRARoutingHeuristic::CGRARoutingHeuristic(CGRA *cgra, FILE *routingFile)
{
    this->gridLineSize = cgra->getGridLineSize();

    unsigned int gridSize = square(this->gridLineSize);
    unsigned int dump, inputsQuantity, swapSource, swapDestination;
    bool isTrivial;

    if ((this->grid = (unsigned int *)malloc(gridSize * sizeof(unsigned int))) == NULL)
    {
        std::cout << "FATAL FAILURE: Memory Allocation Error on CGRA Routing Heuristic Grid" << std::endl;
        exit(1);
    }

    for (size_t i = 0; i < gridSize; i++)
    {
        this->grid[i] = 0;
    }

    fscanf(routingFile, "%u %u\n\n", &dump, &inputsQuantity);
    this->inputsQuantity = inputsQuantity;

    if ((this->inputs = (GraphEdge *)malloc(inputsQuantity * sizeof(GraphEdge))) == NULL)
    {
        std::cout << "FATAL FAILURE: Memory Allocation Error on CGRA Routing Heuristic Inputs Vector" << std::endl;
        exit(1);
    }

    for (size_t i = 0; i < inputsQuantity; i++)
    {
        fscanf(routingFile, "%u %u\n", &swapSource, &swapDestination);

        // Changing to placement position
        swapSource = cgra->mappingPosition(swapSource);
        swapDestination = cgra->mappingPosition(swapDestination);

        if ((abs((int)(cgra->xRelativePosition(swapSource) - cgra->xRelativePosition(swapDestination))) + abs(((int)(cgra->yRelativePosition(swapSource) - cgra->yRelativePosition(swapDestination))))) <= 1)
        {
            isTrivial = true;
        }
        else
        {
            isTrivial = false;

            for (size_t j = min(cgra->yRelativePosition(swapSource), cgra->yRelativePosition(swapDestination)); j <= max(cgra->yRelativePosition(swapSource), cgra->yRelativePosition(swapDestination)); j++)
            {
                for (size_t i = min(cgra->xRelativePosition(swapSource), cgra->xRelativePosition(swapDestination)); i <= max(cgra->xRelativePosition(swapSource), cgra->xRelativePosition(swapDestination)); i++)
                {
                    this->grid[cgra->fromXYPosition(i, j)]++;
                }
            }
        }

        this->inputs[i] = GraphEdge(swapSource, swapDestination, isTrivial);
    }
}

CGRARoutingHeuristic::~CGRARoutingHeuristic()
{
    free((void *)this->grid);
    free((void *)this->inputs);
}

void CGRARoutingHeuristic::info()
{
    std::cout << "[Grid]" << std::endl;
    for (size_t j = 0; j < this->gridLineSize; j++)
    {
        for (size_t i = 0; i < this->gridLineSize; i++)
        {
            std::cout << this->grid[j * gridLineSize + i] << " ";
        }

        std::cout << std::endl;
    }

    std::cout << "[Input Edges]" << std::endl;
    for (size_t i = 0; i < this->inputsQuantity; i++)
    {
        this->inputs[i].info();
    }
}

void CGRARoutingHeuristic::infoXY(CGRA *cgra)
{
    std::cout << "[Grid]" << std::endl;
    for (size_t j = 0; j < this->gridLineSize; j++)
    {
        for (size_t i = 0; i < this->gridLineSize; i++)
        {
            std::cout << this->grid[j * gridLineSize + i] << " ";
        }

        std::cout << std::endl;
    }

    std::cout << "[Input Edges]" << std::endl;
    for (size_t i = 0; i < this->inputsQuantity; i++)
    {
        this->inputs[i].infoXY(cgra->getGridLineSize());
    }
}

#endif // HEURISTIC_HPP