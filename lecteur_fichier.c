#include "lecteur_fichier.h"
#include "reader_binaire.h"
#include <string.h>



void remplirMagic(FILE *fichier, ELF_Header *Header, int taille){
    for(int i = 0; i < taille; i++){
        Header->e_ident[i] = lecture1octet(fichier);
    }
}

ELF_Header *init(FILE *fichier){
    ELF_Header *elf = malloc(sizeof(ELF_Header));
    if(elf == NULL){
        fprintf(stderr, "ERREUR: Pas assez d'espace mémoire");
        exit(1);
    }
    remplirMagic(fichier, elf, EI_NIDENT);
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


void init_section_header(FILE *fichier, uint16_t nb, unsigned int adrStart, Elf32_Section_Header *tab, unsigned int indexStringTable){
    int i = 0;
    unsigned int adressStringTable;
    while ( i < nb){
        int j = 0;
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

        if(i == indexStringTable){
            adressStringTable = tab[i].sh_addr+tab[i].sh_offset;
        }
        i++;     
    }
    
    
    for(i = 0; i < nb; i++){
        tab[i].sh_name = adressStringTable+tab[i].sh_name;
    }

}



ELF_Symbol *remplirSymbol(FILE *fichier, ELF_Symbol *table, int taille){
    for(int i = 0; i < taille; i++){
        table[i].st_name = lecture4octet(fichier);
        table[i].st_value = lecture4octet(fichier);
        table[i].st_size = lecture4octet(fichier);
        table[i].st_info = lecture1octet(fichier);
        table[i].st_other = lecture1octet(fichier);
        table[i].st_shndx = lecture2octet(fichier);
    } 
    return table;      
}


ELF_Symbol *tableSymbol(FILE *fichier, Elf32_Section_Header *sectionHead, int tailleSectionTable){
    if(sectionHead == NULL){
        fprintf(stderr, "Pas de table des sections(tableSymbol)");
        exit(3);
    }
    int i = 0;
    while( i < tailleSectionTable && sectionHead[i].sh_type != SHT_SYMTAB){
        i++;
    }
    int taille = tailleTableSymbol(sectionHead, tailleSectionTable);
    ELF_Symbol *table = malloc(sizeof(ELF_Symbol)*taille);
    if(table == NULL){
        fprintf(stderr, "Pas assez de place mémoire");
        exit(1);
    }
    fseek(fichier, sectionHead[i].sh_offset, 0);
    remplirSymbol(fichier, table, taille);
    return table;
}


int tailleTableSymbol(Elf32_Section_Header *sectionHead, int tailleSectionTable){
    if(sectionHead == NULL){
        fprintf(stderr, "Pas de table des sections (tailleTableSymbol)");
        exit(3);
    }
    int i = 0;
    while( i < tailleSectionTable && sectionHead[i].sh_type != SHT_SYMTAB){
        i++;
    }
    return sectionHead[i].sh_size / 16;
}


ELF_Rel *tableRelocation(FILE *fichier, Elf32_Section_Header *sectionHead){
    return NULL;
}
