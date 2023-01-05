#include "lecteur_fichier.h"
#include "reader_binaire.h"
#include <string.h>


 //-----------------------------------------------------------------------


char* getName(Lecteur *lecteur, unsigned int address){
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

void remplirMagic(Lecteur *lecteur, ELF_Header *Header, int taille){
    for(int i = 0; i < taille; i++){
        Header->e_ident[i] = lecture1octet(lecteur);
    }
}

ELF_Header *init_header(Lecteur *lecteur){
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

Elf32_Section_Header *init_section_header(Lecteur *lecteur, ELF_Header *elf_header){
    lecteur->adr=0;
    Elf32_Section_Header *section_header = malloc(sizeof(Elf32_Section_Header)*elf_header->e_shnum);
    if(section_header == NULL){
        fprintf(stderr, "ERREUR: Pas assez d'espace mémoire pour Section Header");
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




ELF_Symbol *init_symbol_table(Lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *sectionHeader){

    int indexSymbolTableSection = getIndexSymbolTableSection(elf_header, sectionHeader);
    int taille = sectionHeader[indexSymbolTableSection].sh_size / 16;

    ELF_Symbol *symbol_table = malloc(sizeof(ELF_Symbol)*taille);
    if(symbol_table == NULL){
        fprintf(stderr, "Pas assez de place mémoire");
        exit(1);
    }

    lecteur->adr= sectionHeader[indexSymbolTableSection].sh_offset;
    for(int i = 0; i < taille; i++){
        symbol_table[i].st_name = lecture4octet(lecteur);
        symbol_table[i].st_value = lecture4octet(lecteur);
        symbol_table[i].st_size = lecture4octet(lecteur);
        symbol_table[i].st_info = lecture1octet(lecteur);
        symbol_table[i].st_other = lecture1octet(lecteur);
        symbol_table[i].st_shndx = lecture2octet(lecteur);

        if((symbol_table[i].st_info & 0xf) == STT_SECTION){
            symbol_table[i].st_name = sectionHeader[symbol_table[i].st_shndx].sh_name;
        }else{
            symbol_table[i].st_name = sectionHeader[sectionHeader[indexSymbolTableSection].sh_link].sh_offset + symbol_table[i].st_name;
        }

    } 

    return symbol_table;
}


int getIndexSymbolTableSection(ELF_Header *elf_header, Elf32_Section_Header *sectionHead){
    int i = 0;
    while( i < elf_header->e_shnum && sectionHead[i].sh_type != SHT_SYMTAB){
        i++;
    }
    if(i >= elf_header->e_shnum){
        fprintf(stderr, "ERREUR : Absence de symbol table");
        exit(1);
    }
    return i;
}


//--------------------- RELOCATION TABLE -----------------------------------

ELF_Rel *init_relocation_table(Lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *section_header_tab){

    int i = 0;
    int nb_rel = 0;

    //On compte le nombre de sections de relocations
    while( i < elf_header->e_shnum){
        if(section_header_tab[i].sh_type == SHT_REL){
            nb_rel = nb_rel + (section_header_tab[i].sh_size/8);
        }
        i++;
    }

    ELF_Rel *relocation_table= malloc(sizeof(ELF_Rel)*nb_rel);
    if(relocation_table == NULL){
        fprintf(stderr, "Pas assez de place mémoire");
        exit(1);
    }

    i = 0;
    int j = 0;
    while(i<elf_header->e_shnum){
        if(section_header_tab[i].sh_type == 9){
            lecteur->adr = section_header_tab[i].sh_offset;
            int k = 0;
            while(k < section_header_tab[i].sh_size/8){
                relocation_table[j].r_offset = lecture4octet(lecteur);
                relocation_table[j].r_info = lecture4octet(lecteur);
                k++;
                j++;
            }
        }
        i++;
    }

    return relocation_table;
}
