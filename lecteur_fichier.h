#ifndef __LECTEUR__
#define __LECTEUR__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define EI_NIDENT 16
#define SHT_SYMTAB 2
#define SHT_STRTAB 3

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    unsigned int e_version;
    unsigned int e_entry;
    unsigned int e_phoff;
    unsigned int e_shoff;
    unsigned int e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} ELF_Header;

typedef struct{
    unsigned int sh_name;
    unsigned int sh_type;
    unsigned int sh_flags;
    unsigned int sh_addr;
    unsigned int sh_offset;
    unsigned int sh_size;
    unsigned int sh_link;
    unsigned int sh_info;
    unsigned int sh_addralign;
    unsigned int sh_entsize;
} Elf32_Section_Header;

typedef struct {
    int st_name;
    int st_value;
    int st_size;
    unsigned char st_info;
    unsigned char st_other;
    uint16_t st_shndx;
} ELF_Symbol;

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
void afficherHeader(ELF_Header *Header);

// -----------------------------------------------

//Permet de remplir le "Magic number"
void remplirMagic(FILE *fichier, ELF_Header *Header, int taille);

ELF_Header *init (FILE *fichier);

void init_section_header(FILE *fichier, uint16_t nb, unsigned int adrStart, Elf32_Section_Header *tab);

void afficher_sect(Elf32_Section_Header *tab, uint16_t nb);

ELF_Symbol *tableSymbol(FILE *fichier, Elf32_Section_Header *sectionHead, int tailleSectionTable);

ELF_Symbol *remplirSymbol(FILE *fichier, ELF_Symbol *table, int taille);

#endif