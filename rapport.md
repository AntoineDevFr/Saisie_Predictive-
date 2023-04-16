# Rapport: Projet Noé et Antoine Algo_Prog 2

## Notre approche
Notre objectif était de se rapprocher le plus possible de ce qu'il existe actuellement en terme de saisi prédictice.

Nous avons décider d'utiliser un trie ou arbre de prefixe pour notre projet de saisie intuitive

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

La structure de trie permet de réaliser un algorithme de saisie prédictive. Chaque nœud de cet arbre contient 27 fils, 26 pour chaque lettre de l'alphabet et le 27ème pour gérer les caractères spéciaux en les remplaçant par le symbole #.

Le principe de fonctionnement de l'algorithme est simple : à partir de la racine de l'arbre, on parcourt l'arbre en suivant les lettres qui composent le mot, une par une. Si un nœud a un poids de 0, cela signifie que ce n'est pas la fin d'un mot. En revanche, si le poids du nœud est supérieur à 0, cela indique que c'est la fin d'un mot et que ce mot est d'une certaine importance.

En résumé, grâce à cette structure de trie, l'algorithme de saisie prédictive peut suggérer des mots pertinents à l'utilisateur en temps réel, en se basant sur les lettres qu'il saisit. 

# create_node
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

Cette fonction permet de créer un nouveau nœud dans un arbre de trie. Elle prend en entrée un caractère "letter" qui va être stocké dans le nœud créé.

La première étape de la fonction est de créer un nouveau nœud en allouant de l'espace mémoire avec la fonction "malloc". La taille de l'espace mémoire alloué est celle d'une structure de type "Trienode", qui est la structure utilisée pour représenter un nœud de l'arbre de trie.

Le champ "weight" est utilisé pour stocker la valeur de poids du nœud, qui permet de savoir si le nœud représente la fin d'un mot et d'indiquer son importance. La fonction renvoie ensuite le nouveau nœud créé, qui pourra être ajouté à l'arbre de trie.

# add_word

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

La fonction "add_word" parcourt le mot caractère par caractère et crée des nœuds pour chaque lettre du mot. Si un nœud pour cette lettre n'existe pas encore, il est créé et placé dans le tableau "child" du nœud courant. Le champ "weight" du dernier nœud créé est alors incrémenté pour indiquer que ce nœud correspond à la fin d'un mot et donner son poids.

# create_trie

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

Cette fonction crée un arbre de trie en lisant des mots à partir d'un fichier texte. La fonction prend en entrée un pointeur sur la structure "Trie" à remplir et le nom du fichier texte.

La fonction ouvre le fichier en mode lecture et vérifie qu'il a bien été ouvert. Si le fichier a été ouvert avec succès, la fonction lit chaque mot du fichier à l'aide de la fonction "fscanf" et ajoute chaque mot à l'arbre de trie en utilisant la fonction "add_word".

Une fois que tous les mots ont été lus et ajoutés à l'arbre, la fonction ferme le fichier. Si le fichier n'a pas pu être ouvert, un message d'erreur est affiché.

# suggest_words

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

Cette fonction est le coeur du programme. La fonction "suggest_words" prend en entrée un préfixe et un arbre de trie contenant des mots. Elle recherche l'ensemble des mots du dictionnaire qui commencent par ce préfixe en parcourant l'arbre de trie à partir de la racine jusqu'au dernier nœud correspondant à la dernière lettre du préfixe. Ensuite, elle utilise une fonction de recherche pour trouver tous les mots qui suivent ce préfixe et les stocke dans une liste chainée, qu'elle trie avant de la renvoyer.

# Sauvegarde
    void serialize_trie(Trienode *root, FILE *file);
    Trienode* deserialize_trie(FILE *file);
    void save_trie(Trie *trie);
    void load_trie(Trie *trie); 

Les 4 fonctions suivantes permettent de sauvegarder l'arbre de trie dans un fichier binaire.

# main.C

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

La fonction print_prediction prend en entrée un Trie trie, une liste chainée de mots liste_mots, un mot word et un tableau de pointeurs vers des chaines de caractères suggestions. Elle utilise la fonction suggest_words pour récupérer la liste des mots possibles en fonction du préfixe word et met à jour le tableau suggestions avec les 3 premiers mots de la liste. Si la liste contient moins de 3 mots, elle met "/" à la place des mots manquants. Enfin, elle affiche les 3 mots de suggestions séparés par "|".

    void saisi_predictive(Trie *trie,HashTable *hashTab)
    {

    List liste_mots;
    char debut[1000] = "";
    char word[100] = "";
    char character;
    // Initialisation de ncurses
    initscr();
    char *suggestions[3];
    printw("Appuyez sur la touche 'tab' pour valider votre mot et pour le rajouter en nombre d'itérations, \nsur espace pour continuer d'écrire et l'ajouter au dictionnaire, ou sur la touche 'escap' pour quitter le programme\n\n");
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

                word[i] = character;
                i++;
                word[i] = '\0';
                printw("%s%s\n\n",debut, word);
                print_prediction(trie,liste_mots,word,suggestions);
                break;
        }

    refresh(); // Mise à jour de l'écran
    }

    endwin();
    }

La fonction saisi_predictive prend en entrée un pointeur vers un Trie et un pointeur vers une HashTable. Elle permet à l'utilisateur d'entrer des mots et propose des suggestions basées sur le contenu du Trie. Les touches spéciales sont traitées différemment : la touche 'tab' valide le mot et l'ajoute au Trie, la touche 'espace' ajoute le mot au dictionnaire sans suggestion et la touche 'escap' quitte le programme.

# Menu

La fonction menu permet de gérer ou non la sauvegarde de l'arbre de trie.27ème


# Notes et remarques

Nous avons voulu nous
- Les librairies hash.h et linked_list.h sont basés sur les algorithmes vu en cours
- Les mots et leurs poids s'ajoutent automatiquement 
- Nous avons rencontré des diffilcultées pour sauvegarder l'arbre de trie. Il est impossible de sauvegarder des pointeurs et donc des listes chainées. Il faut donc écrire dans le fichier binaire lettre par lettre dans l'arbre avec le poids associés.
- il est important de lancer le programme dans une fenêtre du terminal et pas dans une sous fenêtre de termianl dans vscode par exemple