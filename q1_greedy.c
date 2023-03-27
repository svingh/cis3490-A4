#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int total = 0;

// Define Node structure for the binary search tree
typedef struct Node 
{
    char *word;
    int frequency;
    double probability;
    struct Node *left, *right;
} Node;

// Define a custom string duplication function
char *my_strdup(const char *s) 
{
    size_t len = strlen(s) + 1;
    char *new_str = (char *)malloc(len);
    if (new_str) 
    {
        memcpy(new_str, s, len);
    }
    return new_str;
}

// Define WordFrequency structure to store the frequency of words

typedef struct WordFrequency 
{
    char *word;
    int frequency;
} WordFrequency;

// Compare function for qsort to sort WordFrequency array in descending order of frequency

int compareFrequency(const void *a, const void *b) 
{
    WordFrequency *word_a = (WordFrequency *)a;
    WordFrequency *word_b = (WordFrequency *)b;

    return word_b->frequency - word_a->frequency;
}

// Function to create a new node for the BST
Node *createNodeGreedy(char *word, int frequency, double probability) 
{
    Node *newNode = (Node *)malloc(sizeof(Node));

    newNode->word = my_strdup(word);
    newNode->frequency = frequency;

    newNode->probability = probability;
    newNode->left = newNode->right = NULL;

    return newNode;
}

// Function to insert a word and its frequency into the BST
Node *insertGreedy(Node *root, char *word, int frequency) 
{
    if (!root) 
    {
        double probability = (double)frequency / total;
        return createNodeGreedy(word, frequency, probability);
    }

    int cmp = strcmp(word, root->word);
    if (cmp < 0) 
    {
        root->left = insertGreedy(root->left, word, frequency);
    }
    else if (cmp > 0) 
    {
        root->right = insertGreedy(root->right, word, frequency);
    } 
    else 
    {
        root->frequency++;
        root->probability = (double)root->frequency / total;
    }

    return root;
}

// Function to read the file and create a WordFrequency array

WordFrequency *createFrequencyList(char *filename, int *num_words) 
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) 
    {
        printf("Unable to open file\n");
        exit(0);
    }

    int c;
    char word[32];
    int char_count = 0;
    int word_count = 0;
    WordFrequency *word_list = NULL;
    int list_size = 0;

    while ((c = fgetc(fp)) != EOF) 
    {
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\0') 
        {
            if (char_count > 0) 
            {
                word[char_count] = '\0';
                total++;
                char_count = 0;

                int found = 0;
                for (int i = 0; i < word_count; i++) 
                {
                    if (strcmp(word_list[i].word, word) == 0) 
                    {
                        word_list[i].frequency++;
                        found = 1;
                        break;
                    }
                }

                if (!found) 
                {
                    if (word_count == list_size) 
                    {
                        list_size = list_size == 0 ? 8 : list_size * 2;
                        word_list = realloc(word_list, list_size * sizeof(WordFrequency));
                    }
                    word_list[word_count].word = my_strdup(word);
                    word_list[word_count].frequency = 1;
                    word_count++;
                }
            }
        } 
        else 
        {
            word[char_count] = c;
            char_count++;
        }
    }

    fclose(fp);

    *num_words = word_count;
    return word_list;
}


// Function to build a greedy BST using the sorted WordFrequency array
void buildGreedyTree(Node **root, WordFrequency *word_list, int num_words) 
{
    for (int i = 0; i < num_words; i++) 
    {
        *root = insertGreedy(*root, word_list[i].word, word_list[i].frequency);
    }
}

// Function to search for a word in the BST and print the comparison results
void searchGreedy(Node *root, char *key) 
{
    if (!root) 
    {
        printf("Not found.\n");
        return;
    }

    int cmp = strcmp(key, root->word);
    if (cmp == 0) 
    {
        printf("Found %s with (%.3f).\n", key, root->probability);
    } 
    else if (cmp < 0) 
    {
        printf("Compared with %s (%.3f), go left subtree.\n", root->word, root->probability);
        searchGreedy(root->left, key);
    } 
    else 
    {
        printf("Compared with %s (%.3f), go right subtree.\n", root->word, root->probability);
        searchGreedy(root->right, key);
    }
}


// Function to free the memory allocated to the BST
void freeTree(Node *root) 
{
    if (root) 
    {
        freeTree(root->left);
        freeTree(root->right);
        free(root->word);
        free(root);
    }
}

int main() {
    Node *root = NULL;

    // Read the file and create a WordFrequency array
    int num_words = 0;
    WordFrequency *word_list = createFrequencyList("data_A4_Q1.txt", &num_words);

    // Sort the WordFrequency array in descending order of frequency
    qsort(word_list, num_words, sizeof(WordFrequency), compareFrequency);

    // Build the greedy BST using the sorted WordFrequency array
    buildGreedyTree(&root, word_list, num_words);

    // Free memory allocated for the word list
    for (int i = 0; i < num_words; i++) 
    {
        free(word_list[i].word);
    }
    free(word_list);
    char input[50];
    while (1) 
    {
        printf("\nEnter a word to searchGreedy (or type 'exit' to quit): ");
        scanf("%s", input);
        if (strcmp(input, "exit") == 0) 
        {
            break;
        }
        searchGreedy(root, input);
    }

    // Free memory and exit
    freeTree(root);
    return 0;
}
