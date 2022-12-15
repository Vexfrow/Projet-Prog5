#include "lecteur_fichier.h"
#include <stdio.h>

int main (int argc, char* argv[]){
    FILE *fichier = fopen(argv[0], "r");
    ELF_Header *header = init(fichier);
    afficher(header,16);
    ELF_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    Elf32_Section_Header *Section = init_section_header(fichier, header->e_shnum, header->e_shoff);
    
    return 0;


}