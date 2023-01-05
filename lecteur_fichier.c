#include "lecteur_fichier.h"
#include "reader_binaire.h"
#include <string.h>

char* getName(lecteur *lecteur, unsigned int address){
    lecteur->adr=0;
    char name[50];

    lecteur->adr= address;
    char c = lecture1octet(lecteur); //ATTENTION SI BUG? CA PUE ICI

    int i = 0;
    while(c != '\0'){
        name[i] = c;
        i++;
        c = lecture1octet(lecteur);
    }
    name[i] = '\0';
    char* res = name;
    return res;
}

void remplirMagic(lecteur *lecteur, ELF_Header *Header, int taille){
    for(int i = 0; i < taille; i++){
        Header->e_ident[i] = lecture1octet(lecteur);
    }
}

ELF_Header *init(lecteur *lect){
    lect->adr=0;
    ELF_Header *elf = malloc(sizeof(ELF_Header));
    uint32_t (*l4o)(lecteur*);
    uint16_t (*l2o)(lecteur*);
    if(elf == NULL){
        fprintf(stderr, "ERREUR: Pas assez d'espace mémoire");
        exit(1);
    }

    remplirMagic(lect, elf, EI_NIDENT);
    
    if(elf->e_ident[5] == 1){
        //Little Endian
        l4o = lecture4octet;
        l2o = lecture2octet;
    }else if(elf->e_ident[5] == 2){
        //Big Endian
        l4o = bigEndianLecture4octet;
        l2o = bigEndianLecture2octet;
    }else{
        //Cas d'erreur
        fprintf(stderr, "Le format de donnée est invalide");
        exit(1);
    }

    elf->e_type = l2o(lect);
    elf->e_machine = l2o(lect);
    elf->e_version = l4o(lect);
    elf->e_entry = l4o(lect);
    elf->e_phoff = l4o(lect);
    elf->e_shoff = l4o(lect);
    elf->e_flags = l4o(lect); 
    elf->e_ehsize = l2o(lect);
    elf->e_phentsize = l2o(lect); 
    elf->e_phnum = l2o(lect);
    elf->e_shentsize = l2o(lect);
    elf->e_shnum = l2o(lect);
    elf->e_shstrndx = l2o(lect); 

    return elf;
}

//Pointeur a finir
void init_section_header(lecteur *lect, uint16_t nb, unsigned int adrStart, Elf32_Section_Header *tab, unsigned int indexStringTable){

    lect->adr = 5;
    char endianness = lecture1octet(lect);
    
    uint32_t (*l4o)(lecteur*);
    if(endianness == 1){
        //Little Endian
        l4o = lecture4octet;
    }else if(endianness == 2){
        //Big Endian
        l4o = bigEndianLecture4octet;
    }else{
        //Cas d'erreur
        fprintf(stderr, "Le format de donnée est invalide");
        exit(1);
    }

    lect->adr=0;
    int i = 0;
    unsigned int adressStringTable;
    while ( i < nb){
        lect->adr = adrStart +40*(i);
        int val = l4o(lect);
        tab[i].sh_name = val;
        val = l4o(lect);
        tab[i].sh_type = val;
        val = l4o(lect);
        tab[i].sh_flags = val;
        val = l4o(lect);
        tab[i].sh_addr = val;
        val = l4o(lect);
        tab[i].sh_offset = val;
        val = l4o(lect);
        tab[i].sh_size = val;
        val = l4o(lect);
        tab[i].sh_link = val;
        val = l4o(lect);
        tab[i].sh_info = val;
        val = l4o(lect);
        tab[i].sh_addralign = val;
        val = l4o(lect);
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


//Pointeur a finir
ELF_Symbol *remplirSymbol(lecteur *lect, ELF_Symbol *table, int taille, uint32_t (*l4o)(lecteur*), uint16_t (*l2o)(lecteur*)){
    for(int i = 0; i < taille; i++){
        table[i].st_name = l4o(lect);
        table[i].st_value = l4o(lect);
        table[i].st_size = l4o(lect);
        table[i].st_info = lecture1octet(lect);
        table[i].st_other = lecture1octet(lect);
        table[i].st_shndx = l2o(lect);
    } 
    return table;      
}

//Pointeur a finir
ELF_Symbol *tableSymbol(lecteur *lect, Elf32_Section_Header *sectionHead, int tailleSectionTable){
    lect->adr = 5;
    int endianness = lecture1octet(lect);
    uint32_t (*l4o)(lecteur*);
    uint16_t (*l2o)(lecteur*);
    if(endianness == 1){
        //Little Endian
        l4o = lecture4octet;
        l2o = lecture2octet;
    }else if(endianness == 2){
        //Big Endian
        l4o = bigEndianLecture4octet;
        l2o = bigEndianLecture2octet;
    }else{
        //Cas d'erreur
        fprintf(stderr, "Le format de donnée est invalide");
        exit(1);
    }

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
    lect->adr= sectionHead[i].sh_offset;

    remplirSymbol(lect, table, taille, l4o, l2o);
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


//Pointeur a finir
void init_relocationTab(Elf32_Section_Header *Rel_section_tab,  ELF_Rel *ELF_tab, int nb, lecteur *lect ){

    lect->adr = 5;
    int endianness = lecture1octet(lect);
    uint32_t (*l4o)(lecteur*);

    lect->adr=0;
    int i = 0;
    lect->adr= (Rel_section_tab[i].sh_addr + Rel_section_tab[i].sh_offset)+i*8;
    while(i<nb){
        ELF_tab[i].r_offset = l4o(lect);
        ELF_tab[i].r_info = l4o(lect);
        i++;
    }
}
