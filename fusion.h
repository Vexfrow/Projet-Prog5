#include "lecteur_fichier.h"
#include "reader_binaire.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int tab1;
    int tab2;
} couple;

Lecteur *fusion(Lecteur *lect1 ,Lecteur *lect2 ,Lecteur *lect3 ,ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2, ELF_Symbol *symbol_table1, ELF_Symbol *symbol_table2, ELF_Rel *relocation_table1, ELF_Rel *relocation_table2 );

Lecteur *fusion_section(Lecteur *lect1 ,Lecteur *lect2 ,Lecteur *lect3, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2);

int *tableauCorrespondanceIndex(Lecteur *lect1 ,Lecteur *lect2, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2);

int tabCorresInverse(int *tab, int taille, int value);

unsigned int endianValue(unsigned int valueLittleEndian, int valueELFEndian, int tailleOctet);

Lecteur *fusion_relocation(Lecteur *lect1, Lecteur *lect2, Lecteur *lect3, Elf32_Section_Header *section_header_tab1, Elf32_Section_Header *section_header_tab2, Elf32_Section_Header * section_header_tab3, ELF_Header *elf_header1, ELF_Header *elf_header2, ELF_Header *elf_header3);
