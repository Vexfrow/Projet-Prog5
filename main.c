#include "lecteur_fichier.h"
#include "afficheur.h"
#include <stdio.h>
#include <string.h>

#define MESSAGE_OPTION "Le programme fonctionne de la manière suivante :\n ./main [option] fichier\n\nOption disponible (Une seule option lors du lancement de la commande) :\n -a (tout afficher - par defaut),\n -h (afficher que le header),\n -sh (afficher que la section header),\n -st (afficher que la table)\n -help (afficher cette aide)\n\n"




void afficherHeader(FILE *fichier){
    ELF_Header *header = init(fichier);
    
    afficher_header(header);
}

void afficherHeaderSection(FILE *fichier){
    ELF_Header *header = init(fichier);
    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    init_section_header(fichier, header->e_shnum, header->e_shoff, Section_header_tab, header->e_shstrndx);
    
    afficher_sectiontable(Section_header_tab, header->e_shnum, fichier);
}


void afficherSymbolTable(FILE *fichier){
    ELF_Header *header = init(fichier);
    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    init_section_header(fichier, header->e_shnum, header->e_shoff, Section_header_tab, header->e_shstrndx);
    ELF_Symbol *sym=tableSymbol(fichier, Section_header_tab, header->e_shnum);

    afficherSymbol(sym, tailleTableSymbol(Section_header_tab, header->e_shnum), fichier, Section_header_tab);
}


void afficherAll(FILE *fichier){
    ELF_Header *header = init(fichier);
    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    init_section_header(fichier, header->e_shnum, header->e_shoff, Section_header_tab, header->e_shstrndx);
    ELF_Symbol *sym=tableSymbol(fichier, Section_header_tab, header->e_shnum);
    
    afficher_header(header);
    afficher_sectiontable(Section_header_tab, header->e_shnum, fichier);
    afficherSymbol(sym, tailleTableSymbol(Section_header_tab, header->e_shnum), fichier, Section_header_tab);
}
void afficher_contenu_section(FILE *fichier, int nb){
    ELF_Header *header = init(fichier);
    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    init_section_header(fichier, header->e_shnum, header->e_shoff, Section_header_tab, header->e_shstrndx);

    afficher_section(Section_header_tab, nb, fichier);
} 

void afficherRelocationsInit(FILE *fichier){

    ELF_Header *header = init(fichier);
    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*header->e_shnum);
    init_section_header(fichier, header->e_shnum, header->e_shoff, Section_header_tab, header->e_shstrndx);
    ELF_Symbol *sym = tableSymbol(fichier, Section_header_tab, header->e_shnum);
    int i = 0;
    int nb_section =0;
    int nb_ELF =0;
    while( i< header->e_shnum){
        if(Section_header_tab[i].sh_type == 9){
            nb_section++;
            nb_ELF = nb_ELF + (Section_header_tab[i].sh_size/8);
        }
        i++;
    }
    Elf32_Section_Header *Rel_section_tab = malloc(sizeof(Elf32_Section_Header)*nb_section);
    int j=0;
    i=0;
    while(i< header->e_shnum){
        if(Section_header_tab[i].sh_type == 9){
            Rel_section_tab[j].sh_name = Section_header_tab[i].sh_name;
            Rel_section_tab[j].sh_type = Section_header_tab[i].sh_type;
            Rel_section_tab[j].sh_flags = Section_header_tab[i].sh_flags;
            Rel_section_tab[j].sh_addr = Section_header_tab[i].sh_addr;
            Rel_section_tab[j].sh_offset = Section_header_tab[i].sh_offset;
            Rel_section_tab[j].sh_size = Section_header_tab[i].sh_size;
            Rel_section_tab[j].sh_link = Section_header_tab[i].sh_link;
            Rel_section_tab[j].sh_info = Section_header_tab[i].sh_info;
            Rel_section_tab[j].sh_addralign = Section_header_tab[i].sh_addralign;
            Rel_section_tab[j].sh_entsize = Section_header_tab[i].sh_entsize;
            j++;
        }
        i++;
    }
    ELF_Rel *ELF_tab= malloc(sizeof(ELF_Rel)*nb_ELF);
    init_relocationTab(Rel_section_tab, ELF_tab, nb_ELF, fichier);
    

    afficherRelocations(Rel_section_tab, ELF_tab, sym, nb_ELF, nb_section, fichier);

}


void gererOption(char *c,FILE* fichier){

    if(!strcmp(c, "-a"))
        afficherAll(fichier);
    else if(!strcmp(c,"-h"))
        afficherHeader(fichier);
    else if(!strcmp(c,"-sh"))
        afficherHeaderSection(fichier);
    else if(!strcmp(c,"-st"))
        afficherSymbolTable(fichier);
    else if (!strcmp(c, "-r"))
        afficherRelocationsInit(fichier);
    else if(!strcmp(c, "-help"))
        printf(MESSAGE_OPTION);
    else    
        printf("ERREUR : Option non prise en compte\n%s", MESSAGE_OPTION);

}



int main (int argc, char *argv[]){

    if(argc == 1){
        printf("ERREUR : Il faut lancer un programme avec un fichier en paramètre\n %s", MESSAGE_OPTION);
        exit(1);
    }else if(argc == 2){
        FILE *fichier = fopen(argv[1], "r");
        if(fichier == NULL){
            printf("ERREUR : Le fichier passé en paramètre n'existe pas\n%s", MESSAGE_OPTION);
            exit(2);
        }
        afficherAll(fichier);

    }else if(argc == 3){
        FILE *fichier = fopen(argv[2], "r");
        if(fichier == NULL){
            printf("ERREUR : Le fichier passé en paramètre n'existe pas\n%s", MESSAGE_OPTION);
            exit(2);
        }
        gererOption(argv[1], fichier);
    }else if(argc == 4){
        FILE *fichier = fopen(argv[3], "r");
        if(fichier == NULL){
            printf("ERREUR : Le fichier passé en paramètre n'existe pas\n%s", MESSAGE_OPTION);
            exit(2);
        }
        afficher_contenu_section(fichier, atoi(argv[2]));
    }else {
        printf("ERREUR : Il faut lancer un programme avec un fichier en paramètre\n%s", MESSAGE_OPTION);
        exit(1);
    }

    return 0;

}