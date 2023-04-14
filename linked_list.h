#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "hash.h"

typedef struct Node {
    struct Node* next;
    char mot[MAX_WORD_LENGHT];
    int weight;
}Node;

typedef struct List{
    Node* head;
} List;

void initializeList(List *liste);
void printList(List* liste);
void addNodeHead(List* liste, int weight, char *mot); //modifier si nécessaire
void deleteFirstNode(List* liste);
void mirrorList(List* liste);
void concatenationList(List* liste1, List* liste2);
List mergeSortList(List liste1, List liste2);
void triFusion(List* liste);
void print_n_last(List *liste,int n);

#endif