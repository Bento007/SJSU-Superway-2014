#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include "Header.h"

using namespace std;

int main()
{

    int mainSrc, destination;
    dijkstra *mainGraph = new dijkstra;
    makeGraph(mainGraph);
    printf("\nSource Vertex: ");
    scanf_s("%i", &mainSrc);

    printf("\nDestination: ");
    scanf_s("%i", &destination);

    dijkstraFunc(mainGraph, mainSrc);
    print(mainGraph, mainSrc, destination);
  	scanf_s("%i", &mainSrc);

        return -1;
}

