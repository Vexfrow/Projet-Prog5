#ifndef __AFFICHEUR__
#define __AFFICHEUR__
#include "lecteur_fichier.h"
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

void afficherMagic(ELF_Header *Header, int taille);

void afficherSymbol(ELF_Symbol *table, int taille, FILE *fichier, Elf32_Section_Header *tab);

void afficher_sh_name(char* name);

void afficher_sh_type(unsigned int type);

void afficher_sh_addr(unsigned int addr);

void afficher_sh_offset(unsigned int offset);

void afficher_sh_size(unsigned int size);

void afficher_sh_link(unsigned int link);

void afficher_sh_info(unsigned int info);

void afficher_sh_addralign(unsigned int addralign);

void afficher_sh_entsize(unsigned int entsize);

void afficher_sh_flags(unsigned int flags);

void afficher_sectiontable(Elf32_Section_Header *tab, uint16_t nb, FILE *fichier);

void afficher_section(Elf32_Section_Header *tab , int nb ,FILE *fichier);

char* getName(FILE *fichier, unsigned int address);


// Affichage de l'Header: ------------------------

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