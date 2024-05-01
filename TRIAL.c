#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INSTRUCTIONS 1000
#define MAX_LENGTH 100

typedef struct {
    char opcode[MAX_LENGTH];
    char *operands; 
} Instruction;

int main() {
    // // Open the assembly file for reading
    // FILE *file = fopen("CA.txt", "r");
    // if (file == NULL) {
    //     fprintf(stderr, "Error opening file\n");
    //     exit(1);
    // }

    // // Array to store instructions
    // Instruction instructions[MAX_INSTRUCTIONS];
    // int num_instructions = 0;

    // // Read each line from the file
    // char line[MAX_LENGTH];
    // while (fgets(line, sizeof(line), file)) {
    //     // Trim leading and trailing whitespace
    //     char *trimmed_line = line;
    //     while (isspace(*trimmed_line)) {
    //         trimmed_line++;
    //     }
    //     char *end = trimmed_line + strlen(trimmed_line) - 1;
    //     while (end > trimmed_line && isspace(*end)) {
    //         *end-- = '\0';
    //     }

    //     // Check if the line is empty or contains only whitespace
    //     if (*trimmed_line == '\0') {
    //         continue;  // Skip empty lines
    //     }

    //     // Tokenize the line to extract opcode and operands
    //     char *token = strtok(trimmed_line, " ");
    //     if (token == NULL) {
    //         fprintf(stderr, "Error parsing instruction\n");
    //         exit(1);
    //     }
    //     strcpy(instructions[num_instructions].opcode, token);
        
    //     // Check if there are operands
    //     token = strtok(NULL, "\n");
    //     if (token == NULL || strlen(token) == 0) {
    //         fprintf(stderr, "Error: Missing operands\n");
    //         exit(1);
    //     }
        
    //     // Dynamically allocate memory for operands
    //     instructions[num_instructions].operands = malloc(strlen(token) + 1);
    //     if (instructions[num_instructions].operands == NULL) {
    //         fprintf(stderr, "Error allocating memory\n");
    //         exit(1);
    //     }
    //     strcpy(instructions[num_instructions].operands, token);

    //     num_instructions++;
    // }

    // // Close the file
    // fclose(file);

    // // Print the parsed instructions
    // printf("Parsed Instructions:\n");
    // for (int i = 0; i < num_instructions; i++) {
    //     printf("%s %s\n", instructions[i].opcode, instructions[i].operands);
    //     // Don't forget to free dynamically allocated memory
    //     free(instructions[i].operands);
    // }
    char string[] = "hello \n";
    printf("%s",string);

    return 0;
}