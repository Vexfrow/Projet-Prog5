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
        Header->e_ident[i] = lecture1octet(fichier);
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
    elf->e_type = lecture2octet(fichier);
    elf->e_machine = lecture2octet(fichier);
    elf->e_version = lecture4octet(fichier);
    elf->e_entry = lecture4octet(fichier);
    elf->e_phoff = lecture4octet(fichier);
    elf->e_shoff = lecture4octet(fichier);
    elf->e_flags = lecture4octet(fichier);
    elf->e_ehsize = lecture2octet(fichier);
    elf->e_phentsize = lecture2octet(fichier);
    elf->e_phnum = lecture2octet(fichier);
    elf->e_shentsize = lecture2octet(fichier);
    elf->e_shnum = lecture2octet(fichier);
    elf->e_shstrndx = lecture2octet(fichier);

    return elf;
}

Elf32_Section_Header *init_section_header(FILE *fichier, uint16_t nb, unsigned int adrStart){
    return NULL;
}


