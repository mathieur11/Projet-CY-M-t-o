//include standard :
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<limits.h>
#include<string.h>
#include<unistd.h>
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
  if ((strcmp(fichierEntre, "height.csv") == 0) || (strcmp(fichierEntre, "moisture.csv") == 0)) {    // fichierEntre est égal à "height.csv"    
    while(curseur != EOF){
      fscanf(fichier, "%ld", &stationLue);
      fseek(fichier, 1, SEEK_CUR);     //permet de passer à la deuxième colonne.
      fscanf(fichier, "%lf", &donnees);
      //printf("main station : %ld - elmt %lf\n", stationLue, donnees);
      if(strcmp(tri, "tab") == 0){
        positionInsert = cherchePos(pliste, donnees, sens);
        pliste = insertPos(pliste, positionInsert, stationLue, donnees);
      }
      else if(strcmp(tri, "abr") == 0){
        parbre = InsereABR(parbre, stationLue, donnees);
      }
      
      
      curseur = fgetc(fichier);
    }
    
    //génération du fichier de sortie
    FILE* fichier=NULL;
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
//  printf("MRT on rentre dans creationChainon\n");
//  printf("MRT %lf : %lf\n", stationC, a);

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
//printf("MRT on rentre dans cherchePos\n");

  Chainon* p1=pliste;
  int compt=0;
  if (p1 == NULL){    // insertion à l'indice 0
    return 0;
  }
  //printf("\n");
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
//  printf("MRT on rentre dans insertPos\n");
//  printf("MRT %lf : %lf\n", station, elmt);
//printf("%d",pos);
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
//    printf("%s" , station);
    nouveau = creationChainon(station, elmt);
//    printf("MRT Creation %lf : %lf\n", nouveau->station, nouveau-> elmt);
    nouveau->suivant = p1; //->suivant;
 //   printf("MRT2 %s : %lf\n", nouveau->station, nouveau-> elmt);
    p1Precedent->suivant = nouveau;
//    printf("MRT precedent %lf : %lf\n", p1Precedent->station, p1Precedent-> elmt);

 //   printf("fin");

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



/*Arbre* RechercheABR(Arbre* A, int x){
  if (!A){
    return NULL;
  }
  if (x < A->elmt){
    return RechercheABR(A->fg, x);
  }
  else{
    if (x > A->elmt){
      return RechercheABR(A->fd, x);
    }
  }
    return A;
}*/

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


/*Arbre* InsereABRIteratif(Arbre* A, int x){
  Arbre* tmp = A;
  if (!A){
    return creerArbre(x);
  }
    do{
      if (x == tmp->elmt){
        return A;
      }
      if (x < tmp->elmt){
        if (tmp->fg){
          tmp = tmp->fg;
        }
        else{
          tmp->fg = creerArbre(x);
          return A;
        }
      }
      else{ // x > tmp->elmt
        if (tmp->fd){
          tmp = tmp->fd;
        }
        else{
          tmp->fd = creerArbre(x);
          return A;
        }
      }
    }while (1);
}*/

// Dans SuppABR, va supprimer le fils le plus grand (a droite) du fils gauche pris en compte
/*Arbre* SuppMax(Arbre* A, int *x){
  Arbre* tmp;
  if (A->fd){
    A->fd = SuppMax(A->fd, x);
  }
  else{
    *x = A->elmt; //recuperation de la valeur a remonter en parametre
    tmp = A;
    A = A->fg;
    free(tmp);
  }
  return A;
}*/


/*Arbre* SuppABR(Arbre* A, int x){
  Arbre* tmp;
  if (A){
    if (x > A->elmt){
      A->fd = SuppABR(A->fd, x);
    }
    else if (x < A->elmt){
      A->fg = SuppABR(A->fg, x);
    }
    else if (A->fg){
      A->fg = SuppMax(A->fg, &(A->elmt)); // on remplace elmt par la grande valeur inferieure
    }
    else{ // si pas de fils gauche : la racine devient le fils droit
      tmp = A;
      A = A->fd;
      free(tmp);
    }
    return A;
  }
}*/


/****************************************************************
*****************************************************************
*****************************************************************
*****************************************************************
****************************************************************/




