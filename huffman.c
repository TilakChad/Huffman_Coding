#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "huffman.h"

struct code 
{
    char data[10];
};

FILE* fp;
char **store;
int ind(char);
void recurse_free(huffman_tree*);
void huffman_coding(huffman_tree*,struct code ,int );

int main(int argc, char **argv)
{

    // Initializes a 30 by 30 array with null character to store the huffman code in character form
    store = malloc(sizeof(char*) * 30);
    for(int i = 0; i < 30; ++i)
    {
        store[i] = malloc(sizeof(char) * 30);
    }


    for(int i = 0; i < 30; ++i)
    {
        for(int j = 0; j < 30; ++j)
        {
            store[i][j] = '\0';
        }
    }

    // File opened for output to store respective huffman code for each character and coded string
    fp = fopen("coded.dat","w");
    if(!fp)
    {
        printf("Error in opening output file ... ");
        exit(1);
    }

    // Count the total occurence of each symbol
    symbols alpha[30];
    for (int i = 0; i < 30; ++i)
    {
        alpha[i].occurence = 0;
        for (int j = 0; j < 20; ++j)
            alpha[i].character[j] = '\0';
    }

    char str[50];
    fgets(str, 49, stdin);
    for (int i = 0; i < strlen(str); ++i)
    {
        int k = ind(str[i]);
        if (k == -1)
            break;
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
    int size = 0;
    huffman_tree *priority_queue[30];
    for (int i = 0; i < 30; ++i)
        priority_queue[i] = NULL;
    int i = 0;
    while (i < 30)
    {
        if (alpha[i].occurence != 0)
        {
            huffman_tree *k = malloc(sizeof(huffman_tree));
            for(int i = 0; i < 20; ++i)
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
            for(int i = 0; i < 20; ++i)
                c->symbol.character[i] = '\0';
            c->symbol.occurence = a->symbol.occurence + b->symbol.occurence;
            huffman_tree *left, *right;
            if (a->symbol.occurence < b->symbol.occurence)
            {
                char temp[20] = {'\0'}; // To overcome the modification of first string by strcat()
                strcpy(temp,a->symbol.character);
                strcpy(c->symbol.character, strcat(temp, b->symbol.character));
                left = a;
                right = b;
            }
            else
            {
                char temp[20] = {'\0'};
                strcpy(temp,b->symbol.character);
                strcpy(c->symbol.character, strcat(temp, a->symbol.character));
                left = b;
                right = a;
            }
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

    struct code a;
    huffman_coding(*priority_queue,a,0);
    recurse_free(*priority_queue);
    for(int i = 0;i < 30; ++i)
    {
        if(alpha[i].occurence!=0)
        {
            printf("%c -> %s.\n",alpha[i].character[0],store[i]);
        }
    }
    for(int i = 0; i < strlen(str); ++i)
    {
        if(ind(str[i])!=-1)
            fprintf(fp,"%s",store[ind(str[i])]);
    }
    return 0;
}

// Feels like supporting only these characters
int ind(char a)
{
    if (a >= 97 && a <= 122)
    {
        return a - 'a';
    }
    else if (a == '_')
    {
        return 27;
    }
    else if (a == ' ')
    {
        return 28;
    }
    else
    {
        return -1;
    }
}

// This function recursively clean up the dynamically allocated memory from the heap data structure
void recurse_free(huffman_tree* queue)
{
    if(queue==NULL)
        return ;
    recurse_free(queue->left);
    recurse_free(queue->right);
    free(queue);
}

void huffman_coding(huffman_tree* queue,struct code a,int k)
{
    // I guess we need to do postorder traversal on obtained huffman tree.
    if(queue==NULL)
        return ;
    
    struct code b = a;
    b.data[k] = '0';
    huffman_coding(queue->left,b,k+1);
    b.data[k] = '1';
    huffman_coding(queue->right,b,k+1);

    if(queue->left == NULL && queue->right == NULL)
    {
        a.data[k] = '\0';
        // printf("Code for %s is %s.\n",queue->symbol.character,a.data);
        fprintf(fp,"%s -> %s\n",queue->symbol.character,a.data);
        strcpy(store[ind(queue->symbol.character[0])],a.data);
    }
}