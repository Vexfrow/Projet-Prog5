#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "afficheur.h"
#include "lecteur_fichier.h"
#include "reader_binaire.h"
#include "fusion.h"


int main(int argc, char *argv[]){
    
    FILE *fichier1;
    FILE *fichier2;
    if(argc >= 3){
        for(int i = 1; i+1 <= argc; i++){   
            if(i == 1){
                fichier1 = fopen(argv[1], "r");
                if(fichier1 == NULL){
                    printf("ERREUR : Le fichier passé en paramètre n'existe pas (file 1) : %s\n",argv[1]);
                    exit(2);
                }
                i++;
            }else{
                fichier1 = fopen("file_fusion.o", "r");
            }
            fichier2 = fopen(argv[i], "r");
            if(fichier2 == NULL){
                printf("ERREUR : Le fichier passé en paramètre n'existe pas (file 2) : %s %d\n",argv[i], i);
                exit(2);
            }

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

            Lecteur *lect3 = init_lecteur(lect1->size+lect2->size);
            lect3 = fusion(lect1 ,lect2 ,lect3, elf_header1 ,elf_header2 ,section_header_tab1, section_header_tab2, symbol_table1, symbol_table2, relocation_table1, relocation_table2 );

            ecrireFichier(lect3);
            fprintf(stdout, "La fusion s'est achevée\n");
        }

    }else{
        fprintf(stderr, "ERREUR: PAS ASSEZ D'ARGUMENTS\n");
    }
  
	return 0;
}