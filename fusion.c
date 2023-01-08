#include "fusion.h"
#include "afficheur.h"
#include <string.h>

Lecteur *fusion(Lecteur *lect1 ,Lecteur *lect2 ,Lecteur *lect3, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2, ELF_Symbol *symbol_table1, ELF_Symbol *symbol_table2, ELF_Rel *relocation_table1, ELF_Rel *relocation_table2 ){
    //On verifie que les deux fichiers sont bien fusionnable en verifiant leur magic number
    int i =0;
    while (i < EI_NIDENT && elf_header1->e_ident[i] == elf_header2->e_ident[i]){
        i++;
    }
    if(i < EI_NIDENT){
        printf("Erreur, fichier non fusionnable");
        exit(12);
    }
    
    
    lect3 = fusion_section(lect1 ,lect2 ,lect3, elf_header1 ,elf_header2 , section_header_tab1, section_header_tab2);
    //fusion_symbol();
    //fusion_realloc();
    return lect3;
}


Lecteur *fusion_section(Lecteur *lect1, Lecteur *lect2, Lecteur *lect3, ELF_Header *elf_header1, ELF_Header *elf_header2, Elf32_Section_Header *section_header_tab1, Elf32_Section_Header *section_header_tab2){
    //On crée une nouvelle table des sections
    Elf32_Section_Header *section_header_tab3 = malloc(sizeof(Elf32_Section_Header)*(elf_header1->e_shnum + elf_header2->e_shnum));
    for(int i = 0; i < elf_header1->e_shnum ; i++){
        section_header_tab3[i].sh_name = section_header_tab1[i].sh_name;
        section_header_tab3[i].sh_type = section_header_tab1[i].sh_type;
        section_header_tab3[i].sh_flags = section_header_tab1[i].sh_flags;
        section_header_tab3[i].sh_addr = section_header_tab1[i].sh_addr;
        section_header_tab3[i].sh_offset = section_header_tab1[i].sh_offset;
        section_header_tab3[i].sh_size = section_header_tab1[i].sh_size;
        section_header_tab3[i].sh_link = section_header_tab1[i].sh_link;
        section_header_tab3[i].sh_info = section_header_tab1[i].sh_info;
        section_header_tab3[i].sh_addralign = section_header_tab1[i].sh_addralign;
        section_header_tab3[i].sh_entsize = section_header_tab1[i].sh_entsize;
    }

    //On initialise les tableaux necessaires.
    unsigned int offsetSh = elf_header1->e_shoff;
    int* tabCorres = tableauCorrespondanceIndex(lect1 ,lect2 ,elf_header1 ,elf_header2 ,section_header_tab1, section_header_tab2);
    uint16_t nbSection = elf_header1->e_shnum;


    //On met à jour les information de taille et d'offset des sections
    for(int i = 0; i < elf_header1->e_shnum ; i++){
        if(tabCorres[i] != -1){
            section_header_tab3[i].sh_size += section_header_tab2[tabCorres[i]].sh_size;
            offsetSh += section_header_tab2[tabCorres[i]].sh_size;

            for(int k = 0; k < elf_header1->e_shnum ; k++){
                if(section_header_tab3[k].sh_offset >= section_header_tab3[i].sh_offset && i != k){
                    section_header_tab3[k].sh_offset += section_header_tab2[tabCorres[i]].sh_size;
                }
            }
        }
    }

    for(int i= 0; i < elf_header2->e_shnum ; i++){
        if(!isInTab(tabCorres, elf_header1->e_shnum, i)){
            section_header_tab3[nbSection].sh_name = section_header_tab2[i].sh_name + offsetSh - elf_header1->e_shoff;
            section_header_tab3[nbSection].sh_type = section_header_tab2[i].sh_type;
            section_header_tab3[nbSection].sh_flags = section_header_tab2[i].sh_flags;
            section_header_tab3[nbSection].sh_addr = section_header_tab2[i].sh_addr;
            section_header_tab3[nbSection].sh_offset = section_header_tab2[i].sh_offset + offsetSh - elf_header1->e_shoff;
            section_header_tab3[nbSection].sh_size = section_header_tab2[i].sh_size;
            section_header_tab3[nbSection].sh_link = section_header_tab2[i].sh_link;
            section_header_tab3[nbSection].sh_info = section_header_tab2[i].sh_info;
            section_header_tab3[nbSection].sh_addralign = section_header_tab2[i].sh_addralign;
            section_header_tab3[nbSection].sh_entsize = section_header_tab2[i].sh_entsize;
            nbSection++;
        }
    }

    //On met à jour l'adresse contenue dans sh_name avec le nouvel offset de la table des strings
    for(int i= 0; i < nbSection ; i++){
        section_header_tab3[i].sh_name +=(section_header_tab3[elf_header1->e_shstrndx].sh_offset - section_header_tab1[elf_header1->e_shstrndx].sh_offset);
    }

    //On copie les nouvelles données dans le lecteur3
    memcpy(lect3->fichier, lect1->fichier, sizeof(ELF_Header)); //ELF_Header 

    if(elf_header1->e_ident[5] == 1){
        memcpy(lect3->fichier + 32, (char *) &offsetSh, sizeof(unsigned int)); //maj de l'offset du section header
        memcpy(lect3->fichier + 48, (char *) &nbSection, sizeof(uint16_t)); //maj du nombre de section
    }else if(elf_header1->e_ident[5] == 2){
        //Gestion du cas où les fichiers sont en big endian (les valeurs dans le lecteur sont aussi en big endian donc il faut adapté notre memcpy pour qu'il puisse copier une valeur à l'origine en little endian en big endian)
        unsigned int res = 0;
        res |= ((0x000000ff & offsetSh) << 24);
        res |= ((0x0000ff00 & offsetSh) << 8);
        res |= ((0x00ff0000 & offsetSh) >> 8);
        res |= ((0xff000000 & offsetSh) >> 24);
        memcpy(lect3->fichier + 32, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header

        res = 0;
        res |= ((0x00ff & nbSection) << 8);
        res |= ((0xff00 & nbSection) >> 8);

        memcpy(lect3->fichier + 48, (char *) &res, sizeof(uint16_t)); //maj de l'offset du section header

    }else{
        //Cas d'erreur
        fprintf(stderr, "Endian inconnue\n");
        exit(1);
    }


    for(int i = 0; i < nbSection ;i++){
        if(i < elf_header1->e_shnum){
            memcpy(lect3->fichier + section_header_tab3[i].sh_offset, lect1->fichier + section_header_tab1[i].sh_offset, section_header_tab1[i].sh_size);
            if(tabCorres[i] != -1 && section_header_tab1[i].sh_type == SHT_PROGBITS){
                memcpy(lect3->fichier + section_header_tab3[i].sh_offset + section_header_tab1[i].sh_size, lect2->fichier + section_header_tab2[tabCorres[i]].sh_offset, section_header_tab2[tabCorres[i]].sh_size);
            }
        }else{
            memcpy(lect3->fichier + section_header_tab3[i].sh_offset, lect2->fichier + section_header_tab1[i-elf_header1->e_shnum].sh_offset, section_header_tab1[i-elf_header1->e_shnum].sh_size);
        }
    }



    //Pour debuger (CTRL + SHIFT + / pour decommenter tout le bloc): 

    ELF_Header *h3 = init_header(lect3);
    afficher_header(h3);
    afficher_section_table(lect3, h3, section_header_tab3);

    for(int i = 0; i < elf_header1->e_shnum; i++){
        afficher_section(lect3, section_header_tab3, i);
        afficher_section(lect1, section_header_tab1, i);
        if(tabCorres[i] != -1)
            afficher_section(lect2, section_header_tab2, tabCorres[i]);
        printf("-----------------------------------------------------------------------------\n");
    }


    //On oublie pas de free les tableaux qu'on a utilisés
    free(tabCorres);
    free(section_header_tab3);

    return lect3;
}




//renvoie un tableau qui respecte la règle suivante : t[i] = j , où i est un index du tableau section_header_tab1 et j un index du tableau section_header_tab2 (j = -1 si il n'y a pas de correspondance à i dans section_header_tab2)
int *tableauCorrespondanceIndex(Lecteur *lect1 ,Lecteur *lect2, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2){
    int *tableauCorres = malloc(sizeof(int)*elf_header1->e_shnum);

    for(int i = 0; i < elf_header1->e_shnum; i++){
        int j= 0;
        char *name1 = getName(lect1, section_header_tab1[i].sh_name);
        char *name2 = getName(lect2, section_header_tab2[j].sh_name);
        while(j < elf_header2->e_shnum && (strcmp(name1,name2) != 0)){
            j++;
            free(name1);
            free(name2);
            name1 = getName(lect1, section_header_tab1[i].sh_name);
            name2 = getName(lect2, section_header_tab2[j].sh_name);
        }
        if(j < elf_header2->e_shnum){
            tableauCorres[i] = j;
        }else{
            tableauCorres[i] = -1;
        }

        free(name1);
        free(name2);
    }

    return tableauCorres;
}


int isInTab(int *tab, int taille, int value){
    int i = 0;
    while(i < taille && tab[i]!=value){
        i++;
    }
    return(i<taille);
}



