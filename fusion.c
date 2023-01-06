#include "fusion.h"
#
Lecteur *fusion(Lecteur *lect1 ,Lecteur *lect2 ,Lecteur *lect3, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2, ELF_Symbol *symbol_table1, ELF_Symbol *symbol_table2, ELF_Rel *relocation_table1, ELF_Rel *relocation_table2 ){
    lect3 =fusion_section(lect1 ,lect2 ,lect3, elf_header1 ,elf_header2 , section_header_tab1, section_header_tab2);
    //fusion_symbol();
    //fusion_realloc();
    return lect1;
}


Lecteur *fusion_section(Lecteur *lect1 ,Lecteur *lect2 ,Lecteur *lect3, ELF_Header * elf_header1 ,ELF_Header *elf_header2 ,Elf32_Section_Header *section_header_tab1,Elf32_Section_Header *section_header_tab2){
    
    return lect3;
}