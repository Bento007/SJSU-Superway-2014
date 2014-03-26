#include <stdio.h>
#include <stdlib.h>
#include "tasks.hpp"

#include "examples/examples.hpp"

#include "StateMachine.hpp"
#include "eint.h"
#include "lpc_pwm.hpp"
#include "lineFollower.hpp"


QueueHandle_t directives=0;     //shared queue, for sending commands from
                                //the state machine to the line follower task

void lineFollower(void *p)
{

}

void stateMachine(void *p)
{
    //TODO do/call initializations here

    while(1){
        //TODO call the state machine function OR migrate code to here
        //All state machine operations code should be here.
        //Task should never end/exit.
        //This task must have highest priority, unless a "safety" task is made.
    }

}

/*
 * dijkstra.h
 *
 *  Created on: Mar 14, 2014
 *      Author: Marjo
 *
 *      Dijkstras is used to find the shortest path
 *      through a track. It uses a 2-d array to
 *      connect nodes.
 *      TESTING TESTING 123
 *
 */


/*Used http://www.geeksforgeeks.org/greedy-algorithms-set-6-dijkstras-shortest-path-algorithm/ graph for reference
the website's 0 = this code's 999 (infinity value)*/

#define MAX 4 // vertices+1
#define INF 999
#define vertices 3

struct gNode
{
    int value;
    bool station;
    bool merge;
    bool fork;
};
struct dijkstra
{
    gNode edgeWeight[MAX][MAX];
    int     curPosition[MAX],
        nWeight[MAX],
        visited[MAX];
};


    void initialize(int);
    void makeGraph(struct dijkstra*);
    void dijkstraFunc(struct dijkstra*, int);
    void print(struct dijkstra*, int, int);
    //int vertices = 3; //TODO: NO GLOBALS MARJO!


int main()
{

    int mainSrc, destination;
    dijkstra *mainGraph = new dijkstra;
    makeGraph(mainGraph);
    //mainGraph.makeGraph();
    printf("\nSource Vertex: ");
    scanf("%i", &mainSrc);

    printf("\nDestination: ");
    scanf("%i", &destination);

    dijkstraFunc(mainGraph, mainSrc);
    print(mainGraph, mainSrc, destination);
    //mainGraph.dijkstraFunc(mainSrc);
    //mainGraph.print(mainSrc);
        scanf("%i", &mainSrc);

        return -1;
}

void makeGraph(struct dijkstra* graph)
{

            int i, j;
            for (i = 1; i <= vertices; i++)
                {
                    for (j = 1; j <= vertices; j++)
                    {
                        graph->edgeWeight[i][j].value=999;
                    }
                }
            graph->edgeWeight[1][2].value=2;
            graph->edgeWeight[2][3].value=1;
            graph->edgeWeight[3][1].value=3;

            /*PUT
            if edgeWeight = [i][j] SET NODE TYPE HERE, have the structs
            does this need to be in main after makeGraph(mainGraph);
            essentially, hardcoding the pointers in the main after the graph is generated
            */

                printf(  "\nWeight matrix:\n" );
                /*Weight matrix graph needs to account for DIRECTED, not bi-directional weights
                i.e. the points should not reflect back B->C is one path but B<-C is not*/
                for (i = 1; i <= vertices; i++)
                {
                    for (j = 1; j <= vertices; j++)
                    {
                    printf("%i", graph->edgeWeight[i][j]);
                    printf("\t");
                    }

                    printf("\n");
            }

}
void initialize(struct dijkstra* graph, int src)
{
    /*src node = 0*/
    for (int i = 1; i <= vertices; i++)
    {
        graph->nWeight[i] = graph->edgeWeight[src][i].value;
        graph->visited[i] = 0;
        graph->curPosition[i] = src;
    }
    graph->visited[src] = 1;

}

/*
Traverses through each vertex and does the "Relaxation" function
*/

void dijkstraFunc(struct dijkstra* graph, int src)
{
    int min, u;
    initialize(graph, src);
    for (int i = 1; i <= vertices; i++)
    {
        min = INF;
        u = 0;
        for (int j = 1; j <= vertices; j++)
        {
            if (!graph->visited[j] && graph->nWeight[j] < min)
            {
                // new min value
                min = graph->nWeight[j];
                u = j;
            }
        }

        graph->visited[u] = 1;

        for (int v = 1; v <= vertices; v++)
            if (!graph->visited[v] && (graph->nWeight[u] + graph->edgeWeight[u][v].value < graph->nWeight[v]))
            {
                // assigns new value into vertex
                graph->nWeight[v] = graph->nWeight[u] + graph->edgeWeight[u][v].value;
                graph->curPosition[v] = u;
            }
    }
}

void print(struct dijkstra* graph, int src, int dest)
{
    int i, k;
    printf("\nShortest Path from vertex %i", src);
    printf(": \n\n");
    for (i = 1; i <= vertices; i++)
    {
        if (i == src)
        continue;

        else if (i == dest)
        {
        printf("to vertex %i", i);
        printf(" is: ");
        k = i;
        printf("%i", k);
        printf(" << ");
        while (graph->curPosition[k] != src)
        {
            printf("%i", graph->curPosition[k]);
            printf(" << ");
            k = graph->curPosition[k];
        }

        //print travel weight
        printf("%i", src);
        printf("\n Weight : %i", graph->nWeight[i]);
        printf("\n\n");
        }
    }
}
