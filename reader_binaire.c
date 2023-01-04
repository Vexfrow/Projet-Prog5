#include "reader_binaire.h"
#include <stdlib.h>


lecteur *init_lecture(FILE *fichier){
    lecteur *lecteur;
    int size = 0;
    int i = 0;
    fseek(fichier, 0, SEEK_END);
    lecteur->size = ftell(fichier);
    lecteur->adr = 0;
    lecteur->fichier=malloc(sizeof(char)*lecteur->size);
    if(!lecteur->fichier){
        free(lecteur->fichier);
        printf("Erreur d'initialisation du malloc fichier");
    }
    unsigned char octet;
    fseek(fichier,0, SEEK_SET);
    while(i<lecteur->size){
        fread(&octet,1,1,fichier);
        lecteur->fichier[i]=octet;
        i++;
    }
    return lecteur;
}


unsigned char lecture1octet(FILE *fichier){
    unsigned char res;
    fread(&res,1,1,fichier);
    return res;
}

uint16_t lecture2octet(FILE *fichier){
    uint16_t octet;
    fread(&octet, 2,1, fichier);
    return octet;
}

int lecture4octet(FILE *fichier){
    int entier;
    fread(&entier,4, 1, fichier);
    return entier;
}
