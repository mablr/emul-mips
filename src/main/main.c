#include <stdio.h>
#include <string.h>
#include "../assembler/assembler.h"

int main(int argc, char * argv[]){
    if(argc == 1){
        /* Mode interactif */
        interactiveMode();
    }else if(argc >= 2 && argc <= 3){
        /* Mode non-interactif */
        simpleMode(argv[1]);
    }else{
        printf("Wrong number of arguments.\n");
    }
    if(argc == 3 && !strcmp(argv[2],"-pas")){
        stepMode(argv[1]);
    }
    return 0;
}
