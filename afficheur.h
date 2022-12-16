#ifndef __AFFICHEUR__
#define __AFFICHEUR__
#include "lecteur_fichier.h"
#include <stdio.h>
#include <stdlib.h>
#include "lecteur_fichier.h"

void afficherMagic(ELF_Header *Header, int taille);

void afficherSymbol(ELF_Symbol *table, int taille);

void afficher_sect(Elf32_Section_Header *tab, uint16_t nb); // il sera dégagé plus tard

void afficher_sh_name(unsigned int name);

void afficher_sh_type(unsigned int type);

void afficher_sh_addr(unsigned int addr);

void afficher_sh_offset(unsigned int offset);

void afficher_sh_size(unsigned int size);

void afficher_sh_link(unsigned int link);

void afficher_sh_info(unsigned int info);

void afficher_sh_addralign(unsigned int addralign);

void afficher_sh_entsize(unsigned int entsize);

void afficher_sh_flags(unsigned int flags);

void afficher_section(Elf32_Section_Header *tab, uint16_t nb);




// Affichage de l'Header: ------------------------

#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2
//Permet de récupèrer la classe de l'ELF
char *getClass(unsigned char c);

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2
//Permet de récuperer la manière dont sont encodées les données
char *getDataEncoding(unsigned char c);

#define EV_NONE 0
#define EV_CURRENT 1
//Permet de récuperer la version du fichier
char *getVersion(unsigned char c);

//TODO MAXIME
//Permet de récuperer la manière dont sont encodées les données
char *getOSABI(unsigned char c);

#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff
//Permet de récuperer le type
char *getType(uint16_t c);

#define EM_NONE 0
#define EM_M32 1 
#define EM_SPARC 2
#define EM_386 3
#define EM_68K 4
#define EM_88K 5
#define EM_860 7
#define EM_MIPS 8
#define EM_MIPS_RS4_BE 10
#define RESERVED 11 ... 16
//Permet de récuperer la machine
char *getMachine(uint16_t c);

#define EI_MAG0 0x00
#define EI_MAG1 0x01
#define EI_MAG2 0x02
#define EI_MAG3 0x03
#define EI_CLASS 0x04
#define EI_DATA 0x05
#define EI_VERSION 0x06
#define EI_OSABI 0x07
#define EI_ABIVERSION 0x08
#define EI_PAD 0x09
// Affiche le header (similaire à 'arm-none-eabi-readelf -a {file_name}.o')
void afficher_header(ELF_Header *Header);


#endif