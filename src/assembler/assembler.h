#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define BUFFER_SIZE 100
#define MAX_OPCODE_SIZE 4
#define MAX_ARGS 3

#define OPCODE_MASK     0xFC000000
#define RS_MASK         0x03E00000
#define RT_MASK         0x001F0000
#define RD_MASK         0x0000F800
#define SA_MASK         0x000007C0
#define FCT_MASK        0x0000003F
#define OFFSET_MASK     0x0000FFFF
#define IMMEDIATE_MASK  0x0000FFFF
#define ROTATE_MASK     0x00200000

#define OPCODE_SHIFT 25
#define RS_SHIFT 21
#define RT_SHIFT 16
#define RD_SHIFT 11
#define SA_SHIFT 6


typedef struct instruction instruction;
struct instruction{
    char name[MAX_OPCODE_SIZE+1];
    char type;
    unsigned int opcode;
    unsigned int function;
};

void asm2hex(char * inputFileName, char * outputFileName);
FILE * openFile(char * fileName, char * mode);
void closeFile(char * fileName, FILE * file);
int purifyLine(char * line);
unsigned int translateAsm(char * line);
int extractOpcode(char * line, char * opcode);
int extractArgs(char * line, int indexToBegin, int arguments[]);
int str2int(char * str, int beginChar, int endChar);
unsigned int getBinSegment(char * opcode, int arguments[], int nbArgs);
#endif
