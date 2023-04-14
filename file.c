#include <time.h>
#include "file.h"
#include "hash.h"

void traiter_file()
{
	FILE *file_source;
	file_source = fopen("mots_courants.txt","r");

	FILE *file_destination;
	file_destination = fopen("dict_prediction.txt","w");

	HashTable hashTab;
	initializeHashTable(&hashTab);
	loadDictionaryFromFile(&hashTab,"mots_courants.txt");
	
	for(int i = 0; i < HASH_SIZE; i++)
	{
		int weight = 0;
		if (hashTab.Elements[i] != NULL)
		{
			Element *elem = hashTab.Elements[i];
			while (elem != NULL)
			{
					//if(strcmp(elem->word,word) == 0)
				weight ++;
				elem = elem->next;
			}
			fprintf(file_destination,"%s %d %d\n", hashTab.Elements[i]->word,weight, (int) time(NULL));
			//met la date de l'occurence en format UNIX (secondes depuis le 1er janvier 1970)

		}
	}

	fclose(file_destination);
	fclose(file_source);
}