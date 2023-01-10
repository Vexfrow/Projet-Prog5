#ifndef __LECTEUR__
#define __LECTEUR__
#include "reader_binaire.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>


//---------------------------------------DEFINITION DES STRUCTURES UTILISEES------------------------------------------------
typedef struct {
    uint8_t e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} ELF_Header;

typedef struct{
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} Elf32_Section_Header;

typedef struct {
    uint32_t st_name;
    uint32_t st_value;
    uint32_t st_size;
    uint8_t st_info;
    uint8_t st_other;
    uint16_t st_shndx;
} ELF_Symbol;


typedef struct {
    uint16_t r_offset;
    uint16_t r_info;
} ELF_Rel;



// ----------------------------------------------------------------------------------------
//Permet de récupèrer un string à partir d'un lecteur et de sa position dans ce dit lecteur (ATTENTION : MALLOC un espace en mémoire pour stocker le résultat -> Il faut penser à le free plus tard)
char* getName(Lecteur *lecteur, unsigned int address);
// ----------------------------------------------------------------------------------------

//Permet de remplir le "Magic number"
void remplirMagic(Lecteur *lecteur, ELF_Header *Header, int taille);

//Initialisateur du Header
ELF_Header *init_header(Lecteur *lecteur);

// ----------------------------------------------------------------------------------------
//Initialisateur de la table des sections
Elf32_Section_Header *init_section_header(Lecteur *lecteur, ELF_Header *elf_header);

// ----------------------------------------------------------------------------------------
//Initialisateur de la tables de relocation
ELF_Rel *init_relocation_table(Lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *section_header_tab);

// ----------------------------------------------------------------------------------------
//Initialisateur de la table des symboles
ELF_Symbol *init_symbol_table(Lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *section_header);

int getIndexSymbolTableSection(ELF_Header *elf_header, Elf32_Section_Header *section_header);

// ----------------------------------------------------------------------------------------
//Choix d'endianness
uint32_t (* lect4o(int endianness))(Lecteur*);

uint16_t (* lect2o(int endianness))(Lecteur*);



#endif