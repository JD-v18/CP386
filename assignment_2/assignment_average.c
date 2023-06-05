/*
 -------------------------------------
 Assignment 2
 File: assignment_average.c
 -------------------------------------
 Author:  Jidaan Dahiya / Andrew Graco
 ID:      211738270 / 210422740
 Email:   dahi8270@mylaurier.ca /  grec2740@mylaurier.ca
 Version  2023-06-05
 -------------------------------------
 */

 //Andrew
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define NUM_STUDENTS 10
#define NUM_ASSIGNMENTS 2
#define NUM_CHAPTERS 3

// Function to calculate the average grade for an assignment in a chapter
void calculateAverage(int assignmentID, int grades[NUM_STUDENTS][NUM_ASSIGNMENTS * NUM_CHAPTERS], int chapterID)
{
    int sum = 0;
    for (int i = 0; i < NUM_STUDENTS; i++)
    {
        sum += grades[i][assignmentID + (chapterID - 1) * NUM_ASSIGNMENTS];
    }
    float average = (float)sum / NUM_STUDENTS;
    printf("Assignment %d - Average = %.6f\n", (chapterID - 1) * NUM_ASSIGNMENTS + assignmentID + 1, average);
}

int main()
{
    // Read data from the file
    int grades[NUM_STUDENTS][NUM_ASSIGNMENTS * NUM_CHAPTERS];
    FILE *file = fopen("sample_in_grades.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    // Store grades in the array
    for (int i = 0; i < NUM_STUDENTS; i++)
    {
        for (int j = 0; j < NUM_ASSIGNMENTS * NUM_CHAPTERS; j++)
        {
            fscanf(file, "%d", &grades[i][j]);
        }
    }
    fclose(file);

    // Process each assignment
    for (int assignmentID = 0; assignmentID < NUM_ASSIGNMENTS; assignmentID++)
    {
        // Process each chapter
        for (int chapterID = 1; chapterID <= NUM_CHAPTERS; chapterID++)
        {
            // Fork a new process for each assignment
            pid_t TAProcess = fork();
            if (TAProcess == 0)
            {
                // Child process: Calculate and print average
                calculateAverage(assignmentID, grades, chapterID);
                exit(0);
            }
        }

        // Wait for all TA processes to finish
        for (int chapterID = 1; chapterID <= NUM_CHAPTERS; chapterID++)
        {
            wait(NULL);
        }
    }

    return 0;
}


