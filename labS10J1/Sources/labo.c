#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <stdint.h>
#include <assert.h>
#include "labo.h"

/*
* Creer une matrice d'adjacence. Mettre len a 0. Mettre max_size a max_nodes.
* Allouer de la memoire pour le max de noeud specifier i.e. pour graph->nodes.
* Pour chaque noeud de graph->nodes mettre le cost, graph_group et path_from a UINT8_MAX et visited a 0.
* Pour adjGraph, allouer une quantiter de int* relatif a la valeur de max_nodes.
* Dans chaque int* allouer un tableau de int relatif a la valeur max_nodes.
* A chaque index de adjGraph mettez son cout a 0. 0 indique qu'il n'y a pas d'adjacence.
* Retourner le graph creer.
*/
AdjMatrix* create_graph(size_t max_nodes) {
	AdjMatrix* graph = allocate(sizeof(AdjMatrix));
	graph->len = 0;
	graph->max_size = max_nodes;
	graph->nodes = allocate(max_nodes * sizeof(Node));

	for (int i = 0; i < max_nodes; i++) {
		graph->nodes[i].cost = UINT8_MAX;
		graph->nodes[i].graph_group = UINT8_MAX;
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
* Creer un node, lui attribuer le data, la position et l'ajouter dans la matrice d'adjacence.
*/
void add_node(AdjMatrix* graph, void* data, Vector2 pos) {
	graph->nodes[graph->len].data = data;
	graph->nodes[graph->len].position = pos;
	graph->len++;
}

/*
* Ajouter un lien dans la matrice d'adjacence, d'un noeud a l'autre noeud, en specifiant le cout y etant relier.
*/
void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost) {
	graph->adjGraph[fromNode][toNode] = cost;
}

/*
* Construire les groupes de nodes. Faire l'algorithme qui determine l'association entre noeud.
*/
void build_groups(AdjMatrix* graph) {

	int currGroup = -1; // Le premier group sera 0, puis le deuxieme sera 1, etc...

	for (int iY = 0; iY < graph->len; iY++) {

		Node* visiting = &graph->nodes[iY];

		if (visiting->path_from == UINT8_MAX) {
			currGroup++;
		}

		visiting->graph_group = currGroup;

		for (int iX = 0; iX < graph->len; iX++) {

			if (graph->adjGraph[iY][iX] != 0) {
				graph->nodes[iX].path_from = iY;
			}
		}
	}

	for (int i = 0; i < graph->len; i++) { // Reset les path_from au cas ou le main.c ne le fait pas.
		graph->nodes[i].path_from = UINT8_MAX;
	}
}

/*
* Implementer l'algorithme A*. Lorsqu'il n'y a aucun chemin entre deux node, la Stack doit être vide.
*/

static Queue* q;

void astar(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath) {
	if (q == NULL) {
		q = allocate(sizeof(Queue));
	}

	queue_init(q);
	Node* currNode = &graph->nodes[startNodeIndex];
	int currNodeIndex = startNodeIndex;
	currNode->cost = 0;

	// Main loop
	while (currNode->data != graph->nodes[endNodeIndex].data) {
		currNode->visited = 1;

		int cheapestCost = UINT8_MAX;
		int cheapestTotalCost = UINT8_MAX;
		int cheapestCostIndex = UINT8_MAX;

		for (int i = 0; i < graph->len; i++) {
			Node* visiting = &graph->nodes[i];
			int edgeValue = graph->adjGraph[currNodeIndex][i];

			if (edgeValue != 0 && visiting->visited != 1) {

				// Calculate distance / heuristic
				int distanceX = graph->nodes[endNodeIndex].position.x - visiting->position.x;
				int distanceY = graph->nodes[endNodeIndex].position.y - visiting->position.y;
				int heuristic = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

				int totalCost = edgeValue - heuristic;

				if (totalCost < cheapestTotalCost) {
					cheapestTotalCost = totalCost;
					cheapestCostIndex = i;
					cheapestCost = edgeValue;

					visiting->cost = currNode->cost + edgeValue;
					visiting->path_from = currNodeIndex;
				}
			}
		}

		if (cheapestCostIndex != UINT8_MAX) {
			currNode = &graph->nodes[cheapestCostIndex];
			currNodeIndex = cheapestCostIndex;
		}
		else {
			return;
		}
	}

	// Reconstruire le chemin
	stack_push(solvedPath, currNode);
	while (currNode->path_from != UINT8_MAX) {
		currNode = &graph->nodes[currNode->path_from];
		stack_push(solvedPath, currNode);
	}

	currNode->cost = 0; // noeud du debut a un cost de 0 (je pense... sinon oublie cet ajustement)
}