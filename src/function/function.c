#include <stdio.h>
#include "../register/register.h"
#include "../mode/mode.h"

/* #ADD WORD */
int add(int args[]){
    storeRegister(args[0], getRegister(args[1]) + getRegister(args[2]));
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Add Immediate Word */
int addi(int args[]){
    storeRegister(args[0], getRegister(args[1]) + args[2]);
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #AND */
int and_(int args[]){
    storeRegister(args[0], getRegister(args[1]) & getRegister(args[2]));
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Branch on Equal */
int beq(int args[]){
    if(getRegister(args[0]) == getRegister(args[1])){
        storeRegister(PC, getRegister(PC) + args[2]);
    }else{
        storeRegister(PC, getRegister(PC) + 4);
    }
    return 0;
} 
/* #Branch on Greater Than Zero */
int bgtz(int args[]){
    if(getRegister(args[0]) > 0){
        storeRegister(PC, getRegister(PC) + args[1]);
    }else{
        storeRegister(PC, getRegister(PC) + 4);
    }
    return 0;
} 
/* #Branch on Less Than or Equal to Zero */
int blez(int args[]){
    if(getRegister(args[0]) <= 0){
        storeRegister(PC, getRegister(PC) + args[1]);
    }else{
        storeRegister(PC, getRegister(PC) + 4);
    }
    return 0;
} 
/* #Branch on Not Equal */
int bne(int args[]){
    if(getRegister(args[1]) != getRegister(args[2])){
        storeRegister(PC, getRegister(PC) + args[2]);
    }else{
        storeRegister(PC, getRegister(PC) + 4);
    }
    return 0;
} 
/* #Divide Word */
int div_(int args[]){
    int exception = 0;
    if(args[1] != 0){
        /* quotient dans le registre LO */
        storeRegister(LO, getRegister(args[0]) / getRegister(args[1]));
        /* reste dans le registre HI */
        storeRegister(HI, getRegister(args[0]) % getRegister(args[1]));
    }else{
        exception = 1;
    }
    storeRegister(PC, getRegister(PC) + 4);
    return exception;
}
/* #Jump */
int j(int args[]){
    /*PC+1  = GPRLEN-1..28 || instr_index || 0^2*/
    storeRegister(PC, (getRegister(PC) & 0xF0000000) | (args[0]<<2 & 0x0FFFFFFC));
    return 0;
}
/* #Jump and Link */
int jal(int args[]){
    /* Return Adress */
    storeRegister(31, getRegister(PC) + 8);
    /* Comme un JUMP normal */
    storeRegister(PC, (getRegister(PC) & 0xF0000000) | (args[0]<<2 & 0x0FFFFFFC));
    return 0;
}
/* XJump Register */
int jr(int args[]){
    /* Alignement de force de l'adresse contenue dans le registre en argument,
     * sans exception comme dans MIPS I, le champ HINT étant ignoré (tjr à 0).
     */
    storeRegister(PC, getRegister(args[0]) & 0x0FFFFFFC);
    return 0;
}
/* #Load Upper Immediate */
int lui(int args[]){
    storeRegister(args[0], args[1]<<16);
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Load Word*/
int lw(int args[]){
    unsigned int address = args[1] + getRegister(args[2]);
    /* Alignée de force sur un mot */
    address -= address%4;
    /* Chargement de la valeur en mémoire vers le registre */
    storeRegister(args[0], getWord(address, &progMem));
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Move From HI Register*/
int mfhi(int args[]){
    storeRegister(args[0], getRegister(HI));
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Move From LO Register */
int mflo(int args[]){
    storeRegister(args[0], getRegister(LO));
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Multiply Word*/
int mult(int args[]){
    long int value = (long int)getRegister(args[0]) * (long int)getRegister(args[1]);
    /* Bits de poids forts dans HI*/
    storeRegister(HI, (value & 0xFFFFFFFF00000000) >> 32);
    /* Bits de poids faibles dans LO*/
    storeRegister(LO, value & 0x00000000FFFFFFFF);
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #No Operation*/
int nop(int args[]){
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Or */
int or_(int args[]){
    storeRegister(args[0], getRegister(args[1]) | getRegister(args[2]));
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* Rotate Word Right */
int rotr(int args[]){
    /* value = GPR[rt] s-1..0 || GPR[rt] 31..s */
    int value = (getRegister(args[1]) << (31 - getRegister(args[2]))) + (getRegister(args[1]) >> getRegister(args[2]));
    storeRegister(args[0], value);
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* Shift Word Left Logical */
int sll(int args[]){
    storeRegister(args[0], getRegister(args[1]) << args[2]);
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Set on Less Than */ 
int slt(int args[]){
    int value = getRegister(args[1]) < getRegister(args[2]) ? 1 : 0;
    storeRegister(args[0], value);
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* Shift Word Right Logical */
int srl(int args[]){
    storeRegister(args[0], getRegister(args[1]) >> args[2]);
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Subtract Word*/
int sub(int args[]){
    storeRegister(args[0], getRegister(args[1]) - getRegister(args[2]));
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Store Word*/
int sw(int args[]){
    unsigned int address = args[1] + getRegister(args[2]);
    /* Alignée de force sur un mot */
    address -= address%4;
    /* Stockage de la valeur du registre rt en mémoire */
    insertWord(address, getRegister(args[0]), &progMem);
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
/* #Exclusive OR*/
int xor_(int args[]){
    storeRegister(args[0], getRegister(args[1]) ^ getRegister(args[2]));
    storeRegister(PC, getRegister(PC) + 4);
    return 0;
}
