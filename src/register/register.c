#include <stdio.h>
#include "register.h"

int registers[NB_REGISTERS];

void storeRegister(int index, int value){
    if(index >= 0 && index < NB_REGISTERS){
        registers[index] = value;
    }
}
int getRegister(int index){
    int value = 0;
    if(index >= 0 && index < NB_REGISTERS){
        value = registers[index];
    }
    return value;
}