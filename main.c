#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "afficheur.h"
#include "lecteur_fichier.h"
#include "reader_binaire.h"
#include "fusion.h"


int main(int argc, char *argv[]) {
    
    FILE *fichier1;
    FILE *fichier2;
    if(argc == 3){
        fichier1 = fopen(argv[1], "r");
        if(fichier1 == NULL){
            printf("ERREUR : Le fichier passé en paramètre n'existe pas\n%s",argv[optind]);
            exit(2);
        }

        fichier2 = fopen(argv[2], "r");
        if(fichier2 == NULL){
            printf("ERREUR : Le fichier passé en paramètre n'existe pas\n%s",argv[optind]);
            exit(2);
        }
    }else{
        exit(50);
    }

<<<<<<< HEAD
    //On inialise toutes les structures necessaires
    Lecteur *lect1 = init_lecture(fichier1);
    ELF_Header *elf_header1 = init_header(lect1);
    Elf32_Section_Header *section_header_tab1 = init_section_header(lect1, elf_header1);
    ELF_Symbol *symbol_table1 = init_symbol_table(lect1, elf_header1, section_header_tab1);
    ELF_Rel *relocation_table1 = init_relocation_table(lect1, elf_header1, section_header_tab1);
    
    Lecteur *lect2 = init_lecture(fichier2);
    ELF_Header *elf_header2 = init_header(lect2);
    Elf32_Section_Header *section_header_tab2 = init_section_header(lect2, elf_header2);
    ELF_Symbol *symbol_table2 = init_symbol_table(lect2, elf_header2, section_header_tab2);
    ELF_Rel *relocation_table2 = init_relocation_table(lect2, elf_header2, section_header_tab2);
=======
    // On inialise toute les structures necessaires
    Lecteur *lecteur = init_lecture(fichier);
    ELF_Header *elf_header = init_header(lecteur);
    Elf32_Section_Header *section_header_tab = init_section_header(lecteur, elf_header);
    ELF_Symbol *symbol_table = init_symbol_table(lecteur, elf_header, section_header_tab);
    ELF_Rel *relocation_table = init_relocation_table(lecteur, elf_header, section_header_tab);

    if(all){
        afficher_header(elf_header);
        afficher_section_table(lecteur, elf_header, section_header_tab);
        afficherSymbol(lecteur, elf_header, section_header_tab, symbol_table);
        afficherRelocations(lecteur, elf_header, section_header_tab, symbol_table, relocation_table);
    }else if(header){
        afficher_header(elf_header);
    }else if (sectionHeader){
        afficher_section_table(lecteur, elf_header, section_header_tab);
    }else if(symbolTable){
        afficherSymbol(lecteur, elf_header, section_header_tab, symbol_table);
    }else if(indexSection){
        if(indexSection <= elf_header->e_shnum){
            afficher_section(lecteur, section_header_tab, indexSection);
        }else{
            fprintf(stderr, "readelf: Warning: Section %d was not dumped because it does not exist!", indexSection);
            exit(1);
        }

    }else if(relocationTable){
        afficherRelocations(lecteur, elf_header, section_header_tab, symbol_table, relocation_table);
    }
>>>>>>> main

    Lecteur *lect3 = init_lecteur(lect1->size+lect2->size);
    lect3 = fusion(lect1 ,lect2 ,lect3, elf_header1 ,elf_header2 ,section_header_tab1, section_header_tab2, symbol_table1, symbol_table2, relocation_table1, relocation_table2 );
    // ELF_Header *elf_header3 = init_header(lect3);
    // Elf32_Section_Header *section_header_tab3 = init_section_header(lect3, elf_header3);

    // afficher_header(elf_header3);
    //afficher_section_table(lect3, elf_header3, section_header_tab3);
    // int m = 0;
    // while(m < 10){
    //     afficher_section(lect3, section_header_tab3, m);
    //     m++;
    // }
	return 0;
}

