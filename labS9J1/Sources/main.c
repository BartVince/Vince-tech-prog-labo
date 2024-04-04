#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "labo.h"

Stack stack_init(size_t max_size) {
	Stack s;
	s.max = max_size;
	s.top = -1;
	s.elements = allocate(sizeof(void*) * s.max);
	return s;
}

void stack_push(Stack* s, void* element) {
	if (s->max != s->top + 1) {
		s->top++;
		s->elements[s->top] = element;
	}
}

void* stack_pop(Stack* s) {
	if (s->top > -1) {
		void* elem = s->elements[s->top];
		s->elements[s->top] = NULL;
		--s->top;
		return elem;
	}
	return NULL;
}
void queue_init(Queue* q) {
	q->next = q->prev = NULL;
	q->count = 0;
}
void queue_push(Queue* q, void* data) {
	QNode* n = allocate(sizeof(QNode));
	n->data = data;
	n->prev = n->next = NULL;
	if (q->prev == NULL) {
		q->prev = q->next = n;
	}
	else {
		QNode* temp = q->next;
		q->next = temp->prev = n;
		n->next = temp;
	}
}
void* queue_pop(Queue* q) {
	if (q->prev != NULL) {
		QNode* temp = q->prev;
		if(temp->prev != NULL)
			temp->prev->next = NULL;
		q->prev = temp->prev;
		q->count++;
		return temp->data;
	}
	return NULL;
}

#define HEAP_SIZE 2048 * 2048 * 4
static uint8_t* heap = NULL;
static size_t heap_top = 0;
void* allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}

int main(int argc, char** argv) {
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);

	Node* list[32] = { 0 };
	uint8_t len = 0;
	list[len++] = create_node("A");
	list[len++] = create_node("B");
	list[len++] = create_node("C");
	list[len++] = create_node("D");
	list[len++] = create_node("E");
	list[len++] = create_node("F");

	add_adjacent_node(list[0], list[4]);
	add_adjacent_node(list[0], list[1]);
	add_adjacent_node(list[1], list[0]);
	add_adjacent_node(list[1], list[2]);
	add_adjacent_node(list[2], list[1]);
	add_adjacent_node(list[2], list[3]);
	add_adjacent_node(list[2], list[4]);
	add_adjacent_node(list[3], list[2]);
	add_adjacent_node(list[4], list[5]);
	add_adjacent_node(list[5], list[4]);

	Stack s = stack_init(100);
	//-----------FIRST CHECK-----------
	dfs(list,len,NULL, "B",&s);
	int nodes_visited = s.top;
	for (int i = nodes_visited; i >= 0; --i) {
		Node* n = stack_pop(&s);
		if(n->data != list[i]->data)
			printf("ERROR: Vous avez un erreur dans la fonction depth first search\n");
	}
	for (int i = 0; i < len; ++i) {
		list[i]->visited = 0;
	}

	//-----------SECOND CHECK-----------
	dfs(list, len, NULL, "F", &s);
	int nodes_visited2 = s.top;
	char* table[5] = { "A","E","F" };
	for (int i = nodes_visited2; i >= 0; --i) {
		Node* n = stack_pop(&s);
		if (n->data != table[i])
			printf("ERROR: Vous avez un erreur dans la fonction depth first search\n");
	}
	for (int i = 0; i < len; ++i) {
		list[i]->visited = 0;
	}
	if (nodes_visited == 1 && nodes_visited2 == 2) {
		printf("Vous avez potentiellement bien programmer la fonction depth first search\n");
	}
	else {
		printf("Vous n'avez pas bien programmer la fonction depth first search, vous avez visité %i nodes et on veut visiter 6 nodes\n",nodes_visited);
	}

	//-----------THIRD CHECK-----------
	list[0]->adj[0] = list[0]->adj[1];
	list[0]->len--;
	dfs(list, len, NULL, "F", &s);
	nodes_visited = s.top;
	char* table2[] = { "A","B","C","E","F" };
	for (int i = nodes_visited; i >= 0; --i) {
		Node* n = stack_pop(&s);
		if (n->data != table2[i])
			printf("ERROR: Vous avez un erreur dans la fonction depth first search\n");
	}
	if (nodes_visited == 4) {
		printf("Vous avez potentiellement bien programmer la fonction depth first search\n");
	}
	else {
		printf("Vous n'avez pas bien programmer la fonction depth first search, vous avez visité %i nodes et on veut visiter 6 nodes\n", nodes_visited);
	}
	for (int i = 0; i < len; ++i) {
		list[i]->visited = 0;
	}

	//-----------FIRST CHECK-----------
	nodes_visited2 = nodes_visited = 0;
	list[0]->len--;

	add_adjacent_node(list[0], list[4]);
	add_adjacent_node(list[0], list[1]);

	list[1]->len = 0;
	nodes_visited = bfs(list, "F",&s);
	for (int i = 0; i < nodes_visited-1; ++i) {
		Node* n = stack_pop(&s);
		if (n->data != table[i])
			printf("ERROR: Vous avez un erreur dans la fonction breath first search\n");
	}


	for (int i = 0; i < len; ++i) {
		list[i]->visited = 0;
	}

	//-----------SECOND CHECK-----------
	list[0]->len = 0;

	add_adjacent_node(list[0], list[1]);
	add_adjacent_node(list[0], list[4]);

	nodes_visited2 = bfs(list, "F", &s);
	for (int i = 0; i < nodes_visited2-1; ++i) {
		Node* n = stack_pop(&s);
		if (n->data != table[i])
			printf("ERROR: Vous avez un erreur dans la fonction breath first search\n");
	}

	if (nodes_visited == 4 && nodes_visited2 == 4) {
		printf("Vous avez potentiellement bien programmer la fonction breath first search\n");
	}
	else {
		printf("Vous n'avez pas bien programmer la fonction breath first search, vous avez visité %i nodes et on veut visiter 6 nodes\n", nodes_visited);
	}
	
	for (int i = 0; i < len; ++i) {
		list[i]->visited = 0;
	}

	//-----------THIRD CHECK-----------
	list[0]->len = 0;
	add_adjacent_node(list[0], list[1]);
	add_adjacent_node(list[1], list[2]);

	bfs(list, "F", &s);
	for (int i = 0; -1 < s.top; ++i) {
		Node* n = stack_pop(&s);
		if (n->data != table2[i])
			printf("ERROR: Vous avez un erreur dans la fonction breath first search\n");
	}

	return 0;
}
