#include "fusion.h"
#include "afficheur.h"
#include <string.h>

Lecteur *fusion(Lecteur *lect1 ,Lecteur *lect2 ,Lecteur *lect3, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2, ELF_Symbol *symbol_table1, ELF_Symbol *symbol_table2, ELF_Rel *relocation_table1, ELF_Rel *relocation_table2 ){
    lect3 =fusion_section(lect1 ,lect2 ,lect3, elf_header1 ,elf_header2 , section_header_tab1, section_header_tab2);
    //fusion_symbol();
    //fusion_realloc();
    return lect3;
}


Lecteur *fusion_section(Lecteur *lect1 ,Lecteur *lect2 ,Lecteur *lect3, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2){
    int i =0;
    while (i < EI_NIDENT && elf_header1->e_ident[i] == elf_header2->e_ident[i]){
        i++;
    }
    if(!( i== EI_NIDENT)){
        printf("Erreur, fichier non fusionnable");
        exit(12);
    }
    

    i = 0;

    Elf32_Section_Header *section_header_tab3 = malloc(sizeof(Elf32_Section_Header)*(elf_header2->e_shnum+elf_header1->e_shnum));
    while(i<elf_header1->e_shnum){
        section_header_tab3[i].sh_name = section_header_tab1[i].sh_name;
        section_header_tab3[i].sh_type = section_header_tab1[i].sh_type;
        section_header_tab3[i].sh_flags = section_header_tab1[i].sh_flags;
        section_header_tab3[i].sh_offset = section_header_tab1[i].sh_offset;
        section_header_tab3[i].sh_size = section_header_tab1[i].sh_size;
        section_header_tab3[i].sh_link = section_header_tab1[i].sh_link;
        section_header_tab3[i].sh_info = section_header_tab1[i].sh_info;
        section_header_tab3[i].sh_addralign = section_header_tab1[i].sh_addralign;
        section_header_tab3[i].sh_entsize = section_header_tab1[i].sh_entsize;
        i++;
    }
    // i correspond a la section de la table 1
    // j ................................... 2
    i =0;
    int j =0;
    int k =0;
    int decalage = 0;
    int correspondance[elf_header2->e_shnum]; // numéro de la section i qui correspond a la section j (j étant la position dans le tableau)
                                              // 0xffffffff signifie que la section j n'a pas de correspondance donc, il faut l'écrire a la suite
    for(int o = 0; o < 22; o++ ){
        correspondance[o]= 0;
    }

    while(i<elf_header1->e_shnum){
        j=0;
        if(section_header_tab1[i].sh_type == SHT_PROGBITS){
            while(j < elf_header2->e_shnum && ((section_header_tab2[j].sh_type != SHT_PROGBITS) ||
                    (strcmp(getName(lect1, section_header_tab1[i].sh_name), getName(lect2, section_header_tab2[j].sh_name))))){//tant qu'on a pas de correspondance
                j++;
                printf("val de j;%d; valeur de i:%d\n",j, i);
            }
            //printf("sortie : %d | %d | %d ",j < elf_header2->e_shnum, (section_header_tab2[j].sh_type != SHT_PROGBITS), (strcmp(getName(lect1, section_header_tab1[i].sh_name), getName(lect2, section_header_tab2[j].sh_name))));
            if(j != elf_header2->e_shnum){ // section a concatener, section i a une correspondance avec la section j de la 2è table, décalage a effectuer
                decalage = section_header_tab2[j].sh_size;
                correspondance[j]=i;
                printf("%d correspondance de %d tableau \n",j, i );
                section_header_tab3[i].sh_size += decalage; // la taille de section est aggrandi car concatene
                k=0;
                while(k<elf_header1->e_shnum){ // tant qu'on a pas décaler toute les sections 
                    if(i !=k && section_header_tab1[i].sh_offset <= section_header_tab1[k].sh_offset){ // si la section est après, on décalage offset, et name
                        section_header_tab3[k].sh_name += decalage;    // !! SEULEMENT SI LA STRING TABLE EST APRES notre section i ET QUE CEST UNE ADR ABSOLUE !!
                        section_header_tab3[k].sh_offset += decalage; 
                    }else{
                        section_header_tab3[k].sh_name += decalage;    // !! SEULEMENT SI LA STRING TABLE EST APRES notre section i ET QUE CEST UNE ADR ABSOLUE !!
                    }
                    k++;
                }

            }else{
                correspondance[j] = 0xffffffff;

            }
        }
        i++;
    }
    int max = 0; // adr de la prochaine section
    i =0;
    while(i<elf_header1->e_shnum){
        if(max < section_header_tab3[i].sh_offset + section_header_tab3[i].sh_size ){
            max = section_header_tab3[i].sh_offset + section_header_tab3[i].sh_size;
        }
        i++;
    }
    i = 0;
    j = elf_header1->e_shnum;
    while (i<elf_header2->e_shnum){
        if(correspondance[i] == 0xffffffff){
            section_header_tab3[j].sh_name = section_header_tab2[i].sh_name;
            section_header_tab3[j].sh_type = section_header_tab2[i].sh_type;
            section_header_tab3[j].sh_flags = section_header_tab2[i].sh_flags;
            section_header_tab3[j].sh_offset = section_header_tab2[i].sh_offset;
            section_header_tab3[j].sh_size = section_header_tab2[i].sh_size;
            section_header_tab3[j].sh_link = section_header_tab2[i].sh_link;
            section_header_tab3[j].sh_info = section_header_tab2[i].sh_info;
            section_header_tab3[j].sh_addralign = section_header_tab2[i].sh_addralign;
            section_header_tab3[j].sh_entsize = section_header_tab2[i].sh_entsize;
            j++;

        }
        printf("%d valeur de %d section\n", correspondance[i],i);
        i++;
    }
    elf_header1->e_shnum=j;
    printf("%d est le nb de section a la fin\n", elf_header1->e_shnum);
    afficher_section_table(lect1, elf_header1, section_header_tab3 );
    return lect3;
}







    // while(i<elf_header1->e_shnum){
    //     int j= 0;
    //     while(j < elf_header2->e_shnum && strcmp(getName(lect1, section_header_tab1[i].sh_name), getName(lect2, section_header_tab2[j].sh_name))){
    //         j++;
    //     }
    //     if(j < elf_header2->e_shnum){
    //         section_header_tab1[i].sh_name += section_header_tab2[j].sh_size;
    //         section_header_tab1[i].sh_size += section_header_tab2[j].sh_size;
    //         elf_header1->e_shoff += section_header_tab2[j].sh_size;
    //         int k = 0;
    //         while(k < elf_header1->e_shnum){
    //             if(section_header_tab1[k].sh_offset > section_header_tab1[i].sh_offset){
    //                 section_header_tab1[k].sh_offset += section_header_tab2[j].sh_size;
    //             }
    //             k++;
    //         }
    //     }
    //     i++;
    // }

    // i = 0;
    // memcpy(lect3->fichier, lect1->fichier, sizeof(elf_header1));
    // memcpy(lect3->fichier + 36, &elf_header1->e_shoff, sizeof(unsigned int));
    // while(i < elf_header1->e_shnum){
    //     memcpy(lect3->fichier + section_header_tab1[i].sh_offset, lect1->fichier + tabOffset[i], section_header_tab1[i].sh_size);
    //     memcpy(lect3->fichier + section_header_tab1[i].sh_offset + tabSize[i], lect2->fichier + section_header_tab2[i].sh_offset, section_header_tab2[i].sh_size);
    //     i++;
    // }