#ifndef __READER__
#define __READER__

#include <stdint.h>
#include <stdio.h>

typedef struct {
    unsigned char *fichier;
    int adr;
    int size;
} Lecteur;

//----------------------------------------------------------


//Création d'une structure Lecteur à partir d'un fichier
Lecteur *init_lecture(FILE *fichier);

//Création d'une structure Lecteur vide à partir de sa taille
Lecteur *init_lecteur(int taille);

//Permet de crée un fichier à partir du contenue d'un Lecteur
void ecrireFichier(Lecteur *lect);

//----------------------------------------------------------

//Permet de lire 1 octet
unsigned char lecture1octet(Lecteur *lecteur);

//Permet de lire 2 octets
uint16_t lecture2octet(Lecteur *lecteur);

//Permet de lire 4 octets
uint32_t lecture4octet(Lecteur *lecteur);

//Permet de lire 2 octets en big endian
uint16_t bigEndianLecture2octet(Lecteur *lecteur);

//Permet de lire 4 octets en big endian
uint32_t bigEndianLecture4octet(Lecteur *lecteur);




#endif 