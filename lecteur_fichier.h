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





void afficher(ELF_Header *Header, int taille);

void remplirMagic(FILE *fichier, ELF_Header *Header, int taille);

ELF_Header *init (FILE *fichier);

void init_section_header(FILE *fichier, uint16_t nb, unsigned int adrStart, Elf32_Section_Header *tab);


ELF_Symbol *tableSymbol(FILE *fichier, Elf32_Section_Header *sectionHead, int tailleSectionTable);

ELF_Symbol *remplirSymbol(FILE *fichier, ELF_Symbol *table, int taille);


#endif