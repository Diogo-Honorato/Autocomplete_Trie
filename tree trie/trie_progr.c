#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE_CHAR 26

typedef struct Node
{
    struct Node *children[SIZE_CHAR];
    bool endword;
} Node;

Node *CreateNode()
{
    Node *newnode = (Node *)malloc(sizeof(Node));

    for (int i = 0; i < SIZE_CHAR; i++)
    {
        newnode->children[i] = NULL;
    }

    newnode->endword = false;

    return newnode;
}

int EmptyTrie(Node *root)
{
    for (int i = 0; i < SIZE_CHAR; i++)
    {
        if (root->children[i] != NULL)
        {
            return false;
        }
    }
    printf("\n\nEMPTY TREE !\n\n");
    exit(0);
    return true;
}

int InsertTrie(Node *root, char *word)
{
    Node *current = root;
    int index;
    int len = strlen(word);

    for (int i = 0; i < len; i++)
    {
        index = word[i] - 'a';

        if (current->children[index] == NULL)
        {
            current->children[index] = CreateNode();
        }

        current = current->children[index];
    }

    current->endword = true;

    return 1;
}

bool search(Node *root, char *word)
{
     Node *current = root;
    int index;
    int len = strlen(word);

    EmptyTrie(root);

    for (int i = 0; i < len; i++)
    {
        index = word[i] - 'a';

        if (current->children[index] == NULL)
        {
            return false;
        }

        current = current->children[index];
    }

    if (current->endword)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int SearchTrie(Node *root, char *word)
{
    if(search(root,word))
    {
        printf("\n\nWord was found: %s\n\n",word);
    }
    else
    {
        printf("\n\nWord NOT found: %s\n\n",word);
    }
}

int PrintTrie(Node *root, char *str, int index)
{
    Node *aux = root;

    if (aux->endword)
    {
        str[index] = '\0';
        printf("%s\n", str);
    }

    for (int i = 0; i < SIZE_CHAR; i++)
    {
        if (aux->children[i] != NULL)
        {
            str[index] = i + 'a';
            PrintTrie(aux->children[i], str, index + 1);
        }
    }

    return 1;
}

bool EmptyNode(Node *root)
{
    for (int i = 0; i < SIZE_CHAR; i++)
    {
        if (root->children[i] != NULL)
        {
            return false;
        }
    }

    return true;
}

Node *removed(Node *root, char *word, int init)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (init == strlen(word))
    {
        root->endword = false;

        if (EmptyNode(root))
        {
            free(root);
            root = NULL;
        }
        return root;
    }

    int index = word[init] - 'a';

    root->children[index] = removed(root->children[index], word, init + 1);

    if (EmptyNode(root) && !root->endword)
    {
        free(root);
        root = NULL;
    }

    return root;
}

int RemovedTrie(Node *root, char *word,int init)
{
    if(search(root,word))
    {
        removed(root,word,0);
        printf("\n\nWord remove: %s\n\n",word);
    }
    else
    {
        printf("\n\nWord NOT found to remove: %s\n\n",word);
    }
}

int InsertFile(Node *root)
{
    FILE *file1, *file2, *file3;
    int cont = 0;
    file1 = fopen("words_short.txt", "r");
    file2 = fopen("words_medium.txt", "r");
    file3 = fopen("words_long.txt", "r");

    char string[50];

    if (file1 == NULL && file2 == NULL && file3 == NULL)
    {
        printf("\n\nERROR OPENING FILE !\n\n");
        return 0;
    }

    while (fscanf(file1, "%s", string) != EOF)
    {
        InsertTrie(root, string);
        cont++;
    }

    while (fscanf(file2, "%s", string) != EOF)
    {
        InsertTrie(root, string);
        cont++;
    }

    while (fscanf(file3, "%s", string) != EOF)
    {
        InsertTrie(root, string);
        cont++;
    }

    fclose(file1);
    fclose(file2);
    fclose(file3);
    
    printf("\n\n%d words.\n\n",cont);
    return 1;
}

int autoprint(Node *root, char *str, char *prefix,int index)
{
    Node *aux = root;

    if (aux->endword)
    {
        str[index] = '\0';
        printf("%s\033[2m%s\033[0m\n", prefix, str);
    }

    for (int i = 0; i < SIZE_CHAR; i++)
    {
        if (aux->children[i] != NULL)
        {
            str[index] = i + 'a';
            autoprint(aux->children[i], str, prefix, index + 1);
        }
    }

    return 1;
}

int AutoComplete(Node *root, char *str, char *word,int init)
{
    Node *aux = root;
    int len = strlen(word);
    
    for(int i = 0; i < len; i++)
    {
    	int index = word[i] - 'a';
        
        aux = aux->children[index];
    }

    if(!aux)
    {   
       printf("\n\nWord not in the files !\n\n"); 
       exit(0); 
    }
    else
    {
        autoprint(aux,str,word,init);
    }
    
}

void FreeTrie(Node *root)
{
    if(!root)
    {
        return;
    }
    else
    {
        for(int i = 0; i < SIZE_CHAR; i++)
        {
            FreeTrie(root->children[i]);
        }
    }
    free(root);
}

int Trie(Node *root)
{
    char buffer[50];
    char word[50];
    int op = 1;

    InsertFile(root);

    while (op != 0)
    {
        printf("\n\nEnter un option:\n\n");
        printf("1-Search word\n");
        printf("2-Remove word\n");
        printf("3-Print tree\n");
        printf("4-Auto complete\n");
        printf("0-exit\n");
        scanf("%d", &op);
        system("clear");
        
        switch (op)
        {
        case 1:
            printf("\nEnter word to search:\n");
            fflush(stdin);
            scanf("%s",word);
            SearchTrie(root, word);
            break;
        case 2:
            printf("\nEnter word to remove:\n");
            fflush(stdin);
            scanf("%s",word);
            RemovedTrie(root, word, 0);
            break;
        case 3:
            printf("\nPrint tree:\n");
            PrintTrie(root, buffer, 0);
            break;
        case 4:
            printf("\nEnter a prefix to complete:\n");
            fflush(stdin);
            scanf("%s",word);
            AutoComplete(root,buffer,word,0);
            break;
        default:
            printf("\n\nClosed program\n\n");
            FreeTrie(root);
            break;
        }
    }
}

int main()
{
    Node *root = CreateNode();

    Trie(root);
}