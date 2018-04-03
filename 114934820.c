
//  main.c
//  toposort
//
//  Created by emily on 16/12/10.
//  Copyright © 2016年 emily. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2


typedef struct Edge
{
    int vertex;
    int operation;
} Edge;

typedef struct Vertex
{
    int color;
    int operation;
} Vertex;


FILE* fp;
Vertex *Vertices;
Edge **adjList;
int *Sorted;
int NVertices;
int vert=0;
clock_t start,end;
int HasCycle = 0;

void Readgraph(FILE* fp,char*argv[])
{
    NVertices = 0;
    char Stream[256];
    char *AdjN;
    int Adjdex, k = 1;
    int y;
    fp = fopen(argv[1],"r");//open the input file argv[1]
    if(!fp)
    {
        printf("open file %s failed!\n", argv[1]);
        exit(1);
    }
    
    while(fgets(Stream, sizeof(Stream), fp)) ++NVertices;//get the number of vertices
    
    y=NVertices + 1;
    adjList = (Edge**)malloc((y)* sizeof(Edge*));
    Vertices = (Vertex*)malloc((y)* sizeof(Vertex));
    rewind(fp);
    
    while(1)
    {
        if (fgets(Stream,256, fp)== NULL) break;
        strtok(Stream, " ");
        Adjdex = 0;
        adjList[k] = (Edge*) malloc(sizeof(Edge) * 101);
        adjList[k][0].operation=0;
        while((AdjN = strtok(NULL, " ")))
              {
            if(*AdjN != '\n')
            {
                adjList[k][++Adjdex].vertex = atoi(AdjN);
                adjList[k][Adjdex].operation = 0;
                ++adjList[k][0].operation;

            }
              }
        ++k;
    }
    
    fclose(fp);

   }



int Dfs_Visit(int Adjdex)
{
    int k=1,m;
    Vertices[Adjdex].color=GRAY;//mark that it has been discovred.
    ++Vertices[Adjdex].operation;//get vertex operation time
    while(k<=adjList[Adjdex][0].operation)
    {
        
        m=adjList[Adjdex][k].vertex;//get the vertex in adjacency list
        ++Vertices[m].operation;//get vertex time opertion
        ++adjList[Adjdex][k].operation;//get edge opertion
        
        if(Vertices[m].color == GRAY)
        {
            
            return 1;
        }
        else if(Vertices[m].color==WHITE)
        {
           Dfs_Visit(m);
        }
        ++k;
    }
    
    Vertices[Adjdex].color=BLACK;
    ++Vertices[Adjdex].operation;//get vertex operation
    Sorted[NVertices-(++vert)]=Adjdex;
    return 0;

}

int TopologicalSort()
{
    int i=1;
    Sorted = (int*)malloc(NVertices* sizeof(int));//get an empty linked list
    while (i<=NVertices)
    {
        Vertices[i].color=WHITE;//initialization color
        Vertices[i].operation=1;//initialization the operation value of each vertex
        ++i;
    }
    
    i=1;
    while((i<=NVertices))
    {
        ++Vertices[i].operation;//get vertex operation
        if(Vertices[i].color==WHITE)
        {
            if(Dfs_Visit(i))//get the cycle
            {
                return 1;
            }
        }
        ++i;
    }
    
    return 0;
    
}

// Print out files
void PrintGraph(FILE* fp,char*argv[])//,
{
    int x = 0;
    int i,j;
    fp = fopen(argv[2], "w");
    if (HasCycle)
    {
        fprintf(fp, "There is a cycle in this directed graph!\n");
    }
    else
    {
        fprintf(fp,"\nSorted vertices can be found below:\n");
        for ( i = 0; i < NVertices; ++i)
        {
            fprintf(fp, "%d ", Sorted[i]);
        }
        fprintf(fp, "\n");
    }
    
    
    fprintf(fp, "\n");
    
    Vertices[0].operation = 0;
    for ( i = 1; i <= NVertices; ++i)
    {
        Vertices[0].operation += Vertices[i].operation;
        if (!strcmp(argv[1], "in1.txt"))
            fprintf(fp, "Vertex %d: %d\n",i,Vertices[i].operation);
    }
    
    fprintf(fp, "\n");

    for (i = 1; i <= NVertices; ++i)
    {
        for ( j = 1; j <= adjList[i][0].operation; ++j)
        {
            x += adjList[i][j].operation;
            if (!strcmp(argv[1], "in1.txt"))
                fprintf(fp, "Edge from Vertex %d to Vertex %d: %d\n", i,adjList[i][j].vertex, adjList[i][j].operation);
        }
    }
    fprintf(fp,"\ntotal operations detail:\n");
    fprintf(fp, "Total number of operations charged to all vertices is: %d\n", Vertices[0].operation);
    fprintf(fp, "Total number of operations charged to edges is: %d\n", x);
    fprintf(fp, "Total number of operations is: %d\n", Vertices[0].operation + x);
    fprintf(fp,"\nthe total runtime of this program is %lums\n",end-start);
    fclose(fp);
}

int main(int argc, char*argv[])
{
    
    start = clock();
    Readgraph(fp,argv);
    HasCycle = TopologicalSort();
    end = clock();
    PrintGraph(fp,argv);
    
    return 0;
    
}


