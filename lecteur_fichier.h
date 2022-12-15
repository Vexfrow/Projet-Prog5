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


typedef struct {
    int st_name;
    int st_value;
    int st_size;
    unsigned char st_info;
    unsigned char st_other;
    uint16_t st_shndx;
} ELF_Symbol;


void afficher(ELF_Header *Header, int taille);

void remplirMagic(FILE *fichier, ELF_Header *Header, int taille);

ELF_Symbol *tableSymbol(Elf32_Symbol_Section *sectionHead, int tailleSectionTable);

ELF_Header *init (FILE *fichier);

#endif