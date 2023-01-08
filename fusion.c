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
        section_header_tab3[i].sh_name = section_header_tab1[i].sh_name - section_header_tab1[elf_header1->e_shstrndx].sh_offset; //On reset l'adresse contenue dans nom (dans le section_header_tab1 c'est une adresse absolue qui est contenue, on la refait passer en une adresse relative)
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
    int* tabCorres = tableauCorrespondanceIndex(lect1 ,lect2 ,elf_header1 ,elf_header2 ,section_header_tab1, section_header_tab2);
    uint16_t nbSection = elf_header1->e_shnum;
    unsigned int offsetSh = elf_header1->e_shoff;


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

//On rajoute les sections présentes dans la deuxieme table des section mais pas dans la première
    for(int i= 0; i < elf_header2->e_shnum ; i++){
        if(tabCorresInverse(tabCorres, elf_header1->e_shnum, i) == -1){
            section_header_tab3[nbSection].sh_name = section_header_tab2[i].sh_name + (offsetSh - elf_header1->e_shoff) - section_header_tab2[elf_header2->e_shstrndx].sh_offset; // MAJ de l'adresse du nom (passage à une adresse relative)
            section_header_tab3[nbSection].sh_type = section_header_tab2[i].sh_type;
            section_header_tab3[nbSection].sh_flags = section_header_tab2[i].sh_flags;
            section_header_tab3[nbSection].sh_addr = section_header_tab2[i].sh_addr;
            section_header_tab3[nbSection].sh_offset = section_header_tab2[i].sh_offset + (offsetSh - elf_header1->e_shoff); // MAJ de l'offset
            section_header_tab3[nbSection].sh_size = section_header_tab2[i].sh_size;
            section_header_tab3[nbSection].sh_link = tabCorresInverse(tabCorres, elf_header1->e_shnum, section_header_tab2[i].sh_link); //MAJ du link avec le nouvel index du symtab
            section_header_tab3[nbSection].sh_info = section_header_tab2[i].sh_info;
            section_header_tab3[nbSection].sh_addralign = section_header_tab2[i].sh_addralign;
            section_header_tab3[nbSection].sh_entsize = section_header_tab2[i].sh_entsize;
            nbSection++;
        }
    }

    //On copie les nouvelles données dans le lecteur3
    memcpy(lect3->fichier, lect1->fichier, sizeof(ELF_Header)); //ELF_Header 

    for(int i = 0; i < nbSection ;i++){
        if(i < elf_header1->e_shnum){
            memcpy(lect3->fichier + section_header_tab3[i].sh_offset, lect1->fichier + section_header_tab1[i].sh_offset, section_header_tab1[i].sh_size);

            if(tabCorres[i] != -1 && section_header_tab1[i].sh_type == SHT_PROGBITS)
                memcpy(lect3->fichier + section_header_tab3[i].sh_offset + section_header_tab1[i].sh_size, lect2->fichier + section_header_tab2[tabCorres[i]].sh_offset, section_header_tab2[tabCorres[i]].sh_size);

        }else{
            memcpy(lect3->fichier + section_header_tab3[i].sh_offset, lect2->fichier + section_header_tab1[i-elf_header1->e_shnum].sh_offset, section_header_tab1[i-elf_header1->e_shnum].sh_size);
        }
    }

    //Gestion du cas où les fichiers sont en big endian (les valeurs dans le lecteur sont aussi en big endian donc il faut adapté notre memcpy pour qu'il puisse copier une valeur à l'origine en little endian en big endian)
    int res = endianValue(offsetSh, elf_header1->e_ident[5], sizeof(int));
    memcpy(lect3->fichier + 32, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header

    res = endianValue(nbSection, elf_header1->e_ident[5], sizeof(uint16_t));
    memcpy(lect3->fichier + 48, (char *) &res, sizeof(uint16_t)); //maj de l'offset du section header

    for(int i = 0; i < nbSection ;i++){
        res = endianValue(section_header_tab3[i].sh_name, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header
        res = endianValue(section_header_tab3[i].sh_type, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i + 4, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header
        res = endianValue(section_header_tab3[i].sh_flags, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i + 8, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header
        res = endianValue(section_header_tab3[i].sh_addr, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i + 12, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header
        res = endianValue(section_header_tab3[i].sh_offset, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i + 16, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header
        res = endianValue(section_header_tab3[i].sh_size, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i + 20, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header
        res = endianValue(section_header_tab3[i].sh_link, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i + 24, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header
        res = endianValue(section_header_tab3[i].sh_info, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i + 28, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header
        res = endianValue(section_header_tab3[i].sh_addralign, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i + 32, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section header
        res = endianValue(section_header_tab3[i].sh_entsize, elf_header1->e_ident[5], sizeof(int));
        memcpy(lect3->fichier + offsetSh + 40*i + 36, (char *) &res, sizeof(unsigned int)); //maj de l'offset du section heade
    }


    //memcpy(lect3->fichier + offsetSh, (char*)&section_header_tab3, sizeof(Elf32_Section_Header)*nbSection); //ELF_Header 






    //Pour debuger (CTRL + SHIFT + / pour decommenter tout le bloc): 

    //ELF_Header *h3 = init_header(lect3);
    //afficher_header(h3);
    //afficher_section_table(lect2, elf_header2, section_header_tab2);

    // for(int i = 0; i < elf_header1->e_shnum; i++){
    //     afficher_section(lect3, section_header_tab3, i);
    //     afficher_section(lect1, section_header_tab1, i);
    //     if(tabCorres[i] != -1)
    //         afficher_section(lect2, section_header_tab2, tabCorres[i]);
    //     printf("-----------------------------------------------------------------------------\n");
    // }


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


int tabCorresInverse(int *tab, int taille, int value){
    int i = 0;
    while(i < taille && tab[i]!=value){
        i++;
    }
    int res = (i<taille) ? i : -1;
    return res;
}


//Si la machine lit en little endian et que l'encodage de l'executable est en little (ou idem mais avec big endian) -> pas de changement
//Sinon on modifie la valeur pour qu'elle corresponde à l'encodage du fichier
unsigned int endianValue(unsigned int valueLittleEndian, int valueELFEndian, int tailleOctet){
    unsigned int res = 0;
    int endian = 1;
    if((*(char *)&endian == 1 && valueELFEndian == 2) || (*(char *)&endian == 0 && valueELFEndian == 1)){
        if(tailleOctet == sizeof(int)){ //Si quatre octets
            res |= ((0x000000ff & valueLittleEndian) << 24);
            res |= ((0x0000ff00 & valueLittleEndian) << 8);
            res |= ((0x00ff0000 & valueLittleEndian) >> 8);
            res |= ((0xff000000 & valueLittleEndian) >> 24);
        }else if(tailleOctet == sizeof(uint16_t)){ //Si deux octets
            res |= ((0x00ff & valueLittleEndian) << 8);
            res |= ((0xff00 & valueLittleEndian) >> 8);           
        }else{
            res = valueLittleEndian; //Si un seul octet
        }

    }else{
        res = valueLittleEndian;
    }

    return res;
}



