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

char *int_to_binary(int num, int num_bits) {
    // Allocate memory for the binary string
    char *binary = (char *)malloc((num_bits + 1) * sizeof(char)); // +1 for the null terminator
    if (binary == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Ensure the number is within the supported range
    if (num < 0 || num >= (1 << num_bits)) { 
        fprintf(stderr, "Number out of range for %d bits\n", num_bits);
        exit(1);
    }

    // Convert the integer to binary
    for (int i = num_bits - 1; i >= 0; i--) {
        int bit = (num >> i) & 1; // Extract each bit
        binary[num_bits - 1 - i] = bit ? '1' : '0'; // Store '1' or '0'
    }        
    binary[num_bits] = '\0'; // Null-terminate the string

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

char *concatenate_J_Format(const char *address, const char *binary_opcode) {
    // Allocate memory for the concatenated string
    char *concatenated = malloc(33); // 32 + 1 el (+1) 3shan fe null terminator fel akher
    if (concatenated == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    // Copy the original binary_opcode to concatenated
    strcpy(concatenated, binary_opcode);
    
    // Concatenate the binary address to concatenated
    strcat(concatenated, address);
    
    // Free memory for the binary address
    // free(address);
    
    return concatenated;
}

char *concatenate_R_Format(const char *R1, const char *R2, const char*R3, const char *SHAMT, const char* binary_opcode)
{
    char *concatenated =malloc(33);
    if(concatenated == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    strcpy(concatenated,binary_opcode);

    strcat(concatenated,R1);
    strcat(concatenated,R2);
    strcat(concatenated,R3);
    strcat(concatenated,SHAMT);
    return concatenated;
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


    for (int i = 0; i < 13; i++)
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


        if (strcmp(type, "J") == 0) {
            char *address = Words_array[1];
            int num = atoi(address);
            address = int_to_binary(num,28); 

            // Call the function to concatenate
            binary_opcode = concatenate_J_Format(address, binary_opcode);

            printf("Final Binary Code Of The %d Instruction: %s\n\n", i + 1, binary_opcode);
        }
        else if(strcmp(type, "R") == 0)
        {
            char *First_reg = Words_array[1]+1; // +1 3shan n skip el char R w ngeb el rkam el b3do 
            char *Sec_reg = Words_array[2]+1;
            int num1 = atoi(First_reg);
            int num2 = atoi(Sec_reg);
            First_reg = int_to_binary(num1,5);
            Sec_reg = int_to_binary(num2,5);
            char *Third_reg;
            char *SHAMT;
            if (strcmp (Words_array[0], "LSL") == 0 || strcmp (Words_array[0], "LSR") == 0)
            {
                Third_reg = "00000";
                SHAMT = Words_array[3];
                int num3 = atoi(SHAMT);
                SHAMT = int_to_binary(num3,13);            

            }
            else
            {
                Third_reg = Words_array[3]+1;
                int num3 = atoi(Third_reg);
                Third_reg = int_to_binary(num3,5);
                SHAMT = "0000000000000";
            }
            binary_opcode = concatenate_R_Format(First_reg, Sec_reg, Third_reg, SHAMT, binary_opcode);

            printf("Final Binary Code Of The %d Instruction: %s\n\n", i + 1, binary_opcode);

        }
        // Free memory for tokens
        for (int j = 0; j < current_pos; j++) {
            free(Words_array[j]);
        }
    }


    return 0;
    
}