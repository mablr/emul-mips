#include <stdio.h>
#include <string.h>
#include "../mode/mode.h"

int main(int argc, char * argv[]){
    if(argc == 1){
        /* Mode interactif */
        interactiveMode();
    }else if(argc == 2){
        /* Mode non-interactif */
        simpleMode(argv[1]);
    }else if(argc == 3 && !strcmp(argv[2],"-pas")){
        /* Mode pas à pas */
        stepMode(argv[1]);
    }else{
        printf("Wrong arguments.\n");
    }

    return 0;
}
