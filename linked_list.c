#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"


void initializeList(List *liste)
{
    liste->head = NULL;
}

void printList(List* liste)
{
    Node *temp = liste->head;
    printf("[");
    while(temp != NULL)
    {
        printf("%s (%d) ",temp->mot,temp->weight);
        temp = temp->next;
    }
    printf("]");
}

void addNodeHead(List* liste, int weight, char* mot)
{
    Node *new = malloc(sizeof(*new));
    new->next = liste->head;
    strcpy(new->mot, mot);
    new->weight = weight;
    liste->head = new;
}

void deleteFirstNode(List* liste)
{
    if(liste->head == NULL)
    {
        printf("La liste est vide\n");
        return;
    }
    Node* temp = liste->head;
    liste->head = liste->head->next;
    free(temp);    
}

void mirrorList(List* liste)
{
    List* result = malloc(sizeof(*result));
    result->head = NULL;
    Node* courant = liste->head;
    while(courant != NULL)
    {
        addNodeHead(result, courant->weight, courant->mot);
        courant = courant->next;
    }
    liste->head = result->head;
}

//concatene et met tout dans la liste1, supprime la liste 2
void concatenationList(List* liste1, List* liste2)
{
    mirrorList(liste1);
    while(liste2->head != NULL)
    {
        addNodeHead(liste1,(liste2->head)->weight, (liste2->head)->mot);
        deleteFirstNode(liste2);
    }
    free(liste2);
    mirrorList(liste1);   
}


List mergeSortList(List liste1, List liste2)
{
    List listResult;
    listResult.head = NULL;
    while (liste1.head != NULL && liste2.head != NULL)
    {
        if (liste1.head->weight < liste2.head->weight) //modifier comparaison si nécessaire
        {
            addNodeHead(&listResult, liste1.head->weight, liste1.head->mot);
            liste1.head = liste1.head->next;
        }
        else
        {
            addNodeHead(&listResult, liste2.head->weight, liste2.head->mot);
            liste2.head = liste2.head->next;
        }
    }
    while (liste1.head != NULL || liste2.head != NULL)
    {
        if (liste1.head != NULL)
        {
            addNodeHead(&listResult, liste1.head->weight, liste1.head->mot);
            liste1.head = liste1.head->next;
        }
        else
        {
            addNodeHead(&listResult, liste2.head->weight, liste2.head->mot);
            liste2.head = liste2.head->next;
        }
    }   
    mirrorList(&listResult);
    return listResult;
}

void triFusion(List* liste)
{
    List list1,list2;
    list1.head = NULL;
    list2.head = NULL;
    int i = 0;
    if (liste->head == NULL || (liste->head)->next == NULL) return;
    // si 1 seul ou 0 élément, on ne fait rien
    else
    {
        while (liste->head != NULL)
        {
            if (i==0)
            {
                addNodeHead(&list1, liste->head->weight, liste->head->mot);
                liste->head = liste->head->next;
                i++;
            }
            else
            {
                addNodeHead(&list2, liste->head->weight, liste->head->mot);
                liste->head = liste->head->next;
                i = 0;
            }
        }
        triFusion(&list1);
	    triFusion(&list2);
	    *liste = mergeSortList(list1, list2);
	}
}

void print_n_last(List *liste,int n)
{
   mirrorList(liste);
   int i = 0;
   Node *temp = liste->head;
    printf("[");
    while(temp != NULL && i < n)
    {
        printf("%s (%d) ",temp->mot,temp->weight);
        temp = temp->next;
        i ++;
    }
    printf("]");
}