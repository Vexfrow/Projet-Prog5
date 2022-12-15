#ifndef __AFFICHEUR__
#define __AFFICHEUR__
#include "lecteur_fichier.h"
#include <stdio.h>
#include <stdlib.h>

void afficher_sect(Elf32_Section_Header *tab, uint16_t nb); // il sera dégagé plus tard

void afficher_sh_name(unsigned int name);

void afficher_sh_type(unsigned int type);

void afficher_sh_addr(unsigned int addr);

void afficher_sh_offset(unsigned int offset);

void afficher_sh_size(unsigned int size);

void afficher_sh_link(unsigned int link);

void afficher_sh_info(unsigned int info);

void afficher_sh_addralign(unsigned int addralign);

void afficher_sh_entsize(unsigned int entsize);

void afficher_sh_flags(unsigned int flags);

void afficher_section(Elf32_Section_Header *tab, uint16_t nb);

#endif