#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "afficheur.h"
#include "lecteur_fichier.h"

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
            printf("tests");
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

    ELF_Header *elf_header = init(fichier);
    Elf32_Section_Header *Section_header_tab = malloc(sizeof(Elf32_Section_Header)*elf_header->e_shnum);
    init_section_header(fichier, elf_header->e_shnum, elf_header->e_shoff, Section_header_tab, elf_header->e_shstrndx);
    ELF_Symbol *sym=tableSymbol(fichier, Section_header_tab, elf_header->e_shnum);

    if(all){
        afficher_header(elf_header);
        afficher_section_table(Section_header_tab, elf_header->e_shnum, fichier);
        afficherSymbol(sym, tailleTableSymbol(Section_header_tab, elf_header->e_shnum), fichier, Section_header_tab, elf_header->e_shnum);
    }else if(header){
        afficher_header(elf_header);
    }else if (sectionHeader){
        afficher_section_table(Section_header_tab, elf_header->e_shnum, fichier);
    }else if(symbolTable){
        afficherSymbol(sym, tailleTableSymbol(Section_header_tab, elf_header->e_shnum), fichier, Section_header_tab, elf_header->e_shnum);

    }else if(uniqueSection){
        if(uniqueSection <= elf_header->e_shnum){
        afficher_section(Section_header_tab, uniqueSection, fichier);
        }else{
            fprintf(stderr, "readelf: Warning: Section %d was not dumped because it does not exist!",uniqueSection);
            exit(1);
        }

    }else if(relocationTable){

    }


	return 0;
}
