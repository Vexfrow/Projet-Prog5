#ifndef __READER__
#define __READER__


#include <stdint.h>
#include <stdio.h>

typedef struct {
    char *fichier;
    int adr;
    int size;
} lecteur;

lecteur *init_lecture(FILE *fichier);

unsigned char lecture1octet(FILE *fichier);

uint16_t lecture2octet(FILE *fichier);

int lecture4octet(FILE *fichier);

#endif 