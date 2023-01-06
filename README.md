
# Projet-Prog5
Projet de Programmation5 du groupe 1
	
	- Thomas GREGOIRE
	- Gaetan COMBE
	- Alban FAURIE
	- Clement BARJON
	- Fabien VIAL
	- Maxime POTELBERG

## Notices : 
### Test : 
- Mettre les fichiers de tests dans le dossier ./tests
 - Lancement du shell en faisant : ./test.sh
 - Le programme parcours tous les fichiers du répertoire ./tests et compare leur affichage avec notre programme, avec leur affichage par la commande arm-none-eabi-readelf
   - S'il n'y a pas de différence le message suivant est affiché :
    **" ['SECTION de l'elf'] OK pour 'fichier' "**
   - Sinon il affiche : 
     **" ['SECTION de l'elf'] ECHEC pour 'fichier' à la ligne x "** ou juste
     **" ['SECTION de l'elf'] ECHEC pour 'fichier' "**
     - Dans ce cas-là, il indique l'endroit où il y a une différence. 
     ATTENTION : La manière pour comparer les affichages n'est pas "très propre" (ie, j'ai récupéré les valeurs "utiles" des deux affichages, en supprimant toute la mise en page, et je les ai comparées), cela implique que le texte renvoyé n'est pas très lisible.
  - S'il y a un problème avec le script (erreur alors que non, ou inverse), n'hésité pas à me faire part du problème (Thomas)



### Les Programmes :
#### AffichageELF :
Le programme fonctionne de la manière suivante : 
			./main [option] fichier

Options disponibles (Une seule option lors du lancement de la commande) :
|L'option| Ce quelle affiche    |
|:---:|:-------------------:|
|-a| Tout  |
|-h | Affiche  Header |
|-S |  Affiche la table des sections|
|-s |  Affiche la table des symboles|
|-x i | Affiche  le contenu de la section numéro i|
|-r |  Affiche les tables de relocations|
|-help |  Affiche une aide pour utiliser le main|


### Fusion:
TODO







## Avancement :
TODO

##### 04/01 : Audit de code de mis projet : Factorisation du code + modification de la manière de récupérer les informations (mise en mémoire du fichier) + modification de la manière de gérer les options lors du lancement de AffichageElf (basé sur le code fourni par l'enseignant au début du projet)
##### 05/01 : Nettoyage du code suite à l'audit de code de mis projet + finalisation de la partie 1

##### 06/01 : Début de la partie 2

## Liste des questions posées au cours du projet : 
> Aucune informations sur le Flag I ? On se limite au 4 flags de la doc ou plus ?
>
> Ou se trouve sh_entsize? Doit-on le preciser dans notre sortie, car il n'apparait aps dans readElf?
>
> Comment savoir en quelle machine on est ? ARM ? 
>
> Problemes dans l'ordre de lecture. Attention au 64 bits et 32 bits


