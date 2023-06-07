/*
 -------------------------------------
 Assignment 2
 File: process_management.c
 -------------------------------------
 Author:  Jidaan Dahiya / Andrew Graco
 ID:      211738270 / 210422740
 Email:   dahi8270@mylaurier.ca / grec2740@mylaurier.ca
 Version  2023-06-05
 -------------------------------------
 */

// JD
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHMSIZE 1024
#define MAX_LINE_LENGTH 256

// utility to print the output
void writeOutput(char *command, char *output)
{
    FILE *fp;
    fp = fopen("output.txt", "a");

    fprintf(fp, "The output of: %s : is\n", command);
    fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);

    fclose(fp);
}

int main()
{
    int shmid;
    char *shm;

    // Open shared memory
    if ((shmid = shm_open("/my_shm", O_CREAT | O_RDWR, 0666)) == -1)
    {
        perror("shm_open");
        exit(1);
    } 
    // Map shared memory
    if ((shm = mmap(NULL, SHMSIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    // Truncate shared memory
    if (ftruncate(shmid, SHMSIZE) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    pid_t pid;
    pid = fork();

    if (pid == 0) {
        // Child process retrieving input
        int fd = open("sample_in_process.txt", O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            exit(1);
        }

        int len;
        char buf[SHMSIZE];
        while ((len = read(fd, buf, SHMSIZE - 1)) > 0) {
            buf[len] = '\0';
            strcat(shm, buf);
        }
        close(fd);
        exit(0);
    } else if (pid > 0) {
        // Parent process
        wait(NULL);

        char *command = strtok(shm, "\n");
        while (command != NULL) {
            pid_t pid = fork();
            if (pid == 0) {
                // Child process
                int pipefd[2];
                if (pipe(pipefd) == -1) {
                    perror("pipe");
                    exit(1);
                }

                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);

                // Executing shell commands
                char *args[] = {"sh", "-c", command, NULL};
                execvp(args[0], args);
                exit(0);
            } else if (pid > 0) {
                // Parent process
                wait(NULL);

                // write to output file
                int pipefd[2];
                if (pipe(pipefd) == -1) {
                    perror("pipe");
                    exit(1);
                }

                close(pipefd[1]);
                char buf[SHMSIZE];
                int len;
                while ((len = read(pipefd[0], buf, SHMSIZE - 1)) > 0) {
                    buf[len] = '\0';
                    writeOutput(command, buf);
                }
                close(pipefd[0]);
            }

            // Get next command
            command = strtok(NULL, "\n");
        }
    }

    return 0;
}

