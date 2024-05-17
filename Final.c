#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #define Memory 2048 // Size of the Full Memory 
// #define Data_Start 1024 // Start of the data
#define PIPELINE_DEPTH 5 // Number of pipeline stages

typedef struct {
    int Fetch_Inst;
    int dest_reg;    // Destination register for write-back
    int result;
    int opcode;      // Result from the execute stage
} PipelineStage;

PipelineStage pipeline[PIPELINE_DEPTH];

// Initialize the pipeline stages
void init_pipeline() {
    for (int i = 0; i < PIPELINE_DEPTH; i++) {
        pipeline[i].Fetch_Inst = 0;
        pipeline[i].dest_reg = -1;
        pipeline[i].result = 0;
        pipeline[i].opcode = -1;
    }
}

// Function to shift the pipeline stages
void shift_pipeline() {
  // Shift results and destination registers one position
    for (int i = 4; i > 0; i--) {
        pipeline[i].Fetch_Inst = pipeline[i - 1].Fetch_Inst;
        pipeline[i].dest_reg = pipeline[i - 1].dest_reg;
        pipeline[i].result = pipeline[i - 1].result;
        pipeline[i].opcode = pipeline[i - 1].opcode;
    }
}

int num_instructions = 0;
int FINAL_RESULT;
int Memory_Array[2048];

int registerFile [32];
int *decodedArray = NULL;
// int FETCH_INST;
int result_reg;
int mem_opcode;
int clk = 1;
int pc = 0;
int WB_NUM = -3;

int FETCH_INST = 0;

int DECODE_INST = 1;

int EXCUTE_INST = 1;
int Excute_Flag = 0;

int MEM_INST = 1;
int MEM_FLAG = 0;

int WB_INST = 1;
int WB_FLAG = 0;


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

void writeback()
{
    if (WB_FLAG == 1 && (clk % 2 == 1))
    {
        int First_Reg;
        int result;
        if (WB_INST <= num_instructions)
        {
            printf("Writing back at instruction number: %d\n",WB_INST);
            WB_INST++;
        } 
    
        First_Reg = pipeline[4].dest_reg;
        // printf("reg: %d\n",First_Reg);
        result = pipeline[4].result;
        if (First_Reg > 0 && First_Reg < 32) 
        {
            registerFile[First_Reg] = result;
            printf("Register Number: %d is changed to %d\n",First_Reg,result);                            
        } 
        else 
        {
            printf("Invalid register index: %d\n", First_Reg);
        }

    }
    if (clk % 2 == 1)
    {
        WB_NUM++;
    }
    

}

void memory() {
    if (clk % 2 == 0)
    {
        if (MEM_FLAG == 1) {
            int temp_opcode = pipeline[3].opcode;
            int temp_result = pipeline[3].result;
            int temp_reg = pipeline[3].dest_reg;
            printf("Opcode in memory: %d\n",temp_opcode);
            if (temp_opcode != -1) {
                if (temp_opcode == 10) {
                    printf("MEM: dest_reg: %d .... result: %d\n ",temp_reg,Memory_Array[temp_result]);
                    registerFile[temp_reg] = Memory_Array[temp_result];
                } else if (temp_opcode == 11) {
                    Memory_Array[temp_result] = registerFile[temp_reg];
                    printf("MEM at index %d is changed to: %d\n ",temp_result,registerFile[temp_reg]);
                }
            }
            if (MEM_INST <= num_instructions) {
                printf("Memory Access At Instruction Number: %d\n", MEM_INST);
                MEM_INST++;
                WB_FLAG = 1;
            }
        }
    }
}


void execute()
{
    int result = 0;
    if (clk % 2 == 0)
    {
        if (decodedArray != NULL && Excute_Flag == 1){
            switch (decodedArray[0])
            {
            case 0:
                result = registerFile[decodedArray[2]] + registerFile[decodedArray[3]];
                break;
            case 1:
                result = registerFile[decodedArray[2]] - registerFile[decodedArray[3]];
                break;
            case 2:
                result = registerFile[decodedArray[2]] * registerFile[decodedArray[3]];
                break;
            case 3:
                result = decodedArray[5]; //dh imm
                break;
            case 4:
                if(registerFile[decodedArray[1]] == registerFile[decodedArray[2]])
                {
                    pc = pc + decodedArray[5]; // deh ana msh 3aref ehna mfrod nsebha pc + 1 wala pc + imm 3ltol
                }
                break;
            case 5:
                result = registerFile[decodedArray[2]] & registerFile[decodedArray[3]];
                break;
            case 6:
                result = registerFile[decodedArray[2]] ^ decodedArray[5]; // ^ deh 3lamet el XOR fe C
                break;
            case 7:
                int temp = (pc & 0b11110000000000000000000000000000) >> 28;
                pc = temp | decodedArray[6];
                // printf("PC: %d\n",pc);
                break;
            case 8:
                result = registerFile[decodedArray[2]] << decodedArray[4];
                break;
            case 9:
                result = registerFile[decodedArray[2]] >> decodedArray[4];
                break;
            case 10:
            case 11:
                result = registerFile[decodedArray[2]] + decodedArray[5];
                break;
            default:
                break;
            }
        
            if (EXCUTE_INST <= num_instructions)
            {
                printf("excuting instruction number: %d\n", EXCUTE_INST);
            }
            FINAL_RESULT = result;
            if (pipeline[2].Fetch_Inst != -1) 
            {
                pipeline[2].result = result;
                // printf("excute result in excute: %d\n",pipeline[2].result);
                result_reg = pipeline[2].dest_reg; 
                mem_opcode = pipeline[2].opcode;
                printf("excute opcode: %d\n",pipeline[2].opcode);

                // printf("excute destination register in excute: %d\n",pipeline[2].dest_reg);
            }
        }
    }
    else
    {   
        if (Excute_Flag == 1)
        {
            if (EXCUTE_INST <= num_instructions)
            {
                printf("excuting instruction number: %d\n", EXCUTE_INST);
                EXCUTE_INST++;
                MEM_FLAG = 1;
                pipeline[2].result = FINAL_RESULT;
                // printf("excute result in excute: %d\n",pipeline[2].result);
                pipeline[2].dest_reg = result_reg;
                pipeline[2].opcode = mem_opcode;
                printf("excute opcode: %d\n",pipeline[2].opcode);
            }
        }
    }
}

void decode()
{
    int instruction = pipeline[1].Fetch_Inst;
    // printf("instruction: %d\n",instruction);

    int *OutgoingArray = malloc(7 * sizeof(int));
    if (OutgoingArray == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    if (clk % 2 == 0)
    {

        int opcode = 0;  // bits31:28
        int R1 = 0;      // bits27:23
        int R2 = 0;      // bit22:18
        int R3 = 0;      // bits17:13
        unsigned int Shamt = 0;   // bits12:0
        int Imm = 0;     // bits17:0
        int Address = 0; // bits27:0
        int sign_bit = 0;
        
        
        //0b00100000100010110010000000000000
        // 256 128 64 32 16 8 4 2 1
        unsigned int opcodebitmask = instruction & 0b11110000000000000000000000000000;
        opcode = opcodebitmask >> 28;
        OutgoingArray[0] = opcode;
        // printf("opcode: %d\n", opcode);
        
        unsigned int R1bitmask = instruction & 0b00001111100000000000000000000000;
        R1 = R1bitmask >> 23;
        OutgoingArray[1] = R1;
            
        unsigned int R2bitmask = instruction & 0b00000000011111000000000000000000;
        R2 = R2bitmask >> 18;
        OutgoingArray[2] = R2;
        
        unsigned int R3bitmask = instruction & 0b00000000000000111110000000000000;
        R3 = R3bitmask >> 13;
        OutgoingArray[3] = R3;

        Shamt = instruction & 0b00000000000000000001111111111111;
        OutgoingArray[4] = Shamt;  

        Imm  = instruction & 0b00000000000000111111111111111111;
        sign_bit = (instruction & 0b100000000000000000) >> 17;
        if (sign_bit == 0b1)
        {
            Imm = Imm | 0b11111111111111000000000000000000;
        }


        OutgoingArray[5] = Imm;    
        Address = instruction & 0b00001111111111111111111111111111;
        OutgoingArray[6] = Address;
        decodedArray = OutgoingArray;
        if (instruction != 0 && (DECODE_INST <= num_instructions)) // deh kanet moshkela el fe clock cycle 16
        {
            printf("Decoding instruction number %d with code : %d\n",DECODE_INST , instruction);
            pipeline[1].dest_reg = decodedArray[1];
            pipeline[1].opcode = decodedArray[0];
            // mem_opcode = decodedArray[0];
            // printf("Pipeline decode stage opcode set to: %d\n", pipeline[1].opcode);
        }
    }
    else 
    {
        if (instruction != 0)
        {
            if (DECODE_INST <= num_instructions)
            {
                pipeline[1].dest_reg = decodedArray[1]; 
                printf("Decoding instruction number %d with code : %d\n",DECODE_INST , instruction);
                pipeline[1].opcode = decodedArray[0];

                // printf("Pipeline decode stage opcode set to: %d\n", pipeline[1].opcode);
                DECODE_INST++;
            }
            Excute_Flag = 1;
        }
    }
    // shift_pipeline();

}

int fetch()
{

    if(pc < num_instructions && (clk % 2 == 1)){
        int instruction = Memory_Array[pc];
        printf("Fetching instruction number %d with code: %d\n", FETCH_INST + 1, instruction);        
        FETCH_INST++;
        pc++;
        // printf("PC in fetch: %d & FETCH NUM: %d\n",pc , FETCH_INST);
        pipeline[0].Fetch_Inst = instruction;
    }
    // else
    // {
    //     // printf("End of program reached.\n");
    //     return -1; // End of program
    // }
    
    
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
            char *IMM;
            if (strcmp (Words_array[0], "MOVI") == 0)
            {
                Sec_reg = "00000";
                IMM = Words_array[2];
                int num3 = atoi(IMM);
                IMM = int_to_binary(num3,18,"IMM");
            }
            else
            {
                Sec_reg = Words_array[2]+1;
                int num2 = atoi(Sec_reg);
                Sec_reg = int_to_binary(num2,5,"Reg");
                IMM = Words_array[3];
                int num3 = atoi(IMM);
                IMM = int_to_binary(num3,18,"IMM");
            }
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
        // for (int j = 0; j < sizeof(Memory_Array); j++) {
        //     free(Memory_Array[j]);
        // }
    }
    // memset(Memory_Array, 0, sizeof(Memory_Array));
    // memset(registerFile, 0, sizeof(registerFile));
    init_pipeline();
    while (num_instructions != WB_NUM )
    {   
        printf("\nPC :%d\n", pc);
        int INST;
        printf("Clock Cycle: %d\n\n", clk);
        

        writeback();
        // WB_NUM++;

        memory(decodedArray,FINAL_RESULT);
    
        execute();
        decode();
        
        fetch(); 
        // if (clk != 1)
        // {
        //     shift_pipeline();
        // } 
        shift_pipeline();
        // printf("Fetch Count: %d\n",FETCH_INST);
        //printf("INST: %d\n\n",INST);
        clk++;
        
    }
    
    // int temppp = Memory_Array[1025];
    // printf("%d\n",temppp);
    // for (int i = 0; i < 2048; i++) {
    //     Memory_Array[i] = 0;
    //}
    // for (int i = 0; i < 32; i++) {
    //     registerFile[i] = 0;
    // }
    printf("\n[");
    for (int i = 0; i < 32; i++) {
        printf("%d", registerFile[i]);
        if (i < 32 - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    
    // printf("[");
    // for (int i = 0; i < 2048; i++) {
    //     printf("%d", Memory_Array[i]);
    //     if (i < 2048 - 1) {
    //         printf(", ");
    //     }
    // }
    // printf("]\n");
    // for (int i = 0; i < 2048; i++) {
    //     free(Memory_Array[i]);
    // }
    // for (int i = 0; i < 32; i++) {
    //     free(registerFile[i]);
    // }    
    return 0;
    
}