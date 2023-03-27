// CIS*3490 The Analysis and Design of Algorithms
// March 26, 2023
// Assignment 4

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100

// Function to read in the data from the file
void readData(int m[][MAXSIZE], int w[][MAXSIZE], int *n)
{
    FILE *fp;
    char filename[MAXSIZE];
    int i, j;

    printf("Please enter the file name: ");
    scanf("%s", filename);
    fp = fopen(filename, "r");

    // Read in n
    fscanf(fp, "%d", n);

    // Read in the preference list of men
    for (i = 0; i < *n; i++)
    {
        for (j = 0; j < *n; j++)
        {
            fscanf(fp, "%d", &m[i][j]);
            m[i][j]--; // Subtract 1 for 0-based indexing
        }
    }

    // Read in the preference list of women
    for (i = 0; i < *n; i++)
    {
        for (j = 0; j < *n; j++)
        {
            fscanf(fp, "%d", &w[i][j]);
            w[i][j]--; // Subtract 1 for 0-based indexing
        }
    }
    fclose(fp);
}
// Function to implement the Gale-Shapley algorithm
void GaleShapley(int m[][MAXSIZE], int w[][MAXSIZE], int n, int mpair[], int wpartner[])
{
    int freeMen = n;
    int i, men, women, men_index;

    // Initialize the men pair array
    for (i = 0; i < n; i++)
    {
        mpair[i] = -1;
    }

    // While there are free men
    while (freeMen > 0)
    {
        // Find a free man
        for (men = 0; men < n; men++)
        {
            if (mpair[men] == -1)
            {
                break;
            }
        }

        // For each of his preference list
        for (men_index = 0; men_index < n && mpair[men] == -1; men_index++)
        {
            women = m[men][men_index];

            // If the woman is free
            if (wpartner[women] == -1)
            {
                mpair[men] = women;
                wpartner[women] = men;
                freeMen--;
            }
            // If the woman is not free
            else
            {
                // Check if the woman prefers the current man to her current partner
                int currentPartnerRank = -1;
                int newPartnerRank = -1;
                for (i = 0; i < n; i++)
                {
                    if (w[women][i] == men)
                    {
                        newPartnerRank = i;
                    }
                    if (w[women][i] == wpartner[women])
                    {
                        currentPartnerRank = i;
                    }
                }
                // Replace the current partner with the current man
                if (newPartnerRank < currentPartnerRank)
                {
                    int previousPartner = wpartner[women];
                    mpair[previousPartner] = -1;
                    mpair[men] = women;
                    wpartner[women] = men;
                    // freeMen should not be decremented here
                }
            }
        }
    }
}

// Function to print the result
void printResult(int mpair[], int n)
{
    int i, j;

    printf("The result is:\n");
    for (i = 0; i < n; i++)
    {
        printf("Man %d is paired with woman %d\n", i + 1, mpair[i] + 1);
    }
    printf("\nMatrix representation:\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (mpair[i] == j)
            {
                printf("1 ");
            }
            else
            {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

int main()
{
    int n;
    int m[MAXSIZE][MAXSIZE];
    int w[MAXSIZE][MAXSIZE];
    int mpair[MAXSIZE];
    int wpartner[MAXSIZE];
    int i;
    // Initialize the women partner array
    for (i = 0; i < MAXSIZE; i++)
    {
        wpartner[i] = -1;
    }

    // Read in the data
    readData(m, w, &n);

    // Implement the Gale-Shapley algorithm
    GaleShapley(m, w, n, mpair, wpartner);

    // Print the result
    printResult(mpair, n);

    return 0;
}

