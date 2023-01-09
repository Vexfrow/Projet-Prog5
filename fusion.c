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
    ELF_Header *h3 = init_header(lect3);
    Elf32_Section_Header *section_header_tab3 = init_section_header(lect3, h3);
    lect3 = fusion_relocation(lect1, lect2, lect3,  section_header_tab1, section_header_tab2, section_header_tab3, elf_header1 ,elf_header2 , h3);
    


































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

            if(tabCorres[i] != -1)
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



    //On oublie pas de free les tableaux qu'on a utilisés
    free(tabCorres);
    free(section_header_tab3);

    return lect3;
}





Lecteur *fusion_symbol(Lecteur *lect1, Lecteur *lect2, Lecteur *lect3, ELF_Header *elf_header1, ELF_Header *elf_header2, Elf32_Section_Header *section_header_tab1, Elf32_Section_Header *section_header_tab2, ELF_Symbol *symbol_table1, ELF_Symbol *symbol_table2){

    //On récupère la taille de la table des symboles 
    int indexSymbolTableSection1 = getIndexSymbolTableSection(elf_header1, section_header_tab1);
    int taille1 = section_header_tab1[indexSymbolTableSection1].sh_size / 16;
    int indexSymbolTableSection2 = getIndexSymbolTableSection(elf_header2, section_header_tab2);
    int taille2 = section_header_tab2[indexSymbolTableSection2].sh_size / 16;

    ELF_Header *elf_header3 = init_header(lect3);
    Elf32_Section_Header *section_header_tab3 = init_section_header(lect3, elf_header3);

    //On crée une nouvelle table des symboles
    ELF_Symbol *symbol_tab3 = malloc(sizeof(ELF_Symbol)*(taille1 + taille2));


    // //On initialise le tableau necessaire
    int* tabCorresSym = tableauCorrespondanceIndexSym(lect1, lect2, elf_header1, elf_header2, section_header_tab1, section_header_tab2, symbol_table1, symbol_table2);
    uint16_t nbSymbol = 0;


    for(int i = 0; i < taille1 + taille2; i++){
        if(i < taille1){
            //Si local
            if((symbol_table1[i].st_info >> 4) == 0){
                symbol_tab3[nbSymbol].st_name = symbol_table1[i].st_name - section_header_tab1[section_header_tab1[getIndexSymbolTableSection(elf_header1, section_header_tab1)].sh_link].sh_offset;
                symbol_tab3[nbSymbol].st_value = symbol_table1[i].st_value;
                symbol_tab3[nbSymbol].st_size = symbol_table1[i].st_size;
                symbol_tab3[nbSymbol].st_info = symbol_table1[i].st_info;
                symbol_tab3[nbSymbol].st_other = symbol_table1[i].st_other;
                symbol_tab3[nbSymbol].st_shndx = majNdx(lect1, lect3, elf_header3, section_header_tab1, section_header_tab3, symbol_table1[i].st_shndx);

                if((symbol_tab3[i].st_info & 0xf) == STT_SECTION){
                    symbol_tab3[i].st_name = section_header_tab3[symbol_tab3[i].st_shndx].sh_name;
                }else{
                    symbol_tab3[i].st_name = section_header_tab3[section_header_tab3[getIndexSymbolTableSection(elf_header3, section_header_tab3)].sh_link].sh_offset + symbol_tab3[i].st_name;
                }


                nbSymbol++;
            //Sinon global
            }else if(symbol_table1[i].st_info >> 4 == 1){
                //Si il est en couple
                if(tabCorresSym[i] != -1){
                    //Si les deux sont définis
                    if(symbol_table1[i].st_shndx != 0 && symbol_table1[tabCorresSym[i]].st_shndx != 0){
                        fprintf(stderr, "L'edition de lien à échoué (deux symboles globaux de même nom, définis), index %d", i);
                        exit(1);
                    //Sinon si le premier est indefinis
                    }else if((symbol_table1[i].st_info & 0xf) == 0){
                        symbol_tab3[nbSymbol].st_name = symbol_table1[i].st_name - section_header_tab1[section_header_tab1[getIndexSymbolTableSection(elf_header1, section_header_tab1)].sh_link].sh_offset;   //C'est NARMOL que ce soit tab1 et pas tab2                    symbol_tab3[nbSymbol].st_value = symbol_table2[tabCorresSym[i]].st_value;
                        symbol_tab3[nbSymbol].st_size = symbol_table2[tabCorresSym[i]].st_size;
                        symbol_tab3[nbSymbol].st_info = symbol_table2[tabCorresSym[i]].st_info;
                        symbol_tab3[nbSymbol].st_other = symbol_table2[tabCorresSym[i]].st_other;
                        symbol_tab3[nbSymbol].st_shndx = majNdx(lect2, lect3, elf_header3, section_header_tab2, section_header_tab3, symbol_table2[tabCorresSym[i]].st_shndx);
                        symbol_tab3[nbSymbol].st_name = section_header_tab3[section_header_tab3[getIndexSymbolTableSection(elf_header3, section_header_tab3)].sh_link].sh_offset + symbol_tab3[i].st_name;
                        
                        nbSymbol++;
                    //Sinon le deuxième est indefnis
                    }else{
                        symbol_tab3[nbSymbol].st_name = symbol_table1[i].st_name - section_header_tab1[section_header_tab1[getIndexSymbolTableSection(elf_header1, section_header_tab1)].sh_link].sh_offset;
                        symbol_tab3[nbSymbol].st_value = symbol_table1[i].st_value;
                        symbol_tab3[nbSymbol].st_size = symbol_table1[i].st_size;
                        symbol_tab3[nbSymbol].st_info = symbol_table1[i].st_info;
                        symbol_tab3[nbSymbol].st_other = symbol_table1[i].st_other;
                        symbol_tab3[nbSymbol].st_shndx = majNdx(lect1, lect3, elf_header3, section_header_tab1, section_header_tab3, symbol_table1[tabCorresSym[i]].st_shndx);
                        
                        if((symbol_tab3[nbSymbol].st_info & 0xf) == STT_SECTION){
                            symbol_tab3[nbSymbol].st_name = section_header_tab3[symbol_tab3[nbSymbol].st_shndx].sh_name;
                        }else{
                            symbol_tab3[nbSymbol].st_name = section_header_tab3[section_header_tab3[getIndexSymbolTableSection(elf_header3, section_header_tab3)].sh_link].sh_offset + symbol_tab3[nbSymbol].st_name;
                        }
                                nbSymbol++;
                    }
                //Sinon il est celibataire
                }else{
                    symbol_tab3[nbSymbol].st_name = symbol_table1[i].st_name - section_header_tab1[section_header_tab1[getIndexSymbolTableSection(elf_header1, section_header_tab1)].sh_link].sh_offset;
                    symbol_tab3[nbSymbol].st_value = symbol_table1[i].st_value;
                    symbol_tab3[nbSymbol].st_size = symbol_table1[i].st_size;
                    symbol_tab3[nbSymbol].st_info = symbol_table1[i].st_info;
                    symbol_tab3[nbSymbol].st_other = symbol_table1[i].st_other;
                    symbol_tab3[nbSymbol].st_shndx = majNdx(lect1, lect3, elf_header3, section_header_tab1, section_header_tab3, symbol_table1[tabCorresSym[i]].st_shndx);
                    if((symbol_tab3[nbSymbol].st_info & 0xf) == STT_SECTION){
                            symbol_tab3[nbSymbol].st_name = section_header_tab3[symbol_tab3[i].st_shndx].sh_name;
                        }else{
                            symbol_tab3[nbSymbol].st_name = section_header_tab3[section_header_tab3[getIndexSymbolTableSection(elf_header3, section_header_tab3)].sh_link].sh_offset + symbol_tab3[nbSymbol].st_name;
                    }
                    nbSymbol++;
                }
            }
        }else{
            //Si local et si type = section et qu'il est celibataire ou que non section et pas le premier de la deuxième table 
            if(((symbol_table2[i-taille1].st_info >> 4) == 0) && (((symbol_table2[i-taille1].st_info & 0xf) == 3 && tabCorresInverse(tabCorresSym, taille1, i-taille1) == -1) || ((symbol_table2[i-taille1].st_info & 0xf) != 3 && i-taille1 != 0))){
                    symbol_tab3[nbSymbol].st_name = symbol_table2[i-taille1].st_name - section_header_tab2[section_header_tab2[getIndexSymbolTableSection(elf_header2, section_header_tab2)].sh_link].sh_offset;
                    symbol_tab3[nbSymbol].st_value = symbol_table2[i-taille1].st_value;
                    symbol_tab3[nbSymbol].st_size = symbol_table2[i-taille1].st_size;
                    symbol_tab3[nbSymbol].st_info = symbol_table2[i-taille1].st_info;
                    symbol_tab3[nbSymbol].st_other = symbol_table2[i-taille1].st_other;
                    symbol_tab3[nbSymbol].st_shndx = majNdx(lect2, lect3, elf_header3, section_header_tab2, section_header_tab3, symbol_table2[i-taille1].st_shndx);
                    
                    if((symbol_tab3[nbSymbol].st_info & 0xf) == STT_SECTION){
                            symbol_tab3[nbSymbol].st_name = section_header_tab3[symbol_tab3[i].st_shndx].sh_name;
                        }else{
                            printf("adr rela = %d  ;; addr gene = %d ;; size = %d\n",  symbol_tab3[nbSymbol].st_name, section_header_tab3[section_header_tab3[getIndexSymbolTableSection(elf_header3, section_header_tab3)].sh_link].sh_offset, section_header_tab1[section_header_tab1[getIndexSymbolTableSection(elf_header1, section_header_tab1)].sh_link].sh_size);
                            symbol_tab3[nbSymbol].st_name += section_header_tab3[section_header_tab3[getIndexSymbolTableSection(elf_header3, section_header_tab3)].sh_link].sh_offset + section_header_tab1[section_header_tab1[getIndexSymbolTableSection(elf_header1, section_header_tab1)].sh_link].sh_size;
                    }

                    nbSymbol++;
            //Sinon global (celib)
            }else if(tabCorresInverse(tabCorresSym, taille1, i-taille1) == -1){
                symbol_tab3[nbSymbol].st_name = symbol_table2[i-taille1].st_name - section_header_tab2[section_header_tab2[getIndexSymbolTableSection(elf_header2, section_header_tab2)].sh_link].sh_offset;
                symbol_tab3[nbSymbol].st_value = symbol_table2[i-taille1].st_value;
                symbol_tab3[nbSymbol].st_size = symbol_table2[i-taille1].st_size;
                symbol_tab3[nbSymbol].st_info = symbol_table2[i-taille1].st_info;
                symbol_tab3[nbSymbol].st_other = symbol_table2[i-taille1].st_other;
                symbol_tab3[nbSymbol].st_shndx = majNdx(lect2, lect3, elf_header3, section_header_tab2, section_header_tab3, symbol_table2[i-taille1].st_shndx);

                symbol_tab3[nbSymbol].st_name += section_header_tab3[section_header_tab3[getIndexSymbolTableSection(elf_header3, section_header_tab3)].sh_link].sh_offset + section_header_tab1[section_header_tab1[getIndexSymbolTableSection(elf_header1, section_header_tab1)].sh_link].sh_size;
                
                nbSymbol++;
            }
        }
    }
    ELF_Header *h3 = init_header(lect3);
    Elf32_Section_Header *sh3 = init_section_header(lect3, h3);
    //ELF_Symbol *s3 = init_symbol_table(lect3, h3, sh3);

    afficherSymbol(lect3, h3, sh3, symbol_tab3);


    //TODO MAXIME
    free(tabCorresSym);
    //free(symbol_tab3);
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



//Même fonction que la précédente mais pour la table des symboles
int *tableauCorrespondanceIndexSym(Lecteur *lect1 ,Lecteur *lect2, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2, ELF_Symbol *symbol_table1, ELF_Symbol *symbol_table2){

    //On récupère la taille de la table des symboles et on initialise la tableau
    int indexSymbolTableSection1 = getIndexSymbolTableSection(elf_header1, section_header_tab1);
    int taille1 = section_header_tab1[indexSymbolTableSection1].sh_size / 16;
    int indexSymbolTableSection2 = getIndexSymbolTableSection(elf_header2, section_header_tab2);
    int taille2 = section_header_tab2[indexSymbolTableSection2].sh_size / 16;
    int *tableauCorresSym = malloc(sizeof(int)*taille1);

    for(int i = 0; i < taille1; i++){
        int j= 0;
        char *name1 = getName(lect1, symbol_table1[i].st_name);
        char *name2 = getName(lect2, symbol_table2[j].st_name);
        while(j < taille2 && (strcmp(name1,name2) != 0)){
            j++;
            free(name1);
            free(name2);
            name1 = getName(lect1, symbol_table1[i].st_name);
            name2 = getName(lect2, symbol_table2[j].st_name);
        }
        if(j < taille2){
            tableauCorresSym[i] = j;
        }else{
            tableauCorresSym[i] = -1;
        }
        free(name1);
        free(name2);
    }

    return tableauCorresSym;
}


//Permet de 
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



//Permet de mettre à jour le ndx des items dans la table des symboles
uint32_t majNdx(Lecteur *lect1, Lecteur *lect3, ELF_Header *elf_header2, Elf32_Section_Header *section_header_tab1, Elf32_Section_Header *section_header_tab2, int oldNdx){

    int i=0;
    int res = -1;
    if(oldNdx > 0 && oldNdx < elf_header2->e_shnum){
        char *name = getName(lect1, section_header_tab1[oldNdx].sh_name);
        while(i < elf_header2->e_shnum && res == -1){
            char *name2 = getName(lect3, section_header_tab2[i].sh_name);
            if (strcmp(name,name2) == 0){
                res = i;
            }
            free(name2);
            i++;
        }
        free(name);
    }else{
        res = oldNdx;
    }
    if(res == -1){
        return oldNdx;
    }

    return res;

}

int taille_relocation_table(Elf32_Section_Header *section, ELF_Header *elf_header){
    //int i=0;
    int nb_rel = 0;
    // while( i < elf_header->e_shnum){
    //     if(section_header_tab[i].sh_type == SHT_REL){
    //         nb_rel = nb_rel + (section_header_tab[i].sh_size/8);
    //     }
    //     i++;
    // }
    return nb_rel;
}

int compStringBool(Lecteur *lect1 ,Lecteur *lect2, Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2, int indice1, int indice2){
    char *name1 = getName(lect1, section_header_tab1[indice1].sh_name);
    char *name2 = getName(lect2, section_header_tab2[indice2].sh_name);
    int res = strcmp(name1,name2) ==0 ;
    free(name1);
    free(name2);
    return res;
}

couple *corresCouple(Lecteur *lect1, Lecteur *lect2, Lecteur *lect3, Elf32_Section_Header *section_header_tab1, Elf32_Section_Header *section_header_tab2, Elf32_Section_Header *section_header_tab3, ELF_Header *elf_header1, ELF_Header *elf_header2, ELF_Header *elf_header3 ){
    couple *correspondance = malloc(sizeof(couple)*elf_header3->e_shnum);
    int i=0;
    int k = 0;

    while(i < elf_header3->e_shnum){
        if(section_header_tab3[i].sh_type == SHT_REL){
            k=0;
            while(k < elf_header1->e_shnum && !compStringBool(lect3, lect1, section_header_tab3, section_header_tab1, i, k)){
                k++;
            }
            if(k >= elf_header1->e_shnum){
                correspondance[i].tab1 = -1;
                correspondance[i].tab2 = -1;
            }else{
                correspondance[i].tab1 = k;
                correspondance[i].tab2 = -1;
            }

            k=0;
            while(k < elf_header2->e_shnum && !compStringBool(lect3, lect2, section_header_tab3, section_header_tab2, i, k)){
                k++;
            }
            if(k < elf_header2->e_shnum){
                correspondance[i].tab2 = k;
            }

        }else{
            correspondance[i].tab1 = -1;
            correspondance[i].tab2 = -1;
        }
        fprintf(stderr, "corres %d : %d, %d\n",i, correspondance[i].tab1, correspondance[i].tab2);
        i++;

    }
    
    
    return correspondance;
}


Lecteur *fusion_relocation(Lecteur *lect1, Lecteur *lect2, Lecteur *lect3, Elf32_Section_Header *section_header_tab1, Elf32_Section_Header *section_header_tab2, Elf32_Section_Header * section_header_tab3, ELF_Header *elf_header1, ELF_Header *elf_header2, ELF_Header *elf_header3){
    int nb_rel=0;
    int i =0;
    int k = 0;
    couple *correspondance = corresCouple(lect1, lect2, lect3, section_header_tab1, section_header_tab2, section_header_tab3, elf_header1, elf_header2, elf_header3);
    ELF_Rel *rel_tab;
    int indice1;
    int indice2;
    int size;
    while(i < elf_header3->e_shnum){
        nb_rel=0;
        if(correspondance[i].tab1 != -1 && correspondance[i].tab2 != -1){
            indice1 = section_header_tab1[correspondance[i].tab1].sh_size/8;
            indice2 = section_header_tab2[correspondance[i].tab2].sh_size/8;
            nb_rel = indice1+ indice2;
            rel_tab = malloc(sizeof(ELF_Rel)*nb_rel);
            k = 0;
            lect1->adr = section_header_tab1[correspondance[i].tab1].sh_offset;
            lect2->adr = section_header_tab2[correspondance[i].tab2].sh_offset;
            while(k < indice1){
                rel_tab[k].r_offset = lecture4octet(lect1);
                rel_tab[k].r_info = lecture4octet(lect1);
                k++;
            }
            while(k < indice1 + indice2){
                rel_tab[k].r_offset = lecture4octet(lect2);
                rel_tab[k].r_info = lecture4octet(lect2);
                k++;
            }
            //Modifications
            size = (indice1+indice2)*8;
            memcpy(lect3->fichier + section_header_tab3[i].sh_offset, rel_tab, size);
            free(rel_tab);
        }else if(correspondance[i].tab1 != -1 && correspondance[i].tab2 == -1){
            indice1 = section_header_tab1[correspondance[i].tab1].sh_size/8;
            rel_tab = malloc(sizeof(ELF_Rel)*indice1);
            lect1->adr = section_header_tab1[correspondance[i].tab1].sh_offset;
            k=0;
            while(k < indice1){
                rel_tab[k].r_offset = lecture4octet(lect1); //Ne pas modifier quand la rel est seule
                rel_tab[k].r_info = lecture4octet(lect1); //a modifier avec la string table
                k++;
            }
            size = indice1*8;
            memcpy(lect3->fichier + section_header_tab3[i].sh_offset, rel_tab, size);
            free(rel_tab);       
        }else if(correspondance[i].tab1 == -1 && correspondance[i].tab2 != -1){
            indice2 = section_header_tab2[correspondance[i].tab2].sh_size/8;
            rel_tab = malloc(sizeof(ELF_Rel)*indice2);
            lect2->adr = section_header_tab2[correspondance[i].tab2].sh_offset;
            k=0;
            while(k < indice2){
                rel_tab[k].r_offset = lecture4octet(lect2); //Ne pas modifier quand la rel est seule
                rel_tab[k].r_info = lecture4octet(lect2); //a modifier avec la string table
                k++;
            }
            size = indice2*8;
            memcpy(lect3->fichier + section_header_tab3[i].sh_offset, rel_tab, size);
            free(rel_tab);  
        }
        i++;
        
    }
    return lect3;
}
