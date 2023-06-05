/*
 -------------------------------------
 Assignment 1
 File: filecopy.c
 -------------------------------------
 Author:  Jidaan Dahiya
 ID:      211738270
 Email:   dahi8270@mylaurier.ca
 Version  2023-05-23
 -------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
    FILE *input, *output;
    int ch;

    // Sufficient Parameters Check
    if (argc != 3) {
        printf("Insufficient parameters passed.\n");
        return 1;
    }
    // Open input file
    if ((input = fopen(argv[1], "r")) == NULL) {
        printf("Error: (fopen) - Error opening input file.\n");
        return 1;
    }
    // Access output file
    if ((output = fopen(argv[2], "w")) == NULL) {
        printf("Error: (fopen) - Error opening output file.\n");
        fclose(input);
        return 1;
    }
    // Copy file contents
    while ((ch = fgetc(input)) != EOF) {
        fputc(ch, output);
    }
    //File Close
    fclose(input);
    fclose(output);
    printf("The contents of file '%s' has been successfully copied into '%s' file\n",argv[1], argv[2]);
    return 0;
}

