
#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <io.h>
#include <windows.h>
#include <conio.h>

#define TAILLE_DE_BLOC 1000
#define Speed 20
#define DEFAULT_FILE "Default_File"
#define FAQ_FILE "FAQ"


#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"


#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define PURPLE "\033[35;1m"
#define PINK "\033[38;5;206m"
#define ORANGE "\033[38;5;208m"
#define GRAY "\033[90m"
#define RESET "\033[0m"
#define INVERSE "\033[7m"
#define BLACK "\033[30m"

//  declaration de type bloc
typedef struct {

    char bloc[TAILLE_DE_BLOC];

}TBloc;

// declaration de type Etudiant , la structure de notre etudiinat
typedef struct {
    bool efface;                            // bit deface
    int taille;             // la taille
    int matricule;                          // la matricule
    char etudiantData[1190];

}Etudiant;


typedef struct Container
{
    char matricule[7];             // 6 chiffres + '\0'
    char efface;             // bit defface logique
    char taille[4];           // 3 chiffres + '\0'
    char info[990];            // le rest , les information

} Container;

typedef struct Header
{
    int adrLastBloc;  //nombre de blocs utilises
    int nbrEnreg;        //nombre d'enregistrement dans le fichiers (qui ne sont pas supprimes)
    int nbrSuprLogiq;          //nombre d'elements supprimes logiquement
} Header;


typedef struct TnOVnC {

    FILE* mainFile;                             // le fichier ou on va ecrire
    Header header;                              // la tete de fichier qui contien  les information sure le fichier

}TnOVnC;

typedef struct Tmatricule {
    int Tmatricule;          // Represente un element dans le tableau de matricule qu'on va utiliser pour ordonner le fichier
}Tmatricule;




// ouvrire le fichier
TnOVnC* openMainFile(char* fileName, char mode);

// ferme est enregistre le header
int closeMainFile(TnOVnC* tnOVnC);

// change la tete selen valeur de "val" , et i pour choisire quoi change
void SetHeader(TnOVnC* tnOVnC, int i, int val);

// lire l'entete dans la as numBloc
int GetHeader(TnOVnC* tnOVnC, int i);

int liredir(TnOVnC* tnOVnC, int i, TBloc* buf);  // lire un bloc

// ecrire un bloc return -1 si lindex est depasse
int ecriredir(TnOVnC* tnOVnC, int i, TBloc buf);

//  sauvgarde les donne de fichier dans le disque
int sauvgardeFichier(TnOVnC* tnOVnC);

// lire un etudint a partir d'un bloc
int lireEtudiant(TBloc tBloc, int debutENregDansLeBloc, Etudiant* etudiantACharge);

// lire un etudint a partir d'un bloc
int ecrireEtudiant(TBloc* buf, Etudiant etudiant);

void creerBloc(TnOVnC* tnOVnC);    // crier un bloc

// remplire les autre information de l'etudiant (les 3 bit de taille) a partire de la taille des donne insere dans l'etudiant
int remplireRestEtudiantDefault(Etudiant* etudiant);

// rechereche un etudiant et renvoi quelque informatios comme les donne de l'etudiant ,l,adress, trouve ou pas, supprime logiquement ou pas (resp dans la corbeille ou pas)
int rechercheEtudiant(TnOVnC* tnOVnC, int matricule, bool* trouv, Etudiant* etudiant, int* numBloc, int* debutEnregEtudt, bool* corbeille);

// insertion d'un dans un bloc
int insertionEtudiant(TnOVnC* tnOVnC, Etudiant etudiant, bool* testInsere);

// supprime un etudiant logiquement (resp envoi le dans la corbeille)
int supprimeLogique(TnOVnC* tnOVnC, int matriculle, bool* trouv, bool* corbeille);

// supprime un etudiant physiquement
int supprimePhysique(TnOVnC* tnOVnC, int matricule);

// supprime tout les etudiant qui sont deja supprimer logiquement (resp vider la corbeille)
int videLaCorbeille(TnOVnC* tnOVnC);

// restore un etudiant supprime deja logiquement
int restoreUnEtudiant(TnOVnC* tnOVnC, int matriculle);

// restore tout les etudiants supprime deja logiquement
int restoreToutLesEtudiantDansLaCorbeille(TnOVnC* tnOVnC);

// chargement initial pour le fichier pas defaut
void chargementInitiale(TnOVnC* tnOVnc);

// crier un fichier par defaut
void crierFichierParDefaut();

// envoi le rest d'un bloc
int RestDeBloc(TnOVnC* tnOVnC, int numBloc);

// reorganisation de fichier apers suppression physique / insertion pour gaggne de l'espace
int reorganisation(TnOVnC* tnOVnC);

// creer un fichier ordonne a partir de fichier selectionne
int ordonner(TnOVnC* tnOVnC, char nomFichier[], bool defaultName);

// test si l'utilisateur shouhitez quitte ou pas
int testRevenireMenu(bool* contin);


// pour affichage
void sFSD_ProjectAffichige();
void affichageDebut(int optionSelectionne, bool selection, char* nomFichierSelectionne, int nbrEnreg, int nbrBlocs, int nbrSuprLogiq);
void asciiArtTextLine(const char* text, int animspeed);
void affichageFin();

// pour le son
void enterSound();
void successSound();
void failSound();

void fAQ();  // pour ameliorer notre tool (app ou projet)

int modifierEtudiant(TnOVnC* tnOVnC, Etudiant etudiant, int numBloc, int debutEnregDansBloc, Etudiant etudiant2);

#endif /* FUNC_H */
