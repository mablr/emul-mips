#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define BUFFER_SIZE 100
#define MAX_INSTRUCTION_SIZE 4
#define MAX_ARGS 3

void asm2hex(char * inputFileName, char * outputFileName);
FILE * openFile(char * fileName, char * mode);
void closeFile(char * fileName, FILE * file);
int purifyLine(char * line);
unsigned int translateAsm(char * line);
int extractInstruction(char * line, char * instruction);
int extractArgs(char * line, int indexToBegin, int arguments[]);
int str2int(char * str, int beginChar, int endChar);
unsigned int getBinSegment(char * instruction, int arguments[], int nbArgs);
#endif
