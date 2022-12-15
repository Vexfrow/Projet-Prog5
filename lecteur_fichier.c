#include "lecteur_fichier.h"
#include "reader_binaire.h"
#include <string.h>

//Permet de remplir le "Magic number"
void remplirMagic(FILE *fichier, ELF_Header *Header, int taille){
    for(int i = 0; i < taille; i++){
        Header->e_ident[i] = lecture1octet(fichier);
    }
}

#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2
//Permet de récupèrer la classe de l'ELF
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

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2
//Permet de récuperer la manière dont sont encodées les données
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

#define EV_NONE 0
#define EV_CURRENT 1
//Permet de récuperer la version du fichier
char *getVersion(unsigned char c){
    char *version;
    switch (c) {
        case EV_NONE:
            version="1 (Current Version)";
            break;
        case EV_CURRENT:
            version="0 (Invalid Version)";
            break;
        default:
            fprintf(stderr, "ERREUR: Valeur de version invalide: e_ident[EI_VERSION] = 0x%.2hx", c);
            exit(1);
            break;
    }
    return version;
}


//TODO MAXIME
//Permet de récuperer la manière dont sont encodées les données
char *getOSABI(unsigned char c){
    char *OSABI="UNIX - temp";
    /*switch (c) {
        case 0: 
            OSABI = "Invalid data encoding";
            break;
        case 1:
            OSABI = "2's complement values, little endian";
            break;
        case 2 :
            OSABI = "2's complement values, big endian";
            break;
        default:
            fprintf(stderr, "Octet de l'encodage de OS/ABI invalide");
            exit(1);
            break;
    }*/
    return OSABI;
}

#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff
//Permet de récuperer le type
char *getType(uint16_t c){
    char *type;
    switch (c) {
        case ET_NONE: 
            type = "No file type";
            break;
        case ET_REL: 
            type = "Relocatable file";
            break;
        case ET_EXEC: 
            type = "Executable file";
            break;
        case ET_DYN: 
            type = "Shared object file";
            break;
        case ET_CORE: 
            type = "Core file";
            break;
        case ET_LOPROC: 
            type = "Processor-specific";
            break;
        case ET_HIPROC: 
            type = "Processor-specific";
            break;
        default:
            fprintf(stderr, "ERREUR: Valeur de type invalide: e_type = 0x%.4hx\n", c);
            exit(1);
            break;
    }
    return type;
}

#define EM_NONE 0
#define EM_M32 1 
#define EM_SPARC 2
#define EM_386 3
#define EM_68K 4
#define EM_88K 5
#define EM_860 7
#define EM_MIPS 8
#define EM_MIPS_RS4_BE 10
#define RESERVED 11 ... 16
//Permet de récuperer la machine
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


#define EI_MAG0 0x00
#define EI_MAG1 0x01
#define EI_MAG2 0x02
#define EI_MAG3 0x03
#define EI_CLASS 0x04
#define EI_DATA 0x05
#define EI_VERSION 0x06
#define EI_OSABI 0x07
#define EI_ABIVERSION 0x08
#define EI_PAD 0x09
// Affiche le header (similaire à 'arm-none-eabi-readelf -a {file_name}.o')
void afficherHeader(ELF_Header *Header){
    printf("ELF Header :\n");
    printf("  Magic : ");
    for(int i =0; i < 16; i++){
        printf("%.2hx ", Header->e_ident[i]);
    }
    printf("\n");
    printf("  Class : %s\n", getClass(Header->e_ident[EI_CLASS]));
    printf("  Data : %s\n", getDataEncoding(Header->e_ident[EI_DATA]));
    printf("  Version : %s\n",getVersion(Header->e_ident[EI_VERSION]));
    printf("  OS/ABI : %s\n", getOSABI(Header->e_ident[EI_OSABI]));
    printf("  ABI Version : %d\n", Header->e_ident[EI_ABIVERSION]);
    printf("  Type : %s\n", getType(Header->e_type));
    printf("  Machine : %s\n", getMachine(Header->e_machine));
    printf("  Version : 0x%hx\n", Header->e_version);
    printf("  Entry point address : 0x%hx\n", Header->e_entry);
    printf("  Start of program headers : %d (bytes into file)\n", Header->e_phoff);
    printf("  Start of system headers : %d (bytes into file)\n", Header->e_shoff);
    printf( " Flags: 0x%hx\n", Header->e_flags);
}







ELF_Header *init(FILE *fichier){
    //Ajouter les Macros pour N_IDENT 16
    ELF_Header *elf = malloc(sizeof(ELF_Header));
    if(elf == NULL){
        fprintf(stderr, "ERREUR: Pas assez d'espace mémoire");
        exit(1);
    }
    remplirMagic(fichier, elf, 16);
    elf->e_type = lecture2octet(fichier);
    elf->e_machine = lecture2octet(fichier);
    elf->e_version = lecture4octet(fichier);
    elf->e_entry = lecture4octet(fichier);
    elf->e_phoff = lecture4octet(fichier);
    elf->e_shoff = lecture4octet(fichier);
    elf->e_flags = lecture4octet(fichier);
    elf->e_ehsize = lecture2octet(fichier);
    elf->e_phentsize = lecture2octet(fichier);
    elf->e_phnum = lecture2octet(fichier);
    elf->e_shentsize = lecture2octet(fichier);
    elf->e_shnum = lecture2octet(fichier);
    elf->e_shstrndx = lecture2octet(fichier);

    return elf;
}


void init_section_header(FILE *fichier, uint16_t nb, unsigned int adrStart, Elf32_Section_Header *tab){
    int i = 0;
    while ( i < nb){
        int j = 0;
        while(j< 10){
            fseek(fichier, adrStart +40*(i), SEEK_SET);
            int val = lecture4octet(fichier);
            tab[i].sh_name = val;
            val = lecture4octet(fichier);
            tab[i].sh_type = val;
            val = lecture4octet(fichier);
            tab[i].sh_flags = val;
            val = lecture4octet(fichier);
            tab[i].sh_addr = val;
            val = lecture4octet(fichier);
            tab[i].sh_offset = val;
            val = lecture4octet(fichier);
            tab[i].sh_size = val;
            val = lecture4octet(fichier);
            tab[i].sh_link = val;
            val = lecture4octet(fichier);
            tab[i].sh_info = val;
            val = lecture4octet(fichier);
            tab[i].sh_addralign = val;
            val = lecture4octet(fichier);
            tab[i].sh_entsize = val;

            j++; 
        }
        i++;
    }
}



ELF_Symbol *remplirSymbol(FILE *fichier, ELF_Symbol *table, int taille){
    for(int i = 0; i < taille; i++){
        table[i].st_name = lecture4octet(fichier);
        table[i].st_value = lecture4octet(fichier);
        table[i].st_size = lecture4octet(fichier);
        table[i].st_info = lecture1octet(fichier);
        table[i].st_other = lecture1octet(fichier);
        table[i].st_shndx = lecture2octet(fichier);
    } 
    return table;      
}


ELF_Symbol *tableSymbol(FILE *fichier, Elf32_Section_Header *sectionHead, int tailleSectionTable){
    int i = 0;
    while( i < tailleSectionTable && sectionHead[i].sh_type != SHT_SYMTAB){
        i++;
    }
    int taille = sectionHead[i].sh_size / 16;
    ELF_Symbol *table = malloc(sizeof(ELF_Symbol)*taille);
    if(table == NULL){
        fprintf(stderr, "Pas assez de place mémoire");
        exit(1);
    }
    fseek(fichier, sectionHead[i].sh_offset, 0);
    remplirSymbol(fichier, table, taille);
    return table;
}