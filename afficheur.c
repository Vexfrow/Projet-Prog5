#include "afficheur.h"

char *binding(char bind){
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


char *type(char vis){
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
                    fprintf(stderr, "Erreur de lecture ndx");
                    exit(2);
            }

        }
}


//table[i].st_info >> 4

//table[i].st_info & 0xf

void afficherSymbol(ELF_Symbol *table, int taille){
    fprintf(stdout, "Num :\tValue \tSize\tType\tBind \tVis \t Ndx\t Name\n");
    for(int i =0; i < taille; i++){
        fprintf(stdout, "%d:\t%.8x %d\t%s\t%s\tDEFAULT\t %s\t %.8x\t\n", i, table[i].st_value, table[i].st_size, type(table[i].st_info & 0xf), binding(table[i].st_info >> 4), calculNdx(table[i].st_shndx, taille),table[i].st_name);
    }      
}


void afficherMagic(ELF_Header *Header, int taille){
    for(int i =0; i < taille; i++){
        printf("%.2hx ", Header->e_ident[i]);
    }
    printf("\n");
}


void afficher_sect(Elf32_Section_Header *tab, uint16_t nb){


    int i = 0;
    while(i<nb){
        afficher_sh_name(tab[i].sh_name);
        printf("sh_name:%x  ",tab[i].sh_name);
        printf("type:%x  ",tab[i].sh_type);
        printf("sh_flags:%x  ",tab[i].sh_flags);
        printf("sh_addr:%x  ",tab[i].sh_addr);
        printf("sh_offset:%x  ",tab[i].sh_offset);
        printf("sh_size:%x  ",tab[i].sh_size);
        printf("sh_link:%x  ",tab[i].sh_link);
        printf("sh_info:%x  ",tab[i].sh_info);
        printf("sh_addralign:%x  ",tab[i].sh_addralign);
        printf("sh_entsize:%x  ",tab[i].sh_entsize);
        i++;
        printf(" \n ");
    }
}

void afficher_sh_name(unsigned int name){

    printf("%x", name);
    printf("\t");
    
}

void afficher_sh_type(unsigned int type){

    switch(type){

        case 0 : printf("SHT_NULL"); break;
        case 1 : printf("SHT_PROGBITS"); break;
        case 2 : printf("SHT_SYMTAB"); break;
        case 3 : printf("SHT_STRTAB"); break;
        case 4 : printf("SHT_RELA "); break;
        case 5 : printf("SHT_HASH"); break;
        case 6 : printf("SHT_DYNAMIC"); break;
        case 7 : printf("SHT_NOBITS"); break;
        case 8 : printf("SHT_REL\t"); break;
        case 9 : printf("SHT_SHLIB"); break;
        case 10 : printf("SHT_DYNSYM"); break;
        case 11 : printf("SHT_NULL"); break;
        case 1879048192 : printf("SHT_LOPROC"); break;
        case 2147483647 : printf("SHT_HIPROC"); break;
        case 2147483648 : printf("SHT_LOUSER"); break;
        case 4294967295 : printf("SHT_HIUSER"); break;
        case 1879048195 : printf("SHT_ARM_ATTR"); break;
        case 1879048194 : printf("SHT_ARM_PREE"); break;
        case 1879048193 : printf("SHT_ARM_EXIDX"); break;
        default : printf("   ?  \t ");
    }
    printf("\t");
}

void afficher_sh_addr(unsigned int addr){

    printf("%x\t", addr);

}

void afficher_sh_offset(unsigned int offset){

    printf("%04x\t", offset);

}

void afficher_sh_size(unsigned int size){

    printf("%04x\t", size);

}

void afficher_sh_link(unsigned int link){

    printf("%x\t", link);

}

void afficher_sh_info(unsigned int info){

    printf("%x\t", info);

}

void afficher_sh_addralign(unsigned int addralign){

    printf("%x\t", addralign);


}

void afficher_sh_entsize(unsigned int entsize){

    printf("%x\t", entsize);

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

void afficher_section(Elf32_Section_Header *tab, uint16_t nb){

    printf("Section Headers:\n");
    printf("  [Nr] \t Name \t    Type \t Addr \t Off \t Size \t ES \t Flg \t Lk \t Inf \t Al \n");
    int i = 0;

    while(i<nb){
        
        printf("[%d]\t", i);
        afficher_sh_name(tab[i].sh_name);
        afficher_sh_type(tab[i].sh_type);
        afficher_sh_addr(tab[i].sh_addr);
        afficher_sh_offset(tab[i].sh_offset);
        afficher_sh_size(tab[i].sh_size);
        afficher_sh_entsize(tab[i].sh_entsize);
        afficher_sh_flags(tab[i].sh_flags);
        afficher_sh_link(tab[i].sh_link);
        afficher_sh_info(tab[i].sh_info);
        afficher_sh_addralign(tab[i].sh_addralign);

        i++;
        printf("\n");
    }

}


//------------------------------------AFFICHAGE ELF HEADER---------------------------


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
            fprintf(stderr, "ERREUR: Valeur de classe invalide: e_ident[EI_CLASS] = 0x%.2hx\n", c);
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
            dataEncoding = "2's complement values, little endian";
            break;
        case ELFDATA2MSB:
            dataEncoding = "2's complement values, big endian";
            break;
        default:
            fprintf(stderr, "ERREUR: Valeur de data encoding invalide: e_ident[EI_DATA] = 0x%.2hx", c);
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
            version="1 (Current Version)";
            break;
        default:
            fprintf(stderr, "ERREUR: Valeur de version invalide: e_ident[EI_VERSION] = 0x%.2hx", c);
            exit(1);
            break;
    }
    return version;
}


//TODO MAXIME
char *getOSABI(unsigned char c){
    char *OSABI="UNIX - TODO";
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
    char *machine;
    switch (c) {
        case EM_NONE: 
            machine = "No machine";
            break;
        case EM_M32: 
            machine = "AT&T WE 32100";
            break;
        case EM_SPARC: 
            machine = "SPARC";
            break;
        case EM_386: 
            machine = "Intel Architecture";
            break;
        case EM_68K: 
            machine = "Motorola 68000";
            break;
        case EM_88K: 
            machine = "Motorola 88000";
            break;
        case EM_860: 
            machine = "Intel 80860";
            break;
        case EM_MIPS: 
            machine = "MIPS RS3000 Big-Endian";
            break;
        case EM_MIPS_RS4_BE: 
            machine = "MIPS RS4000 Big-Endian";
            break;
        case RESERVED: 
            machine = "Reserved for future use";
            break;
        default:
            machine = "Machine non répertoriée";
            //fprintf(stderr, "ERREUR: Valeur de machine invalide: e_type = 0x%.4hx\n", c);
            //exit(1);
            break;
    }
    return machine;
}


void afficher_header(ELF_Header *Header){
    printf("ELF Header :\n");
    printf("  Magic : ");
    for(int i =0; i < 16; i++){
        printf("%.2hx ", Header->e_ident[i]);
    }
    printf("\n");
    printf("  Class:                             %s\n", getClass(Header->e_ident[EI_CLASS]));
    printf("  Data:                              %s\n", getDataEncoding(Header->e_ident[EI_DATA]));
    printf("  Version:                           %s\n",getVersion(Header->e_ident[EI_VERSION]));
    printf("  OS/ABI:                            %s\n", getOSABI(Header->e_ident[EI_OSABI]));
    printf("  ABI Version:                       %d\n", Header->e_ident[EI_ABIVERSION]);
    printf("  Type:                              %s\n", getType(Header->e_type));
    printf("  Machine:                           %s\n", getMachine(Header->e_machine));
    printf("  Version:                           0x%hx\n", Header->e_version);
    printf("  Entry point address:               0x%hx\n", Header->e_entry);
    printf("  Start of program headers:          %d (bytes into file)\n", Header->e_phoff);
    printf("  Start of system headers:           %d (bytes into file)\n", Header->e_shoff);
    printf("  Flags:                             0x%x\n", Header->e_flags);
    printf("  Size of headers:                   %d (bytes)\n", Header->e_ehsize);
    printf("  Size of program headers:           %d (bytes)\n", Header->e_phentsize);
    printf("  Number of program headers:         %d\n", Header->e_phnum);
    printf("  Size of section headers:           %d (bytes)\n", Header->e_shentsize);
    printf("  Number of section headers:         %d\n", Header->e_shnum);
    printf("  Section header string table index: %d\n", Header->e_shstrndx);
    printf("\n");
}