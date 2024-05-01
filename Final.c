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

char *int_to_binary_5bits(int num) {
    // Ensure num is within the range [0, 31]
    if (num < 0 || num > 31) {
        fprintf(stderr, "Number must be in the range [0, 31]\n");
        exit(1);
    }

    // Allocate memory for the binary string
    char *binary_str = malloc(6); // 5 bits + 1 for the null terminator
    if (binary_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Convert the integer to a binary string
    for (int i = 4; i >= 0; i--) {
        binary_str[4 - i] = (num & (1 << i)) ? '1' : '0';
    }

    // Add the null terminator
    binary_str[5] = '\0';

    return binary_str;
}

char *int_to_binary_28bits(int num) {
    // Allocate memory for the binary string
    char *binary = (char *)malloc(29 * sizeof(char)); // 28 bits + '\0'
    if (binary == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Ensure the number is within the supported range
    if (num < 0 || num >= (1 << 28)) {
        fprintf(stderr, "Number out of range\n");
        exit(1);
    }

    // Convert the integer to binary
    for (int i = 27; i >= 0; i--) {
        int bit = (num >> i) & 1; // Extract each bit
        binary[27 - i] = bit ? '1' : '0'; // Store '1' or '0'
    }
    binary[28] = '\0'; // Null-terminate the string

    return binary;
}

char *Type_opcode (const char opcode [])
{
    if (strcmp(opcode, "0000") == 0 || strcmp(opcode, "0001") == 0 ||
        strcmp(opcode, "0010") == 0 || strcmp(opcode, "0101") == 0 ||
        strcmp(opcode, "1000") == 0 || strcmp(opcode, "1001") == 0) {
        return "R";
    } else if (strcmp(opcode, "0011") == 0 || strcmp(opcode, "0100") == 0 ||
               strcmp(opcode, "0110") == 0 || strcmp(opcode, "1010") == 0 ||
               strcmp(opcode, "1011") == 0)  {
        return "I";
    } else if (strcmp(opcode, "0111") == 0) {
        return "J";
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


    for (int i = 7; i < 8; i++)
    {
        char *binary_opcode; // string for each instruction in binary
        char *type;
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
            printf("Token %d of instruction %d: %s\n",j+1, i + 1,Words_array[j]);
        }
            
            binary_opcode = opcode_to_binary(Words_array[0]);
            printf("Binary Opcode of the %d instruction: %s\n", i+1, binary_opcode);
            type = Type_opcode(binary_opcode);
            printf("This opcode have the %s-Format\n\n",type);


        if (type == "J")
        {
            char result[100]; // Make sure result has enough space to hold the concatenated string
            char *address = Words_array[1];
            int num = atoi(address);
            address = int_to_binary_28bits(num);
            strcpy(result,binary_opcode);
            strcat(result,address);
            printf("Binary Opcode of the %d instruction: %s\n", i+1, binary_opcode);
        }
        else{
            printf("\nNOT J");
        }

        // Free memory for tokens
        for (int j = 0; j < current_pos; j++) {
            free(Words_array[j]);
        }
    }


    return 0;
    
}