#include "lecteur_fichier.h"
#include "reader_binaire.h"



void afficher(ELF_Header *Header, int taille){
    for(int i =0; i < taille; i++){
        printf("%.2hx ", Header->e_ident[i]);
    }
    printf("\n");
}


void remplirMagic(FILE *fichier, ELF_Header *Header, int taille){
    for(int i = 0; i < taille; i++){
        Header->e_ident[i] = lectureOctet(fichier);
    }
}


ELF_Header *init(FILE *fichier){
    //Ajouter les Macros pour N_IDENT 16
    ELF_Header *elf = malloc(sizeof(ELF_Header));
    if(elf == NULL){
        fprintf(stderr, "Pas assez d'espace mémoire");
        exit(1);
    }
    remplirMagic(fichier, elf, 16);

    return elf;
    
}