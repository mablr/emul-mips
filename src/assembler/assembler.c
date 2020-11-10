#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "assembler.h"

instruction instructions[] = {{"ADD",  'R', 0x00, 0x20},
                              {"ADDI", 'I', 0x08},
                              {"AND",  'R', 0x00, 0x24},
                              {"BEQ",  'I', 0x04},
                              {"BGTZ", 'I', 0x07},
                              {"BLEZ", 'I', 0x06},
                              {"BNE",  'I', 0x05},
                              {"DIV",  'R', 0x00, 0x1A},
                              {"J",    'J', 0x02},
                              {"JAL",  'J', 0x03},
                              {"JR",   'R', 0x00, 0x08},
                              {"LUI",  'I', 0x0F},
                              {"LW",   'I', 0x23},
                              {"MFHI", 'R', 0x00, 0x10},
                              {"MFLO", 'R', 0x00, 0x12},
                              {"MULT", 'R', 0x00, 0x18},
                              {"NOP",  '0', 0x00, 0x00},
                              {"OR",   'R', 0x00, 0x25},
                              {"ROTR", 'R', 0x00, 0x02},
                              {"SLL",  'R', 0x00, 0x00},
                              {"SLT",  'R', 0x00, 0x2A},
                              {"SRL",  'R', 0x00, 0x02},
                              {"SUB",  'R', 0x00, 0x22},
                              {"SW",   'I', 0x2B},
                              {"XOR",  'R', 0x00, 0x26}
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
    return getBinSegment(opcode, arguments, nbArgs);
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

unsigned int getBinSegment(char * opcode, int arguments[], int nbArgs){
    unsigned int segment = 0;
    int index,instructionIndex=-1;
    for(index=0;index<sizeof(instructions)/sizeof(instruction); index++){
        if(!strcmp(instructions[index].name, opcode)){
            instructionIndex = index;
        }
    }
    if(instructionIndex != -1){
        /* OPCODE */
        segment += (instructions[instructionIndex].opcode << OPCODE_SHIFT) & OPCODE_MASK;
        if(instructions[instructionIndex].type == 'R'){
            /* FUNCTION */
            segment += instructions[instructionIndex].function & FCT_MASK;
            /* RS */
            segment += instructions[instructionIndex].function & FCT_MASK;
            printf("NAME: %s, TYPE: %c, OPCODE : 0x%06x, FUNCTION : 0x%06x\n", instructions[instructionIndex].name, instructions[instructionIndex].type, instructions[instructionIndex].opcode, instructions[instructionIndex].function);
        }else{
            printf("NAME: %s, TYPE: %c, OPCODE : 0x%06x\n", instructions[instructionIndex].name, instructions[instructionIndex].type, instructions[instructionIndex].opcode);
        }
    }
    return segment;
}
