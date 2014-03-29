/*Used http://www.geeksforgeeks.org/greedy-algorithms-set-6-dijkstras-shortest-path-algorithm/ graph for reference
the website's 0 = this code's 999 (infinity value)*/
#include <queue>
#include <stack>

using namespace std;
#define MAX 12 // vertices+1
#define INF 999
#define vertices 11

struct gNode
{
    int value;
    bool station;
    bool merge;
    bool fork;
	bool right;
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


void makeGraph(struct dijkstra* graph)
{
			std::queue<gNode> graphq;
            int i, j;
            for (i = 1; i <= vertices; i++)
                {
                    for (j = 1; j <= vertices; j++)
                    {
                        graph->edgeWeight[i][j].value=999;
                    }
                }
			
			/********************************/
			/* ***** BEGIN: STATION 1 ***** */
			/********************************/

			// F4 to S1
			graph->edgeWeight[11][1].value= 2;
			graph->edgeWeight[11][1].station= false;
			graph->edgeWeight[11][1].merge= false;
			graph->edgeWeight[11][1].fork= true;
			graph->edgeWeight[11][1].right= true;

			// S1 to M2
            graph->edgeWeight[1][2].value=2;
			graph->edgeWeight[1][2].station=true;
			graph->edgeWeight[1][2].merge=false;
			graph->edgeWeight[1][2].fork=false;
			graph->edgeWeight[1][2].right=false;
			
			// F4 to M2
			graph->edgeWeight[11][2].value= 5;
			graph->edgeWeight[11][2].station= false;
			graph->edgeWeight[11][2].merge= false;
			graph->edgeWeight[11][2].fork= true;
			graph->edgeWeight[11][2].right= false;

			/********************************/
			/* ****** END: STATION 1 ****** */
			/********************************/

			// M2 to F1
			graph->edgeWeight[2][3].value= 3;
			graph->edgeWeight[2][3].station= false;
			graph->edgeWeight[2][3].merge= true;
			graph->edgeWeight[2][3].fork= false;
			graph->edgeWeight[2][3].right= false;


			// F1 going to S3 via F3
			graph->edgeWeight[3][4].value= 6;
			graph->edgeWeight[3][4].station= false;
			graph->edgeWeight[3][4].merge= false;
			graph->edgeWeight[3][4].fork= true;
			graph->edgeWeight[3][4].right= false;

			// F1 going to S2 via F2
			graph->edgeWeight[3][7].value= 4;
			graph->edgeWeight[3][7].station= false;
			graph->edgeWeight[3][7].merge= false;
			graph->edgeWeight[3][7].fork= true;
			graph->edgeWeight[3][7].right= true;

			/********************************/
			/* ***** BEGIN: STATION 2 ***** */
			/********************************/

			// F2 to S2
			graph->edgeWeight[7][8].value= 2;
			graph->edgeWeight[7][8].station= false;
			graph->edgeWeight[7][8].merge= false;
			graph->edgeWeight[7][8].fork= true;
			graph->edgeWeight[7][8].right= true;

			// S2 to M3
			graph->edgeWeight[8][9].value= 2;
			graph->edgeWeight[8][9].station= true;
			graph->edgeWeight[8][9].merge= false;
			graph->edgeWeight[8][9].fork= false;
			graph->edgeWeight[8][9].right= false;

			// F2 to M3
			graph->edgeWeight[7][9].value= 5;
			graph->edgeWeight[7][9].station= false;
			graph->edgeWeight[7][9].merge= false;
			graph->edgeWeight[7][9].fork= true;
			graph->edgeWeight[7][9].right= false;

			/********************************/
			/* ****** END: STATION 2 ****** */
			/********************************/


			/********************************/
			/* ***** BEGIN: STATION 3 ***** */
			/********************************/

			// F3 to S3
			graph->edgeWeight[4][5].value= 2;
			graph->edgeWeight[4][5].station= false;
			graph->edgeWeight[4][5].merge= false;
			graph->edgeWeight[4][5].fork= true;
			graph->edgeWeight[4][5].right= true;

			// S3 to M4
			graph->edgeWeight[5][6].value= 2;
			graph->edgeWeight[5][6].station= true;
			graph->edgeWeight[5][6].merge= false;
			graph->edgeWeight[5][6].fork= false;
			graph->edgeWeight[5][6].right= false;

			// F3 to M4
			graph->edgeWeight[4][6].value= 5;
			graph->edgeWeight[4][6].station= false;
			graph->edgeWeight[4][6].merge= false;
			graph->edgeWeight[4][6].fork= true;
			graph->edgeWeight[4][6].right= false;

			/********************************/
			/* ****** END: STATION 3 ****** */
			/********************************/

			// M3 to M5
			graph->edgeWeight[9][10].value= 3;
			graph->edgeWeight[9][10].station= false;
			graph->edgeWeight[9][10].merge= true;
			graph->edgeWeight[9][10].fork= false;
			graph->edgeWeight[9][10].right= false;

			// M4 to M5
			graph->edgeWeight[6][10].value= 8;
			graph->edgeWeight[6][10].station= false;
			graph->edgeWeight[6][10].merge= true;
			graph->edgeWeight[6][10].fork= false;
			graph->edgeWeight[6][10].right= false;

			// M5 to F4
			graph->edgeWeight[10][11].value= 7;
			graph->edgeWeight[10][11].station= false;
			graph->edgeWeight[10][11].merge= true;
			graph->edgeWeight[10][11].fork= false;
			graph->edgeWeight[10][11].right= false;

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

                printf(  "\nWeight matrix:\n" );
                /*Weight matrix graph needs to be DIRECTED, not bi-directional
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
    

	//queue<int> mystack;
	stack<int> mystack;
	
	int i, k, node1, node2;
	//gNode direction;
	
    printf("\nShortest Path from vertex %i", src);
    printf(": \n\n");

    for (i = 0; i <= vertices; i++)
    {	
		if (i == dest)
        {
			
			printf("to vertex %i", i);

			printf(" is: ");
			k = i;

			printf("%i", k);
			printf(" << ");
			mystack.push (k);

			while (graph->curPosition[k] != src)
			{		
				
				printf("(((%i)))", graph->curPosition[k]);
				printf(" << ");
				
				k = graph->curPosition[k];

				
				node2 = mystack.top();
				printf("~node2~ is %i\n", node2);
				mystack.push (k);
				
				node1 = mystack.top();
				printf("~node1~ is %i\n", node1);
				
				if( graph->edgeWeight[node1][node2].right == true)
					{
						printf("***TURN RIGHT\n");
					}
					else
					{
						printf("***Go Straight\n");
					}
			}
			mystack.push(src);
							if( graph->edgeWeight[src][node1].right == true)
					{
						printf("***TURN RIGHT\n");
					}
					else
					{
						printf("***Go Straight\n");
					}

			//print travel weight
			printf("%i", src);
			printf("\n Weight : %i", graph->nWeight[i]);
			printf("\n\n");

        }
    }

	
		//queue content
		printf("Stack content is: ");
		while(!mystack.empty())
		{
			printf("%i ", mystack.top());
			mystack.pop();
		}
}
