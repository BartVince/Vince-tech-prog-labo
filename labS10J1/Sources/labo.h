#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <stdint.h>
#include <assert.h>

void* allocate(size_t size);

typedef struct {
	void** elements;
	int top;
	size_t max;
} Stack;

Stack stack_init(size_t max_size);
void stack_push(Stack* s, void* element);
void* stack_pop(Stack* s);

typedef struct QNode QNode;
typedef struct QNode {
	void* data;
	QNode* prev;
	QNode* next;
	size_t count;
}Queue;

void queue_init(Queue* q);
void queue_push(Queue* q, void* data);
void* queue_pop(Queue* q);

typedef struct {
	double x;
	double y;
} Vector2;

/*
* Vous devez definir une noeud que vous appelerez Node. Node va avoir une propriete data lui permettant de recevoir de l'information de different type.
* Pour calculer notre heuristique, Node devra avoir une propriete position de type Vector2.
* Node a aussi une propriete uint8_t graph_group qui va contenir l'association entre noeud,  uint8_t cost qui va contenir le cout pour atteindre ce noeud,  une propriete de type uint8_t nommer visited et
* une propriete de type uint8_t nommer path_from. Cette derniere, sera utiliser pour rebrousser chemin.
*/

typedef struct Node Node;
struct Node {
	void* data;
	Vector2 position;
	uint8_t graph_group;
	uint8_t cost;
	uint8_t visited;
	uint8_t path_from;
};

typedef struct AdjMatrix AdjMatrix;

struct AdjMatrix {
	int** adjGraph;// Essentiellement: int[][]
	Node* nodes;
	size_t len;
	size_t max_size;
};





/*
* Creer une matrice d'adjacence. Mettre len a 0. Mettre max_size a max_nodes.
* Allouer de la memoire pour le max de noeud specifier i.e. pour graph->nodes.
* Pour chaque noeud de graph->nodes mettre le cost, graph_group et path_from a UINT8_MAX et visited a 0.
* Pour adjGraph, allouer une quantiter de int* relatif a la valeur de max_nodes.
* Dans chaque int* allouer un tableau de int relatif a la valeur max_nodes.
* A chaque index de adjGraph mettez son cout a 0. 0 indique qu'il n'y a pas d'adjacence.
* Retourner le graph creer.
*/
AdjMatrix* create_graph(size_t max_nodes);

/*
* Creer un node, lui attribuer le data, la position et l'ajouter dans la matrice d'adjacence.
*/
void add_node(AdjMatrix* graph, void* data, Vector2 pos);

/*
* Ajouter un lien dans la matrice d'adjacence, d'un noeud a l'autre noeud, en specifiant le cout y etant relier.
*/
void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost);

/*
* Construire les groupes de nodes. Faire l'algorithme qui determine l'association entre noeud.
*/
void build_groups(AdjMatrix* graph);

/*
* Implementer l'algorithme A*. Lorsqu'il n'y a aucun chemin entre deux node, la Stack doit être vide.
*/
void astar(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath);