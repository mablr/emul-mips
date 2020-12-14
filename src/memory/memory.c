#include <stdio.h>
#include <stdlib.h>
#include "memory.h"


void insertWord(unsigned int address, int data, memory * mem){
    int elemStored = 0;
    wordMem *prevElem, *curElem = *mem;
    wordMem *elem = (wordMem *) malloc(sizeof(wordMem));
    /* Vérification malloc */
    if(elem == NULL)
        exit(EXIT_FAILURE);
    /* Affectation de la valeur de l'élément inséré */
    elem->address = address;
    elem->data = data;
    /* Si l'adresse du mot est correcte */
    if(address % 4 == 0 && address < MEM0RY_SIZE){
        /* Si la liste est vide */
        if(*mem == NULL){
            *mem = elem;
            elem->next = NULL;
        }
        /* Sinon si la valeur de l'adresse de l'élément à ajouter est 
        * inférieure à la valeur de l'adresse du 1er élément de la liste.
        * ==> Insertion en tête <==
        */
        else if(address < (*mem)->address){
            *mem = elem;
            elem->next = curElem;
        }
        /* Sinon détermination insertion en milieu ou queue
        * de liste.
        */
        else{
            /* Tant que la fin de la liste n'est pas atteinte ou
            * que l'élément n'est pas déjà inséré dans la liste.
            */
            while(curElem != NULL && !elemStored){
                /* Si la valeur de l'adresse de l'elem à insérer est inférieure
                * à la valeur de l'adresse de l'élément courant.
                * ==> Insertion en tête <==
                */
                if(address < curElem->address){
                    prevElem->next = elem;
                    elem->next = curElem;
                    elemStored = 1;
                /* Sinon si l'élément existe déjà (optimisation possible) */
                }else if(address == curElem->address){
                    /* libération du nouvel elem inutile + remplacement données */
                    curElem->data = data;
                    free(elem);
                    elemStored = 1;
                /* Sinon, avancer dans la memoire */
                }else{
                    prevElem = curElem;
                    curElem = curElem->next;
                }
            }
            /* Si la fin de la liste a été atteinte et l'élément
            * n'a pas été inséré.
            * ==> Insertion en queue <== 
            */
            if(!elemStored){
                prevElem->next = elem;
                elem->next = NULL;
            }
        }
    }
}

int getWord(unsigned int address, memory * mem){
    int data = 0, byteFound = 0;
    wordMem *elem = *mem;
    /* Tant que la liste n'est pas terminée ou que l'emplacement 
     * mémoire à l'adresse donnée n'est pas trouvé
     */
    while(elem != NULL && !byteFound){
        /* Si adresse trouvée */
        if(elem->address == address){
            /* Valeur contenue dans l'emplacement récupérée */
            data = elem->data;
            byteFound = 1;
        }
        elem = elem->next;
    }
    return data;
}


void deleteWord(unsigned int address, memory * mem){
    int delFlag = 0;
    wordMem *prevElem, *curElem = *mem;
    /* Si la liste n'est pas vide */
    if(curElem != NULL){
        /* Suppresion du premier élément */
        if(curElem->address == address){
            *mem = curElem->next;
            free(curElem); /* Libération mémoire */ 
        }else{
            /* Parcourir liste ... */
            while(curElem != NULL && !delFlag){
                prevElem = curElem;
                curElem = curElem->next;
                if(curElem->address == address){
                    delFlag = 1;
                }
            }
            if(delFlag){
                prevElem->next = curElem->next;
                free(curElem); /* Libération mémoire */
            }
        }
    }
}

void freeMem(memory * mem){
    wordMem *prevElem, *curElem = *mem;
    /* Avance jusqu'à la fin de la liste 
     * chaînée pour supprimer tout les éléments
     */ 
    while(curElem != NULL){
        prevElem = curElem;
        curElem = curElem->next;
        free(prevElem);
    }
}


int isAllocatedWord(unsigned int address, memory * mem){
    int byteFound = 0;
    wordMem *elem = *mem;
    /* Tant que la liste n'est pas terminée ou que l'emplacement 
     * mémoire à l'adresse donnée n'est pas trouvé
     */
    while(elem != NULL && !byteFound){
        /* Si adresse trouvée */
        if(elem->address == address){
            byteFound = 1;
        }
        elem = elem->next;
    }
    return byteFound;
}

void showMemory(memory * mem){
    int index, columnIndex = 0;
    /* Création d'un pointeur local pour ne pas modifier *mem */
    for(index = 0; index < NB_BLOCKS_TO_PRINT*4; index+=4){
        if(isAllocatedWord(index, mem)){
            printf("@%08x : 0x%08x", index, getWord(index, mem));
        }else{
            printf("@%08x : 0x%08x", index, 0);
        }
        columnIndex++;
        if(columnIndex == 4){
            printf("\n");
            columnIndex = 0;
        }else{
            printf("\t\t");
        }
    }
}

void showAllocatedSectors(memory * mem){
    /* Création d'un pointeur local pour ne pas modifier *mem */
    memory memPointer = *mem;
    if(memPointer == NULL){
        printf("Mémoire vide !\n");
    }else{
        while(memPointer != NULL){
            /* Affichage de la valeur */
            printf("@%08x : 0x%08x\n", memPointer->address, memPointer->data);
            /* Passage à l'élément suivant */
            memPointer = memPointer->next;
        }
    }
}