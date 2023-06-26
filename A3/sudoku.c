/*
 -------------------------------------
 Assignment 3
 File: sudoku.c
 -------------------------------------
 Author:  Jidaan Dahiya / Andrew Graco
 ID:      211738270 / 210422740
 Email:   dahi8270@mylaurier.ca / grec2740@mylaurier.ca
 Version  2023-06-25
 -------------------------------------
 */

// Andrew

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Sudoku puzzle solution
int sudoku[SIZE][SIZE];

// Structure for passing data to threads
typedef struct
{
    int startRow;
    int startCol;
} ThreadArgs;

// Thread function to check that each column contains the digits 1 through 9
void *check_column(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int check[SIZE] = {0};
    for (int row = args->startRow; row < SIZE; row++)
    {
        int num = sudoku[row][args->startCol];
        if (num < 1 || num > SIZE || check[num - 1])
        {
            pthread_exit((void *)0);
        }
        check[num - 1] = 1;
    }
    pthread_exit((void *)1);
}

// Thread function to check that each row contains the digits 1 through 9
void *check_row(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int check[SIZE] = {0};
    for (int col = args->startCol; col < SIZE; col++)
    {
        int num = sudoku[args->startRow][col];
        if (num < 1 || num > SIZE || check[num - 1])
        {
            pthread_exit((void *)0);
        }
        check[num - 1] = 1;
    }
    pthread_exit((void *)1);
}

// Thread function to check that each 3x3 sub-grid contains the digits 1 to 9
void *check_subgrid(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int check[SIZE] = {0};
    for (int row = args->startRow; row < args->startRow + 3; row++)
    {
        for (int col = args->startCol; col < args->startCol + 3; col++)
        {
            int num = sudoku[row][col];
            if (num < 1 || num > SIZE || check[num - 1])
            {
                pthread_exit((void *)0);
            }
            check[num - 1] = 1;
        }
    }
    pthread_exit((void *)1);
}

int main()
{
    // Read Sudoku puzzle solution from file
    FILE *file = fopen("sample_in_sudoku.txt", "r");
    if (file == NULL)
    {
        printf("Failed to open the input file.\n");
        return 1;
    }

    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            if (fscanf(file, "%d", &sudoku[row][col]) != 1)
            {
                printf("Invalid input file format.\n");
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);

    // Create threads to check each column in Sudoku
    pthread_t column_threads[SIZE];
    ThreadArgs column_args[SIZE];
    for (int col = 0; col < SIZE; col++)
    {
        column_args[col].startRow = 0;
        column_args[col].startCol = col;
        pthread_create(&column_threads[col], NULL, check_column, &column_args[col]);
    }

    // Create threads to check each row in Sudoku
    pthread_t row_threads[SIZE];
    ThreadArgs row_args[SIZE];
    for (int row = 0; row < SIZE; row++)
    {
        row_args[row].startRow = row;
        row_args[row].startCol = 0;
        pthread_create(&row_threads[row], NULL, check_row, &row_args[row]);
    }

    // Create threads to check each sub-grid in Sudoku
    pthread_t subgrid_threads[SIZE];
    ThreadArgs subgrid_args[SIZE];
    int subgridIndex = 0;
    for (int startRow = 0; startRow < SIZE; startRow += 3)
    {
        for (int startCol = 0; startCol < SIZE; startCol += 3)
        {
            subgrid_args[subgridIndex].startRow = startRow;
            subgrid_args[subgridIndex].startCol = startCol;
            pthread_create(&subgrid_threads[subgridIndex], NULL, check_subgrid, &subgrid_args[subgridIndex]);
            subgridIndex++;
        }
    }

    // Wait for all threads to finish
    int column_results[SIZE];
    int row_results[SIZE];
    int subgrid_results[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        pthread_join(column_threads[i], (void **)&column_results[i]);
        pthread_join(row_threads[i], (void **)&row_results[i]);
        pthread_join(subgrid_threads[i], (void **)&subgrid_results[i]);
    }

    // Check if Sudoku puzzle solution is valid
    int valid = 1;
    for (int i = 0; i < SIZE; i++)
    {
        if (!column_results[i] || !row_results[i] || !subgrid_results[i])
        {
            valid = 0;
            break;
        }
    }

    // Output the Sudoku puzzle solution and its validity
    printf("Solution is:\n");
    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            printf("%d ", sudoku[row][col]);
        }
        printf("\n");
    }

    if (valid)
    {
        printf("Sudoku puzzle is valid.\n");
    }
    else
    {
        printf("Sudoku puzzle is invalid.\n");
    }

    return 0;
}
