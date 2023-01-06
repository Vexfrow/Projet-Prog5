#ifndef __READER__
#define __READER__

#include <stdint.h>
#include <stdio.h>

typedef struct {
    unsigned char *fichier;
    int adr;
    int size;
} Lecteur;

Lecteur *init_lecture(FILE *fichier);
Lecteur *init_lecteur(int taille);
unsigned char lecture1octet(Lecteur *lecteur);

uint16_t lecture2octet(Lecteur *lecteur);
uint32_t lecture4octet(Lecteur *lecteur);

uint16_t bigEndianLecture2octet(Lecteur *lecteur);

uint32_t bigEndianLecture4octet(Lecteur *lecteur);


#endif 