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


void init_section_header(FILE *fichier, uint16_t nb, unsigned int adrStart, Elf32_Section_Header *tab){
    printf("adrstart est de %x    ", adrStart);
    int i = 0;
    while ( i < nb){
        int j = 0;
        while(j< 10){
            fseek(fichier, adrStart +40*(i), SEEK_SET);
            int val = lecture4octet(fichier);
            tab[i].sh_name = val;
            val = lecture4octet(fichier);
            tab[i].sh_type = val;
            val = lecture4octet(fichier);
            tab[i].sh_flags = val;
            val = lecture4octet(fichier);
            tab[i].sh_addr = val;
            val = lecture4octet(fichier);
            tab[i].sh_offset = val;
            val = lecture4octet(fichier);
            tab[i].sh_size = val;
            val = lecture4octet(fichier);
            tab[i].sh_link = val;
            val = lecture4octet(fichier);
            tab[i].sh_info = val;
            val = lecture4octet(fichier);
            tab[i].sh_addralign = val;
            val = lecture4octet(fichier);
            tab[i].sh_entsize = val;

            j++; 
        }
        i++;
    }
}
void afficher_sect(Elf32_Section_Header *tab, uint16_t nb){


    int i = 0;
    printf("%d est nb   ", nb);
    while(i<nb){
        printf("sh_name:%x  ",tab[i].sh_name);
        printf("type:%x  ",tab[i].sh_type);
        printf("sh_flags:%x  ",tab[i].sh_flags);
        printf("sh_addr:%x  ",tab[i].sh_addr);
        printf("sh_offset:%x  ",tab[i].sh_offset);
        printf("sh_size:%x  ",tab[i].sh_size);
        printf("sh_link:%x  ",tab[i].sh_link);
        printf("sh_info:%x  ",tab[i].sh_info);
        printf("sh_addralign:%x  ",tab[i].sh_addralign);
        printf("sh_entsize:%x  ",tab[i].sh_entsize);
        i++;
        printf(" \n ");
    }


ELF_Symbol *remplirSymbol(FILE *fichier, ELF_Symbol *table, int taille){
    for(int i = 0; i < taille; i++){
        table[i].st_name = lecture4octet(fichier);
        table[i].st_value = lecture4octet(fichier);
        table[i].st_size = lecture4octet(fichier);
        table[i].st_info = lecture1octet(fichier);
        table[i].st_others = lecture1octet(fichier);
        table[i].shndx = lecture2octet(fichier);
    } 
    return table;      
}


ELF_Symbol *tableSymbol(FILE *fichier, Elf32_Section_Header *sectionHead, int tailleSectionTable){
    int i = 0;
    while( i < tailleSectionTable && sectionHead[i].sh_type != SHT_SYMTAB){
        i++;
    }
    int taille = sectionHead[i].sh_size / 16;
    ELF_Symbol *table = malloc(sizeof(ELF_Symbol)*taille);
    if(table == NULL){
        fprintf(stderr, "Pas assez de place mémoire");
        exit(1);
    }
    fseek(fichier, sectionHead[i].sh_offset, 0);
    remplirSymbol(fichier, table, taille);
    return table;
}