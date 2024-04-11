#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <stdint.h>
#include <assert.h>

void* allocate(size_t size);

/*
* Vous definir une struct nommer HashTable. Elle va contenir une liste de void* sous le nom de data.
* Elle va contenir aussi une propriete nommer max_size de type size_t.
*/

typedef struct {
	void** data;
	size_t max_size;

}HashTable;


/*
* Creer une HashTable, assigner lui la valeur max_size, allouer de la memoire pour la quantiter de d'information possible dans la liste data.
* Pour chaque index du tableau data, assigner la valeur NULL. Retourner ensuite le pointeur vers le HashTable.
*/
HashTable* create_hashtable(size_t max_size);

/*
* Implementer la fonction de hashing tel que vu en classe. Retourner l'index
*/
size_t hash(char* key, size_t max);

/*
* Ajouter la cle dans le hashtable avec la valeur donner.
* Si l'operation reussi retourner 1. Si jamais il y a deja une valeur dans le tableau retourner 0.
*/
int add_kv(HashTable* table, char* key, void* data);

/*
* Si jamais la cle se retrouve dans le tableau, on assigne la valeur du tableau a NULL et on retourne la valeur qu'il y avait dans le tableau.
* Sinon, on retourne NULL.
*/
void* del_kv(HashTable* table, char* key);

/*
* Si jamais la cle se retrouve dans le tableau,on retourne la valeur qu'il y avait dans le tableau.
* Sinon, on retourne NULL.
*/
void* get_value(HashTable* table, char* key);