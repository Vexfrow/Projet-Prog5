#ifndef __LECTEUR__
#define __LECTEUR__
#include "reader_binaire.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

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


typedef struct {
    unsigned int r_offset;
    unsigned int r_info;
} ELF_Rel;



// ----------------------------------------------------------------------------------------
//Permet de récupèrer un string à partir d'un lecteur et de sa position dans ce dit lecteur
char* getName(Lecteur *lecteur, unsigned int address);
// ----------------------------------------------------------------------------------------

//Permet de remplir le "Magic number"
void remplirMagic(Lecteur *lecteur, ELF_Header *Header, int taille);

ELF_Header *init_header(Lecteur *lecteur);

// ----------------------------------------------------------------------------------------

Elf32_Section_Header *init_section_header(Lecteur *lecteur, ELF_Header *elf_header);

// ----------------------------------------------------------------------------------------

ELF_Rel *init_relocation_table(Lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *section_header_tab);

// ----------------------------------------------------------------------------------------

ELF_Symbol *init_symbol_table(Lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *section_header);

//ELF_Symbol *remplirTableSymbol(Lecteur *lecteur, ELF_Symbol *table, int taille);

int getIndexSymbolTableSection(ELF_Header *elf_header, Elf32_Section_Header *section_header);

// ----------------------------------------------------------------------------------------


#endif