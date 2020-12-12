#ifndef MODE_H
#define MODE_H
#include "../memory/memory.h"

void interactiveMode();
void simpleMode(char * asmFile);
void stepMode(char * asmFile);
void loadProgMem(char * asmFile, int step, memory * mem);
void runInstructions(int step, memory * instructionsMem, memory * progMem);
#endif