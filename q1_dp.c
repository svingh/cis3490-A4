// CIS*3490 The Analysis and Design of Algorithms
// March 25 ,2023
// Assignment 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BST.h"

double C[2000][2000];
int R[2000][2000];
double P[2000];
char *KEYS[2000];
int in = 1;
int total = 0;

// Reads the file and placing all of the word in a temporary tree, while counting
// the frequency of each word
Node *readFile(char *filename, Node *root)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Unable to open file\n");
        exit(0);
    }

    int c;
    char word[32];
    int charCount = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\0')
        {
            if (charCount > 0)
            { // get rid of extra whitespace
                word[charCount] = '\0';
                // look through tree
                root = addNode(root, word);
                charCount = 0;
                total++;
            }
        }
        else
        {
            word[charCount] = c;
            charCount++;
        }
    }
    fclose(fp);
    return root;
}

// sets the cost matrix and the root matrix
double setC()
{
    int n = in;
    int i, j;
    for (i = 1; i <= n + 1; i++)
    {
        for (j = 1; j <= n; j++)
        {
            C[i][i] = 0;
            R[i][i] = 0;
        }
    }

    for (i = 1; i <= n; i++)
    {
        C[i][i] = P[i];
        R[i][i] = i;
    }

    int l;
    for (l = 1; l < n; l++)
    {
        for (i = 1; i <= n - l; i++)
        {
            int j = i + l;
            double minval = 100000;
            int k;
            int kmin = 0;
            for (k = i; k <= j; k++)
            {
                if ((C[i][k - 1] + C[k + 1][j]) < minval)
                {
                    minval = C[i][k - 1] + C[k + 1][j];
                    kmin = k;
                }
            }
            R[i][j] = kmin;
            double sum = P[i];
            for (int m = i + 1; m <= j; m++)
            {
                sum = sum + P[m];
            }
            C[i][j] = minval + sum;
        }
    }
    return C[1][n];
}

// populates the keys and probability arrays
void setProbs(Node *root)
{
    KEYS[in] = root->word;
    P[in] = (double)root->total / total;
    in++;
}

// creates the optimal tree using the root matrix
Node *createOBST(int i, int j)
{
    Node *newNode;
    if (i > j)
        newNode = NULL;
    else
    {
        // find the root
        newNode = (Node *)malloc(sizeof(Node));
        char *newWord = malloc(sizeof(char) * (strlen(KEYS[R[i][j]]) + 1));
        strcpy(newWord, KEYS[R[i][j]]);
        newNode->word = newWord;
        newNode->total = C[i][j];
        newNode->left = createOBST(i, R[i][j] - 1);  // left subtree
        newNode->right = createOBST(R[i][j] + 1, j); // right subtree
    }
    return newNode;
}

// searches the tree for a given string
Node *searchTree(Node *root, char *word)
{
    if (root == NULL)
    {
        printf("%s not found!\n", word);
    }
    else
    {
        int comp = strcmp(word, root->word);
        if (comp == 0)
        {
            // same: update the fequency of the word given
            printf("Compared with %s (%.2lf), found.\n", root->word, root->total);
        }
        else if (comp < 0)
        {
            // go left
            printf("Compared with %s (%.2lf), go left subtree.\n", root->word, root->total);
            root->left = searchTree(root->left, word);
        }
        else
        {
            // go right
            printf("Compared with %s (%.2lf), go right subtree.\n", root->word, root->total);
            root->right = searchTree(root->right, word);
        }
    }
    return root;
}

int main()
{
    Node *root = NULL;
    root = readFile("data_A4_Q1.txt", root);
    traverse(root, setProbs);
    in = in - 1;
    // traverse(root, printNode);

    setC();
    Node *obst = createOBST(1, in);
    // traverse(obst, printNode);

    char userInput[32];
    while (1) 
    {
        printf("\nEnter a word (exit to quit): ");
        fgets(userInput, 32, stdin);
        userInput[strlen(userInput) - 1] = '\0';
        if(strcmp(userInput,"exit") == 0)
        {
            break;
        }
        searchTree(obst, userInput);
    }

    deleteTree(root);
    deleteTree(obst);

    return 0;
}