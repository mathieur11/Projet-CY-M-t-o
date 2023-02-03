//include personnel :
#include"header.h"


int main(int argc, char* argv[]) {
  if ((argc < 3) || (argc > 5 )) {
    printf("WARNING : Le nombre de paramètres passés au programme n'est pas correct\n");
    return 1;
  }
  // récupération des paramètres
  char* fichierEntre = argv[1];
  char* fichierSortie = argv[2];
  char* tri = argv[3];
  char* sens = "c";
  FILE* fichier = NULL;
  char curseur = 0;
  long stationLue = 0;
  double donnees = 0;
  int positionInsert = 0;

  
  fichier = fopen(fichierEntre, "r");
  if (fichier == NULL){
    printf("WARNING : Le fichier d'entré n'existe pas\n");
    return 2;
  }
  if (argv[4]){
    sens = "d";
  }
  Arbre* parbre = (Arbre*) malloc(sizeof(Arbre)); parbre = NULL;
  Chainon* pliste = (Chainon*) malloc(sizeof(Chainon)); pliste = NULL;   
    while(curseur != EOF){
      fscanf(fichier, "%ld", &stationLue);
      fseek(fichier, 1, SEEK_CUR);     //permet de passer à la deuxième colonne.
      fscanf(fichier, "%lf", &donnees);
      if(strcmp(tri, "tab") == 0){
        positionInsert = cherchePos(pliste, donnees, sens);
        pliste = insertPos(pliste, positionInsert, stationLue, donnees);
      }
      else if(strcmp(tri, "abr") == 0){
        parbre = InsereABR(parbre, stationLue, donnees);
      }
      
      
      curseur = fgetc(fichier);
  //  }
    
    //génération du fichier de sortie
    fichier = fopen(fichierSortie, "w");  
    if(strcmp(tri, "tab") == 0){
      afficheListe2(pliste, fichier);
    } 
    else if(strcmp(tri, "abr") == 0){
      afficherABR(parbre, sens, fichier);
    }
    fclose(fichier);
    
    //libération de la mémoire
    free(pliste);
    
    
    if (access(fichierSortie, F_OK) != -1) {
      return 0;
    } 
    else{
      printf("WARNING : Le fichier de sortie n'a pas pu être généré\n");
      return 3;
    }  
}



int min(int a,int b){
  if(a<b){
    return a;
  }
  else if(b<a){
    return b;
  }
  else{
    return a;
  }
}

int max(int a,int b){
  if(b<a){
    return a;
  }
  else if(a<b){
    return b;
  }
  else{
    return a;
  }
}



Chainon* creationChainon(long station, double a){
  Chainon* c = (Chainon *) malloc(sizeof(Chainon));
  if(c==NULL){
    exit(1);
  }
  c->elmt=a;
  c->station= station;
  c->suivant=NULL;
  return c;
}


Chainon * insertDebut(Chainon* pliste,long station, double a){
  Chainon* nouveau=creationChainon(station, a);
  nouveau -> suivant = pliste;
  pliste = nouveau;
  return nouveau;
}


Chainon* insertFin(Chainon* pliste,long station, double a){
  Chainon *nouveau= creationChainon(station,a);
  // si la liste est vide
  if(pliste==NULL){
    pliste= nouveau ;
  }
  else{
    Chainon *p1=pliste;
    while(p1->suivant != NULL){
      p1=p1->suivant;
    }
    p1->suivant=nouveau;
  }
  return pliste;
}


void afficheListe2(Chainon *pliste, FILE* fichierTrie){
  Chainon* p1=pliste;  
  while(p1!=NULL){
    fprintf(fichierTrie, "%ld;", p1->station);
    fprintf(fichierTrie, "%lf ", p1->elmt);
    if(p1 ->suivant != NULL){
      fprintf(fichierTrie, "\n");
    }
    p1=p1->suivant;
  }
  fprintf(fichierTrie, "\n");
 }


int cherchePos(Chainon* pliste, double valeurCherchee, char* sens){
  Chainon* p1=pliste;
  int compt=0;
  if (p1 == NULL){    // insertion à l'indice 0
    return 0;
  }
  while(p1!=NULL){
    if (sens == "c"){
      if (valeurCherchee > p1->elmt){
        compt = compt + 1;
      }
    }
    else {
      if (valeurCherchee < p1->elmt){
        compt = compt + 1;
      }
    }
    if(p1->suivant == NULL){
      return compt;
    }
    p1=p1->suivant;
  }
  return compt;
}






Chainon *insertPos( Chainon* pliste, int pos, long station, double elmt){
  Chainon* nouveau = malloc(sizeof(Chainon*)); Chainon* p1 = malloc(sizeof(Chainon*));
  Chainon* p1Precedent = malloc(sizeof(Chainon*));
  int i;
  if ((pos == 0) || (pliste == NULL)){    // insertion à l'indice 0
    pliste = insertDebut(pliste,station,elmt);
  }
  else{
    p1 = pliste;
    for (i=0;i<pos;i++){
      if (p1 == NULL){
        insertFin(pliste,station,elmt);
        return pliste;            // pos > nombre de chainons
      }
      else{
        p1Precedent = p1;
        p1 = p1->suivant;
      }
    }
    nouveau = creationChainon(station, elmt);
    nouveau->suivant = p1; //->suivant;
    p1Precedent->suivant = nouveau;
  }
  free(p1);
  free(nouveau);
  free(p1Precedent);
  return pliste;
}






/***************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************/



void afficherABR(Arbre* parbre, char* sens, FILE* fichierTrie){
    if (parbre != NULL){
       if(sens == "c"){
         afficherABR(parbre->fg, sens, fichierTrie);
         fprintf(fichierTrie, "%ld;", parbre->station);
         fprintf(fichierTrie, "%lf\n", parbre->elmt);
         afficherABR(parbre->fd, sens, fichierTrie);
       }
       else{
         afficherABR(parbre->fd, sens, fichierTrie);
         fprintf(fichierTrie, "%ld;", parbre->station);
         fprintf(fichierTrie, "%lf\n", parbre->elmt);
         afficherABR(parbre->fg, sens, fichierTrie);
      }
    }
    
}


Arbre* creerArbre(long station, double donnees){
  Arbre* noeud;
  noeud = (Arbre*) malloc(sizeof(Arbre));
  noeud->station = station;
  noeud->elmt = donnees;
  noeud->fg = NULL;
  noeud->fd = NULL;
  return noeud;
}


Arbre* InsereABR(Arbre* A, long station, double donnees){
  if (!A){
    return creerArbre(station, donnees);
  }
  if (donnees < A->elmt){
    A->fg = InsereABR(A->fg, station, donnees);
  }
  else{
    if (donnees >= A->elmt){
      A->fd = InsereABR(A->fd, station, donnees);
    }
  }
  return A;
}


