#include "BST.h"

Node *createNode(char *word)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    char *newWord = malloc(sizeof(char) * (strlen(word) + 1));
    strcpy(newWord, word);
    newNode->word = newWord;
    newNode->total = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/* Adds a newly created node to the tree
   Input:
   Output:
*/
Node *addNode(Node *root, char *word)
{

    if (root == NULL)
    {
        return createNode(word);
    }
    else
    {
        int comp = strcmp(word, root->word);
        if (comp == 0)
        {
            // same: update the fequency of the word given
            root->total++;
            // printf("%s:%d\n", word, root->total);
        }
        else if (comp < 0)
        {
            // go left
            root->left = addNode(root->left, word);
        }
        else
        {
            // go right
            root->right = addNode(root->right, word);
        }
    }
    return root;
}

void traverse(Node *root, void (*action)(Node *node))
{
    if (root == NULL)
    {
        return;
    }
    if (root->left != NULL)
    {
        traverse(root->left, action);
    }
    action(root);
    if (root->right != NULL)
    {
        traverse(root->right, action);
    }
}

void deleteTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    if (root->left != NULL)
    {
        deleteTree(root->left);
    }
    if (root->right != NULL)
    {
        deleteTree(root->right);
    }
    free(root->word);
    free(root);
}

void printNode(Node *node)
{
    printf("%s:%lf\n", node->word, node->total);
}