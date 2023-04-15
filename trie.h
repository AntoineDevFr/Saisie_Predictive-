#ifndef TRIE_H
#define TRIE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "linked_list.h"

typedef struct trienode
{
    char letter;
    int weight;
    struct trienode *child[27];
} Trienode;

typedef struct trie
{
    Trienode *root;
} Trie;



void initialize_trie(Trie *root);
Trienode *create_node(char letter);
void add_word(Trie *root, char *word);
void create_trie(Trie *trie,char *fileName);
void print_trie(Trienode* node, char* str, int level);
void print_trie_wrapper(Trie* trie);
void print_trie_graph(Trienode* node, int level,FILE *file);
void print_trie_graph_wrapper(Trie* trie);
void search_prefix(Trienode* node, char* prefix, char* buffer, int depth,List *liste_mots);
List suggest_words(Trie* trie, char* prefix);
#endif