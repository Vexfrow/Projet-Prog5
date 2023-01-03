#ifndef __AFFICHEUR__
#define __AFFICHEUR__
#include "lecteur_fichier.h"
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>



void afficherSymbol(ELF_Symbol *table, int taille, FILE *fichier, Elf32_Section_Header *tab, int tailleSectionHeader);


//Permet de récupèrer le type de la section
char* getShType(unsigned int type);

//Permet d'afficher les flags
void afficher_sh_flags(unsigned int flags);


//Permet d'afficher la table des sections 
void afficher_section_table(Elf32_Section_Header *tab, uint16_t nb, FILE *fichier);


//Permet d'afficher le contenue d'une fonction à partir de son idex dans la table des sections 
void afficher_section(Elf32_Section_Header *tab , int nb ,FILE *fichier);


//Permet de récupèrer un string à partir d'un fichier et de sa position dans ce dit fichier
char* getName(FILE *fichier, unsigned int address);


// Affichage de l'Header: ------------------------

void afficherMagic(ELF_Header *Header, int taille);

//Permet de récupèrer la classe de l'ELF
char *getClass(unsigned char c);

//Permet de récuperer la manière dont sont encodées les données
char *getDataEncoding(unsigned char c);

//Permet de récuperer la version du fichier
char *getVersion(unsigned char c);

//TODO MAXIME
//Permet de récuperer la manière dont sont encodées les données
char *getOSABI(unsigned char c);

//Permet de récuperer le type
char *getType(uint16_t c);


#define EM_MIPS_RS4_BE 10
#define RESERVED 11 ... 16
//Permet de récuperer la machine
char *getMachine(uint16_t c);

// Affiche le header (similaire à 'arm-none-eabi-readelf -h {file_name}.o')
void afficher_header(ELF_Header *Header);


#endif