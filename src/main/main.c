#include <stdio.h>
#include "../assembler/assembler.h"

int main(int argc, char * argv[]){
    if(argc == 3){
        asm2hex(argv[1], argv[2]);
    }else{
        printf("Wrong number of arguments.\n");
    }
    return 0;
}
