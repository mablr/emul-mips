#include <stdio.h>
#include "../register/register.h"

/*Pas faites :  Store Word , Load Word, Load Upper Immediate, Jump Register, Jump and Link, No Operation*/

/* ADD WORD */
int add(int args[]){
    int overflowStatus=0;
    long fullResult = getRegister(args[1]) + getRegister(args[2]);
    int value;
    /* Si débordement (overflow) */
    if(fullResult & 0xFFFFFFFF00000000){
        overflowStatus = 1;
    }else{
        value = fullResult & 0xFFFFFFFF;
        storeRegister(args[0], value);
    }
    return overflowStatus;
}
/* Add Immediate Word */
int addi(int args[]){
    int overflowStatus=0;
    int value = getRegister(args[1]) + args[2];
    if(value +1 == 0)
        overflowStatus = 1;
    if(!overflowStatus)
        storeRegister(args[0], value);
    return overflowStatus;
}
/* AND */
int and_(int args[]){
    int value = getRegister(args[1]) & getRegister(args[2]);
    storeRegister(args[0],value);
    return 0;
}
/* Branch on Equal */
int beq(int args[]){
    if(getRegister(args[0]) == getRegister(args[1]))
        registers[32] += getRegister(args[2]);
    return 0;
} 
/* Branch on Greater Than Zero */
int bgtz(int args[]){
    if(getRegister(args[0]) > 0)
        registers[32] += getRegister(args[1]);
    return 0;
} 
/* Branch on Less Than or Equal to Zero */
int blez(int args[]){
    if(getRegister(args[0]) <= 0)
        registers[32] += getRegister(args[1]);
    return 0;
} 
/* Branch on Not Equal */
int bne(int args[]){
    int condition = getRegister(args[1]) != getRegister(args[2]);
    if(condition)
        registers[32] += getRegister(args[2]);
    return 0;
} 
/* Divide Word */
int div_(int args[]){
    int reste, quotient;
    reste=getRegister(args[1])% getRegister(args[2]);
    quotient= getRegister(args[1]) / getRegister(args[2]);
    storeRegister(LO, quotient); /*quotient dans le registre LO*/
    storeRegister(HI, reste);    /*reste dans le registre HI*/
    return 0;
}
/* Jump */
int j(int args[]){
    storeRegister(32, getRegister(args[0])); 
    return 0;
}
/* Or */
int or_(int args[]){
    int value = getRegister(args[1]) | getRegister(args[2]);
    storeRegister(args[0],value);
    return 0;
}
/* Rotate Word Right */
int rotr(int args[]){
    int errorFlag=0;
    int value = getRegister(args[1]) >> getRegister(args[2]);
    storeRegister(args[0], value);
    return errorFlag;
}
/* Shift Word Left Logical */
int sll(int args[]){
    int value = getRegister(args[1]) << getRegister(args[2]);
    storeRegister(args[0], value);
    return 0;
}

/* Shift Word Right Logical */
int srl(int args[]){
    int value = getRegister(args[1]) >> getRegister(args[2]);
    storeRegister(args[0], value);
    return 0;
}

/* Set on Less Than */ 
int slt(int args[]){
    int status=0;
    if(getRegister(args[1]) < getRegister(args[2]))
        status=1;
    return status;
}
/*Subtract Word*/
int sub(int args[]){
    int overflowStatus = 0;
    int value = getRegister(args[1]) - getRegister(args[2]);
    storeRegister(args[0], value);
    return overflowStatus;
}
/* Exclusive OR*/
int xor_(int args[]){
    int value = getRegister(args[1]) ^ getRegister(args[2]);
    storeRegister(args[0],value);
    return 0;
}
/*Multiply Word*/
int mult(int args[]){
    long value = getRegister(args[1])*getRegister(args[2]);
    registers[HI]=(value & 0xFFFFFFFF00000000) >> 32;  /* Bits de poids forts dans HI*/
    registers[LO]=(value & 0x00000000FFFFFFFF);        /* Bits de poids faibles dans LO*/
    return 0;
}
/*Move From LO Register */
int mflo(int args[]){
    storeRegister(args[0], getRegister(args[LO]));
    return 0;
}
/*Move From HI Register*/
int mfhi(int args[]){
    storeRegister(args[0], getRegister(args[HI]));
    return 0;
}
/*No Operation*/
int nop(int args[]){
    return 0;
}
/*Load Word*/
int lw(int args[]){
    return 0;
}
/*Store Word*/
int sw(int args[]){
    return 0;
}
/* Load Upper Immediate */
int lui(int args[]){
    return 0;
}
/* Jump Register */
int jr(int args[]){
    return 0;
}
/* Jump and Link */
int jal(int args[]){
    return 0;
}