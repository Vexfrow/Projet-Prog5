#include "lecteur_fichier.h"
#include "afficheur.h"
#include <stdio.h>
#include <string.h>

#define MESSAGE_OPTION "Le programme fonctionne de la manière suivante :\n ./main [option] fichier\n\nOption disponible (Une seule option lors du lancement de la commande) :\n -a (afficher tout - par defaut),\n -h (afficher que le header),\n -sh (afficher que la section header),\n -st (afficher que la table)\n -help (afficher cette aide)\n\n"


void gererOption(char *c,FILE* fichier){

    if(!strcmp(c, "-a"))
        afficherAll(fichier);
    else if(!strcmp(c,"-h"))
        afficherHeader(fichier);
    else if(!strcmp(c,"-sh"))
        afficherHeaderSection(fichier);
    else if(!strcmp(c,"-st"))
        afficherSymbolTable(fichier);
    else if(!strcmp(c, "-help"))
        printf(MESSAGE_OPTION);
    else    
        printf("ERREUR : Option non prise en compte\n%s", MESSAGE_OPTION);

}

void afficherHeader(FILE *fichier){
    ELF_Header *header = init(fichier);
    
    afficher_header(header);
}

void afficherHeaderSection(FILE *fichier){
    ELF_Header *header = init(fichier);
    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    init_section_header(fichier, header->e_shnum, header->e_shoff, Section_header_tab, header->e_shstrndx);
    
    afficher_section(Section_header_tab, header->e_shnum, fichier);
}


void afficherSymbolTable(FILE *fichier){
    ELF_Header *header = init(fichier);
    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    init_section_header(fichier, header->e_shnum, header->e_shoff, Section_header_tab, header->e_shstrndx);
    ELF_Symbol *sym=tableSymbol(fichier, Section_header_tab, header->e_shnum);

    afficherSymbol(sym, tailleTableSymbol(Section_header_tab, header->e_shnum), fichier, Section_header_tab);
}


void afficherAll(FILE *fichier){
    ELF_Header *header = init(fichier);
    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    init_section_header(fichier, header->e_shnum, header->e_shoff, Section_header_tab, header->e_shstrndx);
    ELF_Symbol *sym=tableSymbol(fichier, Section_header_tab, header->e_shnum);
    
    afficher_header(header);
    afficher_section(Section_header_tab, header->e_shnum, fichier);
    afficherSymbol(sym, tailleTableSymbol(Section_header_tab, header->e_shnum), fichier, Section_header_tab);
}




int main (int argc, char *argv[]){

    if(argc == 1){
        printf("ERREUR : Il faut lancer un programme avec un fichier en paramètre\n %s", MESSAGE_OPTION);
        exit(1);
    }else if(argc == 2){
        FILE *fichier = fopen(argv[1], "r");
        if(fichier == NULL){
            printf("ERREUR : Le fichier passer en paramètre n'existe pas\n%s", MESSAGE_OPTION);
            exit(2);
        }
        afficherAll(fichier);

    }else if(argc == 3){
        FILE *fichier = fopen(argv[2], "r");
        if(fichier == NULL){
            printf("ERREUR : Le fichier passer en paramètre n'existe pas\n%s", MESSAGE_OPTION);
            exit(2);
        }
        gererOption(argv[1], fichier);
    }else{
        printf("ERREUR : Il faut lancer un programme avec un fichier en paramètre\n%s", MESSAGE_OPTION);
        exit(1);
    }

    return 0;

}