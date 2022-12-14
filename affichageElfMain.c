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
    "--help (afficher cette aide)\n\n");
}


int main(int argc, char *argv[]) {
	int opt;
	int all = 0, header = 0, sectionHeader = 0, indexSection = 0, symbolTable = 0, relocationTable = 0;

	struct option longopts[] = {
		{ "-all", no_argument, NULL, 'a' },
        { "-header", no_argument, NULL, 'h' },
		{ "-sectionHeader", no_argument, NULL, 'S' },
		{ "-uniqueSection", required_argument, NULL, 'x' },
		{ "-symbolTable", no_argument, NULL, 's' },
        { "-relocationTable", no_argument, NULL, 'r' },
        { "-help", no_argument, NULL, 'z' },
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
			indexSection = atoi(optarg);
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

    //On inialise toutes les structures necessaires
    Lecteur *lecteur = init_lecture(fichier);
    ELF_Header *elf_header = init_header(lecteur);


    if(all){
        Elf32_Section_Header *section_header_tab = init_section_header(lecteur, elf_header);
        ELF_Symbol *symbol_table = init_symbol_table(lecteur, elf_header, section_header_tab);
        ELF_Rel *relocation_table = init_relocation_table(lecteur, elf_header, section_header_tab);
        afficher_header(elf_header);
        afficher_section_table(lecteur, elf_header, section_header_tab);
        afficherSymbol(lecteur, elf_header, section_header_tab, symbol_table);
        afficherRelocations(lecteur, elf_header, section_header_tab, symbol_table, relocation_table);
    }else if(header){
        afficher_header(elf_header);
    }else if (sectionHeader){
        Elf32_Section_Header *section_header_tab = init_section_header(lecteur, elf_header);
        afficher_section_table(lecteur, elf_header, section_header_tab);
    }else if(symbolTable){
        Elf32_Section_Header *section_header_tab = init_section_header(lecteur, elf_header);
        ELF_Symbol *symbol_table = init_symbol_table(lecteur, elf_header, section_header_tab);
        afficherSymbol(lecteur, elf_header, section_header_tab, symbol_table);
    }else if(indexSection){
        if(indexSection <= elf_header->e_shnum){
            Elf32_Section_Header *section_header_tab = init_section_header(lecteur, elf_header);
            afficher_section(lecteur, section_header_tab, indexSection);
        }else{
            fprintf(stderr, "readelf: Warning: Section %d was not dumped because it does not exist!\n", indexSection);
            exit(1);
        }

    }else if(relocationTable){
        Elf32_Section_Header *section_header_tab = init_section_header(lecteur, elf_header);
        ELF_Symbol *symbol_table = init_symbol_table(lecteur, elf_header, section_header_tab);
        ELF_Rel *relocation_table = init_relocation_table(lecteur, elf_header, section_header_tab);
        afficherRelocations(lecteur, elf_header, section_header_tab, symbol_table, relocation_table);
    }


	return 0;
}




