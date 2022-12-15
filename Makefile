main: main.o lecteur_fichier.o reader_binaire.o afficheur.o

main.o: lecteur_fichier.h afficheur.h

lecteur_fichier.o: lecteur_fichier.h reader_binaire.h

reader_binaire.o: reader_binaire.h

afficheur.o: afficheur.h lecteur_fichier.h