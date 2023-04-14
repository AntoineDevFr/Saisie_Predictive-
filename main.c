#include "hash.h"
#include "trie.h"
#include "file.h"

void saisi_predictive(Trie *trie,HashTable *hashTab)
{

    char word[100];
    printf("Entrez un mot: \n");
    scanf("%s",word);

    suggest_words(trie,word);

    if(checkExistenceWordInDictionary(hashTab,word)) //si le mot entré par l'utilisateur est dans le dictionnaire Fr
    {
        add_word(trie,word);
        printf("on ajoute le mot\n"); 
    }

}

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


    while (1)
    {
        saisi_predictive(&trie,&hashTab);
    }
    
    return 0;
}
