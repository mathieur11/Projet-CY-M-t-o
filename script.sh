#!/bin/bash


# Dans le cas où l'utilisateur a besoin d'aide
if [ $# == 0 ] || [ $1 == "--help" ] ; then
  echo "Utilisation : ./script.sh [OPTION]..."
  echo "Permet d'afficher des graphiques selon un ou plusieurs types de données météorologiques [TYPE DE DONNEES], un seul lieu ou aucun [LIEUX] et une période définit ou non par l'utilisateur [DATES]."
  echo "Il est également possible d’imposer le mode de tri des données [TRIS] : soit à l’aide d’un tableau (ou  liste chaînée), soit à l’aide d’une structure d’arbre binaire, ABR ou AVL. Une seule opttion de tri peut être activé."
  echo -e "Le nom du fichier d’entrée [FICHIER] doit être renseigné pour que le script puisse acquérir toutes les données.\n"
  echo -e "L'ordre dans lequel les options sont rentrées n'importe pas.\n"
  echo -e "Options :\n"
  echo -e "--help		Affiche cette aide\n"
  echo -e "[TYPE DE DONNEES] : \n"
  echo -e "-w		traite les données sur le vent\n"
  echo -e "-m		traite les données sur l'humidité\n"
  echo -e "-h		traite les données sur l'altitude\n"
  echo "-t1		traite les données sur les températures minimales, maximales et 
  		moyennes par station dans l'ordre croissant du numéro de station"
  echo "-t2		traite les données sur les températures moyennes par date/heure, 
  			triées dans l'odre chronologique"
  echo "-t3		traite les données sur les températures par date/heure,
  		triées par ordre chronologique, puis par ordre croissant de 
  			l'identifiant de la station"
  echo "-p1		traite les données sur les pressions minimales, maximales et 
  		moyennes par station dans l'ordre croissant du numéro de station"
  echo "-p2		traite les données sur les pressions moyennes par date/heure, 
  			triées dans l'odre chronologique"
  echo -e "-p3		traite les données sur les pressions par date/heure, 
  		triées par ordre chronologique, puis par ordre croissant de 
  			l'identifiant de la station\n"
  echo -e "[LIEUX] : \n"
  echo -e "-F		France métropolitaine + Corse\n"
  echo -e "-G		Guyane française\n"
  echo -e "-S		Saint-Pierre et Miquelon : ile située à l'Est du Canada\n"
  echo -e "-A		Antilles\n"
  echo -e "-O		Océan indien\n"
  echo -e "-Q		Antarctique\n"
  echo -e "[DATES] : \n"
  echo -e "-d<min><max>	traite les données dans l'intervalles de dates [min>..<max>]
  		 incluses. Le format des dates est une chaine de 
  		 type YYYY-MM-DD(année-mois-jour)."
  echo -e "[TRIS] : \n"
  echo -e "--tab		tri effectué à l’aide d’une structure linéaire\n"
  echo -e "--abr		tri effectué l’aide d’une structure de type ABR\n"
  echo -e "--avl		tri effectué à l’aide d’une structure de type AVL\n"
  echo -e "[FICHIER] :\n"
  echo -e "-f<nom_fichier>		hier d'entrée\n"
  exit 0
fi


#Initialisation des variables :
tempArg=     #tempArg (température argument), vérifie que le nombre d'argument passé pour la température est correct
temp=
pres=
presArg=     #presArg (pression argument), vérifie que le nombre d'argument passé pour la pression est correct
codeRetour=
localisation=
fichier="vide"
PresLoc=0    #PresLoc (présence de localisation) correspond au nombre de localisation rentrée par l'utilisateur, si PresLoc>1, erreur
dateMin=1980-01-01
dateMax=2100-01-01
CodemMin=0  # Code postal minimum de la localisation demandée
CodeMax=0   # Code postal maximum de la localisation demandée
tri="avl" 



# Traitement des options
while getopts ":p:t:wmhf:d:FGASOQ-:" option; do
  echo "ca marche"
  case "$option" in
    t) tempArg=$OPTARG  #temperature
      #vérifie que l'argument de t est entre 1 et 3 inclus
      if [ "$tempArg" -ge 4 ] || [ "$tempArg" -le 0 ] ; then
        echo "Argument invalide" 
        exit 1
      else
        temp="1,11,12,13"
      fi 
      ;; 
    p) presArg=$OPTARG  #pression
       #vérifie que l'argument de p est entre 1 et 3 inclus
       if [ "$presArg" -ge 4 ] || [ "$presArg" -le 0 ] ; then
         echo "Argument invalide" 
         exit 1
       else
         pres="1,3,7,8"
      fi 
      ;; 
    w) vent="1,4,5"
       ;; #vent
    m) hum="1,6"
       ;; #humidité
    h) alt="1,14"
       ;; #altitude
    :) echo "L'option -$OPTARG nécessite un argument."
       ;;
    -) if [ $OPTARG == "abr" ] ; then
         tri=$OPTARG
       elif [ $OPTARG == "avl" ] ; then
         tri=$OPTARG
       elif [ $OPTARG == "tab" ] ; then
         tri=$OPTARG
       else
         echo "le tri renseigné n'est pas valide"
         exit 1
       fi
       ;;
    f) if [ $OPTARG ] ; then
         fichier=$OPTARG
       else
         echo "L'option -f nécessite un argument."
         exit 1
       fi
       ;;
    d) dateMin=$OPTARG
       dateMax=$(eval "echo \${$OPTIND}")
       shift #$((OPTIND - 1))
       ;;
    F) localisation="France"
       let "PresLoc+=1" 
       CodeMin=01000
       CodeMax=95999 
       ;;
    G) localisation="Guyane" 
       let "PresLoc+=1"
       CodeMin=97300
       CodeMax=97399 
       ;;
    S) localisation="Saint-Pierre et Miquelon" 
       let "PresLoc+=1"
       CodeMin=97500
       CodeMax=97599 
       ;;
    A) localisation="Antilles"
       let "PresLoc+=1"
       CodeMin=97100
       CodeMax=97199 
       ;;
    O) localisation="Ocean Indien"
       let "PresLoc+=1"
       CodeMin=97400
       CodeMax=97499 
       ;;
    Q) localisation="Antarctique"
       let "PresLoc+=1"
       CodeMin=
       CodeMax=95999 
       ;;
    ?) echo "Option inconnue : -$OPTARG" 
       ;;
       #exit 0 ;;  
  esac
done
shift $((OPTIND - 1))

if [ $PresLoc -ge 2 ] ; then
  echo "Trop d'options de localisation, une seule maximum"
  exit 1
fi

if [ ! -f $fichier ] ; then
  echo "le fichier $fichier passé en paramètre n'existe pas"
  exit 1
fi

#filtre les lignes selon la localisation et la date
awk -F';' '($15 >= '$CodeMin' && $15 < '$CodeMax' && $2 >= "'$dateMin'T00:00:00+00:00" && $2 < "'$dateMax'T99:99:99+99:99")' $fichier > "reduit.csv"

taille=$(du reduit.csv | cut -f1)
if [ $taille -eq 0 ] ; then
  echo "Aucune donnée sur cette plage de dates"
  exit 1
fi

#On compile si le programme n'existe pas
if [ ! -f 'tri' ] ; then
  make
  make clean 
fi
if [ $temp ] ; then
  cut -d';' -f$temp "reduit.csv" > "temperature.csv"
  ./tri temperature.csv temperatureTrie.csv $tri r
  codeRetour=$?
  if [ $codeRetour -eq 0 ] ; then
    echo "Affichage du graphique Temperature"
  fi
fi
if [ $pres ] ; then
  cut -d';' -f$pres "reduit.csv" > "pression.csv"
  ./tri pression.csv pressionTrie.csv $tri r
  codeRetour=$?
  if [ $codeRetour -eq 0 ] ; then
    echo "Affichage du graphique Pression"
  fi
fi
if [ $vent ] ; then
  cut -d';' -f$vent "reduit.csv" > "vent.csv"
  ./tri vent.csv ventTrie.csv $tri r
  codeRetour=$?
  if [ $codeRetour -eq 0 ] ; then
    echo "Affichage du graphique Vent"
  fi
fi
if [ $hum ] ; then
  cut -d';' -f$hum "reduit.csv" > "humidité.csv"
  ./tri humidité.csv humiditéTrie.csv $tri r
  codeRetour=$?
  if [ $codeRetour -eq 0 ] ; then
    echo "Affichage du graphique Humidité"
  fi
fi
if [ $alt ] ; then
  cut -d';' -f$alt "reduit.csv" > "alt.csv"
  ./tri alt.csv altitudeTrie.csv $tri r
  codeRetour=$?
  if [ $codeRetour -eq 0 ] ; then
    echo "Affichage du graphique Altitude"
  fi
fi





