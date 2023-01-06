CC=gcc
CFLAGS=-Wall -Werror


all: main.o affichageElfMain.o lecteur_fichier.o reader_binaire.o afficheur.o fusion.o
	$(CC) -o main main.o lecteur_fichier.o reader_binaire.o afficheur.o fusion.o
	$(CC) -o affichageElf affichageElfMain.o lecteur_fichier.o reader_binaire.o afficheur.o


main.o: lecteur_fichier.h afficheur.h fusion.h

affichageElfMain.o: lecteur_fichier.h afficheur.h reader_binaire.o

lecteur_fichier.o: lecteur_fichier.h reader_binaire.h

reader_binaire.o: reader_binaire.h

afficheur.o: afficheur.h lecteur_fichier.h

fusion.o: fusion.h lecteur_fichier.h reader_binaire.h