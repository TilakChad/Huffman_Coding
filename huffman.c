#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "huffman.h"

struct code
{
    char data[CHAR_LENGTH];
};

FILE *fp;
char **store;
int ind(char);
void recurse_free(huffman_tree *);
void huffman_coding(huffman_tree *, struct code, int);
int cfree, cmalloc;

int main(int argc, char **argv)
{

    // Initializes a 30 by 30 array with null character to store the huffman code in character form
    store = malloc(sizeof(char *) * CHAR_TYPES);
    cmalloc++;
    for (int i = 0; i < CHAR_TYPES; ++i)
    {
        store[i] = malloc(sizeof(char) * CHAR_LENGTH);
        cmalloc++;
    }

    for (int i = 0; i < CHAR_TYPES; ++i)
    {
        for (int j = 0; j < CHAR_LENGTH; ++j)
        {
            store[i][j] = '\0';
        }
    }

    // File opened for output to store respective huffman code for each character and coded string
    fp = fopen("dict.dat", "w");
    if (!fp)
    {
        printf("Error in opening output file ... ");
        exit(1);
    }

    // Count the total occurence of each symbol
    symbols alpha[CHAR_TYPES];
    for (int i = 0; i < CHAR_TYPES; ++i)
    {
        alpha[i].occurence = 0;
        for (int j = 0; j < CHAR_TYPES; ++j)
            alpha[i].character[j] = '\0';
    }

    // Takes input that need to be coded.
    // 50 is string length here.. Need to be modified by dynamic array

    // This one for encoding of input string.


    // char str[50];
    // fgets(str, 49, stdin);

    // To read from file either input can be redirected or this approach.
    FILE* input_file = fopen(argv[1],"r");
    if(!input_file)
    {
        fprintf(stderr,"Error in opening input file....%s.\nExiting ........\n",argv[1]);
        return -1;
    }
    char *str;
    str = malloc(sizeof(char) * 1024*1024);
    int size_counter = 0;
    char read_char;
    while((read_char = fgetc(input_file))!=EOF)
    {
        if(size_counter>=MAX_TXTSIZE)
            break;
        str[size_counter++] = read_char;
    }
    str[size_counter] = '\0';
    fclose(input_file);
    // printf("Total character read were %d.\n and is %s.",size_counter,str);
    for (int i = 0; i < strlen(str); ++i)
    {
        int k = ind(str[i]);
        if (k == -1)
        {
            // break;
            printf("Invalid character is : %d.",str[i]);
            // skip;
        }
        else
        {
            if (alpha[k].occurence == 0)
            {
                alpha[k].character[0] = str[i];
            }
            alpha[k].occurence++;
        }
    }
    // for (int i = 0; i < 30; ++i)
    // {
    //     printf("Occurence of %s is : %d.\n", alpha[i].character, alpha[i].occurence);
    // }

    // Created huffman tree
    // Huffman code
    int size = 0;
    huffman_tree *priority_queue[CHAR_TYPES];
    for (int i = 0; i < CHAR_TYPES; ++i)
        priority_queue[i] = NULL;
    int i = 0;
    while (i < CHAR_TYPES)
    {
        if (alpha[i].occurence != 0)
        {
            huffman_tree *k = malloc(sizeof(huffman_tree));
            cmalloc++;
            for (int i = 0; i < CHAR_TYPES; ++i)
                k->symbol.character[i] = '\0';
            k->left = NULL;
            k->right = NULL;
            k->symbol = alpha[i];
            insert(priority_queue, k, size++);
        }
        ++i;
    }

    // for(int i = 0; i < a; ++i)
    // {
    //     printf("(%c %d)   ",priority_queue[0]->symbol.character[0],priority_queue[0]->symbol.occurence);
    //     delete_min(priority_queue,size--);
    // }
    while (size > 1)
    {
        huffman_tree *a = priority_queue[0];
        delete_min(priority_queue, size--);
        huffman_tree *b = priority_queue[0];
        delete_min(priority_queue, size--);

        // Let's combine two huffman_nodes
        huffman_tree *c;
        {
            c = malloc(sizeof(huffman_tree));
            cmalloc++;
            for (int i = 0; i < CHAR_TYPES; ++i)
                c->symbol.character[i] = '\0';
            c->symbol.occurence = a->symbol.occurence + b->symbol.occurence;
            huffman_tree *left, *right;
            char*temp;
            temp = malloc(sizeof(char)* CHAR_TYPES);
            for(int i = 0; i < CHAR_TYPES; ++i)
                temp[i] = '\0';
            if (a->symbol.occurence < b->symbol.occurence)
            {
                // char temp[20] = {'\0'}; // Error malicious code // To overcome the modification of first string by strcat()
                strcpy(temp, a->symbol.character);
                strcpy(c->symbol.character, strcat(temp, b->symbol.character));
                left = a;
                right = b;
            }
            else
            {
                strcpy(temp, b->symbol.character);
                strcpy(c->symbol.character, strcat(temp, a->symbol.character));
                left = b;
                right = a;
            }
            free(temp);
            c->left = left;
            c->right = right;
            insert(priority_queue, c, size);
            size++;
        }
    }
    for (int i = 0; i < size; ++i)
    {
        printf("(%s %d)   ", priority_queue[i]->symbol.character, priority_queue[i]->symbol.occurence);
    }
    printf("Rached again here...");
    struct code a;
    huffman_coding(*priority_queue, a, 0);
    recurse_free(*priority_queue);
    for (int i = 0; i < CHAR_TYPES; ++i)
    {
        if (alpha[i].occurence != 0)
        {
            printf("%c -> %s.\n", alpha[i].character[0], store[i]);
        }
    }
    fclose(fp);
    fp = fopen("code.dat", "w");
    if (!fp)
    {
        fprintf(stdout, "Error in writing code....");
    }

    for (int i = 0; i < strlen(str); ++i)
    {
        if (ind(str[i])!= -1)
            fprintf(fp, "%s", store[ind(str[i])]);
    }

    for (int i = 0; i < CHAR_TYPES; ++i)
    {
        free(store[i]);
        cfree++;
    }
    free(store);
    cfree++;
    free(str);
    fclose(fp);
    printf("\nTotal free and total malloc are : %d %d.\nTheir difference is : %d.\n", cfree, cmalloc, cmalloc - cfree);
    return 0;
}

// Feels like supporting only these characters
int ind(char a)
{
    if (a >= 97 && a <= 122)
        return a - 'a';
    if (a == '_')
        return 27;
    if (a == ' ')
        return 28;
    if (a >= 'A' && a<='Z')
        return a-'A'+29;
    if (a == '\n')
        return 55;
    // Additional characters 
    if(a>='0' && a<='9')
        return a-'0'+56;

    switch (a)
    {
        case '!' : return 66;
        case '@' : return 67;
        case '#' : return 68;
        case '$' : return 69;
        case '%' : return 70;
        case '^' : return 71;
        case '*' : return 72;
        case '(' : return 73;
        case ')' : return 74;
        case '-' : return 75;
        case '_' : return 76;
        case '+' : return 77;
        case '=' : return 78;
        case '{' : return 79;
        case '}' : return 80;
        case '[' : return 81;
        case ']' : return 82;
        case '\\' : return 83;
        case '|' : return 84;
        case ':' : return 85;
        case ';' : return 86;
        case '"' : return 87;
        case '\'' : return 88;
        case ',' : return 89;
        case '<' : return 90;
        case '>' : return 91;
        case '.' : return 92;
        case '?' : return 93;
        case '/' : return 94;
        case '&' : return 95;
        default : break;
    }
    return -1;
}

// This function recursively clean up the dynamically allocated memory from the heap data structure
void recurse_free(huffman_tree *queue)
{
    if (queue == NULL)
        return;
    recurse_free(queue->left);
    recurse_free(queue->right);
    free(queue);
    cfree++;
}

void huffman_coding(huffman_tree *queue, struct code a, int k)
{
    // I guess we need to do postorder traversal on obtained huffman tree.
    if (queue == NULL)
        return;

    struct code b = a;
    b.data[k] = '0';
    huffman_coding(queue->left, b, k + 1);
    b.data[k] = '1';
    huffman_coding(queue->right, b, k + 1);

    if (queue->left == NULL && queue->right == NULL)
    {
        a.data[k] = '\0';
        // printf("Code for %s is %s.\n",queue->symbol.character,a.data);
        fprintf(fp, "%s -> %s\n", queue->symbol.character, a.data);
        strcpy(store[ind(queue->symbol.character[0])], a.data);
    }
}