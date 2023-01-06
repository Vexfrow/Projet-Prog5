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

ELF_Header *init_header(Lecteur *lect){
    
    ELF_Header *elf = malloc(sizeof(ELF_Header));
    uint32_t (*l4o)(Lecteur*);
    uint16_t (*l2o)(Lecteur*);
    if(elf == NULL){
        fprintf(stderr, "ERREUR: Pas assez d'espace mémoire");
        exit(1);
    }
    lect->adr=0;
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
        fprintf(stderr, "Le format de donnée est invalide dans le header\n");
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




//-----------------------SECTION HEADER ------------------------------------

Elf32_Section_Header *init_section_header(Lecteur *lecteur, ELF_Header *elf_header){

    lecteur->adr = 5;
    char endianness = lecture1octet(lecteur);
    
    uint32_t (*l4o)(Lecteur*);
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
        section_header[i].sh_name = l4o(lecteur);;
        section_header[i].sh_type = l4o(lecteur);;
        section_header[i].sh_flags = l4o(lecteur);
        section_header[i].sh_addr = l4o(lecteur);
        section_header[i].sh_offset = l4o(lecteur);
        section_header[i].sh_size = l4o(lecteur);
        section_header[i].sh_link = l4o(lecteur);
        section_header[i].sh_info = l4o(lecteur);
        section_header[i].sh_addralign = l4o(lecteur);
        section_header[i].sh_entsize = l4o(lecteur);

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
    int endianness = elf_header->e_ident[5];
    uint32_t (*l4o)(Lecteur*);
    uint16_t (*l2o)(Lecteur*);
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

    ELF_Symbol *symbol_table = malloc(sizeof(ELF_Symbol)*taille);
    if(symbol_table == NULL){
        fprintf(stderr, "Pas assez de place mémoire");
        exit(1);
    }

    lecteur->adr= sectionHeader[indexSymbolTableSection].sh_offset;
    for(int i = 0; i < taille; i++){
        symbol_table[i].st_name = l4o(lecteur);
        symbol_table[i].st_value = l4o(lecteur);
        symbol_table[i].st_size = l4o(lecteur);
        symbol_table[i].st_info = lecture1octet(lecteur);
        symbol_table[i].st_other = lecture1octet(lecteur);
        symbol_table[i].st_shndx = l2o(lecteur);

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
    int endianness = elf_header->e_ident[5];
    uint32_t (*l4o)(Lecteur*);
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
                relocation_table[j].r_offset = l4o(lecteur);
                relocation_table[j].r_info = l4o(lecteur);
                k++;
                j++;
            }
        }
        i++;
    }

    return relocation_table;
}
