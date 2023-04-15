#include "hash.h"
#include "trie.h"
#include "file.h"
#include "linked_list.h"
#include <ncurses.h>

void saisi_predictive(Trie *trie,HashTable *hashTab);


int main()
{

    const char* dictionaryFileName = "dict_francais.txt";
    const char* courantsFileName = "mots_courants.txt";


    //traiter_file();
    
    
    Trie trie;
    initialize_trie(&trie);
    create_trie(&trie, "mots_courants.txt");

    
	HashTable hashTab;
	initializeHashTable(&hashTab);
	loadDictionaryFromFile(&hashTab, dictionaryFileName);


    
    saisi_predictive(&trie,&hashTab);

    
    return 0;
}

void saisi_predictive(Trie *trie,HashTable *hashTab)
{

    List liste_mots;
    char word[100];

    // Initialisation de ncurses
    initscr();

    // Affichage du message "Entrez un mot:"
    printw("Entrez un mot: ");
    getstr(word);

    liste_mots = suggest_words(trie,word); //la fonction suggest words renvoie la liste chainé des mots dans leur poids croissants
    
    mirrorList(&liste_mots);
    Node *temp = liste_mots.head;

    char *suggestions[3]; //tableau qui va contenir les 3 mots de suggestions

    for (int i = 0; i < 3; i++)     //on remplit le tableau
    {
        char *mot = temp->mot;
        suggestions[i] = mot;
        temp = temp->next;
    }
   
    int x = 0; //on affiche 
    for (int i = 0; i < 3; i++) {
        mvprintw(2, x, "%s", suggestions[i]);
        x += strlen(suggestions[i]) + 2; // Ajouter une marge de 2 espaces entre chaque suggestion
    }

    if(checkExistenceWordInDictionary(hashTab,word)) //si le mot entré par l'utilisateur est dans le dictionnaire Fr
    {
        add_word(trie,word);
    }
     // Mise à jour de l'écran
    refresh();

    // Attente de l'utilisateur
    getch();

    // Fermeture de ncurses
    endwin();

}

