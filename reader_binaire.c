#include "reader_binaire.h"
#include <stdlib.h>


Lecteur *init_lecture(FILE *fichier){
    Lecteur *lecteur = malloc(sizeof(Lecteur));
    fseek(fichier, 0, SEEK_END);
    lecteur->size = ftell(fichier);
    lecteur->adr = 0;
    lecteur->fichier = malloc(sizeof(char)*lecteur->size);
    if(!lecteur->fichier){
        free(lecteur->fichier);
        printf("Erreur d'initialisation du malloc fichier");
    }
    fseek(fichier,0, SEEK_SET);
    fread(lecteur->fichier,sizeof(unsigned char),lecteur->size,fichier);
    fclose(fichier);
    return lecteur;
}


unsigned char lecture1octet(Lecteur *lecteur){
    unsigned char i =0;
    if(lecteur->adr < lecteur->size){
        i = lecteur->fichier[lecteur->adr];
        lecteur->adr++;
    }
    return i;
}

uint16_t lecture2octet(Lecteur *lecteur){
    uint16_t octet = 0;
    uint16_t tampon =0;
    if(lecteur->adr < lecteur->size-1){
        octet = lecteur->fichier[lecteur->adr];
        lecteur->adr++;
        tampon = lecteur->fichier[lecteur->adr];
        octet = (tampon << 8) + octet;
        lecteur->adr++;
    }
    return octet;   
}

uint32_t lecture4octet(Lecteur *lecteur){
    uint32_t octet = 0;
    uint32_t tampon =0;
    if(lecteur->adr < lecteur->size-3){
        octet = lecture2octet(lecteur);
        tampon = lecture2octet(lecteur);
        octet = (tampon << 16)+ octet;
    }
    return octet; 
}

uint16_t bigEndianLecture2octet(Lecteur *lecteur){
    uint16_t octet = 0;
    uint16_t tampon =0;
    if(lecteur->adr < lecteur->size-1){
        octet = lecteur->fichier[lecteur->adr];
        lecteur->adr++;
        tampon = lecteur->fichier[lecteur->adr];
        octet = (octet << 8) + tampon;
        lecteur->adr++;
    }
    return octet;   
}

uint32_t bigEndianLecture4octet(Lecteur *lecteur){
    uint32_t octet = 0;
    uint32_t tampon =0;
    if(lecteur->adr < lecteur->size-3){
        octet = bigEndianLecture2octet(lecteur);
        tampon = bigEndianLecture2octet(lecteur);
        octet = (octet << 16)+ tampon;
    }
    return octet; 
}