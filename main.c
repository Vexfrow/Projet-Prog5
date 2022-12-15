#include "lecteur_fichier.h"
#include "afficheur.h"
#include <stdio.h>

int main (int argc, char *argv[]){
    FILE *fichier = fopen(argv[1], "r");
    ELF_Header *header = init(fichier);

    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    init_section_header(fichier, header->e_shnum, header->e_shoff, Section_header_tab);
    return 0;


}