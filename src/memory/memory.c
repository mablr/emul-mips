#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

void insertByteMem(unsigned int address, unsigned char data, memory * mem){
    int elemStored = 0;
    byteMem *prevElem = *mem, *curElem = *mem;
    byteMem *elem = (byteMem *) malloc(sizeof(byteMem));
    
    /* Vérification malloc */
    if(elem == NULL)
        exit(EXIT_FAILURE);

    /* Si la liste est vide */
    if(*mem == NULL){
        *mem = elem;
        elem->next = NULL;
    }
    /* Sinon si la valeur de l'élément à ajouter est 
     * inférieure à la valeur du 1er élément de la liste.
     * ==> Insertion en tête <==
     */
    else if(address <= (*mem)->address){
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
            /* Si la valeur de l'élément à insérer est inférieure
             * à la valeur de l'élément courant.
             * ==> Insertion en tête <==
             */
            if(address <= curElem->address){
                prevElem->next = elem;
                elem->next = curElem;
                elemStored = 1;
            /* Sinon, avancer dans la memory */
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
    /* Affectation de la valeur de l'élément inséré */
    elem->address = address;
    elem->data = data;

}

void insertWord(unsigned int address, unsigned int data, memory * mem){
    /* Ajout d'un mot en mémoire en big endian */
    insertByteMem(address, data & 0xFF000000, mem);
    insertByteMem(address + 1, data & 0xFF0000, mem);
    insertByteMem(address + 2, data & 0xFF00, mem);
    insertByteMem(address + 3, data & 0xFF, mem);
}

unsigned char getByteMem(unsigned int address, memory * mem){
    unsigned char data = 0, byteFound = 0;
    byteMem *elem = *mem;
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

unsigned int getWord(unsigned int address, memory * mem){
    unsigned int word = 0;
    word += getByteMem(address, mem) << 24;
    word += getByteMem(address+1, mem) << 16;
    word += getByteMem(address+2, mem) << 8;
    word += getByteMem(address+3, mem);
    return word;
}

void deleteByteMem(unsigned int address, memory * mem){
}

int isAllocatedByteMem(unsigned int address, memory * mem){
    int byteFound = 0;
    byteMem *elem = *mem;
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
    int index;
    /* Création d'un pointeur local pour ne pas modifier *mem */
    for(index = 0; index < NB_BLOCKS_TO_PRINT*4; index+=4){
        if(isAllocatedByteMem(index, mem)){
            printf("@0x%x08 : %d\n", index, getWord(index, mem));
        }else{
            printf("@0x%x08 : 0", index);
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
            printf("@%d : 0x%x02\n", memPointer->address, memPointer->data);
            /* Passage à l'élément suivant */
            memPointer = memPointer->next;
        }
    }
}