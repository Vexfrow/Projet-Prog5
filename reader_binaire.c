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

uint16_t littleEndianLecture2octet(FILE *fichier){
    uint16_t octet = 0;
    char tmp;
    fread(&tmp, 1,1, fichier);
    octet |= ((uint16_t)tmp << 8);
    fread(&tmp, 1,1, fichier);
    octet |= ((uint16_t)tmp);
    return octet;
}

int littleEndianLecture4octet(FILE *fichier){
    int entier = 0;
    char tmp;
    fread(&tmp, 1,1, fichier);
    entier |= ((int)tmp << 24);
    fread(&tmp, 1,1, fichier);
    entier |= ((int)tmp << 16);
    fread(&tmp, 1,1, fichier);
    entier |= ((int)tmp << 8);
    fread(&tmp, 1,1, fichier);
    entier |= ((int)tmp);
    printf("%x\n",entier);
    return entier;

}