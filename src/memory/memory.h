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

void insertWord(unsigned int address, unsigned int data, liste * memory);
void insertByteMem(unsigned int address, unsigned char data, liste * memory);
unsigned int getWord(unsigned int address, liste * memory);
unsigned char getByteMem(unsigned int address, liste * memory);
void deleteWord(unsigned int address, liste * memory);
void deleteByteMem(unsigned int address, liste * memory);
void showMemory(liste * memory);
#endif