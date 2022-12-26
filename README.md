# Projet-Prog5
Projet de Programmation5 du groupe 1

### Jour 2:

#### Avancement:
Grande partie de l'affichage du header fait

#### Questions:
Maxime:
> Pour l'affichage header: Grand nombre de machine et OS/ABI, tous les traiter dans un switch case ?

-----------------------------------------------

### Jour 1:

#### Avancement:
ELF Header

#EI_NIDENT 16
unsigned char e_ident[EI_NIDENT]; Correspond au Magic[0-3] + EI_CLASS + EI_DATA + EI_VERSION + EI_PAD + Tout les autres octets à 0 de [7-15] soit 16 octet
Elf32_Half e_type; 2 octet
Elf32_Half e_machine; 2 octet
Elf32_Word e_version; 4 octet
Elf32_Addr e_entry; 4 octet
Elf32_Off e_phoff; 4 octet
Elf32_Off e_shoff; 4 octet
Elf32_Word e_flags; 4 octet
Elf32_Half e_ehsize; 2 octet
Elf32_Half e_phentsize; 2 octet
Elf32_Half e_phnum; 2 octet
Elf32_Half e_shentsize; 2 octet
Elf32_Half e_shnum; 2 octet
Elf32_Half e_shstrndx; 2 octet

52 octets à lire, octet par octet avec fread() (Attention au big endian et little endian) et interpreté dans l'ordre.




Sections Header à lire à partir de l'addresse contenue dans e_entry + e_shoff (offset par rapport à e_entry)

Chaque section est formé de e_shentsize octets contigüe, representatn differentes informations permmettant de remplir la table, toutes sur 4 octets: //Attention que se passe-t-il avec e_shentsize != 40 (plus ou moins d'octet dque sur les exemples; données supplémentaires? interpretes differemment ?)
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
Les sections sont contigües et il y en  a e_shnum.




#### Questions: 
> Aucune informations sur le Flag I ? On se limite au 4 flags de la doc ou plus ?

> Ou se trouve sh_entsize? Doit-on le preciser dans notre sortie, car il n'apparait aps dans readElf?

> Comment savoir en quelle machine on est ? ARM ? 

Questions: 
    - Aucune informations sur le Flag I ? On se limite au 4 flags de la doc ou plus ?
    - Ou se trouve sh_entsize? Doit-on le preciser dans notre sortie, car il n'apparait aps dans readElf?
    - Comment savoir en quelle machine on est ? ARM ? 


Jour 2: 

    Problemes dans l'ordre de lecture. Attention au 64 bits et 32 bits





NOTICE POUR LES TESTS :
  - Mettre les fichiers de tests dans le dossier ./tests
  - Lancement du shell en faisant : ./test.sh
  - Le programme parcours tout les fichiers du repertoire ./tests et compare leur affichage avec notre programme, avec leur affichage par la commande arm-none-eabi-readelf
  - Si il n'y a pas de difference : Affichage du message "['SECTION de l'elf'] OK pour 'fichier'"
  - Sinon il affiche "['SECTION de l'elf'] ECHEC pour 'fichier'"
  - Dans ce cas là il indique l'endroit où il y a une difference. ATTENTION : La manière pour comparer les affichages n'est pas "tres propre" (ie, j'ai récupéré les valeurs "utiles" des deux affichages, en supprimant tout la mise en page, et je les ai comparées), cela implique que le texte renvoyé n'est pas très lisible.
  - Si il y a un probleme avec le script (erreur alors que non, ou inverse), n'hesiter pas à me faire part du problème 
Thomas
