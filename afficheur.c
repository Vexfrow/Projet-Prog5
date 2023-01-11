#include "afficheur.h"
#include "reader_binaire.h"
#include "string.h"



//------------------------- SYMBOL TABLE ----------------------------

char *getBinding(char bind){
    switch(bind){
            case 0:
                return "LOCAL";
                break;
            case 1:
                return "GLOBAL";
                break;
            case 2:
                return "WEAK";
                break;
            case 13:
                return "LOPROC";
                break;
            case 15:
                return "HIPROC";
                break;
            default:
                fprintf(stderr, "Erreur de lecture info");
                exit(2);
                break;
    }
    return NULL;
}


char *getStType(char vis){
    switch(vis){
        case 0:
            return "NOTYPE";
            break;
        case 1:
            return "OBJECT";
            break;
        case 2:
            return "FUNC";
            break;
        case 3:
            return "SECTION";
            break;
        case 4:
            return "FILE";
            break;
        case 13:
            return "LOPROC";
            break;
        case 15:
            return "HIPROC";
            break;
        default:
            fprintf(stderr, "Erreur de lecture info");
            exit(2);
            break;
    }
    return NULL;

}


char *calculNdx(uint16_t ndx, int taille ){
    if(ndx > 0 && ndx <= taille){
            char *res = malloc(sizeof(char)*12);
            sprintf(res, "%d", ndx);
            return res;
        }else{
            switch(ndx){
                case 0:
                    return "UND";
                    break;
                case 0xff00:
                    return "LOPROC";
                    break;
                case 0xff1f:
                    return "HIPROC";
                    break;
                case 0xfff1:
                    return "ABS";
                    break;
                case 0xfff2:
                    return "COMMON";
                    break;
                case 0xffff:
                    return "HIRESERVE";
                    break;
                default:
                    printf("ndx vaut:%d",ndx);
                    fprintf(stderr, "Erreur de lecture ndx");
                    exit(2);
            }

        }
}


//table[i].st_info >> 4

//table[i].st_info & 0xf

void afficherSymbol(Lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *section_header_tab, ELF_Symbol *symbol_table){
    fprintf(stdout, "Num :\tValue \tSize\tType\tBind \tVis \t Ndx\t Name\n");


    int indexSymbolTableSection = getIndexSymbolTableSection(elf_header, section_header_tab);
    int taille = section_header_tab[indexSymbolTableSection].sh_size / 16;

    for(int i =0; i < taille; i++){
        char * name = getName(lecteur, symbol_table[i].st_name);
        char* res = calculNdx(symbol_table[i].st_shndx, elf_header->e_shnum);
        fprintf(stdout, "%d:\t%.8x %d\t%s\t%s\tDEFAULT\t %s\t %s\t\n", i, symbol_table[i].st_value, symbol_table[i].st_size, getStType(symbol_table[i].st_info & 0xf), getBinding(symbol_table[i].st_info >> 4), res,name);
        free(name);
    }
    fprintf(stdout, "\n\n");     
    
}


//---------------------------------- SECTION HEADER -------------------------------------------


char* getShType(unsigned int type){

    switch(type){
        case SHT_NULL : return "SHT_NULL"; break;
        case SHT_PROGBITS : return "SHT_PROGBITS"; break;
        case SHT_SYMTAB : return "SHT_SYMTAB"; break;
        case SHT_STRTAB : return "SHT_STRTAB"; break;
        case SHT_RELA : return "SHT_RELA\t"; break;
        case SHT_HASH : return "SHT_HASH"; break;
        case SHT_DYNAMIC : return "SHT_DYNAMIC"; break;
        case SHT_NOTE : return "SHT_NOTE"; break;
        case SHT_NOBITS : return "SHT_NOBITS"; break;
        case SHT_REL : return "SHT_REL\t"; break;
        case SHT_SHLIB : return "SHT_SHLIB"; break;
        case SHT_DYNSYM : return "SHT_DYNSYM"; break;
        case SHT_LOPROC : return "SHT_LOPROC"; break;
        case SHT_HIPROC : return "SHT_HIPROC"; break;
        case SHT_LOUSER : return "SHT_LOUSER"; break;
        case SHT_HIUSER : return "SHT_HIUSER"; break;
        case SHT_ARM_ATTRIBUTES : return "SHT_ARM_ATTR"; break;
        case SHT_ARM_PREEMPTMAP : return "SHT_ARM_PREE"; break;
        case SHT_ARM_EXIDX : return "SHT_ARM_EXIDX"; break;
        default : return "   ?  \t ";
    }
}


void afficher_sh_flags(unsigned int flags){
    unsigned int res = (flags & SHF_WRITE);
    if (res == SHF_WRITE){
        printf("W");
    }
    res = (flags & SHF_ALLOC );
    if (res == SHF_ALLOC){
        printf("A");
    }
    res = (flags & SHF_EXECINSTR );
    if (res == SHF_EXECINSTR){
        printf("X");
    }
    res = (flags & SHF_MERGE );
    if (res == SHF_MERGE){
        printf("M");
    }
    res = (flags & SHF_STRINGS );
    if (res == SHF_STRINGS){
        printf("S");
    }
    res = (flags & SHF_INFO_LINK );
    if (res == SHF_INFO_LINK){
        printf("I");
    }
    res = (flags & SHF_LINK_ORDER );
    if (res == SHF_LINK_ORDER){
        printf("L");
    }
    res = (flags & SHF_OS_NONCONFORMING );
    if (res == SHF_OS_NONCONFORMING){
        printf("O");
    }
    res = (flags & SHF_GROUP );
    if (res == SHF_GROUP){
        printf("G");
    }
    res = (flags & SHF_TLS );
    if (res == SHF_TLS){
        printf("T");
    }
    res = (flags & SHF_COMPRESSED );
    if (res == SHF_COMPRESSED){
        printf("C");
    }
    res = (flags & SHF_EXCLUDE );
    if (res == SHF_EXCLUDE){
        printf("E");
    }
    if (flags == SHF_MASKOS ){
        printf("o");
    }
    if (flags == SHF_MASKPROC ){
        printf("p");
    }
    printf("\t");

}

void afficher_section_table(Lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *tab){
    printf("Section Headers:\n");
    printf("  [Nr] \t          Name \t         Type \t         Addr \t          Off \t Size \tES \tFlg \tLk\tInf \tAl \n");
    int i = 0;

    while(i < elf_header->e_shnum){
        char *name = getName(lecteur, tab[i].sh_name);
        printf("  [%d]%20s\t%s\t%.8x\t%.6x\t%06x\t%02x\t", i, name, getShType(tab[i].sh_type), tab[i].sh_addr, tab[i].sh_offset, tab[i].sh_size, tab[i].sh_entsize);
        afficher_sh_flags(tab[i].sh_flags);
        printf("%d\t%d\t%x",tab[i].sh_link,tab[i].sh_info,tab[i].sh_addralign);
        free(name);
        i++;
        printf("\n");
    }
    printf("\n\n");
}



void afficher_section(Lecteur *lecteur, Elf32_Section_Header *section_header_tab, int indexSection){
    char *name = getName(lecteur, section_header_tab[indexSection].sh_name);
    printf("Affichage de la section numero %d, de nom %s\n", indexSection, name);
    //printf("Affichage de la section numero %d\n", indexSection);

    free(name);
    int size = section_header_tab[indexSection].sh_size;
    if(!size){
        printf("Il n'y a pas de data dans cette section ");
    }else{
        lecteur->adr = section_header_tab[indexSection].sh_offset;
        int i = 0;
        unsigned char octet =0;
        while(i<size){
            if( !(i%4) && i)
                printf(" ");
            
            if(!(i%16))
                printf("\n 0x%.8x ",i);
            
            octet = lecture1octet(lecteur);
            printf("%.2hhx",octet);
            i++;
        }
        printf("\n");
    }
    printf("\n");
}



//------------------------------------ AFFICHAGE ELF HEADER ---------------------------


char *getClass(unsigned char c){
    char *class;
    switch (c) {
        case ELFCLASSNONE: 
            class = "No Class";
            break;
        case ELFCLASS32:
            class = "ELF32";
            break;
        case ELFCLASS64:
            class = "ELF64";
            break;
        default:
            fprintf(stderr, "ERREUR: Valeur de classe invalide: e_ident[EI_CLASS] = 0x%.2hhx\n", c);
            exit(1);
            break;
    }
    return class;
}

char *getDataEncoding(unsigned char c){
    char *dataEncoding;
    switch (c) {
        case ELFDATANONE: 
            dataEncoding = "Invalid data encoding";
            break;
        case ELFDATA2LSB:
            dataEncoding = "2's complement, little endian";
            break;
        case ELFDATA2MSB:
            dataEncoding = "2's complement, big endian";
            break;
        default:
            fprintf(stderr, "ERREUR: Valeur de data encoding invalide: e_ident[EI_DATA] = 0x%.2hhx", c);
            exit(1);
            break;
    }
    return dataEncoding;
}

char *getVersion(unsigned char c){
    char *version;
    switch (c) {
        case EV_NONE:
            version="0 (Invalid Version)";
            break;
        case EV_CURRENT:
            version="1 (current)";
            break;
        default:
            fprintf(stderr, "ERREUR: Valeur de version invalide: e_ident[EI_VERSION] = 0x%.2hhx", c);
            exit(1);
            break;
    }
    return version;
}


char *getOSABI(unsigned char c){
    char *OSABI="UNIX - System V";
    return OSABI;
}


char *getType(uint16_t c){
    char *type;
    switch (c) {
        case ET_NONE: 
            type = "NONE (No file type)";
            break;
        case ET_REL: 
            type = "REL (Relocatable file)";
            break;
        case ET_EXEC: 
            type = "EXEC (Executable file)";
            break;
        case ET_DYN: 
            type = "DYN (Shared object file)";
            break;
        case ET_CORE: 
            type = "CORE (Core file)";
            break;
        case ET_LOPROC: 
            type = "LOPROC (Processor-specific)";
            break;
        case ET_HIPROC: 
            type = "HIPROC (Processor-specific)";
            break;
        default:
            fprintf(stderr, "ERREUR: Valeur de type invalide: e_type = 0x%.4hx\n", c);
            exit(1);
            break;
    }
    return type;
}

char *getMachine(uint16_t c){
    char *machine ="ARM";
    return machine;
}



void afficherMagic(ELF_Header *Header, int taille){
    for(int i =0; i < taille; i++){
        printf("%.2hhx ", Header->e_ident[i]);
    }
    printf("\n");
}



void afficher_header(ELF_Header *Header){
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for(int i =0; i < 16; i++){
        printf("%.2hhx ", Header->e_ident[i]);
    }
    printf("\n");
    printf("  Class:                             %s\n", getClass(Header->e_ident[EI_CLASS]));
    printf("  Data:                              %s\n", getDataEncoding(Header->e_ident[EI_DATA]));
    printf("  Version:                           %s\n",getVersion(Header->e_ident[EI_VERSION]));
    printf("  OS/ABI:                            %s\n", getOSABI(Header->e_ident[EI_OSABI]));
    printf("  ABI Version:                       %d\n", Header->e_ident[EI_ABIVERSION]);
    printf("  Type:                              %s\n", getType(Header->e_type));
    printf("  Machine:                           %s\n", getMachine(Header->e_machine));
    printf("  Version:                           0x%x\n", Header->e_version);
    printf("  Entry point address:               0x%x\n", Header->e_entry);
    printf("  Start of program headers:          %d (bytes into file)\n", Header->e_phoff);
    printf("  Start of section headers:          %d (bytes into file)\n", Header->e_shoff);
    printf("  Flags:                             0x%x, Version5 EABI\n", Header->e_flags);
    printf("  Size of this header:               %d (bytes)\n", Header->e_ehsize);
    printf("  Size of program headers:           %d (bytes)\n", Header->e_phentsize);
    printf("  Number of program headers:         %d\n", Header->e_phnum);
    printf("  Size of section headers:           %d (bytes)\n", Header->e_shentsize);
    printf("  Number of section headers:         %d\n", Header->e_shnum);
    printf("  Section header string table index: %d\n\n", Header->e_shstrndx);
}


//--------------------------------- RELOCATION TABLE -------------------------------------------------

void afficherRelocations(Lecteur *lecteur, ELF_Header *elf_header, Elf32_Section_Header *section_header_table , ELF_Symbol *symbol_table, ELF_Rel *relocation_table){
    lecteur->adr=0;
    int i =0;
    int j =0;
    int nb=0;


    while(j < elf_header->e_shnum){
        if(section_header_table[j].sh_type == 9){
            i=0;
            char *name = getName(lecteur, section_header_table[j].sh_name);
            if(section_header_table[j].sh_size/8 == 1)
                printf("Relocation section '%s' at offset 0x%x contains %d entry:\n",name, section_header_table[j].sh_offset,(section_header_table[j].sh_size/8) );
            else
                printf("Relocation section '%s' at offset 0x%x contains %d entries:\n", name, section_header_table[j].sh_offset,(section_header_table[j].sh_size/8) );
            free(name);
            printf("Offset \t Info \t \t Type \t \t Sym.Value \t Symbol's Name\n");
            while(i < (section_header_table[j].sh_size/8)){
                printf("%.8x ",relocation_table[nb].r_offset);
                printf("%.8x\t",relocation_table[nb].r_info);
                unsigned char type = relocation_table[nb].r_info;
                affichertypereloc(type);
                int value = relocation_table[nb].r_info >> 8;
                char *name = getName(lecteur, symbol_table[value].st_name);
                if(strcmp(name, ""))
                    printf("%.8x\t",symbol_table[value].st_value);
              
                printf("%s\t",name);
                free(name);
                i++;
                nb++;
                printf("\n");
            }
            printf("\n");
        }
        j++;
    }
}

void affichertypereloc(unsigned char t) {
    switch (t) {
    case R_ARM_NONE:
        printf("R_ARM_NONE        ");
        break;
    case R_ARM_PC24:
        printf("R_ARM_PC24        ");
        break;
    case R_ARM_ABS32:
        printf("R_ARM_ABS32       ");
        break;
    case R_ARM_REL32:
        printf("R_ARM_REL32       ");
        break;
    case R_ARM_ABS16:
        printf("R_ARM_ABS16       ");
        break;
    case R_ARM_ABS12:
        printf("R_ARM_ABS12       ");
        break;
    case R_ARM_THM_ABS5:
        printf("R_ARM_THM_ABS5    ");
        break;
    case R_ARM_ABS8:
        printf("R_ARM_ABS8        ");
        break;
    case R_ARM_SBREL32:
        printf("R_ARM_SBREL32     ");
        break;
    case R_ARM_THM_PC8:
        printf("R_ARM_THM_PC8     ");
        break;
    case R_ARM_TLS_DESC:
        printf("R_ARM_TLS_DESC    ");
        break;
    case R_ARM_THM_SWI8:
        printf("R_ARM_THM_SWI8    ");
        break;
    case R_ARM_XPC25:
        printf("R_ARM_XPC25       ");
        break;
    case R_ARM_THM_XPC22:
        printf("R_ARM_THM_XPC22   ");
        break;
    case R_ARM_TLS_DTPMOD32:
        printf("R_ARM_TLS_DTPMOD32 ");
        break;
    case R_ARM_GLOB_DAT:
        printf("R_ARM_GLOB_DAT    ");
        break;
    case R_ARM_TLS_DTPOFF32:
        printf("R_ARM_TLS_DTPOFF32 ");
        break;
    case R_ARM_TLS_TPOFF32:
        printf("R_ARM_TLS_TPOFF32 ");
        break;
    case R_ARM_COPY:
        printf("R_ARM_COPY        ");
        break;
    case R_ARM_JUMP_SLOT:
        printf("R_ARM_JUMP_SLOT   ");
        break;
    case R_ARM_RELATIVE:
        printf("R_ARM_RELATIVE    ");
        break;
    case R_ARM_PLT32:
        printf("R_ARM_PLT32       ");
        break;
    case R_ARM_CALL:
        printf("R_ARM_CALL        ");
        break;
    case R_ARM_JUMP24:
        printf("R_ARM_JUMP24      ");
        break;
    case R_ARM_THM_JUMP24:
        printf("R_ARM_THM_JUMP24  ");
        break;
    case R_ARM_BASE_ABS:
        printf("R_ARM_BASE_ABS    ");
        break;
    case R_ARM_ALU_PCREL_7_0:
        printf("R_ARM_ALU_PCREL_7_0 ");
        break;
    case R_ARM_ALU_PCREL_15_8:
        printf("R_ARM_ALU_PCREL_15_8 ");
        break;
    case R_ARM_ALU_PCREL_23_15:
        printf("R_ARM_ALU_PCREL_23_15 ");
        break;
    case R_ARM_TARGET1:
        printf("R_ARM_TARGET1     ");
        break;
    case R_ARM_SBREL31:
        printf("R_ARM_SBREL31     ");
        break;
    case R_ARM_V4BX:
        printf("R_ARM_V4BX        ");
        break;
    case R_ARM_TARGET2:
        printf("R_ARM_TARGET2     ");
        break;
    case R_ARM_PREL31:
        printf("R_ARM_PREL31      ");
        break;
    case R_ARM_MOVW_ABS_NC:
        printf("R_ARM_MOVW_ABS_NC ");
        break;
    case R_ARM_THM_MOVT_ABS:
        printf("R_ARM_THM_MOVT_ABS ");
        break;
    case R_ARM_THM_MOVW_PREL_NC:
        printf("R_ARM_THM_MOVW_PREL_NC ");
        break;
    case R_ARM_THM_MOVT_PREL:
        printf("R_ARM_THM_MOVT_PREL ");
        break;
    case R_ARM_THM_JUMP19:
        printf("R_ARM_THM_JUMP19  ");
        break;
    case R_ARM_THM_JUMP6:
        printf("R_ARM_THM_JUMP6   ");
        break;
    case R_ARM_THM_ALU_PREL_11_0:
        printf("R_ARM_THM_ALU_PREL_11_0 ");
        break;
    case R_ARM_THM_PC12:
        printf("R_ARM_THM_PC12    ");
        break;
    case R_ARM_ABS32_NOI:
        printf("R_ARM_ABS32_NOI   ");
        break;
    case R_ARM_REL32_NOI:
        printf("R_ARM_REL32_NOI   ");
        break;
    case R_ARM_MOVT_ABS:
        printf("R_ARM_MOVT_ABS    ");
        break;
    case R_ARM_MOVW_PREL_NC:
        printf("R_ARM_MOVW_PREL_NC ");
        break;
    case R_ARM_MOVT_PREL:
        printf("R_ARM_MOVT_PREL   ");
        break;
    case R_ARM_THM_MOVW_ABS_NC:
        printf("R_ARM_THM_MOVW_ABS_NC ");
        break;
    case R_ARM_ALU_PC_G0_NC:
        printf("R_ARM_ALU_PC_G0_NC ");
        break;
    case R_ARM_ALU_PC_G0:
        printf("R_ARM_ALU_PC_G0   ");
        break;
    case R_ARM_ALU_PC_G1_NC:
        printf("R_ARM_ALU_PC_G1_NC ");
        break;
    case R_ARM_ALU_PC_G1:
        printf("R_ARM_ALU_PC_G1   ");
        break;
    case R_ARM_ALU_PC_G2:
        printf("R_ARM_ALU_PC_G2   ");
        break;
    case R_ARM_LDR_PC_G1:
        printf("R_ARM_LDR_PC_G1   ");
        break;
    case R_ARM_LDR_PC_G2:
        printf("R_ARM_LDR_PC_G2   ");
        break;
    case R_ARM_LDRS_PC_G0:
        printf("R_ARM_LDRS_PC_G0  ");
        break;
    case R_ARM_LDRS_PC_G1:
        printf("R_ARM_LDRS_PC_G1  ");
        break;
    case R_ARM_ALU_SB_G0_NC:
        printf("R_ARM_ALU_SB_G0_NC ");
        break;
    case R_ARM_ALU_SB_G0:
        printf("R_ARM_ALU_SB_G0   ");
        break;
    case R_ARM_ALU_SB_G1_NC:
        printf("R_ARM_ALU_SB_G1_NC ");
        break;
    case R_ARM_ALU_SB_G1:
        printf("R_ARM_ALU_SB_G1   ");
        break;
    case R_ARM_ALU_SB_G2:
        printf("R_ARM_ALU_SB_G2   ");
        break;
    case R_ARM_LDR_SB_G0:
        printf("R_ARM_LDR_SB_G0   ");
        break;
    case R_ARM_LDRS_PC_G2:
        printf("R_ARM_LDRS_PC_G2  ");
        break;
    case R_ARM_LDC_PC_G0:
        printf("R_ARM_LDC_PC_G0   ");
        break;
    case R_ARM_LDRS_SB_G1:
        printf("R_ARM_LDRS_SB_G1  ");
        break;
    case R_ARM_LDRS_SB_G2:
        printf("R_ARM_LDRS_SB_G2  ");
        break;
    case R_ARM_LDC_SB_G0:
        printf("R_ARM_LDC_SB_G0   ");
        break;
    case R_ARM_LDC_SB_G1:
        printf("R_ARM_LDC_SB_G1   ");
        break;
    case R_ARM_LDC_PC_G1:
        printf("R_ARM_LDC_PC_G1   ");
        break;
    case R_ARM_LDC_PC_G2:
        printf("R_ARM_LDC_PC_G2   ");
        break;
    case R_ARM_LDR_SB_G1:
        printf("R_ARM_LDR_SB_G1   ");
        break;
    case R_ARM_LDR_SB_G2:
        printf("R_ARM_LDR_SB_G2   ");
        break;
    case R_ARM_LDRS_SB_G0:
        printf("R_ARM_LDRS_SB_G0  ");
        break;
    case R_ARM_THM_MOVW_BREL_NC:
        printf("R_ARM_THM_MOVW_BREL_NC ");
        break;
    case R_ARM_THM_MOVT_BREL:
        printf("R_ARM_THM_MOVT_BREL ");
        break;
    case R_ARM_THM_MOVW_BREL:
        printf("R_ARM_THM_MOVW_BREL ");
        break;
    case R_ARM_TLS_GOTDESC:
        printf("R_ARM_TLS_GOTDESC ");
        break;
    case R_ARM_LDC_SB_G2:
        printf("R_ARM_LDC_SB_G2   ");
        break;
    case R_ARM_MOVW_BREL_NC:
        printf("R_ARM_MOVW_BREL_NC ");
        break;
    case R_ARM_MOVT_BREL:
        printf("R_ARM_MOVT_BREL   ");
        break;
    case R_ARM_MOVW_BREL:
        printf("R_ARM_MOVW_BREL   ");
        break;
    case R_ARM_TLS_CALL:
        printf("R_ARM_TLS_CALL    ");
        break;
    case R_ARM_TLS_DESCSEQ:
        printf("R_ARM_TLS_DESCSEQ ");
        break;
    case R_ARM_THM_TLS_CALL:
        printf("R_ARM_THM_TLS_CALL ");
        break;
    case R_ARM_PLT32_ABS:
        printf("R_ARM_PLT32_ABS   ");
        break;
    case R_ARM_GOT_ABS:
        printf("R_ARM_GOT_ABS     ");
        break;
    case R_ARM_GOT_PREL:
        printf("R_ARM_GOT_PREL    ");
        break;
    case R_ARM_GOT_BREL12:
        printf("R_ARM_GOT_BREL12  ");
        break;
    case R_ARM_GOTOFF12:
        printf("R_ARM_GOTOFF12    ");
        break;
    case R_ARM_GOTRELAX:
        printf("R_ARM_GOTRELAX    ");
        break;
    case R_ARM_GNU_VTENTRY:
        printf("R_ARM_GNU_VTENTRY ");
        break;
    case R_ARM_TLS_IE32:
        printf("R_ARM_TLS_IE32    ");
        break;
    case R_ARM_TLS_LE32:
        printf("R_ARM_TLS_LE32    ");
        break;
    case R_ARM_TLS_LDO12:
        printf("R_ARM_TLS_LDO12   ");
        break;
    case R_ARM_TLS_LE12:
        printf("R_ARM_TLS_LE12    ");
        break;
    case R_ARM_TLS_IE12GP:
        printf("R_ARM_TLS_IE12GP  ");
        break;
    case R_ARM_ME_TOO:
        printf("R_ARM_ME_TOO      ");
        break;
    case R_ARM_THM_TLS_DESCSEQ16:
        printf("R_ARM_THM_TLS_DESCSEQ16 ");
        break;
    case R_ARM_THM_TLS_DESCSEQ32:
        printf("R_ARM_THM_TLS_DESCSEQ32 ");
        break;
    case R_ARM_THM_GOT_BREL12:
        printf("R_ARM_THM_GOT_BREL12 ");
        break;
    case R_ARM_GNU_VTINHERIT:
        printf("R_ARM_GNU_VTINHERIT ");
        break;
    case R_ARM_TLS_GD32:
        printf("R_ARM_TLS_GD32    ");
        break;
    case R_ARM_TLS_LDM32:
        printf("R_ARM_TLS_LDM32   ");
        break;
    case R_ARM_TLS_LDO32:
        printf("R_ARM_TLS_LDO32   ");
        break;
    case R_ARM_IRELATIVE:
        printf("R_ARM_IRELATIVE   ");
        break;
    default:
        printf("                  ");
        break;
    }
}