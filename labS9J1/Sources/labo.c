#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "labo.h"

/*
* Creer un Noeud, assigner lui la valeur de data, faite un memset 0 sur adj, visited a faux, len a 0, allouer de la m�moire pour le QNode et assurer vous de mettre la propiete prev de QNode a NULL.
* Retourner ensuite le pointeur vers le Node.
*/
Node* create_node(void* data) {
	Node* n = allocate(sizeof(Node));
	n->data = data;
	memset(n->adj, 0, sizeof(UINT8_MAX));
	n->visited = 0;
	n->len = 0;
	QNode* qn = allocate(sizeof(QNode));
	qn->prev = NULL;
	n->revPath = *qn;
}

/*
* Ajouter le node dans la liste d'adjacence de root.
*/
void add_adjacent_node(Node* root, Node* node) {
	root->adj[root->len] = node;
	root->len++;
}

/*
* Programmer l'algorithme de depth first search afin de trouver la cle. Pour programmer cet algorithme, vous avez besoins d'une pile/stack, ca definition est fourni.
* La Stack devrait contenir la liste en ordre inverse de celle parcouru. i.e. si le chemin est A -> B -> C la stack avec son pop devrait retourner C -> B -> A
*/
int dfs(Node* root[], int len, Node* curr, void* key, Stack* s) {
	if (curr == NULL) {
		curr = root[0];
	}

	stack_push(s, curr);
	curr->visited = 1;

	if (curr->data == key) {
		return 1;
	}

	for (int i = 0; i < curr->len; i++) {
		if (curr->adj[i]->visited == 1) {
			continue;
		}
		else {
			return 1 + dfs(root, len, curr->adj[i], key, s);
		}
	}

	Node* temp = stack_pop(s);
	temp = stack_pop(s);
	return 1 + dfs(root, len, temp, key, s);
}

/*
* Programmer l'algorithme de breath first search afin de trouver la cle. Pour programmer cet algorithme, vous avez besoins d'une file/queue, ca definition est fourni.
* La Stack devrait contenir la liste en ordre du chemin parcouru. i.e. si le chemin est A -> B -> C la stack avec son pop devrait retourner A -> B -> C
*/

static Queue* q;

int bfs(Node* root[], void* key, Stack* s) {
	if (q == NULL) {
		q = allocate(sizeof(Queue));
	}

	queue_init(q);
	for (int i = 0; i <= 5; i++) {
		root[i]->revPath.prev = NULL;
	}
	queue_push(q, root[0]);
	root[0]->visited = 1;

	int nodesVisited = 0;
	Node* temp = NULL;


	while (q->prev != NULL) {
		temp = queue_pop(q);
		nodesVisited++;
		if (temp->data == key) {
			break;
		}
		else {
			for (int i = 0; i < temp->len; i++) {
				if (temp->adj[i]->visited != 1) {
					queue_push(q, temp->adj[i]);
					temp->adj[i]->visited = 1;
					temp->adj[i]->revPath.prev = temp;
				}
			}
		}
	}

	stack_push(s, temp);
	while (temp->revPath.prev != NULL) {
		temp = temp->revPath.prev;
		stack_push(s, temp);
	}

	return nodesVisited;

}