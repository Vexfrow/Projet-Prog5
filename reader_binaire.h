#ifndef __READER__
#define __READER__


#include <stdint.h>
#include <stdio.h>

unsigned char lectureOctet(FILE *fichier);

uint16_t lecture2octet(FILE *fichier);

int lecture4octet(FILE *fichier);

#endif 