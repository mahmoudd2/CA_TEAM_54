#include <stdio.h>

int registerFile [] = {22,16,87,54,102,434,33,67,98,49,55,9,18,1,32,45};
int instructionMemory[] = {-1321039047,992777138};
int NumberofInstructions = sizeof(instructionMemory)/sizeof(int);
int pc = 0;

// Binary int format in c is 0b00000000000000000000000000000000 (32 bits)

void fetch() {
        for(int i = 0; i < NumberofInstructions; i++){
            int instruction = 0;
            
            instruction = instructionMemory[pc];
            
            decode(instruction);
            
            pc++;
        }
}

void decode(int instruction) {
        
        int opcode = 0;  // bits31:26
        int rs = 0;      // bits25:21
        int rt = 0;      // bit20:16
        int rd = 0;      // bits15:11
        int shamt = 0;   // bits10:6
        int funct = 0;   // bits5:0
        int imm = 0;     // bits15:0
        int address = 0; // bits25:0
        
        int valueRS = 0;
        int valueRT = 0;
        
        int opcodebitmask = instruction & 0b11110000000000000000000000000000;
         opcode = opcodebitmask >> 28;
        
        int rsbitmask = instruction & 0b00001111000000000000000000000000;
         rs = rsbitmask >> 24;
        valueRS = registerFile[rs];
        
        
        int rtbitmask = instruction & 0b00000000111100000000000000000000;
         rt = rtbitmask >> 20;
        valueRT = registerFile[rt];
        
        int rdbitmask = instruction & 0b00000000000011110000000000000000;
         rd = rdbitmask >> 16;
        
        int shamitbitmask = instruction & 0b00000000000000001111000000000000;
         shamt = shamitbitmask >> 12;
        
         funct = instruction & 0b00000000000000000000111111111111;
        
         imm  = instruction & 0b00000000000011111111111111111111;
        
         address = instruction & 0b00001111111111111111111111111111;
        
        // Printings
        
        printf("Instruction %i\n",pc);
		printf("opcode = %i\n",opcode);
		printf("rs = %i\n",rs);
		printf("rt = %i\n",rt);
		printf("rd = %i\n",rd);
		printf("shift amount = %i\n",shamt);
		printf("function = %i\n",funct);
		printf("immediate = %i\n",imm);
		printf("address = %i\n",address);
		printf("value[rs] = %i\n",valueRS);
		printf("value[rt] = %i\n",valueRT);
		printf("---------- \n");
             
}

void main() {
	   
    fetch();
    
    // Expected output
    
    /* 
    
    Instruction 0
    opcode = -5 (signed) or 11 (unsigned)
    rs = 1
    rt = 4
    rd = 2
    shift amount = 8
    function = 2873
    immediate = 166713
    address = 21138233
    value[rs] = 16
    value[rt] = 102
    
    ----------
    
    Instruction 1
    opcode = 3
    rs = 11
    rt = 2
    rd = 12
    shift amount = 9
    function = 946
    immediate = 824242
    address = 187470770
    value[rs] = 9
    value[rt] = 87
    
    ----------
    
    */  
		
}