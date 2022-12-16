#include "afficheur.h"

void afficher_sect(Elf32_Section_Header *tab, uint16_t nb){


    int i = 0;
    while(i<nb){
        afficher_sh_name(tab[i].sh_name);
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
}

void afficher_sh_name(unsigned int name){

    printf("%x", name);
    printf("\t");
    
}

void afficher_sh_type(unsigned int type){

    switch(type){

        case 0 : printf("SHT_NULL"); break;
        case 1 : printf("SHT_PROGBITS"); break;
        case 2 : printf("SHT_SYMTAB"); break;
        case 3 : printf("SHT_STRTAB"); break;
        case 4 : printf("SHT_RELA "); break;
        case 5 : printf("SHT_HASH"); break;
        case 6 : printf("SHT_DYNAMIC"); break;
        case 7 : printf("SHT_NOBITS"); break;
        case 8 : printf("SHT_REL\t"); break;
        case 9 : printf("SHT_SHLIB"); break;
        case 10 : printf("SHT_DYNSYM"); break;
        case 11 : printf("SHT_NULL"); break;
        case 1879048192 : printf("SHT_LOPROC"); break;
        case 2147483647 : printf("SHT_HIPROC"); break;
        case 2147483648 : printf("SHT_LOUSER"); break;
        case 4294967295 : printf("SHT_HIUSER"); break;
        default : printf("   ?  \t ");
    }
    printf("\t");
}

void afficher_sh_addr(unsigned int addr){

    printf("%x", addr);
    printf("\t");

}

void afficher_sh_offset(unsigned int offset){

    printf("%04x", offset);
    printf("\t");

}

void afficher_sh_size(unsigned int size){

    printf("%04x", size);
    printf("\t");

}

void afficher_sh_link(unsigned int link){

    printf("%x", link);
    printf("\t");

}

void afficher_sh_info(unsigned int info){

    printf("%x", info);
    printf("\t");

}

void afficher_sh_addralign(unsigned int addralign){

    printf("%x", addralign);
    printf("\t");

}

void afficher_sh_entsize(unsigned int entsize){

    printf("%x", entsize);
    printf("\t");

}

void afficher_sh_flags(unsigned int flags){

    printf("%x", flags);
    printf("\t");

}

void afficher_section(Elf32_Section_Header *tab, uint16_t nb){

    printf("\n");
    printf("Section Headers:\n");
    printf("  [Nr] \t Name \t    Type \t Addr \t Off \t Size \t ES \t Flg \t Lk \t Inf \t Al \n");
    int i = 0;

    while(i<nb){
        
        printf("  [%d]", i);
        printf("\t");
        afficher_sh_name(tab[i].sh_name);
        afficher_sh_type(tab[i].sh_type);
        afficher_sh_addr(tab[i].sh_addr);
        afficher_sh_offset(tab[i].sh_offset);
        afficher_sh_size(tab[i].sh_size);
        afficher_sh_entsize(tab[i].sh_entsize);
        afficher_sh_flags(tab[i].sh_flags);
        afficher_sh_link(tab[i].sh_link);
        afficher_sh_info(tab[i].sh_info);
        afficher_sh_addralign(tab[i].sh_addralign);

        i++;
        printf("\n");
    }

}