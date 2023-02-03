Descriptif :
Ce programme sert à extraire des données météorologiques de différentes stations en territoire francophone entre 2010 et 2022 et à les afficher sous forme de graphique.
Les données exploitables sont :
-	Les températures
-	Les pressions atmosphériques
-	La vitesse du vent
-	L’altitude
-	L’humidité
Ces données sont affichées par station météorologique et par date. 
Des fonctions de filtre par zone et par date sont disponibles.



Instruction de compilation :
Il y a 2 programmes distincts :
	Un programme écrit en bash, qui ne nécessite pas de compilation
	Un programme écrit C, qui sera compilé à la demande, au premier appel si le fichier n’existe pas.
La compilation du programme sera lancée automatiquement à la demande grâce au fichier MAKEFILE.
La compilation manuelle peut aussi se faire, depuis un terminal, avec la commande suivante : make dans le répertoire où se trouve les fichiers téléchargés.



Lancement du programme :
Dans un terminal Linux, lancez la commande « scripth.sh --help » pour avoir le détail de tous les paramètres.
