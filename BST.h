// CIS*3490 The Analysis and Design of Algorithms
// March 25 ,2023
// Assignment 4


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct treeNode {
  char * word;
  double total;
  struct treeNode * left;
  struct treeNode * right;
} Node;

// creates a node struct for the word given
Node * createNode(char * word);

// adds a node to the given tree, based on the word given
Node * addNode(Node * root, char * word);

// traverses the tree and preforms an action given by action
void traverse(Node * root, void (*action)(Node * node));

// deletes a tree from the bottom up
void deleteTree(Node *root);

// prints a nodes word and probabililty
void printNode(Node * node);