#include "hash.h"
#include "trie.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

void menu(Trie *trie,HashTable *hashTab);
void saisi_predictive(Trie *trie,HashTable *hashTab);


int main()
{

    const char* dictionaryFileName = "dict_francais.txt";
    const char* courantsFileName = "mots_courants.txt";
    
    
    Trie trie;
    initialize_trie(&trie);
    create_trie(&trie, "mots_courants.txt");
    create_trie(&trie, "dict_francais.txt");
    // on part du principe que les mots dans mots courants sont dans le dictionnaire francais
    // donc comme on les ajoute deux fois, ils ont une pondération plus grande et seront donc plus prioritaires
	HashTable hashTab;
	initializeHashTable(&hashTab);
	loadDictionaryFromFile(&hashTab, dictionaryFileName);

    menu(&trie,&hashTab);
    return 0;
}

void print_prediction(Trie *trie, List liste_mots,char *word, char* suggestions[3])
{
    liste_mots = suggest_words(trie,word); //la fonction suggest words renvoie la liste chainé des mots dans leur poids croissants
    mirrorList(&liste_mots);
    Node *temp = liste_mots.head;

         //tableau qui va contenir les 3 mots de suggestions

    for (int i = 0; i < 3; i++)     //on remplit le tableau
    {
        if (temp != NULL)
        {
            char *mot = temp->mot;
            suggestions[i] = mot;
            temp = temp->next;
        }
        else
        {
            suggestions[i] = "/";
        }
    }
   
    for (int i = 0; i < 3; i++) 
    {
        printw("%s | ", suggestions[i]);
    }

   
     // Mise à jour de l'écran
    refresh();

}

void saisi_predictive(Trie *trie,HashTable *hashTab)
{

    List liste_mots;
    char debut[1000] = "";
    char word[100] = "";
    char character;
    // Initialisation de ncurses
    initscr();
    char *suggestions[3];
    printw("Appuyez sur la touche 'tab' pour compléter votre mot et pour le rajouter en nombre d'itérations, \n appuyez sur 'espace' pour continuer d'écrire et l'ajouter au dictionnaire s'il existe, ou sur la touche 'escap' pour quitter le programme\n\n");
    printw("Entrez un mot: \n\n");
    int i = 0;
    int continuer = 1;
    while  (continuer)
    {
        clear();
        // Affichage du message
        printw("Appuyez sur la touche 'tab' pour valider votre mot et pour le rajouter en nombre d'itérations, \nsur espace pour continuer d'écrire et l'ajouter au dictionnaire, ou sur la touche 'escap' pour quitter le programme\n\n");
        printw("Entrez un mot: \n\n");
        character = getchar();
        switch (character)
        {
            case 8: //backspace
                if (i>0)
                {
                    i--;
                    word[i] = '\0';
                }
                printw("%s%s\n\n",debut, word);
                if (i>0)
                    print_prediction(trie,liste_mots,word,suggestions);
                break;

            case 32: //space
                // on ajoute le mot sans la prediction et on l'affiche
                strcat(debut, word);
                strcat(debut, " ");
                if (checkExistenceWordInDictionary(hashTab,word))
                    add_word(trie,word);
                i = 0;
                word[i] = '\0';
                printw("%s%s\n\n",debut, word);
                suggestions[0] = "/";
                suggestions[1] = "/";
                suggestions[2] = "/";
                break;

            case 9: //tab
                //ajouter le mot et l'afficher
                if (suggestions[0] != "/")
                {
                    strcpy(word, suggestions[0]);
                    if (checkExistenceWordInDictionary(hashTab,word))
                        add_word(trie,word);
                    strcat(debut,word);
                    strcat(debut, " ");
                    i = 0;
                    word[i] = '\0';
                    
                }
                printw("%s%s\n\n",debut, word);
                suggestions[0] = "/";
                suggestions[1] = "/";
                suggestions[2] = "/";
                break;

            case 27: //escape
                continuer = 0;
                break;

            
            default:
                if (character <= 122 && character >= 97 || character == 39) //si c'est une lettre minuscule ou un apostrophe
                {
                    word[i] = character;
                    i++;
                    word[i] = '\0';
                    printw("%s%s\n\n",debut, word);
                    print_prediction(trie,liste_mots,word,suggestions);
                }
                else 
                {
                    // ne fais rien
                    printw("%s%s\n\n",debut, word);
                    print_prediction(trie,liste_mots,word,suggestions);
                }
                break;
        }

    refresh(); // Mise à jour de l'écran
    }

    endwin();
}

void menu(Trie *trie, HashTable *hashTab)
{
    int continuer = 1;
    int choix;
    while (continuer)
    {
        printf("Bienvenue dans le menu de l'application de saisie prédictive\n\n");
        printf("0. Quitter le programme\n");
        printf("1. Accéder à la saisie prédictive de base\n");
        printf("2. Accéder à la saisie prédictive à partir de la dernière sauvegarde\n");
        printf("3. Sauvegarder la saisie prédictive\n");
        scanf("%d", &choix);
        switch (choix)
        {
        case 0:
            continuer = 0;
            break;
        case 1:
            printf("Appuyez sur une lettre pour commencer\n\n");
            getchar();
            saisi_predictive(trie,hashTab);
            printf("\nVous avez quitté le programme\n\n");
            break;
        case 2:
            load_trie(trie);
            printf("Appuyez sur une lettre pour commencer\n\n");
            getchar();
            saisi_predictive(trie,hashTab);
            printf("\nVous avez quitté le programme\n\n");
            break;
        case 3:
            save_trie(trie);
            break;
        }
    }
}