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


 //-----------------------------HEADER------------------------------------
void remplirMagic(lecteur *lecteur, ELF_Header *Header, int taille){
    for(int i = 0; i < taille; i++){
        Header->e_ident[i] = lecture1octet(lecteur);
    }
}

ELF_Header *init_header(lecteur *lecteur){
    lecteur->adr=0;
    ELF_Header *elf = malloc(sizeof(ELF_Header));
    if(elf == NULL){
        fprintf(stderr, "ERREUR: Pas assez d'espace mémoire");
        exit(1);
    }
    remplirMagic(lecteur, elf, EI_NIDENT);
    elf->e_type = lecture2octet(lecteur);
    elf->e_machine = lecture2octet(lecteur);
    elf->e_version = lecture4octet(lecteur);
    elf->e_entry = lecture4octet(lecteur);
    elf->e_phoff = lecture4octet(lecteur);
    elf->e_shoff = lecture4octet(lecteur);
    elf->e_flags = lecture4octet(lecteur); 
    elf->e_ehsize = lecture2octet(lecteur);
    elf->e_phentsize = lecture2octet(lecteur); 
    elf->e_phnum = lecture2octet(lecteur);
    elf->e_shentsize = lecture2octet(lecteur);
    elf->e_shnum = lecture2octet(lecteur);
    elf->e_shstrndx = lecture2octet(lecteur); 

    return elf;
}



//-----------------------SECTION HEADER ------------------------------------

Elf32_Section_Header *init_section_header(lecteur *lecteur, ELF_Header *elf_header){
    lecteur->adr=0;
    Elf32_Section_Header *section_header = malloc(sizeof(Elf32_Section_Header)*elf_header->e_shnum);
    if(section_header == NULL){
        fprintf(stderr, "ERREUR: Pas assez d'espace mémoire");
        exit(1);
    }

    int i = 0;
    unsigned int adressStringTable;
    while ( i < elf_header->e_shnum){
        lecteur->adr = elf_header->e_shoff + elf_header->e_shentsize*(i);
        section_header[i].sh_name = lecture4octet(lecteur);;
        section_header[i].sh_type = lecture4octet(lecteur);;
        section_header[i].sh_flags = lecture4octet(lecteur);
        section_header[i].sh_addr = lecture4octet(lecteur);
        section_header[i].sh_offset = lecture4octet(lecteur);
        section_header[i].sh_size = lecture4octet(lecteur);
        section_header[i].sh_link = lecture4octet(lecteur);
        section_header[i].sh_info = lecture4octet(lecteur);
        section_header[i].sh_addralign = lecture4octet(lecteur);
        section_header[i].sh_entsize = lecture4octet(lecteur);

        if(i == elf_header->e_shstrndx){
            adressStringTable = section_header[i].sh_addr+section_header[i].sh_offset;
        }
        i++;    
    }
    
    
    for(i = 0; i < elf_header->e_shnum; i++){
        section_header[i].sh_name = adressStringTable+section_header[i].sh_name;
    }

    return section_header;

}


//--------------------SYMBOL TABLE ---------------------------------


ELF_Symbol *remplirSymbol(lecteur *lecteur, ELF_Symbol *table, int taille){
    for(int i = 0; i < taille; i++){
        table[i].st_name = lecture4octet(lecteur);
        table[i].st_value = lecture4octet(lecteur);
        table[i].st_size = lecture4octet(lecteur);
        table[i].st_info = lecture1octet(lecteur);
        table[i].st_other = lecture1octet(lecteur);
        table[i].st_shndx = lecture2octet(lecteur);
    } 
    return table;      
}


ELF_Symbol *init_symbol_table(lecteur *lecteur, Elf32_Section_Header *sectionHead, int tailleSectionTable){
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
    lecteur->adr= sectionHead[i].sh_offset;
    remplirSymbol(lecteur, table, taille);
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




//--------------------- RELOCATION TABLE -----------------------------------

void init_relocationTab(Elf32_Section_Header *Rel_section_tab,  ELF_Rel *ELF_tab, int nb, lecteur *lecteur){
    lecteur->adr=0;
    int i = 0;
    lecteur->adr= (Rel_section_tab[i].sh_addr + Rel_section_tab[i].sh_offset)+i*8;
    while(i<nb){
        ELF_tab[i].r_offset = lecture4octet(lecteur);
        ELF_tab[i].r_info = lecture4octet(lecteur);
        i++;
    }
}
