#include "trie.h"
#include "hash.h"
#include "linked_list.h"

#include <string.h>

void initialize_trie(Trie *trie)
{
    trie->root = create_node('/');
}


Trienode *create_node(char letter)
{
    Trienode *newnode = (Trienode*) malloc(sizeof(Trienode));

    for (int i = 0; i < 27; i++)
    {
        newnode->child[i] = NULL;
        //on initialise tous les enfants a NULL, a chaque fois qu'on rajoute un mot,
        // on mettra les nouveaux noeuds dans les pointeurs correspondants
    }

    newnode->letter = letter;
    newnode->weight = 0;
    return newnode;
}

void add_word(Trie *trie, char *word)
{
    Trienode* current = trie->root;
    while((*word) != '\0')
    {
        int index = *word-'a'; // difference de valeur ASCII
        if ((*word) < 97||(*word) >  122) // 
        {
            index = 27;
            *word = '#';
        }
        if (current -> child[index] == NULL)
        {
            current->child[index] = create_node(*word);
        }
        current = current->child[index];  
        word++;
    }
    current->weight++;
}

void create_trie(Trie *trie, char *fileName)
{
    FILE *file;
    file = fopen(fileName,"r");

    if (file !=NULL)
    {
        char word[MAX_WORD_LENGHT];
        
        while(fscanf(file,"%s",word) >= 1)
        {
            //printf("%s\n",word);
            add_word(trie,word);
        }
    }
    else printf("Le fichier n'a pas été trouvé.");
	fclose(file);
}


void print_trie(Trienode* node, char* str, int level) {
    if (node == NULL) {
        return;
    }

    str[level] = node->letter;
    if (node->weight > 0) {
        str[level+1] = '\0';
        printf("%s\n", str);
    }

    int i;
    for (i = 0; i < 27; i++) {
        print_trie(node->child[i], str, level+1);
    }
}

void print_trie_wrapper(Trie* trie) {
    char str[100];
    print_trie(trie->root, str, 0);
}

void print_trie_graph(Trienode* node, int level, FILE *file) {
    if (node == NULL) {
        return;
    }

    int i;
    for (i = 0; i < level; i++) {
        fprintf(file,"|  ");
    }

    if (node->weight > 0) {
        fprintf(file,"'%c' (weight : %d)\n", node->letter, node->weight);
    } else {
        fprintf(file,"'%c'\n", node->letter);
    }

    for (i = 0; i < 27; i++) {
        print_trie_graph(node->child[i], level+1,file);
    }
}

void print_trie_graph_wrapper(Trie* trie) {
    FILE *file = fopen("test.txt", "w");

    fprintf(file,"root\n");
    print_trie_graph(trie->root, 1,file);
}

void search_prefix(Trienode* node, char* prefix, char* buffer, int depth,List *liste_mots) {

    if (node == NULL) {
        return;
    }

    buffer[depth] = node->letter;
    if (node->weight > 0) {
        buffer[depth+1] = '\0';
        //printf("%s%s\n",prefix, buffer);

        char word[strlen(prefix) + strlen(buffer)];
        strcpy(word, prefix);
        strcat(word, buffer);

        addNodeHead(liste_mots,node->weight,word);
    }

    for (int i = 0; i < 27; i++) {
        search_prefix(node->child[i], prefix, buffer, depth+1,liste_mots);
    }
}

List suggest_words(Trie* trie, char* prefix)
{
    List liste_mots;
    initializeList(&liste_mots);

    Trienode* current = trie->root;
    char prefix_copy[strlen(prefix)];
    strcpy(prefix_copy, prefix);
    prefix_copy[strlen(prefix)-1] = '\0';

    while((*prefix) != '\0')
    {
        int index = *prefix-'a'; // difference de valeur ASCII
        if ((*prefix)<97 || (*prefix) >  122) // cas du "ç"  
        {
           
            index = 27;

        }
        if (current -> child[index] == NULL)
        {
            printf("Le préfixe n'existe pas dans le dictionnaire.\n");
            return liste_mots;
        }
        current = current->child[index];  
        prefix++;
    }
    char buffer[100];
    search_prefix(current, prefix_copy, buffer, 0,&liste_mots);
    triFusion(&liste_mots);

    return liste_mots;
}
