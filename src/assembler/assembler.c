#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "assembler.h"

instruction instructions[] = {{"ADD",  0x149, 0x00, 0x20},
                              {"ADDI", 0x150, 0x08},
                              {"AND",  0x149, 0x00, 0x24},
                              {"BEQ",  0x160, 0x04},
                              {"BGTZ", 0x120, 0x07},
                              {"BLEZ", 0x120, 0x06},
                              {"BNE",  0x160, 0x05},
                              {"DIV",  0x141, 0x00, 0x1A},
                              {"J",    0x400, 0x02},
                              {"JAL",  0x400, 0x03},
                              {"JR",   0x103, 0x00, 0x08},
                              {"LUI",  0x050, 0x0F},
                              {"LW",   0x260, 0x23},
                              {"MFHI", 0x009, 0x00, 0x10},
                              {"MFLO", 0x009, 0x00, 0x12},
                              {"MULT", 0x141, 0x00, 0x18},
                              {"NOP",  0x000, 0x00, 0x00},
                              {"OR",   0x149, 0x00, 0x25},
                              {"ROTR", 0x0CD, 0x00, 0x02},
                              {"SLL",  0x04D, 0x00, 0x00},
                              {"SLT",  0x149, 0x00, 0x2A},
                              {"SRL",  0x04D, 0x00, 0x02},
                              {"SUB",  0x149, 0x00, 0x22},
                              {"SW",   0x260, 0x2B},
                              {"XOR",  0x149, 0x00, 0x26}
                            };

FILE * openFile(char * fileName, char * mode){
    FILE * file = fopen(fileName, mode);
    if(file == NULL){
        fprintf(stderr,"Unable to open %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    return file;
}

void closeFile(char * fileName, FILE * file){
    if(fclose(file) ==  EOF){
        fprintf(stderr,"Unable to close %s\n", fileName);
        exit(EXIT_FAILURE);
    }
}

void asm2hex(char * inputFileName, char * outputFileName){
    FILE * asmFile, * hexFile;
    /* unsigned int hexCode = 0; */
    char * lineBuffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
    if(lineBuffer == NULL)
        exit(EXIT_FAILURE);
    asmFile = openFile(inputFileName, "r");
    hexFile = openFile(outputFileName, "w");
    while(fgets(lineBuffer, BUFFER_SIZE, asmFile) != NULL){
        if(purifyLine(lineBuffer)){
            translateAsm(lineBuffer);
        }
    }
    free(lineBuffer);
    closeFile(inputFileName, asmFile);
    closeFile(outputFileName, hexFile);
}

int purifyLine(char * line){
    int notEmpty = 0, indexDirt = 0, indexPure = 0, comment = 0;
    while(indexDirt < BUFFER_SIZE && line[indexDirt] != '\0' && !comment){
        if(line[indexDirt] == '#'){
            comment = 1;
            if(indexPure != 0){
                notEmpty = 1;
                line[indexPure] = '\n';
                indexPure++;
                indexDirt++;
            }
        }else if(line[indexDirt] != ' ' && line[indexDirt] != '\t'){
            if(line[indexDirt] != '\n'){
                notEmpty = 1;
            }
            line[indexPure] = line[indexDirt];
            indexPure++;
            indexDirt++;
        }else{
            indexDirt++;
        }
    }
    if(indexPure < indexDirt || comment){
        for(; indexDirt < BUFFER_SIZE; indexDirt++){
            line[indexPure++] = '\0';
        }        
    }
    return notEmpty;
}

unsigned int translateAsm(char * line){
    int arguments[MAX_ARGS], index, nbArgs = 0, nextCharIndex;
    char * opcode = (char *) malloc(MAX_OPCODE_SIZE*sizeof(char));
    if(opcode == NULL)
        exit(EXIT_FAILURE);
    nextCharIndex = extractOpcode(line, opcode);
    nbArgs = extractArgs(line, nextCharIndex, arguments);

    /* Testing */
    printf("> %s ", opcode);
    for(index = 0; index < nbArgs; index++)
        printf(" %d ", arguments[index]);

    printf(", nbArgs: %d\n", nbArgs);
    /**/
    return getBinSegment(opcode, arguments);
}

int extractOpcode(char * line, char * opcode){
    int index;
    for(index = 0; isalpha(line[index]) && index < MAX_OPCODE_SIZE; index++){
        opcode[index] = line[index];
    }
    return index;
}

int extractArgs(char * line, int indexToBegin, int arguments[]){
    int index, argIndex = 0, beginArg = 0, errorFlag = 0, nbArgsFound;
    for(index = indexToBegin; line[index] != '\n' && index < BUFFER_SIZE && !errorFlag; index++){
        if((isdigit(line[index]) || line[index] == '-') && !beginArg){ /* À modifier pour gérer les étiquettes */
            beginArg = index;
        }else if((line[index] == ',' || line[index] == '(' || line[index] == ')') && beginArg){
            arguments[argIndex] = str2int(line, beginArg, index);
            beginArg = 0; 
            argIndex++;
        }else if(!(isdigit(line[index]) || line[index] == '-' || line[index] == ',' || line[index] == '$' || line[index] == '(' || line[index] == ')')){
            errorFlag = 1;
        }
    }
    if(indexToBegin != index && !errorFlag){
        if(isalnum(line[index-1])){
            arguments[argIndex] = str2int(line, beginArg, index);
            nbArgsFound = argIndex + 1;
        }else{
            nbArgsFound = argIndex;
        }
    }else{
        nbArgsFound = 0;
    }
    return nbArgsFound;
}

int str2int(char * str, int beginChar, int endChar){
    int sign = 1, value = 0, index = beginChar;
    if(str[index] == '-'){
        index++;
        sign = -1;
    }
    while(index < endChar){
        value *= 10;
        value += str[index]-48;
        index++;
    }
    return sign*value;
}

unsigned int getBinSegment(char * opcode, int arguments[]){
    unsigned int segment = 0;
    int index,instructionIndex=-1;
    for(index=0;index<sizeof(instructions)/sizeof(instruction); index++){
        if(!strcmp(instructions[index].name, opcode)){
            instructionIndex = index;
        }
    }
    if(instructionIndex != -1){
        /* L'application d'un tableau de karnaugh pourrait éventuellement factoriser 
         * certaines conditions au détriment de la lisibilité du code .
         */
        /* OPCODE */
        segment += (instructions[instructionIndex].opcode << OPCODE_SHIFT) & OPCODE_MASK;
        if(instructions[instructionIndex].type & FCT_FIELD){
            /* FUNCTION */
            segment += instructions[instructionIndex].function & FCT_MASK;
        }
        if( !(instructions[instructionIndex].type ^ (RS_FIELD|RT_FIELD|RD_FIELD|FCT_FIELD)) ){
            /* ADD,AND,OR,SLT,SUB,XOR */
            /* RS */
            segment += (arguments[1] << RS_SHIFT) & RS_MASK;
            /* RT */
            segment += (arguments[2] << RT_SHIFT) & RT_MASK;
            /* RD */
            segment += (arguments[0] << RD_SHIFT) & RD_MASK;
        }else if( !(instructions[instructionIndex].type ^ (RS_FIELD|RT_FIELD|OFFSET_FIELD)) ){
            /* BEQ,BNE */
            /* RS */
            segment += (arguments[0] << RS_SHIFT) & RS_MASK;
            /* RT */
            segment += (arguments[1] << RT_SHIFT) & RT_MASK;
            /* OFFSET */
            segment += (arguments[2] << OFFSET_SHIFT) & OFFSET_MASK;
        }else if( !(instructions[instructionIndex].type ^ (RS_FIELD|OFFSET_FIELD)) ){
            /* BGTZ, BLEZ */
            /* RS */
            segment += (arguments[0] << RS_SHIFT) & RS_MASK;
            /* OFFSET */
            segment += (arguments[1] << OFFSET_SHIFT) & OFFSET_MASK;
        }else if( !(instructions[instructionIndex].type ^ (RS_FIELD|RT_FIELD|FCT_FIELD)) ){
            /* DIV,MUL */
            /* RS */
            segment += (arguments[0] << RS_SHIFT) & RS_MASK;
            /* RT */
            segment += (arguments[1] << RT_SHIFT) & RT_MASK;
        }else if( !(instructions[instructionIndex].type ^ (RS_FIELD|RT_FIELD|IMMEDIATE_FIELD)) ){
            /* ADDI */
            /* RT */
            segment += (arguments[0] << RT_SHIFT) & RT_MASK;
            /* RS */
            segment += (arguments[1] << RS_SHIFT) & RS_MASK;
            /* IMMEDIATE */
            segment += (arguments[2] << IMMEDIATE_SHIFT) & IMMEDIATE_MASK;
        }else if( !(instructions[instructionIndex].type ^ (RT_FIELD|IMMEDIATE_FIELD)) ){
            /* LUI */
            /* RT */
            segment += (arguments[0] << RT_SHIFT) & RT_MASK;
            /* IMMEDIATE */
            segment += (arguments[1] << IMMEDIATE_SHIFT) & IMMEDIATE_MASK;
        }else if( !(instructions[instructionIndex].type ^ (BASE_FIELD|RT_FIELD|OFFSET_FIELD)) ){
            /* LW,SW */
            /* BASE */
            segment += (arguments[2] << BASE_SHIFT) & BASE_MASK;
            /* RT */
            segment += (arguments[0] << RT_SHIFT) & RT_MASK;
            /* OFFSET */
            segment += (arguments[1] << OFFSET_SHIFT) & OFFSET_MASK;
        }else if( !(instructions[instructionIndex].type ^ (RD_FIELD|FCT_FIELD)) ){
            /* MFHI,MFLO */
            /* RD */
            segment += (arguments[0] << RD_SHIFT) & RD_MASK;
        }else if( !(instructions[instructionIndex].type ^ INDEX_FIELD) ){
            /* J,JAL */
            /* INDEX */
            segment += (arguments[0] << INDEX_SHIFT) & INDEX_MASK;
        }else if( !(instructions[instructionIndex].type ^ (RS_FIELD|HINT_FIELD|FCT_FIELD)) ){
            /* JR */
            /* RS */
            segment += (arguments[0] << RS_SHIFT) & RS_MASK;
            /* HINT : assuming we use MIPS I version, thus HINT = 0*/
        }else if( !(instructions[instructionIndex].type ^ (RT_FIELD|RD_FIELD|SA_FIELD|FCT_FIELD)) ){
            /* SLL,SRL */
            /* RT */
            segment += (arguments[1] << RT_SHIFT) & RT_MASK;
            /* RD */
            segment += (arguments[0] << RD_SHIFT) & RD_MASK;
            /* SA */
            segment += (arguments[2] << SA_SHIFT) & SA_MASK;
        }else if( !(instructions[instructionIndex].type ^ (ROTATE_FIELD|RT_FIELD|RD_FIELD|SA_FIELD|FCT_FIELD)) ){
            /* ROTR */
            /* R Flag */
            segment |= ROTATE_MASK;
            /* RT */
            segment += (arguments[1] << RT_SHIFT) & RT_MASK;
            /* RD */
            segment += (arguments[0] << RD_SHIFT) & RD_MASK;
            /* SA */
            segment += (arguments[2] << SA_SHIFT) & SA_MASK;
        }
        printf("SEG: 0x%08x\n", segment);
    }
    return segment;
}
