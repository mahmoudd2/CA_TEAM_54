#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #define Memory 2048 // Size of the Full Memory 
// #define Data_Start 1024 // Start of the data

// typedef struct Instruction {
//     int opcode;
//     int operands[1];
// } Instruction;

int Memory_Array[2048];

char *opcode_to_binary(const char opcode []) {
    if (strcmp(opcode, "ADD") == 0) {
        return "0000";
    } else if (strcmp(opcode, "SUB") == 0) {
        return "0001";
    } else if (strcmp(opcode, "MUL") == 0) {
        return "0010";
    } else if (strcmp(opcode, "MOVI") == 0) {
        return "0011";
    } else if (strcmp(opcode, "JEQ") == 0) {
        return "0100";
    } else if (strcmp(opcode, "AND") == 0) {
        return "0101";
    } else if (strcmp(opcode, "XORI") == 0) {
        return "0110";
    } else if (strcmp(opcode, "JMP") == 0) {
        return "0111";
    } else if (strcmp(opcode, "LSL") == 0) {
        return "1000";
    } else if (strcmp(opcode, "LSR") == 0) {
        return "1001";
    } else if (strcmp(opcode, "MOVR") == 0) {
        return "1010";
    } else if (strcmp(opcode, "MOVM") == 0) {
        return "1011";
    }else {
        return "NO OPCODE!"; // Default case
    }
}


int main() 
{
    // Open the assembly file for reading
    FILE *file = fopen("CA.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    // Array to store instructions as strings
    char *instructions[13];
    int num_instructions = 0;

    // Read each line from the file
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        // Trim leading and trailing whitespace
        char *trimmed_line = line;
        while (isspace(*trimmed_line)) {
            trimmed_line++;
        }
        char *end = trimmed_line + strlen(trimmed_line) - 1;
        while (end > trimmed_line && isspace(*end)) {
            *end-- = '\0';
        }

        // Check if the line is empty or contains only whitespace
        if (*trimmed_line == '\0') {
            continue;  // Skip empty lines
        }

        // Dynamically allocate memory for the instruction string
        instructions[num_instructions] = malloc(strlen(trimmed_line) + 1);
        if (instructions[num_instructions] == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            exit(1);
        }


        // Copy the instruction into the array
        strcpy(instructions[num_instructions], trimmed_line);

        num_instructions++;
    }

    //printf("Parsed Instructions:\n");
    for (int i = 0; i < num_instructions; i++) {
        //printf("%s\n", instructions[i]);
    }


    for (int i = 0; i < 1; i++)
    {
        char inst_binary[32]; // string for each instruction in binary
        int inst_length = strlen(instructions[i]);
        char *Words_array[inst_length]; // array to store tokens
        int current_pos = 0;

        // Tokenize the instruction
        char *token = strtok(instructions[i], " ");
        while (token != NULL) {
            // Allocate memory for the token
            Words_array[current_pos] = malloc(strlen(token) + 1);
            if (Words_array[current_pos] == NULL) {
                fprintf(stderr, "Error allocating memory\n");
                exit(1);
            }

            // Copy the token into the array
            strcpy(Words_array[current_pos], token);
            
            // Move to the next position for the next token
            current_pos++;

            // Get the next token
            token = strtok(NULL, " ");
        }

        // Print the first token
        for(int j = 0; j < current_pos; j++)
        {
            printf("%d token of instruction %d: %s\n",j, i + 1,Words_array[j]);
        }

        // Free memory for tokens
        for (int j = 0; j < current_pos; j++) {
            free(Words_array[j]);
        }
    }


    return 0;
}