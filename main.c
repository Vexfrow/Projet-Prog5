#include "lecteur_fichier.h"
#include <stdio.h>

int main (int argc, char* argv[]){
    FILE *fichier = fopen(argv[0], "r");
    ELF_Header *header = init(fichier);
    afficher(header,16);
    return 0;
}