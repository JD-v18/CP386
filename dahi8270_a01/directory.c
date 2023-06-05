/*
 -------------------------------------
 Assignment 1
 File: directory.c
 -------------------------------------
 Author:  Jidaan Dahiya
 ID:      211738270
 Email:   dahi8270@mylaurier.ca
 Version  2023-05-22
 -------------------------------------
 */

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

void create_dir(char *name) {  // Creating a directory
    if (mkdir(name, 0777) == 0) {
        printf("Directory is Created Successfully.\n");
    }
    else {
        printf("Error: (mkdir) - Failed to create directory.\n");
    }
}

void remove_dir(char *name) {  // Removing a directory
    if (rmdir(name) == 0) {
        printf("Directory is removed Successfully.\n");
    }
    else {
        printf("Error: (rmdir) - Failed to remove directory.\n");
    }
}

void get_curr_dir() {  // Getting the current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current Working Directory is: %s\n", cwd);
    }
    else {
        printf("Error: (getcwd) - Failed to get current working directory.\n");
    }
}

void change_dir() {  // Changing the directory (to one level up from the current directory)
    if (chdir("..") >= 0) {
        printf("Directory Changed Successfully.\n");
    }
    else {
        printf("Error: (chdir) - Failed to change directory.\n");
    }
}

DIR* read_dir() {  // Reading a directory
    DIR *directory;
    char *name = ".."; //Parent Directory
    struct dirent *dircontents;
    if ((directory = opendir(name)) == NULL) {
        printf("Error (opendir) - Failed to open directory.\n");
        return NULL;
    }
    else {
        while ((dircontents = readdir(directory)) != NULL) {
            printf("%s\n", dircontents->d_name);
        }
        return directory;
    }
}

void close_dir(DIR *currdir) {  // Closing a directory
    if (currdir != NULL) {
        if ((closedir(currdir)) == 0) {
            printf("Directory Closed Successfully.\n");
        }
        else {
            printf("Error: No Directory open.\n");
        }
    }
}

int main() {
    char ch, name[100];
    DIR *currdir = NULL;
    do {
        printf("Select the option(s) appropriately by entering the number:\n"
        "Enter 1 for creating a directory\n"
        "Enter 2 for removing directory\n"
        "Enter 3 for printing working directory\n"
        "Enter 4 for changing directory one level up\n"
        "Enter 5 for reading the contents of directory\n"
        "Enter 6 for closing the current directory\n"
        "Enter q to exit the program\n");
        scanf(" %c", &ch);
        switch (ch) {
            case '1':
                printf("Enter the Directory name you want to create:\n");
                scanf("%s", name);
                create_dir(name);
                break;
            case '2':
                printf("Enter the Directory name you want to remove:\n");
                scanf("%s", name);
                remove_dir(name);
                break;
            case '3':
                get_curr_dir();
                break;
            case '4':
                change_dir();
                break;
            case '5':
                currdir = read_dir();
                break;
            case '6':
                close_dir(currdir);
                currdir = NULL;
                break;
        }
    }
    while (ch != 'q');
    return 0;
}

