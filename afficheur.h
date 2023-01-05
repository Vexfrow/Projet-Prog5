#ifndef __AFFICHEUR__
#define __AFFICHEUR__
#include "lecteur_fichier.h"
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>


//------------------------- SYMBOL TABLE -------------------------------------------

void afficherSymbol(lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *section_header_tab, ELF_Symbol *symbol_table);

char *getBinding(char bind);

char *getStType(char vis);

char *calculNdx(uint16_t ndx, int taille );

//------------------------- SECTION HEADER  -------------------------------------------

//Permet de récupèrer le type de la section
char* getShType(unsigned int type);

//Permet d'afficher les flags
void afficher_sh_flags(unsigned int flags);

//Permet d'afficher la table des sections 
void afficher_section_table(lecteur *lecteur, ELF_Header *Header, Elf32_Section_Header *tab);


//Permet d'afficher le contenue d'une fonction à partir de son idex dans la table des sections 
void afficher_section(Elf32_Section_Header *tab , int nb ,lecteur *lecteur);

//------------------------- HEADER -------------------------------------------

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

//Permet de récuperer la machine
char *getMachine(uint16_t c);

// Affiche le header (similaire à 'arm-none-eabi-readelf -h {file_name}.o')
void afficher_header(ELF_Header *Header);

//------------------------- RELOCATION TABLE -------------------------------------------

void afficherRelocations(Elf32_Section_Header *Rel_section_tab ,ELF_Rel *ELF_tab, ELF_Symbol *sym, int nb_ELF, int nb_section, lecteur *lecteur);

void affichertypereloc(unsigned char t);


#endif