#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <stdint.h>
#include <assert.h>
#include "labo.h"

/*
* Creer une matrice d'adjacence. Mettre len a 0. Mettre max_size a max_nodes.
* Allouer de la memoire pour le max de noeud specifier i.e. pour graph->nodes.
* Pour chaque noeud de graph->nodes mettre le cost et path_from a UINT8_MAX et visited a 0.
* Pour adjGraph, allouer une quantiter de int* relatif a la valeur de max_nodes.
* Dans chaque int* allouer un tableau de int relatif a la valeur max_nodes.
* A chaque index de adjGraph mettez son cout a 0.
* Retourner le graph creer.
*/
AdjMatrix* create_graph(size_t max_nodes) {

    AdjMatrix* graph = allocate(sizeof(AdjMatrix));
    graph->len = 0;
    graph->max_size = max_nodes;
    graph->nodes = allocate(max_nodes * sizeof(Node));

    for (int i = 0; i < max_nodes; i++) {
        graph->nodes[i].cost = UINT8_MAX;
        graph->nodes[i].path_from = UINT8_MAX;
        graph->nodes[i].visited = 0;
    }

    graph->adjGraph = allocate(max_nodes * sizeof(int*));

    for (int iY = 0; iY < max_nodes; iY++) {

        graph->adjGraph[iY] = allocate(max_nodes * sizeof(int));

        for (int iX = 0; iX < max_nodes; iX++) {
            graph->adjGraph[iY][iX] = 0;
        }

    }
    return graph;
}

/*
* Creer un node, lui attribuer le data et l'ajouter dans la matrice d'adjacence.
*/
void add_node(AdjMatrix* graph, void* data) {
    graph->nodes[graph->len].data = data;
    graph->len++;
}

/*
* Ajouter un lien dans la matrice d'adjacence, d'un noeud a l'autre noeud, en specifiant le cout y etant relier.
*/
void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost) {
    graph->adjGraph[fromNode][toNode] = cost;
}

/*
* Implementer l'algorithme de dijkstra.
*/

static Queue* q;
void dijkstra(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath) {
    if (q == NULL) {
        q = allocate(sizeof(Queue));
    }
   
    queue_init(q);
    Node* currNode = &graph->nodes[startNodeIndex];
    int currNodeIndex = startNodeIndex;
    currNode->cost = 0;

    // Main loop
    while (currNode->data != graph->nodes[endNodeIndex].data) { // tant que le node current n'est pas le node recherché
    currNode->visited = 1;

        int cheapestCost = UINT8_MAX;
        int cheapestCostIndex = UINT8_MAX;
        for (int i = 0; i < graph->len; i++) { // loop de la taille de la matrice d'adjacence     
            Node visiting = graph->nodes[i];
            int edgeValue = graph->adjGraph[currNodeIndex][i];
            visiting.cost = edgeValue;
            if (edgeValue != 0 && visiting.visited != 1 && visiting.cost < cheapestCost) { // check si le chemin n'est pas 0 et si le node n'est pas visité
                cheapestCostIndex = i;
                cheapestCost = edgeValue;
                currNode->cost = edgeValue;
            }
        }

        if (cheapestCostIndex != UINT8_MAX) {
            graph->nodes[cheapestCostIndex].path_from = currNodeIndex; // Set le path_from au node avec le cost le plus cheap au node current
            currNode = &graph->nodes[cheapestCostIndex]; // Set current node au node prochain
            currNodeIndex = cheapestCostIndex;           
        }
        else {
            return;
        }
    }
    
    currNode->cost = 0; // noeud de la fin a un cost de 0

    // Reconstruire le chemin
    stack_push(solvedPath, currNode);
    while (currNode->path_from != UINT8_MAX)
    {
        currNode = &graph->nodes[currNode->path_from];
        stack_push(solvedPath, currNode);
    }

    currNode->cost = 0; // noeud du debut a un cost de 0 (je pense... sinon oublie cet ajustement)
}