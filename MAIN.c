#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 2048  // Words (32 bits per word)
#define DATA_START 1024     // Address where data starts

// Define instruction struct based on your instruction formats (opcode, operands)
typedef struct Instruction {
    int opcode;
    int operands[/* number of operands */];
} Instruction;

Instruction memory[MEMORY_SIZE];

int main() {
    FILE *file = fopen("instructions.txt", "r");
    if (file == NULL) {
        printf("Error opening instruction file.\n");
        return 1;
    }

    int instruction_address = 0;
    while (fscanf(file, "%d", &memory[instruction_address].opcode) != EOF) {
        // Parse remaining operands based on instruction format
        for (int i = 0; i < 12 ; i++) {
            fscanf(file, "%d", &memory[instruction_address].operands[i]);
        }
        instruction_address++;
    }

    fclose(file);

    // Rest of the program can call assembly functions for execution
    // You'll pass the parsed instructions stored in memory to the assembly functions

    return 0;
}
