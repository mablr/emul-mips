#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "assembler.h"
#include "../stream/stream.h"
#include "../memory/memory.h"

/* Instructions MIPS prises en charge par l'assembleur */
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

int asm2hex(char * line, int * hexCode){
    int arguments[MAX_ARGS], nbArgs = 0, nextCharIndex, instructionIndex;
    int asmConverted = 0;
    /* Allocations mémoire */
    char * opcode = (char *) malloc(MAX_OPCODE_SIZE*sizeof(char));
    if(opcode == NULL)
        exit(EXIT_FAILURE);
    /* Si la ligne n'est pas vide (les espaces et commentaires sont supprimés) */
    if(purifyLine(line)){
        /* Extraction de l'opcode, et stockage de la valeur d'index juste après la fin de celui-ci */
        nextCharIndex = extractOpcode(line, opcode);
        /* Extraction des arguments, et récupération de leur nombre */
        nbArgs = extractArgs(line, nextCharIndex, arguments);
        /* Recherche du rang de l'instruction dans le tableau des instructions supportées */
        instructionIndex = searchInstruction(opcode);
        /* Si l'instruction est supportée et que le nombre d'arguments est bon */
        if(instructionIndex != -1 && validateArgs(instructionIndex, arguments, nbArgs)){
            /* Encodage hexadécimal et écriture dans le fichier de sortie */
            *hexCode = getBinSegment(instructionIndex, arguments);
            /* Correctly converted asm */
            asmConverted = 1;
        }
    }
    /* Libération mémoire */
    free(opcode);
    return asmConverted;
}

int purifyLine(char * line){
    int notEmpty = 0, indexDirt = 0, indexPure = 0, comment = 0;
    while(indexDirt < BUFFER_SIZE && line[indexDirt] != '\0' && !comment){
        /* Suppression commentaires */
        if(line[indexDirt] == '#'){
            comment = 1;
            if(indexPure != 0){
                notEmpty = 1;
                line[indexPure] = '\n';
                indexPure++;
                indexDirt++;
            }
        /* Suppression des espaces et tabulations */
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
    /* Si de l'espace a été libéré dans le buffer*/
    if(indexPure < indexDirt || comment){
        /* Remplisasage avec des '\0' */
        for(; indexDirt < BUFFER_SIZE; indexDirt++){
            line[indexPure++] = '\0';
        }        
    }
    /* Retourne l'état de la ligne : Vide ou Non vide */
    return notEmpty;
}

int extractOpcode(char * line, char * opcode){
    int index, argBeginIndex;
    /* Recopie l'opcode depuis le BUFFER*/
    for(index = 0; isalpha(line[index]) && index < MAX_OPCODE_SIZE; index++){
        opcode[index] = line[index];
    }
    /* Valeur de l'index juste après la fin de l'opcode */
    argBeginIndex = index;
    /* Si l'opcode est plus court que MAX_OPCODE_SIZE, remplissage avec des '\0' */
    while(index < MAX_OPCODE_SIZE){
        opcode[index] = 0;
        index++;
    }
    return argBeginIndex;
}

int extractArgs(char * line, int indexToBegin, int arguments[]){
    int index, argIndex = 0, beginArg = 0, errorFlag = 0, nbArgsFound;
    for(index = indexToBegin; line[index] != '\n' && index < BUFFER_SIZE && !errorFlag; index++){
        /* Si le caractère constitue un argument et pas d'autre argument en cours de traitement */ 
        if((isdigit(line[index]) || line[index] == '-') && !beginArg){ /* À modifier pour gérer les étiquettes */
            /* début du traitement de l'argument */
            beginArg = index;
        /* Si caractère délimiteur et argument en cours de traitement */
        }else if((line[index] == ',' || line[index] == '(' || line[index] == ')') && beginArg){
            /* Stockage de l'argument */
            arguments[argIndex] = str2int(line, beginArg, index);
            /* Passage à l'argument suivant */
            beginArg = 0; 
            argIndex++;
        /* Sinon si caractère interdit */
        }else if(!(isdigit(line[index]) || line[index] == '-' || line[index] == ',' || line[index] == '$' || line[index] == '(' || line[index] == ')')){
            errorFlag = 1;
        }
    }
    /* Traitement du dernier argument */
    if(indexToBegin != index && !errorFlag){
        /* pas de délimiteur */
        if(isalnum(line[index-1])){
            arguments[argIndex] = str2int(line, beginArg, index);
            nbArgsFound = argIndex + 1;
        }else{ /* déjà traité (cas des LW et SW) */
            nbArgsFound = argIndex;
        }
    /* Pas d'argument ou drapeau d'erreur levé */
    }else{ 
        nbArgsFound = 0;
    }
    /* Retourne le nb d'arguments trouvés */
    return nbArgsFound;
}

/* Convertisseur de portions de chaînes de caractères en entiers signés */
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

int searchInstruction(char * opcode){
    int index = 0, instructionRank = -1, nbInstructions = sizeof(instructions)/sizeof(instruction);
    while(index < nbInstructions && instructionRank == -1){
        if(!strcmp(instructions[index].name, opcode)){
            instructionRank = index;
        }
        index++;
    }
    /* Retourne la position de l'instruction recherchée dans le tableau des instructions supportées,
     * Retourne -1 en cas d'échec
     */
    return instructionRank;
}

int validateArgs(int instructionRank, int arguments[], int nbArgs){
    int cnt = 0, field;
    /* Comptage du nombre de champs du registre ayant besoin d'être remplis avec
     * un argument de l'instruction.
     */
    for(field = 2; field <= INDEX_FIELD; field = field<<1){
        if(field != ROTATE_FIELD && instructions[instructionRank].type & field){
            cnt++;
        }
    }
    /* Si le compteur coincide avec le nbArgs retourne 1, sinon 0 */
    return cnt == nbArgs ? 1 : 0;
}


int getBinSegment(int instructionRank, int arguments[]){
    int segment = 0;
    /* L'application d'un tableau de karnaugh pourrait éventuellement factoriser 
        * certaines conditions au détriment de la lisibilité du code .
        */
    /* OPCODE */
    segment += (instructions[instructionRank].opcode << OPCODE_SHIFT) & OPCODE_MASK;
    if(instructions[instructionRank].type & FCT_FIELD){
        /* FUNCTION */
        segment += instructions[instructionRank].function & FCT_MASK;
    }
    if( !(instructions[instructionRank].type ^ (RS_FIELD|RT_FIELD|RD_FIELD|FCT_FIELD)) ){
        /* ADD,AND,OR,SLT,SUB,XOR */
        /* RS */
        segment += (arguments[1] << RS_SHIFT) & RS_MASK;
        /* RT */
        segment += (arguments[2] << RT_SHIFT) & RT_MASK;
        /* RD */
        segment += (arguments[0] << RD_SHIFT) & RD_MASK;
    }else if( !(instructions[instructionRank].type ^ (RS_FIELD|RT_FIELD|OFFSET_FIELD)) ){
        /* BEQ,BNE */
        /* RS */
        segment += (arguments[0] << RS_SHIFT) & RS_MASK;
        /* RT */
        segment += (arguments[1] << RT_SHIFT) & RT_MASK;
        /* OFFSET */
        segment += (arguments[2] << OFFSET_SHIFT) & OFFSET_MASK;
    }else if( !(instructions[instructionRank].type ^ (RS_FIELD|OFFSET_FIELD)) ){
        /* BGTZ, BLEZ */
        /* RS */
        segment += (arguments[0] << RS_SHIFT) & RS_MASK;
        /* OFFSET */
        segment += (arguments[1] << OFFSET_SHIFT) & OFFSET_MASK;
    }else if( !(instructions[instructionRank].type ^ (RS_FIELD|RT_FIELD|FCT_FIELD)) ){
        /* DIV,MUL */
        /* RS */
        segment += (arguments[0] << RS_SHIFT) & RS_MASK;
        /* RT */
        segment += (arguments[1] << RT_SHIFT) & RT_MASK;
    }else if( !(instructions[instructionRank].type ^ (RS_FIELD|RT_FIELD|IMMEDIATE_FIELD)) ){
        /* ADDI */
        /* RT */
        segment += (arguments[0] << RT_SHIFT) & RT_MASK;
        /* RS */
        segment += (arguments[1] << RS_SHIFT) & RS_MASK;
        /* IMMEDIATE */
        segment += (arguments[2] << IMMEDIATE_SHIFT) & IMMEDIATE_MASK;
    }else if( !(instructions[instructionRank].type ^ (RT_FIELD|IMMEDIATE_FIELD)) ){
        /* LUI */
        /* RT */
        segment += (arguments[0] << RT_SHIFT) & RT_MASK;
        /* IMMEDIATE */
        segment += (arguments[1] << IMMEDIATE_SHIFT) & IMMEDIATE_MASK;
    }else if( !(instructions[instructionRank].type ^ (BASE_FIELD|RT_FIELD|OFFSET_FIELD)) ){
        /* LW,SW */
        /* BASE */
        segment += (arguments[2] << BASE_SHIFT) & BASE_MASK;
        /* RT */
        segment += (arguments[0] << RT_SHIFT) & RT_MASK;
        /* OFFSET */
        segment += (arguments[1] << OFFSET_SHIFT) & OFFSET_MASK;
    }else if( !(instructions[instructionRank].type ^ (RD_FIELD|FCT_FIELD)) ){
        /* MFHI,MFLO */
        /* RD */
        segment += (arguments[0] << RD_SHIFT) & RD_MASK;
    }else if( !(instructions[instructionRank].type ^ INDEX_FIELD) ){
        /* J,JAL */
        /* INDEX */
        segment += (arguments[0] << INDEX_SHIFT) & INDEX_MASK;
    }else if( !(instructions[instructionRank].type ^ (RS_FIELD|HINT_FIELD|FCT_FIELD)) ){
        /* JR */
        /* RS */
        segment += (arguments[0] << RS_SHIFT) & RS_MASK;
        /* HINT : assuming we use MIPS I version, thus HINT = 0*/
    }else if( !(instructions[instructionRank].type ^ (RT_FIELD|RD_FIELD|SA_FIELD|FCT_FIELD)) ){
        /* SLL,SRL */
        /* RT */
        segment += (arguments[1] << RT_SHIFT) & RT_MASK;
        /* RD */
        segment += (arguments[0] << RD_SHIFT) & RD_MASK;
        /* SA */
        segment += (arguments[2] << SA_SHIFT) & SA_MASK;
    }else if( !(instructions[instructionRank].type ^ (ROTATE_FIELD|RT_FIELD|RD_FIELD|SA_FIELD|FCT_FIELD)) ){
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
    /* Retourne l'instruction encodée */
    return segment;
}
