#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_WORD_LENGHT 30		
#define HASH_SIZE		37987	
#define BASE			128

typedef struct _element
{
	char word[MAX_WORD_LENGHT];
	struct _element	*next;
} Element;

typedef struct _hashTable
{
	unsigned int	size;
	unsigned int	nbOccupiedEntries;
	unsigned int	nbElements;
	Element			**Elements;
} HashTable;

void loadDictionaryFromFile(HashTable* hashTab, const char* dictionaryFileName);
void insertElementToHashTable(HashTable* hashTab, char* word);
void initializeHashTable(HashTable *hashTab);
bool checkExistenceWordInDictionary(HashTable* hashTab, char * word);
unsigned long getHashValue(char *string);

#endif