#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mode.h"
#include "../memory/memory.h"
#include "../assembler/assembler.h"
#include "../stream/stream.h"


void interactiveMode(){

}
void simpleMode(char * asmFile){
    memory InstructionsMem = NULL;
    loadProgMem(asmFile, 0, &InstructionsMem);
    showMemory(&InstructionsMem);
}
void stepMode(char * asmFile){
    memory InstructionsMem = NULL;
    loadProgMem(asmFile, 1, &InstructionsMem);
    showMemory(&InstructionsMem);
}

void loadProgMem(char * asmFile, int step, memory * mem){
    FILE * inputFile;
    unsigned int memAddress = 0;
    int stepActivated = step, hexCode = 0, lineIndex = 1;
    char * userInputStep = (char *) malloc(3 * sizeof(char));
    char * lineBuffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
    if(lineBuffer == NULL)
        exit(EXIT_FAILURE);
    inputFile = openFile(asmFile, "r");
    
    /* Lecture ligne par ligne du fichier d'entrée */
    while(fgets(lineBuffer, BUFFER_SIZE, inputFile) != NULL){
        if(asm2hex(lineBuffer, &hexCode)){
            if(stepActivated){
                while(strcmp(userInputStep, "\n") && strcmp(userInputStep, "s\n")){
                    printf("\n\nAssembling line %d:\n%s\nHexadecimal instruction code:\n%08x\n*** [entry]: go to next instruction; [s] skip to execution\n", lineIndex, lineBuffer, hexCode);
                    fgets(userInputStep, 3, stdin);
                    if(!strcmp(userInputStep, "s\n")){
                        stepActivated = 0;
                    }
                }
                userInputStep[0] = '\0';
            }
            insertWord(memAddress, hexCode, mem);
            lineIndex++;
            memAddress += 4;
        }
    }
    
    closeFile(asmFile, inputFile);
    free(lineBuffer);
}