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
        swapSource = cgra->absPosition(swapSource);
        swapDestination = cgra->absPosition(swapDestination);

        if ((abs((int)(cgra->xPosition(swapSource) - cgra->xPosition(swapDestination))) + abs(((int)(cgra->yPosition(swapSource) - cgra->yPosition(swapDestination))))) <= 1)
        {
            isTrivial = true;
        }
        else
        {
            isTrivial = false;

            for (size_t j = min(cgra->yPosition(swapSource), cgra->yPosition(swapDestination)); j <= max(cgra->yPosition(swapSource), cgra->yPosition(swapDestination)); j++)
            {
                for (size_t i = min(cgra->xPosition(swapSource), cgra->xPosition(swapDestination)); i <= max(cgra->xPosition(swapSource), cgra->xPosition(swapDestination)); i++)
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

#endif // HEURISTIC_HPP