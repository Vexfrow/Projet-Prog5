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

Lecteur *fusion_symbol(Lecteur *lect1 ,Lecteur *lect2 ,Lecteur *lect3, ELF_Header *elf_header1, ELF_Header *elf_header2, Elf32_Section_Header *section_header_tab1, Elf32_Section_Header *section_header_tab2, ELF_Symbol *symbol_table1, ELF_Symbol *symbol_table2);

Lecteur *fusion_relocation(Lecteur *lect1, Lecteur *lect2, Lecteur *lect3, Elf32_Section_Header *section_header_tab1, Elf32_Section_Header *section_header_tab2, Elf32_Section_Header * section_header_tab3, ELF_Header *elf_header1, ELF_Header *elf_header2, ELF_Header *elf_header3, ELF_Symbol *symTab2 ,ELF_Symbol *symTab3);


//---------------------------------------------------------------------------------------------------------

int *tableauCorrespondanceIndex(Lecteur *lect1 ,Lecteur *lect2, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2);

int *tableauCorrespondanceIndexSym(Lecteur *lect1 ,Lecteur *lect2, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2, ELF_Symbol *symbol_table1, ELF_Symbol *symbol_table2);

int tabCorresInverse(int *tab, int taille, int value);

//---------------------------------------------------------------------------------------------------------
unsigned int endianValue(unsigned int valueLittleEndian, int valueELFEndian, int tailleOctet);
//----------------------------------------------------------------------------------------------------------

uint32_t majNdx(Lecteur *lect1, Lecteur *lect3, ELF_Header *elf_header2, Elf32_Section_Header *section_header_tab1, Elf32_Section_Header *section_header_tab2, int oldNdx);


    //ELF_Header *h3 = init_header(lect3);
    //afficher_header(h3);
    //afficher_section_table(lect2, elf_header2, section_header_tab2);
    // for(int i = 0; i < elf_header1->e_shnum; i++){
    //     afficher_section(lect3, section_header_tab3, i);
    //     afficher_section(lect1, section_header_tab1, i);
    //     if(tabCorres[i] != -1)
    //         afficher_section(lect2, section_header_tab2, tabCorres[i]);
    //     printf("-----------------------------------------------------------------------------\n");
    // }
//----------------------------------------------------------------------------------------------------------
