#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "assembler.h"


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
    char instruction[MAX_INSTRUCTION_SIZE];
    int arguments[MAX_ARGS], index, nbArgs = 0, nextCharIndex;

    nextCharIndex = extractInstruction(line, instruction);
    nbArgs = extractArgs(line, nextCharIndex, arguments);

    /* Testing */
    for(index = 0; index < nbArgs; index++)
        printf(" %d ", arguments[index]);

    printf("\n");
    /**/
    return getBinSegment(instruction, arguments, nbArgs);
}

int extractInstruction(char * line, char * instruction){
    int index;
    for(index = 0; isalpha(line[index]) && index < MAX_INSTRUCTION_SIZE; index++){
        instruction[index] = line[index];
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

unsigned int getBinSegment(char * instruction, int arguments[], int nbArgs){
    
}
