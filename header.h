//Liste chainée
typedef struct chainon {
    long station;
    double elmt;
    struct chainon* suivant;
}Chainon;



Chainon* creationChainon(long stationC, double a);
Chainon * insertDebut(Chainon* pliste,long station, double a);
Chainon* insertFin(Chainon* pliste,long station, double a);
void afficheListe2(Chainon *pliste, FILE* fichierTrie);
int cherchePos(Chainon* pliste, double valeurCherchee, char* sens);
Chainon *insertPos( Chainon* pliste, int pos, long station, double elmt);

//Arbre :
typedef struct arbre {
    long station;
    double elmt;
    struct arbre* fg;
    struct arbre* fd;
    int equilibre;
}Arbre;


//Arbre binaire de recherche
Arbre* creerArbre(long station, double donnees);
Arbre* InsereABR(Arbre* A, long station, double donnees);
void afficherABR(Arbre* parbre, char* sens, FILE* fichierTrie);

//Autre
int min(int a,int b);
int max(int a,int b);


















