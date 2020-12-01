#include <stdio.h>
#include "register.h"
/*Pas faites :  Store Word , Load Word, Load Upper Immediate, Jump Register, Jump and Link, No Operation, SystemCall*/
/* Tableau d'entiers signés pour stocker les valeurs de registres */
int registers[NB_REGISTERS];
/* ADD WORD */
int add(int regs, int reg1, int reg2){
    int overflowStatus=0;
    int value = getRegister(reg1) + getRegister(reg2);
    if(value +1 == 0)
        overflowStatus = 1;
    if(!overflowStatus)
        storeRegister(regs, value);
    return overflowStatus;
}
/* Add Immediate Word */
int addi(int regs, int reg, int immediate){
    int overflowStatus=0;
    int value = getRegister(reg) + immediate;
    if(value +1 == 0)
        overflowStatus = 1;
    if(!overflowStatus)
        storeRegister(regs, value);
    return overflowStatus;
}
/* AND */
void and(int regs, int reg1, int reg2){
    int value = getRegister(reg1) & getRegister(reg2);
    storeRegister(regs,value);
}
/* Branch on Equal */
void beq(int reg1, int reg2, int offset){
    int condition = reg1 == reg2;
    if(condition)
        registers[32] += offset;
} 
/* Branch on Greater Than Zero */
void bgtz(int reg, int offset){
    int condition = getRegister(reg) > 0;
    if(condition)
        registers[32] += offset;
} 
/* Branch on Less Than or Equal to Zero */
void blez(int reg, int offset){
    int condition = getRegister(reg) <= 0;
    if(condition)
        registers[32] += offset;
} 
/* Branch on Not Equal */
void bne(int reg1, int reg2, int offset){
    int condition = getRegister(reg1) != getRegister(reg2);
    if(condition)
        registers[32] += offset;
} 
/* Divide Word */
void div(int reg1, int reg2){
    int reste, quotient;
    reste=getRegister(reg1)% getRegister(reg2);
    quotient= getRegister(reg1) / getRegister(reg2);
    storeRegister(34, quotient); /*quotient dans le registre LO*/
    storeRegister(33, reste);    /*reste dans le registre HI*/
}
/* Jump */
void jump(int targetAddress){
    storeRegister(32, targetAddress); 
}
/* Or */
void or(int regs, int reg1, int reg2){
    int value = getRegister(reg1) | getRegister(reg2);
    storeRegister(regs,value);
}
/* Rotate Word Right */
int rot(int regs, int reg, int bitRotate){
    int errorFlag=0;
    int value = getRegister(reg) >> bitRotate;
    storeRegister(regs, value);
    return errorFlag;
}
/* Shift Word Left Logical */
void sll(int regs, int reg, int bitRotate){
    int value = getRegister(reg) << bitRotate;
    storeRegister(regs, value);
}

/* Set on Less Than */ 
int slt(int regs, int reg1, int reg2){
    int status=0;
    if(getRegister(reg1) < getRegister(reg2))
        status=1;
    return status;
}
/*Subtract Word*/
int sub(int regs, int reg1, int reg2){
    int overflowStatus = 0;
    int value = getRegister(reg1) - getRegister(reg2);
    storeRegister(regs, value);
    return overflowStatus;
}
/* Exclusive OR*/
void xor(int regs, int reg1, int reg2){
    int value = getRegister(reg1) ^ getRegister(reg2);
    storeRegister(regs,value);
}
/*Multiply Word*/
void mult(int reg1, int reg2){
    long value = getRegister(reg1)*getRegister(reg2);
    registers[33]=(value & 0xFFFFFFFF00000000) >> 32;  /* Bits de poids forts dans HI*/
    registers[34]=(value & 0x00000000FFFFFFFF);        /* Bits de poids faibles dans LO*/
}
/*Move From LO Register */
void mflo(int reg){
    int value = getRegister(34);
    storeRegister(reg, value);
}
/*Move From HI Register*/
void mfhi(int reg){
    int value = getRegister(33);
    storeRegister(reg, value);
}


