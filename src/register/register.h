#ifndef REGISTER_H
#define REGISTER_H

#define GENERAL_REGISTERS 32
#define NB_REGISTERS 35

void storeRegister(int index, int value);
int getRegister(int index);

#endif