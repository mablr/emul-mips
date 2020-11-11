#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define BUFFER_SIZE 100
#define MAX_OPCODE_SIZE 4
#define MAX_ARGS 3

#define OPCODE_MASK     0xFC000000
#define INDEX_MASK      0x03FFFFFF
#define BASE_MASK       0x03E00000
#define RS_MASK         0x03E00000
#define ROTATE_MASK     0x00200000
#define RT_MASK         0x001F0000
#define OFFSET_MASK     0x0000FFFF
#define IMMEDIATE_MASK  0x0000FFFF
#define RD_MASK         0x0000F800
#define SA_MASK         0x000007C0
#define HINT_MASK       0x000007C0
#define FCT_MASK        0x0000003F

#define OPCODE_SHIFT    26
#define INDEX_SHIFT     0
#define BASE_SHIFT      21
#define RS_SHIFT        21
#define ROTATE_SHIFT    21
#define RT_SHIFT        16
#define OFFSET_SHIFT    0
#define IMMEDIATE_SHIFT 0
#define RD_SHIFT        11
#define SA_SHIFT        6
#define HINT_SHIFT      6
#define FCT_SHIFT       0

#define INDEX_FIELD     1024
#define BASE_FIELD      512
#define RS_FIELD        256
#define ROTATE_FIELD    128
#define RT_FIELD        64
#define OFFSET_FIELD    32
#define IMMEDIATE_FIELD 16
#define RD_FIELD        8
#define SA_FIELD        4
#define HINT_FIELD      2
#define FCT_FIELD       1

/*
 * Le champ type de la structure indique comment doit être encodée
 * l'instruction. La valeur est obtenue par un ET bit à bit des 
 * valeurs de "XXX_FIELD" définis ci-dessus.
 */
typedef struct instruction instruction;
struct instruction{
    char name[MAX_OPCODE_SIZE+1];
    unsigned int type;
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
int searchInstruction(char * opcode);
int validateArgs(int instructionRank, int arguments[], int nbArgs);
unsigned int getBinSegment(int instructionRank, int arguments[]);

#endif
