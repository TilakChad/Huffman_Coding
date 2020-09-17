#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"


void top_down_heapify(huffman_tree **arr, int a, int n)
{
    huffman_tree **p = arr - 1;
    int k = a + 1;
    while (2 * k <= n)
    {
        int current = 2 * k;
        if (2 * k != n)
        {
            // if (p[2 * k + 1] < p[2 * k])
            //     current = 2 * k + 1;
            if(p[2*k+1]->symbol.occurence < p[2*k]->symbol.occurence)
                current = 2*k+1;
        }
        if (p[k]->symbol.occurence> p[current]->symbol.occurence)
        {
            huffman_tree* temp = p[k];
            p[k] = p[current];
            p[current] = temp;
        }
        else
        {
            break;
        }
        
        k = current;
    }
}

void insert(huffman_tree ** arr,huffman_tree* k, int n)
{
    arr[n] = k;
    bottom_up_heapify(arr-1,n+1,n+1);
}

// bottom up heapify operation to reduce insertion time
void bottom_up_heapify(huffman_tree **arr, int k, int n)
{
    while(k>1 && (arr[k/2]->symbol.occurence > arr[k]->symbol.occurence))
    {
        huffman_tree* temp = arr[k/2];
        arr[k/2] = arr[k];
        arr[k] = temp;
        k = k / 2;
    }
}


void delete_min(huffman_tree **arr, int size)
{
    huffman_tree** p = arr-1;
    // Exchanging the last and the first element
    // NO need to exchange .. just assign the first to last and discard the last one.
    p[1] = p[size];
    top_down_heapify(arr,0,size-1);
}