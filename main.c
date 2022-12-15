#include "lecteur_fichier.h"
#include "afficheur.h"
#include <stdio.h>

int main (int argc, char *argv[]){
    FILE *fichier = fopen(argv[1], "r");
    ELF_Header *header = init(fichier);

    Elf32_Section_Header *Section = init_section_header(fichier, header->e_shnum, header->e_shoff);

    ELF_Symbol *sym=tableSymbol(fichier, Section, header->e_shnum);
    afficherSymbol(sym, tailleTableSymbol(Section, header->e_shnum));
    
    return 0;


}