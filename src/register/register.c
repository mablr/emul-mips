#include <stdio.h>
#include "register.h"

/* Tableau d'entiers signés pour stocker les valeurs de registres */
int registers[NB_REGISTERS];

/* Stocke la valeur donnée si le registre indiqué existe */
void storeRegister(int index, int value){
    if(index > 0 && index < NB_REGISTERS){
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
/* Affiche les registres */
void showRegisters(void){
    int i, columnIndex = 0;
    for(i = 0; i < GENERAL_REGISTERS; i++){
        /* Affichage avec une largeur fixe du contenu de chaque registre */
        printf("$%02d : %-*d", i, 12, registers[i]);
        columnIndex++;
        if(columnIndex == 4){
            printf("\n");
            columnIndex = 0;
        }else{
            printf("\t");
        }
    }
    printf("\nHI : %d \nLO : %d \nPC : %d\n\n", registers[33], registers[34], registers[32]);
}
