#ifndef __READER__
#define __READER__


#include <stdint.h>
#include <stdio.h>

typedef struct {
    unsigned char *fichier;
    int adr;
    int size;
} lecteur;

lecteur *init_lecture(FILE *fichier);

unsigned char lecture1octet(lecteur *lecteur);

uint16_t lecture2octet(lecteur *lecteur);
uint32_t lecture4octet(lecteur *lecteur);

uint16_t littleEndianLecture2octet(FILE *fichier);

int littleEndianLecture4octet(FILE *fichier);


#endif 