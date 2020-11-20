#ifndef MEMORY_H
#define MEMORY_H
#define MEM0RY_SIZE 1<<32

typedef struct byteMem byteMem;
struct byteMem{
    unsigned int address;
    unsigned char data;
    byteMem * nextByteMem;
};
typedef byteMem * memory;

/* Ajoute un octet de données dans la mémoire à l'adresse indiquée */
void insertByteMem(unsigned int address, unsigned char data, liste * memory);

/* Ajoute un mot de données (4 octets) dans la mémoire à l'adresse indiquée */
void insertWord(unsigned int address, unsigned int data, liste * memory);

/* Récupère en mémoire un octet de données à l'adresse indiquée */
unsigned char getByteMem(unsigned int address, liste * memory);

/* Récupère en mémoire un mot de données à l'adresse indiquée */
unsigned int getWord(unsigned int address, liste * memory);

/* Libère l'emplacement mémoire à l'adresse indiquée */
void deleteByteMem(unsigned int address, liste * memory);

/* Vérifie si un emplacement mémoire est alloué */
unsigned int isAllocatedByteMem(unsigned int address, liste * memory);

/* Affiche une représentation de la mémoire */
void showMemory(liste * memory);
#endif