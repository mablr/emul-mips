#include <stdio.h>
#include <stdlib.h>

/* Ouverture sécurisée de fichiers */ 
FILE * openFile(char * fileName, char * mode){
    FILE * file = fopen(fileName, mode);
    if(file == NULL){
        fprintf(stderr,"Unable to open %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    return file;
}

/* Fermeture sécurisée de fichiers */ 
void closeFile(char * fileName, FILE * file){
    if(fclose(file) ==  EOF){
        fprintf(stderr,"Unable to close %s\n", fileName);
        exit(EXIT_FAILURE);
    }
}