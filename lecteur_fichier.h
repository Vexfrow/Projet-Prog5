#ifndef __LECTEUR__
#define __LECTEUR__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define EI_NIDENT 16


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





void afficher(ELF_Header *Header, int taille);

void remplirMagic(FILE *fichier, ELF_Header *Header, int taille);

ELF_Header *init (FILE *fichier);

#endif