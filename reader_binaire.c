#include "reader_binaire.h"

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
