#include <stdint.h>
#include <assert.h>
#include "labo.h"

/*
* Programmer l'algorithme de fibonacci. Commencer par faire l'algorithme de base et par la suite ajouter la memoization.
* Faire attention lorsque vous utilisez un type pour une variable qui represente un nombre, int ne depasse pas 2^31-1 ou 2,147,483,647
*/

static long long cache[256];

long long fibonacci(long n) {
	if (n <= 1) {
		return n;
	}

	if (cache[n] == 0) {
		cache[n] = fibonacci(n - 1) + fibonacci(n - 2);
	}
	return cache[n];
}

/*
* Creer un Noeud, assigner lui la valeur de data, metter c'est reference a NULL et retourner le noeud.
*/
TreeNode* tree_create_node(void* data) {
	TreeNode* tr = allocate(sizeof(TreeNode));
	tr->data = data;
	tr->left = NULL;
	tr->right = NULL;
	return tr;
}

/*
* Ajouter le noeud  node dans la propriete left ou right qui est NULL de root. On priorise de gauche a droite. Si right et left son NULL on le met dans left.
* Si jamais aucun des noeuds est NULL, ajouter rien.
*/
void tree_add_node(TreeNode* root, TreeNode* node) {
	if (root->left == NULL) {
		root->left = node;
	}
	else if (root->right == NULL) {
		root->right = node;
	}
}

/*
* Programmer l'algorithme de depth first search afin de trouver la cle. Pour programmer cet algorithme, vous avez besoins d'une pile/stack, ca definition est fourni.
*/

static Stack s;

int dfs(TreeNode* root, void* key) {

	if (s.max != 16) {
		s = stack_init(16);
	}

	if (root->data == key) {
		return 1;
	}

	if (root->right != NULL) {
		stack_push(&s, root->right);
	}

	if (root->left != NULL) {
		stack_push(&s, root->left);
	}

	TreeNode* temp = stack_pop(&s);

	if (temp != NULL) {
		return 1 + dfs(temp, key);
	}
}

/*
* Programmer l'algorithme de breath first search afin de trouver la cle. Pour programmer cet algorithme, vous avez besoins d'une file/queue, ca definition est fourni.
*/

static Queue* q;

int bfs(TreeNode* root, void* key) {

	if (q == NULL) {
		q = allocate(sizeof(Queue));
		queue_init(q);
	}

	if (root->data == key) {
		return 1;
	}

	if (root->left != NULL) {
		queue_push(q, root->left);
	}

	if (root->right != NULL) {
		queue_push(q, root->right);
	}

	TreeNode* temp = queue_pop(q);

	if (temp != NULL) {
		return 1 + bfs(temp, key);
	}

	return 1;
}