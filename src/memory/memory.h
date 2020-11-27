#ifndef MEMORY_H
#define MEMORY_H
#define MEM0RY_SIZE 1<<28
#define NB_BLOCKS_TO_PRINT 20

typedef struct wordMem wordMem;
struct wordMem{
    unsigned int address;
    int data;
    wordMem * next;
};
typedef wordMem * memory;

/* Ajoute un mot de données (4 octets) dans la mémoire à l'adresse indiquée */
void insertWord(unsigned int address, int data, memory * mem);

/* Récupère en mémoire un mot de données à l'adresse indiquée */
int getWord(unsigned int address, memory * mem);

/* Libère l'emplacement mémoire à l'adresse indiquée */
void deleteWord(unsigned int address, memory * mem);

/* Vérifie si un emplacement mémoire est alloué */
int isAllocatedWord(unsigned int address, memory * mem);

/* Affiche une représentation de la mémoire */
void showMemory(memory * mem);
void showAllocatedSectors(memory * mem);

#endif