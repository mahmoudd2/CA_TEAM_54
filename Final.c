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

int num_instructions = 0;

int Memory_Array[2048];
// char *Memory_Array[2048]; 

int registerFile [32];
// int Num_Inst = sizeof(Memory_Array)/sizeof(int);
int *decodedArray;

// Define the pipeline stages
// Instruction IF, ID, EX, MEM, WB;
int clk = 1;
int pc = 0;
// const int R0 = 0;

// int R1  =0;
// int R2  =0;
// int R3  =0;
// int R4  =0;
// int R5  =0;
// int R6  =0;
// int R7  =0;
// int R8  =0;
// int R9  =0;
// int R10 =0;
// int R11 =0;
// int R12 =0;
// int R13 =0;
// int R14 =0;
// int R15 =0;
// int R16 =0;
// int R17 =0;
// int R18 =0;
// int R19 =0;
// int R20 =0;
// int R21 =0;
// int R22 =0;
// int R23 =0;
// int R24 =0;
// int R25 =0;
// int R26 =0;
// int R27 =0;
// int R28 =0;
// int R29 =0;
// int R30 =0;
// int R31 =0;

char *int_to_binary(int num, int num_bits, const char *type) {
    // Allocate memory for the binary string
    char *binary = (char *)malloc((num_bits + 1) * sizeof(char)); // +1 for the null terminator
    if (binary == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    if (strcmp(type, "IMM") == 0) { // 2's complement handling
        if (num < 0) { // If negative, convert to 2's complement
            num += (1 << num_bits); // num = -5 + (10000) = 11
        }
        for (int i = num_bits - 1; i >= 0; i--) {
            if (num & (1 << i)) {
                binary[num_bits - 1 - i] = '1'; // Set the bit to '1' if it's set in num
            } else {
                binary[num_bits - 1 - i] = '0'; // Set the bit to '0' if it's not set in num
            }
        }
    } else { // Normal positive number handling
        for (int i = num_bits - 1; i >= 0; i--) { //num = 5   num_bits = 13 bit  i = 12 // 
            if (num & (1 << i)) {
                binary[num_bits - 1 - i] = '1'; // Set the bit to '1' if it's set in num
            } else {
                binary[num_bits - 1 - i] = '0'; // Set the bit to '0' if it's not set in num
            }
        }
    }        
    binary[num_bits] = '\0'; // Null-terminate the string

    return binary;
}

void writeback(int *decodedArray,int result)
{
    int First_Reg;
    if (decodedArray != NULL) {
        First_Reg = decodedArray[1];
        if (First_Reg >= 0 && First_Reg < 32) {
            registerFile[First_Reg] = result;
        } else {
            printf("Invalid register index: %d\n", First_Reg);
        }
    } else {
        printf("Writeback is not doing anything rn\n");
    }
    registerFile[First_Reg] = result;
    // printf("operandddd: %d\n", WB.operands[0]);
    printf("WB: Writeback operation\n");

}

void memory(int *decodedArray,int result)
{
    if (decodedArray != NULL){
        if (decodedArray[0] == 10)
        {
            registerFile[decodedArray[1]] = Memory_Array[result];
        }
        else if (decodedArray[0] == 11)
        {
            Memory_Array[result] = decodedArray[1];
        }
    }
    else
    {
        printf("memory is not doing anything rn\n");
    }
    printf("MEM: Memory operation\n");
    
}


int execute(int *decodedArray)
{
    int result = 0;
    // int resultpc = 0;
    if (decodedArray != NULL){
        switch (decodedArray[0])
        {
        case 0:
            result = decodedArray[2] + decodedArray[3];
            break;
        case 1:
            result = decodedArray[2] - decodedArray[3];
            break;
        case 2:
            result = decodedArray[2] * decodedArray[3];
            break;
        case 3:
            result = decodedArray[5];
            break;
        case 4:
            if(decodedArray[1] == decodedArray[2])
            {
                pc = pc + 1 + decodedArray[5];
            }
            break;
        case 5:
            result = decodedArray[2] & decodedArray[3];
            break;
        case 6:
            result = decodedArray[2] ^ decodedArray[5]; // ^ deh 3lamet el XOR fe C
            break;
        case 7:
            int temp = (pc & 0b11110000000000000000000000000000) >> 28;
            pc = temp | decodedArray[6];
            // printf("PC: %d\n",pc);
            break;
        case 8:
            result = decodedArray[2] << decodedArray[4];
            break;
        case 9:
            result = decodedArray[2] >> decodedArray[4];
            break;
        case 10:
        case 11:
            result = decodedArray[2] + decodedArray[5];
            break;
        default:
            break;
        }
        // printf("result: %d\n", result);
        if (decodedArray[0] != 7 && decodedArray[0] != 4 &&  decodedArray[1]!= 0)
        {
            // writeback(result);
        } 
    }
    else
    {
        printf("execute is not doing anything rn\n\n");
    } 

    // printf("operand 1: %d\n",decodedArray[3]);
    // printf("pc in excute %i\n",pc);
    printf("EX: Executing instruction\n");
    return result;
}

int * decode(int instruction)
{
    int *decodedArray = malloc(7 * sizeof(int));
    if (decodedArray == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
    }
    int opcode = 0;  // bits31:28
    int R1 = 0;      // bits27:23
    int R2 = 0;      // bit22:18
    int R3 = 0;      // bits17:13
    unsigned int Shamt = 0;   // bits12:0
    signed int Imm = 0;     // bits17:0
    int Address = 0; // bits27:0
    int sign_bit = 0;
    // int R1Value;
    // int R2Value;
    // int R3Value;
    
    //0b00100000100010110010000000000000
    // 256 128 64 32 16 8 4 2 1
    unsigned int opcodebitmask = instruction & 0b11110000000000000000000000000000;
    opcode = opcodebitmask >> 28;
    decodedArray[0] = opcode;
    // printf("opcode: %d\n", opcode);
    
    unsigned int R1bitmask = instruction & 0b00001111100000000000000000000000;
    R1 = R1bitmask >> 23;
    decodedArray[1] = R1;
    // R1Value = registerFile[R1];
    //printf("R1 Value: %d\n",R1Value);
        
    unsigned int R2bitmask = instruction & 0b00000000011111000000000000000000;
    R2 = R2bitmask >> 18;
    decodedArray[2] = R2;
    // R2Value = registerFile[R2];
    
    unsigned int R3bitmask = instruction & 0b00000000000000111110000000000000;
    R3 = R3bitmask >> 13;
    decodedArray[3] = R3;
    // R3Value = registerFile[R3];

    Shamt = instruction & 0b00000000000000000001111111111111;
    decodedArray[4] = Shamt;  

    Imm  = instruction & 0b00000000000000111111111111111111;
    sign_bit = (instruction & 0b100000000000000000) >> 17;
    if (sign_bit == 1)
    {
        Imm = Imm | 0b11111111111111000000000000000000;
    }
    // printf("Imm: %d\n",Imm);


    decodedArray[5] = Imm;    
    Address = instruction & 0b00001111111111111111111111111111;
    decodedArray[6] = Address;

    // printf("[");
    // for (int i = 0; i < sizeof(decodedArray); i++) {
    //     printf("%d", decodedArray[i]);
    //     if (i < sizeof(decodedArray) - 1) {
    //         printf(", ");
    //     }
    // }
    // printf("]\n");

    printf("ID: Decoding instruction %d\n", instruction);
    return decodedArray;



    // switch(opcode)
    // {
    //     case 0:
    //     case 1:
    //     case 2:
    //     case 5:
    //         // R-format mn gher SHAMT
    //         Imm     = 0;
    //         Shamt   = 0;
    //         Address = 0;
    //         ID.operands[0] = R1;
    //         ID.operands[1] = registerFile[R2];
    //         ID.operands[2] = registerFile[R3];

    //         break;
    //     case 8:
    //     case 9:
    //         // R-format with SHAMT
    //         R3      = 0;
    //         Imm     = 0;
    //         Address = 0;
    //         ID.operands[0] = R1;
    //         ID.operands[1] = registerFile[R2];
    //         ID.operands[2] = registerFile[R3];
    //         break;

    //     case 4:
    //         // I-format
    //         R3      = 0;
    //         Shamt   = 0;
    //         Address = 0;
    //         ID.operands[0] = R1;
    //         ID.operands[1] = registerFile[R2];
    //         ID.operands[2] = Imm;
    //         break;
    //     case 3:
    //     case 6:
    //     case 10:
    //         // I-format
    //         R3      = 0;
    //         Shamt   = 0;
    //         Address = 0;
    //         ID.operands[0] = R1;
    //         ID.operands[1] = registerFile[R2];
    //         ID.operands[2] = Imm;
    //         break;
    //     case 11:
    //         // I-format
    //         R3      = 0;
    //         Shamt   = 0;
    //         Address = 0;
    //         ID.operands[0] = registerFile[R1];
    //         ID.operands[1] = registerFile[R2];
    //         ID.operands[2] = Imm;
    //         break;
    //     case 7:
    //         // J-format
    //         R1    = 0;
    //         R2    = 0;
    //         R3    = 0;
    //         Imm   = 0;
    //         Shamt = 0;
    //         ID.operands[0] = Address;
    //         break;
    //     default:
    //         break;    

    // }
    // execute(opcode);

    // printf("PC %i\n",pc);
    // printf("Instruction %i\n",pc + 1);
    // printf("opcode = %i\n",opcode);
    // printf("First Reg = %i\n",R1);
    // printf("Second Reg = %i\n",R2);
    // printf("Third reg = %i\n",R3);
    // printf("Shift Amount = %i\n",Shamt);
    // printf("Immediate Value = %i\n",Imm);
    // printf("Address = %i\n",Address);
    // printf("value[First Reg] = %i\n",registerFile[R1]);
    // printf("value[Second Reg] = %i\n",registerFile[R2]);
    // printf("value[Third Reg] = %i\n",registerFile[R3]);
    // printf("---------- \n");

    // if (opcode == 11)
    // {    
    //     printf("[");
    //     for (int i = 0; i < 32; i++) {
    //         printf("%d", registerFile[i]);
    //         if (i < 32 - 1) {
    //             printf(", ");
    //         }
    //     }
    //     printf("]\n");

    // }
    // ID = IF;
    // excute(opcode,R1,R2,R3,Shamt,Imm,Address);
}

int fetch()
{
    if(pc < num_instructions){
        int instruction = Memory_Array[pc];
        pc++;
        printf("IF: Fetching instruction %d at pc = %d\n", instruction, pc);
        return instruction;
    }
    else
    {
        printf("IF: End of program reached.\n");
        return -1; // End of program
    }
    
}



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

// (1 << num_bits) deh bt shift left bel number of bits ya3ni lw el bits be 4 fa ha shift left hena 0001 be 4 fa htb2a 10000 el hya
// equal 16 fa msln lw el num = -5 fa mfrod el binary representation bt3ha be equal 1011 el hya bardo equal lw 3mlna el -5 + 16 = 11
// w gebna el binary representation bta3 el 11 el hwa = 1011  


char *Type_opcode (const char opcode [])
{
    if (strcmp(opcode, "ADD") == 0 || strcmp(opcode, "SUB") == 0 ||
        strcmp(opcode, "MUL") == 0 || strcmp(opcode, "AND") == 0 ||
        strcmp(opcode, "LSL") == 0 || strcmp(opcode, "LSR") == 0) {
        return "R";
    } else if (
        strcmp(opcode, "MOVI") == 0 || strcmp(opcode, "JEQ") == 0 ||
        strcmp(opcode, "XORI") == 0 || strcmp(opcode, "MOVR") == 0 ||
        strcmp(opcode, "MOVM") == 0)  {
        return "I";
    } else if (strcmp(opcode, "JMP") == 0) {
        return "J";
    }else {
        return "Unkown";
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

char *concatenate_I_Format(const char *R1, const char *R2, const char *IMM, const char* binary_opcode)
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
    strcat(concatenated,IMM);
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
    char *instructions[1024];

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

    printf("Parsed Instructions:%d\n",num_instructions);
    // for (int i = 0; i < num_instructions; i++) {
    //     //printf("%s\n", instructions[i]);
    // }

    // Initialize Pipeline stages
    // memset(&IF, 0, sizeof(Instruction));
    // memset(&ID, 0, sizeof(Instruction));
    // memset(&EX, 0, sizeof(Instruction));
    // memset(&MEM, 0, sizeof(Instruction));
    // memset(&WB, 0, sizeof(Instruction));

    for (int i = 0; i < num_instructions; i++)
    {
        int *decodedArray = malloc(7 * sizeof(int));
        if (decodedArray == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
}
        char *Final_inst; // string for each instruction in binary
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

            // for(int j = 0; j < current_pos; j++)
            // {
            //     // printf("Token %d of instruction %d: %s\n",j+1, i + 1,Words_array[j]);
            // }      EHNA MSH MEHTAGEN EL FOR LOOP DEH
            
        Final_inst = opcode_to_binary(Words_array[0]);
        // printf("Binary Opcode of the %d instruction: %s\n", i+1, Final_inst);
        type = Type_opcode(Words_array[0]);
        // printf("This opcode have the %s-Format\n\n",type);


        if (type != NULL && strcmp(type, "J") == 0) {
            char *address = Words_array[1];
            int num = atoi(address); //10
            address = int_to_binary(num,28,"Address"); 

            // Call the function to concatenate
            Final_inst = concatenate_J_Format(address, Final_inst);

            // printf("Final Binary Code Of The %d Instruction: %s\n\n", i + 1, Final_inst);
        }
        else if(type != NULL && strcmp(type, "R") == 0)
        {
            char *First_reg = Words_array[1]+1; // +1 3shan n skip el char R w ngeb el rkam el b3do 
            char *Sec_reg = Words_array[2]+1;
            int num1 = atoi(First_reg);
            int num2 = atoi(Sec_reg);
            First_reg = int_to_binary(num1,5,"Reg");
            Sec_reg = int_to_binary(num2,5,"Reg");
            char *Third_reg;
            char *SHAMT;
            if (strcmp (Words_array[0], "LSL") == 0 || strcmp (Words_array[0], "LSR") == 0)
            {
                Third_reg = "00000";
                SHAMT = Words_array[3];
                int num3 = atoi(SHAMT);
                SHAMT = int_to_binary(num3,13,"SHAMT");            

            }
            else
            {
                Third_reg = Words_array[3]+1;
                unsigned int num3 = atoi(Third_reg);
                Third_reg = int_to_binary(num3,5,"Reg");
                SHAMT = "0000000000000";

            }
            Final_inst = concatenate_R_Format(First_reg, Sec_reg, Third_reg, SHAMT, Final_inst);
            // printf("Final Binary Code Of The %d Instruction: %s\n\n", i + 1, Final_inst);

        }
        else if (type != NULL && strcmp(type, "I") == 0)
        {
            char *First_reg = Words_array[1]+1;
            int num1 = atoi(First_reg);
            First_reg = int_to_binary(num1,5,"Reg");
            char *Sec_reg;
            if (strcmp (Words_array[0], "MOVI") == 0)
            {
                Sec_reg = "00000";
            }
            else
            {
                Sec_reg = Words_array[1]+1;
                int num2 = atoi(Sec_reg);
                Sec_reg = int_to_binary(num2,5,"Reg");
            }
            char *IMM = Words_array[2];
            int num3 = atoi(IMM);
            IMM = int_to_binary(num3,18,"IMM");
            Final_inst = concatenate_I_Format(First_reg, Sec_reg, IMM, Final_inst);

            // printf("Final Binary Code Of The %d Instruction: %s\n\n", i + 1, Final_inst);
        } // 11000000000010000000000011000000
        // Memory_Array[i] = strtol(Final_inst, NULL, 2); // Convert binary string to integer directly
        // Convert binary string to an Instruction object and assign it to Memory_Array[i]
        Memory_Array[i] = strtol(Final_inst, NULL, 2);

        // printf("Instruction %d %d\n", i + 1, Memory_Array[i]);
        for (int j = 0; j < current_pos; j++) {
            free(Words_array[j]);
        }
    }

    int IF_NUM = num_instructions * 2;
    int WB_NUM = 0;
    while (IF_NUM != WB_NUM )
    {
        int result;
        int INST;
        printf("Clock Cycle: %d\n\n", clk);
        
        if (clk % 2 == 1)
        {
            writeback(decodedArray,result);
            WB_NUM++;
            printf("WB COUNT: %d\n",WB_NUM);

        }
        memory(decodedArray,result);
        result = execute(decodedArray);
        decodedArray = decode(INST);
        
        if (clk % 2 == 1)
        {
            INST = fetch();
            IF_NUM--;
            printf("IF COUNT: %d\n",IF_NUM);
            printf("INST: %d\n\n",INST);

        }
        clk++;
    }
    // printf("[");

    //     for (int i = 0; i < sizeof(array); i++) {
    //         printf("%d", array[i]);
    //         if (i < sizeof(array) - 1) {
    //             printf(", ");
    //         }
    //     }
    //     printf("]\n");
    // }
    // // printf("[");
    // for (int i = 0; i < sizeof(array); i++) {
    //     printf("%d", array[i]);
    //     if (i < sizeof(array) - 1) {
    //         printf(", ");
    //     }
    // }
    // printf("]\n");

    // Start simulation
    // int clock_cycles = 0;
    // while (WB.opcode == 0 || clock_cycles < 7 + ((12 - 1) * 2)) {
    //     decode();
    //     fetch();
    //     clock_cycles++;
    // }

    // printf("Total clock cycles: %d\n", clock_cycles);

    return 0;
    
}