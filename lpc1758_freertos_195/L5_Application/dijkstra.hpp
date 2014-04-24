#ifndef DIJKSTRA_HPP_
#define DIJKSTRA_HPP_

/*
 * djikstra.hpp
 *
 *  Created on: Apr 10, 2014
 *      Author: Eriberto
 */

#include <queue>
#include <stack>
#include <iostream>
#include "shared_queues.hpp"

using namespace std;

#define MAX 12 // vertices+1
#define INF 999
#define vertices 11

//typedef struct{
//        int source;
//        int destination;
//}path;

//struct gNode
//{
//    int value;
//    bool station;
//    bool merge;
//    bool fork;
//    bool right;
//};
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
int* print(struct dijkstra*, int, int, int, int);
void dijkstraFunction(path_t);
void setEdge(struct dijkstra *graphQ,int i,int j, uint32_t weight,
        uint32_t ticks, track_t type, uint32_t source,
        uint32_t destination, bool station, bool merge, bool fork,
        bool right)
{
    graphQ->edgeWeight[i][j].weight= weight;
    graphQ->edgeWeight[i][j].ticks = ticks;
    graphQ->edgeWeight[i][j].type = type;
    graphQ->edgeWeight[i][j].name.source = source;
    graphQ->edgeWeight[i][j].name.destination = destination;
    graphQ->edgeWeight[i][j].station= station;
    graphQ->edgeWeight[i][j].merge= merge;
    graphQ->edgeWeight[i][j].fork= fork;
    graphQ->edgeWeight[i][j].right= right;
}

void makeGraph(struct dijkstra* graph)
{
            std::queue<gNode> graphq;
            int i, j;
            for (i = 1; i <= vertices; i++)
                {
                    for (j = 1; j <= vertices; j++)
                    {
                        graph->edgeWeight[i][j].weight=999;
                    }
                }

            /********************************/
            /* ***** BEGIN: STATION 1 ***** */
            /********************************/

            // F4 to S1
            setEdge(graph, 11, 1, 2,10,fork,4,1,false,false,true,true);
//            graph->edgeWeight[11][1].weight= 2;
//            graph->edgeWeight[11][1].station= false;
//            graph->edgeWeight[11][1].merge= false;
//            graph->edgeWeight[11][1].fork= true;
//            graph->edgeWeight[11][1].right= true;

            // S1 to M2
            setEdge(graph, 1, 2, 2,10,stations,1,2,true,false,false,false);
//            graph->edgeWeight[1][2].weight=2;
//            graph->edgeWeight[1][2].station=true;
//            graph->edgeWeight[1][2].merge=false;
//            graph->edgeWeight[1][2].fork=false;
//            graph->edgeWeight[1][2].right=false;

            // F4 to M2
            setEdge(graph, 11, 2, 5,10,fork,4,2,false,false,true,false);
//            graph->edgeWeight[11][2].weight= 5;
//            graph->edgeWeight[11][2].station= false;
//            graph->edgeWeight[11][2].merge= false;
//            graph->edgeWeight[11][2].fork= true;
//            graph->edgeWeight[11][2].right= false;

            /********************************/
            /* ****** END: STATION 1 ****** */
            /********************************/

            // M2 to M1
            setEdge(graph, 2, 3, 3,10,merge,2,1,false,true,false,false);
//            graph->edgeWeight[2][3].weight= 3;
//            graph->edgeWeight[2][3].station= false;
//            graph->edgeWeight[2][3].merge= true;
//            graph->edgeWeight[2][3].fork= false;
//            graph->edgeWeight[2][3].right= false;


            // S1 going to S3 via M1
            // M1 to F3
            setEdge(graph, 3, 4, 6,10,merge,1,3,false,true,false,false);
//            graph->edgeWeight[3][4].weight= 6;
//            graph->edgeWeight[3][4].station= false;
//            graph->edgeWeight[3][4].merge= true;
//            graph->edgeWeight[3][4].fork= false;
//            graph->edgeWeight[3][4].right= false;

            /********************************/
            /* ***** BEGIN: STATION 2 ***** */
            /********************************/

            // F2 to S2
            setEdge(graph, 7, 8, 2,10,fork,2,2,false,false,true,true);
//            graph->edgeWeight[7][8].weight= 2;
//            graph->edgeWeight[7][8].station= false;
//            graph->edgeWeight[7][8].merge= false;
//            graph->edgeWeight[7][8].fork= true;
//            graph->edgeWeight[7][8].right= true;

            // S2 to M3
            setEdge(graph, 8, 9, 2,10,stations,2,3,true,false,false,false);
//            graph->edgeWeight[8][9].weight= 2;
//            graph->edgeWeight[8][9].station= true;
//            graph->edgeWeight[8][9].merge= false;
//            graph->edgeWeight[8][9].fork= false;
//            graph->edgeWeight[8][9].right= false;

            // F2 to M3
            setEdge(graph, 7, 9, 5,10,fork,2,3,false,false,true,false);
//            graph->edgeWeight[7][9].weight= 5;
//            graph->edgeWeight[7][9].station= false;
//            graph->edgeWeight[7][9].merge= false;
//            graph->edgeWeight[7][9].fork= true;
//            graph->edgeWeight[7][9].right= false;

            /********************************/
            /* ****** END: STATION 2 ****** */
            /********************************/


            /********************************/
            /* ***** BEGIN: STATION 3 ***** */
            /********************************/

            // F3 to S3
            setEdge(graph, 4, 5, 2,10,fork,3,3,false,false,true,true);
//            graph->edgeWeight[4][5].weight= 2;
//            graph->edgeWeight[4][5].station= false;
//            graph->edgeWeight[4][5].merge= false;
//            graph->edgeWeight[4][5].fork= true;
//            graph->edgeWeight[4][5].right= true;

            // S3 to M4
            setEdge(graph, 5, 6, 2,10,stations,3,4,true,false,false,false);
//            graph->edgeWeight[5][6].weight= 2;
//            graph->edgeWeight[5][6].station= true;
//            graph->edgeWeight[5][6].merge= false;
//            graph->edgeWeight[5][6].fork= false;
//            graph->edgeWeight[5][6].right= false;

            // F3 to M4
            setEdge(graph, 4, 6, 3,10,fork,3,4,false,false,true,false);
//            graph->edgeWeight[4][6].weight= 5;
//            graph->edgeWeight[4][6].station= false;
//            graph->edgeWeight[4][6].merge= false;
//            graph->edgeWeight[4][6].fork= true;
//            graph->edgeWeight[4][6].right= false;

            /********************************/
            /* ****** END: STATION 3 ****** */
            /********************************/

            // M4 to F5
            setEdge(graph, 6, 10, 8,10,merge,4,5,false,true,false,false);
//            graph->edgeWeight[6][10].weight= 8;
//            graph->edgeWeight[6][10].station= false;
//            graph->edgeWeight[6][10].merge= true;
//            graph->edgeWeight[6][10].fork= false;
//            graph->edgeWeight[6][10].right= false;

            // F5 to F4
            setEdge(graph, 10, 11, 7,10,fork,5,4,false,false,true,false);
//            graph->edgeWeight[10][11].weight= 7;
//            graph->edgeWeight[10][11].station= false;
//            graph->edgeWeight[10][11].merge= false;
//            graph->edgeWeight[10][11].fork=true;
//            graph->edgeWeight[10][11].right= false;

            // F5 TO F2
            // graph, i, j, weight, ticks, type, source, destination, station, merge, fork, right
            setEdge(graph, 10, 7, 3, 10, fork, 5, 2, false, false, true, true);

            // M3 TO M1
            setEdge(graph, 9, 3, 5, 10, merge, 3, 1, false, true, false, false);


            /*Checking what node type is: Station, Merge, or Fork*/
            /*for (i = 1; i <= vertices; i++)
                {
                    for (j = 1; j <= vertices; j++)
                    {
                        if (graph->edgeWeight[i][j].station==true)
                        {
                            printf("I'm a station\n");
                        }
                        else
                        {
                            printf("I'm a merge or a fork\n ");
                        }

                    }
                }*/

//                printf(  "\nWeight matrix:\n" );
//                /*Weight matrix graph needs to be DIRECTED, not bi-directional
//                i.e. the points should not reflect back B->C is one path but B<-C is not*/
//                for (i = 1; i <= vertices; i++)
//                {
//                    for (j = 1; j <= vertices; j++)
//                    {
//                    printf("%i", graph->edgeWeight[i][j]);
//                    printf("\t");
//                    }
//
//                    printf("\n");
//                }

}

/*
- Flags all nodes as unvisited
*/
void initialize(struct dijkstra* graph, int src)
{
    /*src node = 0*/
    for (int i = 1; i <= vertices; i++)
    {
        graph->nWeight[i] = graph->edgeWeight[src][i].weight;
        graph->visited[i] = 0;
        graph->curPosition[i] = src;
    }
    graph->visited[src] = 1;

}

/*
- Performs Dijkstra algorithm
- Iterates through all nodes that are connected to src node
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
            if (!graph->visited[v] && (graph->nWeight[u] + graph->edgeWeight[u][v].weight < graph->nWeight[v]))
            {
                // assigns new value into vertex
                graph->nWeight[v] = graph->nWeight[u] + graph->edgeWeight[u][v].weight;
                graph->curPosition[v] = u;
            }
    }
}

/*
- Creates an int stack of node labels used for the Dijkstra algorithm
- Prints turn/straight instructions backwards because the
    print() function iterates until src == dest
    and then prints from dest --> backwards --> src
- I made a stack just because to make it easier to visualize which nodes are being visited
*/
int* print(struct dijkstra* graph, int src, int dest, int *array, int size)
{
    stack<int> mystack, pDir;
    stack<gNode> visitedNodes;

    int i, k, node1, node2;

    printf("\nInstructions from source { %i } ", src);
    for (i = 0; i <= vertices; i++)
    {
        if (i == dest)
        {

            printf("to vertex { %i }", i);

            printf(" is: \n");
            k = i;
            pDir.push(0);
            mystack.push (k);
            printf("(Read from the bottom up) \n\n");
            while (graph->curPosition[k] != src)
            {
                k = graph->curPosition[k];

                node2 = mystack.top();
                /* Stores "Next" node (int) value so that I can access the node object! */
                mystack.push (k);
                node1 = mystack.top();
                /* Stores "Current" node (int) value so that I can access the node object! */


                /*EDITING FOR FREE RTOS*/

                if( graph->edgeWeight[node1][node2].right == true)
                // graph->edgeWeight[node1][node2].right is my node object
                {
                    printf("TURN RIGHT\n");
                    pDir.push(2);
                }
                else
                {
                    printf("Move forward\n");
                    pDir.push(1);
                }

                visitedNodes.push(graph->edgeWeight[node1][node2]);//TODO: trying to get nodes info to statemachine
            }//end while loop

            /*
            Pushing the src node outside of while loop because
            src is not needed to find neighboring nodes
            */
            mystack.push(src);
            /*MODIFIED FOR FREERTOS; ORIGINAL IN COMMENTS*/
            if( graph->edgeWeight[src][node1].right == true)
            {
                printf("***TURN RIGHT\n");
                pDir.push(2);
            }
            else
            {
                printf("Move forward\n");
                pDir.push(1);
            }

            visitedNodes.push(graph->edgeWeight[src][node2]);//TODO: trying to get nodes info to statemachine

            printf("\nTravel distance (total edge weight traveled) : %i", graph->nWeight[i]);
            printf("\n\n");

        }//end-if

    }//end for-loop

    printf("Node traversal: ");
    while(!mystack.empty())
    {
        printf("%i >> ", mystack.top());
        mystack.pop();
    }
    printf("Done!\n\n");
    printf("pDir size: %i\n\n", pDir.size());
    int arrSize = pDir.size();
    int *arrayDir = array;
    int index=0;

//        pDir.push(0);
    printf(" 0 = stop \n 1 = forward \n 2 = right \n Directions traversal: ");
    while(!pDir.empty())
    {
        printf("%i", pDir.top());
        arrayDir[index]=pDir.top();
        index++;
        pDir.pop();
    }
//        index =0;
//        printf("\n\nArray Contents: ", index);
//        while(index <= arrSize)
//        {
//            printf("%i ", arrayDir[index]);
//            index++;
//        }
    printf("\nEnd Print\n");

    return arrayDir;
}

//void dijkstraFunction(path initPath){
////    path initPath;
//    int size = 15;
//    int array[size];
//    int send = 50;
//    bool done = false;
//    bool start = true;
//
//    dijkstra *mainGraph = new dijkstra;
//    makeGraph(mainGraph);
//    dijkstraFunc(mainGraph, initPath.source);
//
//    int *directions=print(mainGraph, initPath.source, initPath.destination, array, size);
//    int i=0;
//    puts("Received directions Dijkstra Function");
//    while(!done)
//    {
//        send = directions[i];
//
//        if(!start && send == 0){
//            xQueueSend(pathToSM, &send,0);
//            done = true;
//            break;
//        }
//
//        xQueueSend(pathToSM, &send, 10);
//        start = false;
//        i++;
//    }
//    printf("\nEnd dijstraFunction returning to SM task \n");
//}

#endif /* DIJKSTRA_HPP_ */
