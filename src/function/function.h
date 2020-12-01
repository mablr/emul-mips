#include <stdio.h>
#include "register.h"

int add(int regs, int reg1, int reg2);
int addi(int regs, int reg, int immediate);
void and_(int regs, int reg1, int reg2);
void beq(int reg1, int reg2, int offset);
void bgtz(int reg, int offset);
void blez(int reg, int offset);
void bne(int reg1, int reg2, int offset);
void div(int reg1, int reg2);
void jump(int targetAddress);
void or_(int regs, int reg1, int reg2);
int rot(int regs, int reg, int bitRotate);
void sll(int regs, int reg, int bitRotate);
int slt(int regs, int reg1, int reg2);
int sub(int regs, int reg1, int reg2);
int xor_(int regs, int reg1, int reg2);
void mult(int reg1, int reg2);
void mflo(int reg);
void mfhi(int reg);
void nop();
void lw(int reg, int targetAddress);
void sw(int reg, int targetAddress);
void lui(int reg, int immediate);
void jr(int reg);
void jal(int reg);