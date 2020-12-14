#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mode.h"
#include "../memory/memory.h"
#include "../register/register.h"
#include "../assembler/assembler.h"
#include "../stream/stream.h"

memory progMem = NULL;

void interactiveMode(){
    char * userInput = (char *) malloc(sizeof(char)*BUFFER_SIZE);
    int hexCode, args[3], instructionIndex = -1;
    if(userInput == NULL){
        exit(EXIT_FAILURE);
    }
    printf("\n###################\n# Mode Intéractif #\n###################\n");
    printf("Les branchements et jumps ne sont pas disponibles dans ce mode\n\n$ ");
    while(fgets(userInput, BUFFER_SIZE, stdin) && asm2hex(userInput, &hexCode)){
        printf("\nInstruction : %s\nCode Hexadécimal : %08x\n", userInput, hexCode);
        instructionIndex = searchInstructionHex(hexCode);
        /* Extraction des arguments */
        extractArgsHex(hexCode, instructionIndex, args);
        /* Exécution de l'instruction */
        instructions[instructionIndex].fct(args);
        /* Interface utilisateur pour afficher l'état de la mémoire et des registres */
        while(strcmp(userInput, "c\n")){
            printf("\n\nProcessing instruction:\n %08x\n*** [r]: display registers; [m]: memory; [c]: continue\n", hexCode);
            fgets(userInput, 3, stdin);
            /* Si entrée = r --> Afficher l'état des registres */
            if(!strcmp(userInput, "r\n")){
                printf("*** Registers state ***\n\n");
                showRegisters();
            }
            /* Si entrée = m --> Afficher l'état de la mémoire */
            if(!strcmp(userInput, "m\n")){
                printf("*** Memory state ***\n\n");
                showMemory(&progMem);
            }
        }
        /* Retour au shell intéractif */
        userInput[0] = '\0';
        printf("\n\n$ ");
    }
    printf("\nFin d'exécution.\n");
    /* Libération mémoire */
    free(userInput);
    freeMem(&progMem);
}
void simpleMode(char * asmFile){
    memory instructionsMem = NULL;
    loadProgMem(asmFile, 0, &instructionsMem);
    runInstructions(0, &instructionsMem, &progMem);
    printf("*** Final registers state ***\n\n");
    showRegisters();
    printf("*** Final memory state ***\n\n");
    showMemory(&progMem);
    /* Libération mémoire */
    freeMem(&instructionsMem);
    freeMem(&progMem);
}
void stepMode(char * asmFile){
    memory instructionsMem = NULL;
    loadProgMem(asmFile, 1, &instructionsMem);
    runInstructions(1, &instructionsMem, &progMem);
    printf("*** Final registers state ***\n\n");
    showRegisters();
    printf("*** Final memory state ***\n\n");
    showMemory(&progMem);
    /* Libération mémoire */
    freeMem(&instructionsMem);
    freeMem(&progMem);
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
                /* Tant que l'entrée est différente de [enter] (continuer) 
                 * ou de s passer l'étape --> afficher le menu et attendre
                 * une entrée de l'utilisateur
                 */
                while(strcmp(userInputStep, "\n") && strcmp(userInputStep, "s\n")){
                    printf("\n\nAssembling line %d:\n%s\nHexadecimal instruction code:\n%08x\n*** [entry]: go to next instruction; [s] skip to execution\n", lineIndex, lineBuffer, hexCode);
                    fgets(userInputStep, 3, stdin);
                    if(!strcmp(userInputStep, "s\n")){
                        /* Désactivation du mode pas à pas pour la lecture et le stockage de instructions */
                        stepActivated = 0;
                    }
                }
                userInputStep[0] = '\0';
            }
            /* Stockage en mémoire de l'instruction*/
            insertWord(memAddress, hexCode, mem);
            lineIndex++;
            memAddress += 4;
        }
    }
    /* Fermeture du fichier */
    closeFile(asmFile, inputFile);
    /* Libération mémoire */
    free(userInputStep);
    free(lineBuffer);
}

void runInstructions(int step, memory * instructionsMem, memory * progMem){
    int hexCode = 0, args[3], instructionIndex = -1;
    char * userInputStep = (char *) malloc(2 * sizeof(char));
    if(userInputStep == NULL)
        exit(EXIT_FAILURE);
    while(isAllocatedWord(getRegister(PC), instructionsMem)){
        hexCode = getWord(getRegister(PC), instructionsMem);
        /* Si l'instruction est prise en charge par l'émulateur */
        if((instructionIndex = searchInstructionHex(hexCode)) != -1){
            /* Extraction des arguments */
            extractArgsHex(hexCode, instructionIndex, args);
            /* Exécution de l'instruction */
            if(instructions[instructionIndex].fct(args)){
                /* Si la fonction appelée retourne 1 --> exception */
                /* Fonctionnalité pas encore implémentée */
                fprintf(stderr,"@%08x Exception détectée !\n", getRegister(PC));
            }
            /* Traitement du "pas à pas" */
            if(step){
                while(strcmp(userInputStep, "c\n")){
                    printf("\n\nProcessing instruction:\n %08x\n*** [r]: display registers; [m]: memory; [c]: continue\n", hexCode);
                    fgets(userInputStep, 3, stdin);
                    if(!strcmp(userInputStep, "r\n")){
                        printf("*** Registers state ***\n\n");
                        showRegisters();
                    }
                    if(!strcmp(userInputStep, "m\n")){
                        printf("*** Memory state ***\n\n");
                        showMemory(progMem);
                    }
                }
                userInputStep[0] = '\0';  
            }
            
        }
    }
    /* Libération mémoire */
    free(userInputStep);
}