#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <iostream>
#include "cgra.hpp"

class GraphEdge
{
private:
    unsigned int source;
    unsigned int destination;

public:
    GraphEdge(unsigned int source, unsigned int destination);
    ~GraphEdge();

    void info();
    unsigned int getSource();
    unsigned int getDestination();
};

GraphEdge::GraphEdge(unsigned int source, unsigned int destination)
{
    this->source = source;
    this->destination = destination;
}

GraphEdge::~GraphEdge()
{
}

void GraphEdge::info()
{
    std::cout << this->source << " -> " << this->destination;
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

        std::cout << "[DEBUG] " << cgra->xPosition(swapSource) << "-" << cgra->yPosition(swapSource) << " " << cgra->xPosition(swapDestination) << "-" << cgra->yPosition(swapDestination)  << std::endl;

        this->inputs[i] = GraphEdge(swapSource, swapDestination);
        
        //For loop
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
        std::cout << std::endl;
    }
}

#endif // HEURISTIC_HPP