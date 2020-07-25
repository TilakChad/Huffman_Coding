#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct symbols
{
    char character[20];
    int occurence;
} symbols;

typedef struct binary
{
    symbols symbol;
    struct binary*left;
    struct binary*right;
} huffman_tree;

void top_down_heapify(huffman_tree **, int, int);
void insert(huffman_tree**, huffman_tree*, int);
void bottom_up_heapify(huffman_tree**, int , int);
void delete_min(huffman_tree**, int);


void tree_insert(huffman_tree*,symbols);
void delete(huffman_tree*,symbols);

#endif