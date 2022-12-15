#include "afficheur.h"

void afficherSymbol(ELF_Symbol *table, int taille){
    fprintf(stdout, "Num : Value\tSize\tType\tBind\tVis\tNdx Name");
    char *type="";
    char *bind="";
    char *ndx="";
    for(int i =0; i < taille; i++){
        //Calcul du binding
        switch(table[i].st_info >> 4){
            case 0:
                bind="LOCAL";
                break;
            case 1:
                bind="GLOBAL";
                break;
            case 2:
                bind ="WEAK";
                break;
            case 13:
                bind = "LOPROC";
                break;
            case 15:
                bind = "HIPROC";
                break;
            default:
                fprintf(stderr, "Erreur de lecture info");
                exit(2);
                break;
        }

        // Calcul de la visibilité
        switch(table[i].st_info & 0xf){
            case 0:
                type="NOTYPE";
                break;
            case 1:
                type="OBJECT";
                break;
            case 2:
                type ="FUNC";
                break;
            case 3:
                type="SECTION";
                break;
            case 4:
                type="FILE";
                break;
            case 13:
                type = "LOPROC";
                break;
            case 15:
                type = "HIPROC";
                break;
            default:
                fprintf(stderr, "Erreur de lecture info");
                exit(2);
                break;
        }

        if(table[i].st_shndx > 0 && table[i].st_shndx <= taille){
            ndx="%d",table[i].st_shndx;
        }else{
            switch(table[i].st_shndx){
                case 0:
                    ndx="UND";
                    break;
                case 0xff00:
                    ndx="LOPROC";
                    break;
                case 0xff1f:
                    ndx="HIPROC";
                    break;
                case 0xfff1:
                    ndx="ABS";
                    break;
                case 0xfff2:
                    ndx="COMMON";
                    break;
                case 0xffff:
                    ndx="HIRESERVE";
                    break;
                default:
                    fprintf(stderr, "Erreur de lecture ndx");
                    exit(2);
            }

        }
        fprintf(stdout, "%d: %.8x %.8x DEFAULT %.8x ", i, table[i].st_value, table[i].st_size, table[i].st_name);
    }      
}

void afficherMagic(ELF_Header *Header, int taille){
    for(int i =0; i < taille; i++){
        printf("%.2hx ", Header->e_ident[i]);
    }
    printf("\n");
}