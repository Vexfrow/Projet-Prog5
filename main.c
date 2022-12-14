#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t lectureOctet(FILE *fichier){
    uint8_t res;
    fread(&res,1,1,fichier);
    return res;
}


//Lecture de 2 octets dans un fichier, en little endian si littleEndian = true
uint16_t lecture2Octet(FILE *fichier, int littleEndian){
    uint16_t octet;

    return  0;
}


void afficher(uint8_t *tab, int taille){
    for(int i =0; i < taille; i++){
        printf("%.2hx ", tab[i]);
    }
    printf("\n");
}


void remplirMagic(FILE *fichier, uint8_t *tab, int taille){
    for(int i = 0; i < taille; i++){
        tab[i] = lectureOctet(fichier);
    }
}


void main(int argc, char *argv[]){
    if(argc != 2){
        //Exit code 2 pas assez d'arguments
        fprintf(stderr,"Argument incorrect");
        exit(2);
    }
    FILE *fichierO = fopen(argv[0],"r");
    if(fichierO == NULL){
        fprintf(stderr, "Fichier illisible");
        exit(1);
    }

    //Ajouter les Macros pour N_IDENT 16
    uint8_t *magic = malloc(16*sizeof(uint8_t));
    remplirMagic(fichierO, magic, 16);
    afficher(magic, 16);
}