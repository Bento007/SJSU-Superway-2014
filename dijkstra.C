#include <stdio.h>
#include <stdlib.h>

/*Used http://www.geeksforgeeks.org/greedy-algorithms-set-6-dijkstras-shortest-path-algorithm/ graph for reference
the website's 0 = this code's 999 (infinity value)*/

#define MAX 10
#define INF 999

struct dijkstra
{
	int edgeWeight[MAX][MAX], 
		curPosition[MAX],
		nWeight[MAX], 
		visited[MAX];
};
	void initialize(int);
	void makeGraph(struct dijkstra*);
	void dijkstraFunc(struct dijkstra*, int);
	void print(struct dijkstra*, int);


int main()
{
	
	int mainSrc;
	dijkstra *mainGraph = new dijkstra;
	makeGraph(mainGraph);
	printf("\nSource Vertex: ");
	scanf("%i", &mainSrc);
	dijkstraFunc(mainGraph, mainSrc);
	print(mainGraph, mainSrc);
	scanf("%i", &mainSrc);
}

void makeGraph(struct dijkstra* graph)
{
	FILE *ifile;

	ifile = fopen ("inputfile.txt" , "r");
	if (ifile == NULL) perror ("inputfile.txt does not exist");
	else
	{
	int i, j;
	printf( "\nEnter **positive** weight matrix of the Graph:\nfor vertices with infinite distance, use value '999'\n\n");
	for (i = 1; i <= 9; i++)
	{
		for (j = 1; j <= 9; j++)
		{
				fscanf(ifile, "%i", &graph->edgeWeight[i][j]);
		}
	}
	fclose(ifile);
	printf(  "\nEntered weight matrix:\n" );
	for (i = 1; i <= 9; i++)
	{
		for (j = 1; j <= 9; j++)
		{
		printf("%i", graph->edgeWeight[i][j]);
		printf("\t");
		}

		printf("\n");
	}
	}
}
void initialize(struct dijkstra* graph, int src)
{
	/*src node = 0*/
	for (int i = 1; i <= 9; i++)
	{
		graph->nWeight[i] = graph->edgeWeight[src][i];
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
	for (int i = 1; i <= 9; i++)
	{
		min = INF;
		u = 0;
		for (int j = 1; j <= 9; j++)
		{
			if (!graph->visited[j] && graph->nWeight[j] < min)
			{
				// new min value
				min = graph->nWeight[j];
				u = j;
			}
		}

		graph->visited[u] = 1;

		for (int v = 1; v <= 9; v++)
			if (!graph->visited[v] && (graph->nWeight[u] + graph->edgeWeight[u][v] < graph->nWeight[v]))
			{
				// assigns new value into vertex
				graph->nWeight[v] = graph->nWeight[u] + graph->edgeWeight[u][v];
				graph->curPosition[v] = u;
			}
	}
}

void print(struct dijkstra* graph, int src)
{
	int i, k;
	printf("\nShortest Path from vertex %i", src);
	printf(": \n\n");
	for (i = 1; i <= 9; i++)
	{
		if (i == src)
		continue;

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
		printf("%i", src);
		printf("\n Weight : %i", graph->nWeight[i]);
		printf("\n\n");
	}
}

