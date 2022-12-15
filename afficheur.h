#ifndef __AFFICHEUR__
#define __AFFICHEUR__

#include <stdio.h>
#include <stdlib.h>
#include "lecteur_fichier.h"

void afficherMagic(ELF_Header *Header, int taille);

void afficherSymbol(ELF_Symbol *table, int taille);

#endif