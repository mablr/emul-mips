#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mode.h"
#include "../memory/memory.h"
#include "../register/register.h"
#include "../assembler/assembler.h"
#include "../stream/stream.h"


void interactiveMode(){
    memory progMem = NULL;
    char * userInput = (char *) malloc(sizeof(char)*BUFFER_SIZE);
    int hexCode, args[3], instructionIndex = -1;
    if(userInput == NULL){
        exit(EXIT_FAILURE);
    }
    printf("\n###################\n# Mode Intéractif #\n###################\n\n$ ");
    while(fgets(userInput, BUFFER_SIZE, stdin) && asm2hex(userInput, &hexCode)){
        printf("\nInstruction : %s\nCode Hexadécimal : %08x\n", userInput, hexCode);
        instructionIndex = searchInstructionHex(hexCode);
        /* Extraction des arguments */
        extractArgsHex(hexCode, instructionIndex, args);
        /* Exécution de l'instruction */
        instructions[instructionIndex].fct(args);
        while(strcmp(userInput, "c\n")){
            printf("\n\nProcessing instruction:\n %08x\n*** [r]: display registers; [m]: memory; [c]: continue\n", hexCode);
            fgets(userInput, 3, stdin);
            if(!strcmp(userInput, "r\n")){
                showRegisters();
            }
            if(!strcmp(userInput, "m\n")){
                showMemory(&progMem);
            }
        }
        /* Retour au shell intéractif */
        userInput[0] = '\0';
        printf("\n\n$ ");
    }
    printf("\nFin d'exécution.\n");
}
void simpleMode(char * asmFile){
    memory instructionsMem = NULL;
    memory progMem = NULL;
    loadProgMem(asmFile, 0, &instructionsMem);
    runInstructions(0, &instructionsMem, &progMem);
    showMemory(&instructionsMem);
}
void stepMode(char * asmFile){
    memory instructionsMem = NULL;
    memory progMem = NULL;
    loadProgMem(asmFile, 1, &instructionsMem);
    runInstructions(1, &instructionsMem, &progMem);
    showMemory(&instructionsMem);
}

void loadProgMem(char * asmFile, int step, memory * mem){
    FILE * inputFile;
    unsigned int memAddress = 0;
    int stepActivated = step, hexCode = 0, lineIndex = 1;
    char * userInputStep = (char *) malloc(2 * sizeof(char));
    char * lineBuffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
    if(lineBuffer == NULL)
        exit(EXIT_FAILURE);
    if(userInputStep == NULL)
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

void runInstructions(int step, memory * instructionsMem, memory * progMem){
    int hexCode = 0, args[3], instructionIndex = -1;
    char * userInputStep = (char *) malloc(2 * sizeof(char));
    if(userInputStep == NULL)
        exit(EXIT_FAILURE);
    while(isAllocatedWord(getRegister(PC), instructionsMem)){
        hexCode = getWord(getRegister(PC), instructionsMem);
        if((instructionIndex = searchInstructionHex(hexCode)) != -1){
            /* Extraction des arguments */
            extractArgsHex(hexCode, instructionIndex, args);
            /* Exécution de l'instruction */
            instructions[instructionIndex].fct(args);
            /* Traitement du "pas à pas" */
            if(step){
                while(strcmp(userInputStep, "c\n")){
                    printf("\n\nProcessing instruction:\n %08x\n*** [r]: display registers; [m]: memory; [c]: continue\n", hexCode);
                    fgets(userInputStep, 3, stdin);
                    if(!strcmp(userInputStep, "r\n")){
                        showRegisters();
                    }
                    if(!strcmp(userInputStep, "m\n")){
                        showMemory(progMem);
                    }
                }
                userInputStep[0] = '\0';  
            }
            
        }
        storeRegister(PC, getRegister(PC)+4); /* Todo : Vérifier le comportement en cas de Jump/Branch */
    }
}