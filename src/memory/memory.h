#ifndef MEMORY_H
#define MEMORY_H
#define MEM0RY_SIZE 1<<32
#define NB_BLOCKS_TO_PRINT 20

typedef struct byteMem byteMem;
struct byteMem{
    unsigned int address;
    unsigned char data;
    byteMem * next;
};
typedef byteMem * memory;

/* Ajoute un octet de données dans la mémoire à l'adresse indiquée */
void insertByteMem(unsigned int address, unsigned char data, memory * mem);

/* Ajoute un mot de données (4 octets) dans la mémoire à l'adresse indiquée */
void insertWord(unsigned int address, unsigned int data, memory * mem);

/* Récupère en mémoire un octet de données à l'adresse indiquée */
unsigned char getByteMem(unsigned int address, memory * mem);

/* Récupère en mémoire un mot de données à l'adresse indiquée */
unsigned int getWord(unsigned int address, memory * mem);

/* Libère l'emplacement mémoire à l'adresse indiquée */
void deleteByteMem(unsigned int address, memory * mem);

/* Vérifie si un emplacement mémoire est alloué */
int isAllocatedByteMem(unsigned int address, memory * mem);

/* Affiche une représentation de la mémoire */
void showMemory(memory * mem);
void showAllocatedSectors(memory * mem);

#endif