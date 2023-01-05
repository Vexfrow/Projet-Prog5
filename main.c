#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "afficheur.h"
#include "lecteur_fichier.h"
#include "reader_binaire.h"
void usage() {
	fprintf(stderr, "Le programme fonctionne de la manière suivante :\n"
    "./main [option] fichier\n\n"
    "Option disponible (Une seule option lors du lancement de la commande) :\n"
    "-a (tout afficher - par defaut),\n"
    "-h (afficher que le header),\n"
    "-S (afficher que la section header),\n"
    "-x indexTable (afficher le contenue de la table à la position indexTable dans le section header)\n"
    "-s (afficher que la table des symboles)\n"
    "-r (afficher que les tables de relocations)\n"
    "-help (afficher cette aide)\n\n");
}

void afficherRelocationsInit(lecteur *lecteur){

    ELF_Header *elf_header = init_header(lecteur);
    Elf32_Section_Header *Section_header_tab = init_section_header(lecteur, elf_header);
    ELF_Symbol *sym = init_symbol_table(lecteur, Section_header_tab, elf_header->e_shnum);
    int i = 0;
    int nb_section =0;
    int nb_ELF =0;
    while( i< elf_header->e_shnum){
        if(Section_header_tab[i].sh_type == 9){
            nb_section++;
            nb_ELF = nb_ELF + (Section_header_tab[i].sh_size/8);
        }
        i++;
    }
    Elf32_Section_Header *Rel_section_tab = malloc(sizeof(Elf32_Section_Header)*nb_section);
    int j=0;
    i=0;
    while(i< elf_header->e_shnum){
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
    init_relocationTab(Rel_section_tab, ELF_tab, nb_ELF, lecteur);
    

    afficherRelocations(Rel_section_tab, ELF_tab, sym, nb_ELF, nb_section, lecteur);

}


int main(int argc, char *argv[]) {
	int opt;
	int all = 0, header = 0, sectionHeader = 0, uniqueSection = 0, symbolTable = 0, relocationTable = 0;
    char* file = NULL;

	struct option longopts[] = {
		{ "all", no_argument, NULL, 'a' },
        { "header", no_argument, NULL, 'h' },
		{ "sectionHeader", no_argument, NULL, 'S' },
		{ "uniqueSection", required_argument, NULL, 'x' },
		{ "symbolTable", no_argument, NULL, 's' },
        { "relocationTable", no_argument, NULL, 'r' },
        { "help", no_argument, NULL, 'z' },
		{ NULL, 0, NULL, 0 }
	};


	opt = getopt_long(argc, argv, "ahSx:srz", longopts, NULL);
	switch(opt) {
		case 'a':
			all = 1;
			break;
		case 'h':
			header = 1;
			break;
		case 'S':
			sectionHeader = 1;
			break;
		case 'x':
			uniqueSection = atoi(optarg);
			break;
        case 's':
			symbolTable = 1;
			break;
        case 'r':
			relocationTable = 1;
			break;
        case 'z':
			usage();
			exit(1);
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			usage();
			exit(1);
	}

    FILE *fichier;
    if(optind == argc-1){
        fichier = fopen(argv[optind], "r");
        if(fichier == NULL){
            printf("ERREUR : Le fichier passé en paramètre n'existe pas\n%s",argv[optind]);
            exit(2);
        }
    }else{
        fprintf(stderr, "Il ne doit y avoir que 2 ou 3 arguments (dans le cas où l'option choisi est 'x')\n");
        exit(1);
    }

    //On inialise toute les structures necessaires
    lecteur *lecteur = init_lecture(fichier);
    ELF_Header *elf_header = init_header(lecteur);
    Elf32_Section_Header *Section_header_tab = init_section_header(lecteur, elf_header);
    ELF_Symbol *sym = init_symbol_table(lecteur, Section_header_tab, elf_header->e_shnum);

    if(all){
        afficher_header(elf_header);
        afficher_section_table(Section_header_tab, elf_header->e_shnum, lecteur);
        afficherSymbol(sym, tailleTableSymbol(Section_header_tab, elf_header->e_shnum), lecteur, Section_header_tab, elf_header->e_shstrndx);
        afficherRelocationsInit(lecteur);
    }else if(header){
        afficher_header(elf_header);
    }else if (sectionHeader){
        afficher_section_table(Section_header_tab, elf_header->e_shnum, lecteur);
    }else if(symbolTable){
        afficherSymbol(sym, tailleTableSymbol(Section_header_tab, elf_header->e_shnum), lecteur, Section_header_tab, elf_header->e_shstrndx);

    }else if(uniqueSection){
        if(uniqueSection <= elf_header->e_shnum){
        afficher_section(Section_header_tab, uniqueSection, lecteur);
        }else{
            fprintf(stderr, "readelf: Warning: Section %d was not dumped because it does not exist!",uniqueSection);
            exit(1);
        }

    }else if(relocationTable){
        afficherRelocationsInit(lecteur);
    }


	return 0;
}




