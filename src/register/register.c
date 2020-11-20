#include <stdio.h>
#include "register.h"

/* Tableau d'entiers signés pour stocker les valeurs de registres */
int registers[NB_REGISTERS];

/* Stocke la valeur donnée si le registre indiqué existe */
void storeRegister(int index, int value){
    if(index >= 0 && index < NB_REGISTERS){
        registers[index] = value;
    }
}

/* Retourne valeur dans le registre indiqué s'il existe */
int getRegister(int index){
    int value = 0;
    if(index >= 0 && index < NB_REGISTERS){
        value = registers[index];
    }
    return value;
}