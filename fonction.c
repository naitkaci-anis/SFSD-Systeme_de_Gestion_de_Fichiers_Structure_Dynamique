#include "header.h"

TnOVnC* openMainFile(char* fileName, char mode) { // open , creat entet !


    TnOVnC* tnOVnC = (TnOVnC*)malloc(sizeof(TnOVnC));
    if (tnOVnC == NULL) {
        perror("\nERROR,   ouverture "); return NULL;
    }

    if ((mode == 'A') || (mode == 'a'))  (tnOVnC->mainFile) = fopen(fileName, "rb+");
    else (tnOVnC->mainFile) = fopen(fileName, "wb+");

    if (tnOVnC->mainFile == NULL) {
        perror("\nERROR, fopen()"); free(tnOVnC); return NULL;
    }

    if ((mode == 'A') || (mode == 'a'))                   //ouverture d'un ancien fichier
    {

        rewind(tnOVnC->mainFile);
        fread(&(tnOVnC->header), sizeof(Header), 1, tnOVnC->mainFile);

    }
    else
    {
        rewind(tnOVnC->mainFile);
        // Initialisation de l'entete
        tnOVnC->header.adrLastBloc = 0;      //d'adresse de dernier bloc=0
        tnOVnC->header.nbrEnreg = 0;            //le nombre des enregestrements=0
        tnOVnC->header.nbrSuprLogiq = 0;              //le nombre des enregestrements supprimes =0
        fwrite(&(tnOVnC->header), sizeof(Header), 1, tnOVnC->mainFile);
    }
    return tnOVnC;
}



//pour la fermeture de fichier  , libre lespace de pointer est ecrire la donne de la tete , est ferme le fichier
int closeMainFile(TnOVnC* tnOVnC) {

    rewind(tnOVnC->mainFile);               // mettre le curseur au debut

    fwrite(&(tnOVnC->header), sizeof(Header), 1, tnOVnC->mainFile);  // Mise a jour de l'entete dans le fichier !
    fclose(tnOVnC->mainFile);

    free(tnOVnC);   // libre la memoire
    tnOVnC = NULL;      // juste pour assure

    return 0;
}



int sauvgardeFichier(TnOVnC* tnOVnC) {

    rewind(tnOVnC->mainFile);               // mettre le curseur au debut
    fwrite(&(tnOVnC->header), sizeof(Header), 1, tnOVnC->mainFile);  // Mise a jour de l'entete dans le fichier !
    fflush(tnOVnC->mainFile);               // envoi les donne dans le buffer (le buffer dans MC)
    _commit(_fileno(tnOVnC->mainFile));     // fonction pour envoi les donne de buffer au disque sure dans windows , et fileno envoi des information de bas niveau de ce fichier
    return 0;
}

int GetHeader(TnOVnC* tnOVnC, int i)   //retourne les parametres de l'entete
{
    if (i == 1) return ((tnOVnC->header).adrLastBloc); //d'adresse de dernier bloc
    else if (i == 2) return ((tnOVnC->header).nbrEnreg);       //nombre des enregestrements
    else return ((tnOVnC->header).nbrSuprLogiq);                   //nombre des enregestrements logiq supprimes
}

// modifier lentet
void SetHeader(TnOVnC* tnOVnC, int i, int val)

{
    if (i == 1) (tnOVnC->header).adrLastBloc = val;  //modifier  d'adresse de dernier bloc
    else if (i == 2) (tnOVnC->header).nbrEnreg = val;   //modifier nombre des enregestrements
    else (tnOVnC->header).nbrSuprLogiq = val;               //modifier nombre des enregestrements supprimes
}


int liredir(TnOVnC* tnOVnC, int numBloc, TBloc* buf)       //fonction  lecture d'un bloc
{

    if (numBloc <= GetHeader(tnOVnC, 1))                        //si le numero de bloc < nbr de bloc totale
    {
        fseek(tnOVnC->mainFile, (long)(sizeof(Header) + (numBloc - 1) * sizeof(TBloc)), SEEK_SET);

        fread(buf, sizeof(TBloc), 1, tnOVnC->mainFile);
        return 0;
    }
    else
    {
        printf("\nERROR, liredir() le bloc nexiste pas !"); return -1;
    }
}

int ecriredir(TnOVnC* tnOVnC, int numBloc, TBloc buf)     //fonction l'ecriture d'un bloc
{

    if (numBloc <= GetHeader(tnOVnC, 1))                       //si le num de bloc < nbr de bloc totale
    {
        fseek(tnOVnC->mainFile, (long)(sizeof(Header) + (numBloc - 1) * sizeof(TBloc)), SEEK_SET);
        fwrite(&buf, sizeof(TBloc), 1, tnOVnC->mainFile);
        return 0;
    }
    else
    {
        printf("\nERROR, ecriredir() le bloc n'existe pas !"); return -1;
    }
}


// fonction pour lire un etudiant
int lireEtudiant(TBloc tBloc, int debutENregDansLeBloc, Etudiant* etudiantACharge) {

    Container enreg;        // joue le role d'un moule ou buffer ,il est le passage decriteur ou leture dans le fichier


    for (int i = 0; i < 3; i++) {

        enreg.taille[i] = tBloc.bloc[debutENregDansLeBloc + i];           // lire la taille
    }
    enreg.taille[3] = '\0';                                               // mettre la fin de chaine de charachtaire ou string
    etudiantACharge->taille = atoi(enreg.taille);                         // convert la taille de type chaine de caractere a type entier
    ////////////////////////////////////////////////////////////////

    for (int i = 0; i < 6; i++) {

        enreg.matricule[i] = tBloc.bloc[debutENregDansLeBloc + i + 3];   // lire la matricule
    }
    enreg.matricule[6] = '\0';
    etudiantACharge->matricule = atoi(enreg.matricule);                // convert la taille de type chaine de caractere a type entier

    ///////////////////////////////////////////////////////////////

    etudiantACharge->efface = (tBloc.bloc[9 + debutENregDansLeBloc] == '1') ? true : false;      // on remplace par true si on lit '1' sinon false
    //////////////////////////////////////////////////////////////

    for (int i = 0; i < (etudiantACharge->taille - 10); i++)  // - 10 car on doit lire just la chaine , ou 10 represent la taille de donne de lenreg externe (matricule ...)
    {
        etudiantACharge->etudiantData[i] = tBloc.bloc[debutENregDansLeBloc + i + 10];              // recevoi le rest de lenrege ,+10 car linfo sont ecrite apartire to 10
    }
    etudiantACharge->etudiantData[etudiantACharge->taille - 10] = '\0';                        // assure la fin de chaine de caractere

    return 0;
}



int ecrireEtudiant(TBloc* buf, Etudiant etudiant) { //ecrit letudiant a la fin

    char* end = buf->bloc + strlen(buf->bloc);
    end += sprintf(end, "%03d", etudiant.taille);
    end += sprintf(end, "%06d", etudiant.matricule);
    end += sprintf(end, "%d", etudiant.efface ? 1 : 0);
    sprintf(end, "%s", etudiant.etudiantData);

    return 0;
}

void creerBloc(TnOVnC* tnOVnC) {

    int nbrBlocAvant = GetHeader(tnOVnC, 1);
    SetHeader(tnOVnC, 1, nbrBlocAvant + 1);   // ajout 1 au nombre de bloc

}


// rechereche un etudiant et renvoi quelque informatios comme les donne de l'etudiant ,l,adress, trouve ou pas, supprime logiquement ou pas (resp dans la corbeille ou pas)
int rechercheEtudiant(TnOVnC* tnOVnC, int matricule, bool* trouv, Etudiant* etudiant, int* numBloc, int* debutEnregEtudt, bool* corbeille) {

    int debutEnregEtudiant;                                           // la position dans le bloc pour qu'il nous aide a determine la postion de etudinat dans le bloc
    *trouv = false;
    TBloc* bufBloc = (TBloc*)malloc(sizeof(TBloc));                  // reserve espace memoire au pointer
    memset(bufBloc->bloc, '\0', sizeof(TBloc));                      // vider le buf avant l'utilise

    if (GetHeader(tnOVnC, 1) > 0) {             // test si nombre de bloc > a zero

        for (int i = 1; i <= tnOVnC->header.adrLastBloc; i++) {
            debutEnregEtudiant = 0;                                   // initialse a chaque fois il sorte dans un bloc a 0
            liredir(tnOVnC, i, bufBloc);                              // lire un bloc dans ce cas on peut dire que elle parcoure le fichier

            while (debutEnregEtudiant < TAILLE_DE_BLOC - 10) {
                lireEtudiant(*bufBloc, debutEnregEtudiant, etudiant);
                if ((etudiant->taille == 0)) { debutEnregEtudiant = 10000; } // pour sortir de la boucle while

                if ((etudiant->matricule == matricule)) {            // si on a trouve la matricule

                    if (!(etudiant->efface)) {                       // cas ou bite deffacement = 0
                        *debutEnregEtudt = debutEnregEtudiant;
                        *trouv = true;
                        *numBloc = i;
                        *corbeille = false;
                        free(bufBloc);                               // corrige : libere la memoire avant de sortir
                        return 0;
                    }
                    else {                                           // cas ou bite deffacement = 1
                        *debutEnregEtudt = debutEnregEtudiant;
                        *trouv = true;
                        *numBloc = i;
                        *corbeille = true;                           // on remarque que le bit de corbeille est vrai dans ce cas
                        free(bufBloc);                               // corrige : libere la memoire avant de sortir
                        return 0;
                    }
                }
                else {
                    debutEnregEtudiant = debutEnregEtudiant + etudiant->taille;                                     //  debut de prochain enregistrement
                    memset(etudiant->etudiantData, '\0', sizeof(etudiant->etudiantData));                           // initiaise les donnes de l'etudiant
                }
            }
            memset(bufBloc->bloc, '\0', sizeof(TBloc)); // initialise le bloc pour prochain l'ecture
        }
    }

    *trouv = false;                 // dans ce cas on a parcouru tout le fichier est on a pas trouve l'etudiant
    free(bufBloc);                  // corrige : libere la memoire avant de sortir
    return -1;
}


int remplireRestEtudiantDefault(Etudiant* etudiant) {

    int len = strlen(etudiant->etudiantData);
    if (len > 0) etudiant->etudiantData[len - 1] = '\0'; // corrige : evite index -1 si chaine vide , ecrase le \n de fgets
    etudiant->efface = 0;
    etudiant->taille = 10 + strlen(etudiant->etudiantData);

    return 0;
}


int insertionEtudiant(TnOVnC* tnOVnC, Etudiant etudiant, bool* testInsere) {

    int numBloc;
    int debutEnregDansBloc;                              // retourne les information c est la recherche bien effectue
    Etudiant et = etudiant;
    bool corbeille;
    bool trouv;
    TBloc buf;
    memset(buf.bloc, '\0', sizeof(TBloc));

    rechercheEtudiant(tnOVnC, etudiant.matricule, &trouv, &et, &numBloc, &debutEnregDansBloc, &corbeille);          // recherche
    *testInsere = !trouv;


    if (*testInsere) {

        if (GetHeader(tnOVnC, 1) > 0) {

            liredir(tnOVnC, GetHeader(tnOVnC, 1), &buf);  //lecture de dernier  bloc
            if (TAILLE_DE_BLOC - strlen(buf.bloc) > etudiant.taille)                 // > et non >= : evite overflow de 1 octet du \0 final
            {
                ecrireEtudiant(&buf, etudiant);
                ecriredir(tnOVnC, GetHeader(tnOVnC, 1), buf);
            }
            else                // sinon on crier un bloc et on ecrit l'etudiant
            {
                creerBloc(tnOVnC);
                memset(buf.bloc, '\0', sizeof(TBloc)); // initialise le buffer
                ecrireEtudiant(&buf, etudiant);
                ecriredir(tnOVnC, GetHeader(tnOVnC, 1), buf);
            }
        }
        else          // crier un nouveau bloc
        {
            creerBloc(tnOVnC);
            ecrireEtudiant(&buf, etudiant);
            ecriredir(tnOVnC, GetHeader(tnOVnC, 1), buf);
        }
        SetHeader(tnOVnC, 2, GetHeader(tnOVnC, 2) + 1);       // incremant le nbr detudiant avec 1
        return  0;

    }


    return 0;

}



int supprimeLogique(TnOVnC* tnOVnC, int matriculle, bool* trouv, bool* corbeille) {

    int numBloc;
    int debutEnregEtudt;                              //  retourn
    Etudiant etudiant;                                //       les information
    //             que on doit  savoire

    TBloc buf;  memset(buf.bloc, '\0', sizeof(TBloc));

    rechercheEtudiant(tnOVnC, matriculle, trouv, &etudiant, &numBloc, &debutEnregEtudt, corbeille);

    if (*trouv && !*corbeille) {        // si on a touve l'etudiant est il est pas deja supprime logiquement
        liredir(tnOVnC, numBloc, &buf);
        buf.bloc[debutEnregEtudt + 9] = '1';                                        // ecrire le bit d'effacement a 1
        ecriredir(tnOVnC, numBloc, buf);
        SetHeader(tnOVnC, 3, (GetHeader(tnOVnC, 3) + 1));                           // ajout 1 nbr dedutiant efface logiqument (dans la corbeille )
        SetHeader(tnOVnC, 2, (GetHeader(tnOVnC, 2) - 1));                           // soustraire 1 nbr detudiant
        return 0;
    }

    return -1;
}



int supprimePhysique(TnOVnC* tnOVnC, int matricule) {


    int numBloc;
    int debutEnregEtudt;                              //  retourn
    Etudiant etudiant;                                //         les information
    bool trouv;                                       //                          que on doit
    bool corbeille;                                   //                                        connaitre dnas la recherche
    TBloc buf;
    int tailleBuf = 0;
    memset(buf.bloc, '\0', sizeof(TBloc));                              // initialise le buf

    char tabAffichage[sizeof(TBloc)] = { 0 };
    char tabAffichage2[sizeof(TBloc)] = { 0 };

    rechercheEtudiant(tnOVnC, matricule, &trouv, &etudiant, &numBloc, &debutEnregEtudt, &corbeille);
    if (trouv) {            // si il existe soit supprime logiquemnt ou pas

        liredir(tnOVnC, numBloc, &buf);
        tailleBuf = strlen(buf.bloc);               // renvoi la taille de buffer

        memmove(tabAffichage, buf.bloc, debutEnregEtudt);       // la chaine de caractaire de bloc avant l'etudiant a efface
        memmove(tabAffichage2, &buf.bloc[debutEnregEtudt + etudiant.taille], tailleBuf - (debutEnregEtudt + etudiant.taille)); // copie le rest de bloc sauf les information de ce etudiant

        printf("\nle bloc avant : \n" GRAY"%s" RED"%03d%06d%d%s" GRAY"%s" WHITE, tabAffichage, etudiant.taille, etudiant.matricule, etudiant.efface, etudiant.etudiantData, tabAffichage2);                // affiche le bloc avant avec les information de letudiant on rouge



        //remplace le bloc de debut enreg que on va supp par le rest de bloc apres le enreg de debutEnregEtudt + etudiant.taille jusqua la fin )
        memmove(&buf.bloc[debutEnregEtudt], &buf.bloc[debutEnregEtudt + etudiant.taille], tailleBuf - (debutEnregEtudt + etudiant.taille));

        // mettre le rest a \0
        memset(&buf.bloc[tailleBuf - etudiant.taille], '\0', etudiant.taille);

        printf("\nLe bloc apres : \n" GRAY"%s" WHITE, buf.bloc);
        (corbeille) ? SetHeader(tnOVnC, 3, (GetHeader(tnOVnC, 3) - 1)) : SetHeader(tnOVnC, 2, (GetHeader(tnOVnC, 2) - 1));

        ecriredir(tnOVnC, numBloc, buf);

        return 0;
    }

    return -1;
}


int RestDeBloc(TnOVnC* tnOVnC, int numBloc) {

    TBloc buf;
    memset(buf.bloc, '\0', sizeof(TBloc)); // initialisation

    liredir(tnOVnC, numBloc, &buf);  //lecture de dernier  bloc
    return (TAILLE_DE_BLOC - strlen(buf.bloc));

}



int videLaCorbeille(TnOVnC* tnOVnC) {

    int numBloc = 1;
    TBloc buf;
    int debutEnregEtudiant = 0;
    Etudiant etudiant1;
    int tailleBuf = 0;
    for (numBloc; numBloc <= tnOVnC->header.adrLastBloc; numBloc++) {               // prcoure le bloc
        debutEnregEtudiant = 0;
        liredir(tnOVnC, numBloc, &buf);
        tailleBuf = strlen(buf.bloc);
        while (debutEnregEtudiant < TAILLE_DE_BLOC - 10) {


            lireEtudiant(buf, debutEnregEtudiant, &etudiant1);

            if ((etudiant1.taille == 0)) { break; }                         // car on a initialise le bloc a \0 alors si ila lue 0 donc ila lu \0\0\0
            //qui indique la fin ou on a atient la fin des etudiant ecrit
//si le bit efface vrai on supprime lenregestrement
            if (etudiant1.efface) {

                memmove(&buf.bloc[debutEnregEtudiant], &buf.bloc[debutEnregEtudiant + etudiant1.taille], tailleBuf - (debutEnregEtudiant + etudiant1.taille));
                // remplace le bloc de debut enreg que on va supp par le rest de bloc apres le enreg de debutEnregEtudt + etudiant.taille jusqua la fin )
                memset(&buf.bloc[tailleBuf - etudiant1.taille], '\0', etudiant1.taille);

            }
            else {
                debutEnregEtudiant = debutEnregEtudiant + etudiant1.taille;

            }
            memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
            // vide le tableau
        }

        ecriredir(tnOVnC, numBloc, buf);
    }

    SetHeader(tnOVnC, 3, 0);
    // printf("\n La corbeille est vide  ");
    return 0;
}

int restoreUnEtudiant(TnOVnC* tnOVnC, int matriculle) {

    int numBloc;
    int debutEnregEtudt;                              //  retourn
    Etudiant etudiant;                                //         les information
    bool trouv;                                       //                          que on doit
    bool corbeille;                                   //                                        connaitre dnas la recherche
    TBloc buf;

    rechercheEtudiant(tnOVnC, matriculle, &trouv, &etudiant, &numBloc, &debutEnregEtudt, &corbeille);
    memset(buf.bloc, '\0', sizeof(TBloc));

    if (trouv && corbeille) {                       // && corbeille indique que cette enreg est efface logiquement
        liredir(tnOVnC, numBloc, &buf);
        buf.bloc[debutEnregEtudt + 9] = '0';   // +9 car c'est la ou on trouve le bit deffacment de letudiant
        ecriredir(tnOVnC, numBloc, buf);
        SetHeader(tnOVnC, 3, (GetHeader(tnOVnC, 3) - 1));                           // soustraire 1 nbr dedutiant efface logiqument (dans la corbeille )
        SetHeader(tnOVnC, 2, (GetHeader(tnOVnC, 2) + 1));                           // ajout 1 nbr detudiant

        return 0;
    }
    else
    {
        return -1;
    }

}


int restoreToutLesEtudiantDansLaCorbeille(TnOVnC* tnOVnC) {


    int numBloc = 1;
    TBloc buf;
    int debutEnregEtudiant = 0;
    Etudiant etudiant1;
    int tailleBuf = 0;
    for (numBloc = 1; numBloc <= tnOVnC->header.adrLastBloc; numBloc++) {               // prcoure le bloc

        debutEnregEtudiant = 0;
        liredir(tnOVnC, numBloc, &buf);
        //   tailleBuf = strlen(buf.bloc);
        while (debutEnregEtudiant < TAILLE_DE_BLOC - 10) {                              // -10 car si la taille de donne est 1 on va avoir une taille 11 pour tout lenregesrement

            lireEtudiant(buf, debutEnregEtudiant, &etudiant1);

            if ((etudiant1.taille == 0)) { break; }                         // car on a initialise le bloc a \0 alors si ila lue 0 donc ila lu \0\0\0
            //qui indique la fin ou on a atient la fin des etudiant ecrit
           //si le bit efface vrai on supprime lenregestrement

            if (etudiant1.efface) { buf.bloc[debutEnregEtudiant + 9] = '0'; }
            // +9 car c'est la ou on trouve le bit deffacment de letudiant

            debutEnregEtudiant = debutEnregEtudiant + etudiant1.taille;
            memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));                               // vide le tableau
        }

        ecriredir(tnOVnC, numBloc, buf);
    }

    SetHeader(tnOVnC, 2, (GetHeader(tnOVnC, 2) + GetHeader(tnOVnC, 3)));  // le nbr detudint + le nombre detudiant dans la corbeille
    SetHeader(tnOVnC, 3, 0);     // la corbeille est a 0

    return 0;
}



// ============================================================================
//  chargementInitiale() : 20 etudiants europeen avec date de naissance, section et groupe.
//
//  IMPORTANT : chaque chaine se termine par un ESPACE avant le guillemet "
//  car remplireRestEtudiantDefault() efface le dernier caractere (le \n de fgets)
//  donc sans cet espace le dernier caractere du texte serait coupe.
// ============================================================================

void chargementInitiale(TnOVnC* tnOVnc) {

    bool test;
    Etudiant etudiant1 = { 0 };

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100001;
    strcpy(etudiant1.etudiantData, "Lukas Muller ne le 14/03/2003 Berlin section A groupe 1 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100002;
    strcpy(etudiant1.etudiantData, "Sofia Rossi nee le 22/07/2002 Milano section A groupe 1 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100003;
    strcpy(etudiant1.etudiantData, "Lucas Martin ne le 05/11/2003 Lyon section A groupe 2 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100004;
    strcpy(etudiant1.etudiantData, "Emma Dubois nee le 30/01/2004 Paris section A groupe 2 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100005;
    strcpy(etudiant1.etudiantData, "Mateo Garcia ne le 18/09/2002 Madrid section B groupe 3 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100006;
    strcpy(etudiant1.etudiantData, "Olivia Nowak nee le 09/06/2003 Warszawa section B groupe 3 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100007;
    strcpy(etudiant1.etudiantData, "Liam Andersson ne le 27/12/2002 Stockholm section B groupe 4 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100008;
    strcpy(etudiant1.etudiantData, "Julia Novak nee le 03/04/2004 Praha section B groupe 4 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100009;
    strcpy(etudiant1.etudiantData, "Noah Jansen ne le 16/08/2003 Amsterdam section C groupe 5 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100010;
    strcpy(etudiant1.etudiantData, "Alice Ferrari nee le 11/02/2002 Roma section C groupe 5 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100011;
    strcpy(etudiant1.etudiantData, "Daniel Schmidt ne le 21/05/2003 Munich section C groupe 6 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100012;
    strcpy(etudiant1.etudiantData, "Chloe Bernard nee le 07/10/2002 Bordeaux section C groupe 6 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100013;
    strcpy(etudiant1.etudiantData, "Marco Bianchi ne le 19/01/2004 Napoli section A groupe 1 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100014;
    strcpy(etudiant1.etudiantData, "Laura Lopez nee le 25/08/2003 Sevilla section A groupe 2 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100015;
    strcpy(etudiant1.etudiantData, "Jonas Weber ne le 02/12/2002 Hamburg section B groupe 3 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100016;
    strcpy(etudiant1.etudiantData, "Marie Lefevre nee le 13/06/2004 Nantes section B groupe 4 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100017;
    strcpy(etudiant1.etudiantData, "Adam Kowalski ne le 28/03/2003 Krakow section C groupe 5 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100018;
    strcpy(etudiant1.etudiantData, "Elena Popescu nee le 04/09/2002 Bucuresti section C groupe 6 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100019;
    strcpy(etudiant1.etudiantData, "Felix Hansen ne le 17/11/2003 Copenhague section A groupe 1 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

    memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));
    etudiant1.matricule = 100020;
    strcpy(etudiant1.etudiantData, "Anna Virtanen nee le 23/02/2004 Helsinki section A groupe 2 ");
    remplireRestEtudiantDefault(&etudiant1);
    insertionEtudiant(tnOVnc, etudiant1, &test);

}

void crierFichierParDefaut() {

    // corrige : malloc supprime , openMainFile alloue deja la memoire
    char filename[30] = DEFAULT_FILE;
    //if (_access(filename, 0) != 0) {
    TnOVnC* tnOVnC = openMainFile(filename, 'w');
    if (tnOVnC == NULL) { printf(RED"\nERROR, crierFichierParDefaut echoue !" WHITE); return; }  // corrige : verifie NULL avant utilise
    chargementInitiale(tnOVnC);
    closeMainFile(tnOVnC);
    //}


}

// pas fini
int reorganisation(TnOVnC* tnOVnC) {

    int numBloc = 1;
    TBloc buf; memset(buf.bloc, '\0', sizeof(TBloc));
    TBloc buf2;  memset(buf2.bloc, '\0', sizeof(TBloc));


    int debutEnregEtudiant = 0;
    Etudiant etudiant = { 0 };
    Etudiant etudiant1 = { 0 };
    int tailleBuf = 0;
    int tailleBuf2 = 0;
    int tailleBufavant = 0;
    int debutEnregEtudiant2 = 0;

    char tabEnreg[sizeof(TBloc)] = { 0 };
    char tabApresEnreg[sizeof(TBloc)] = { 0 };

    for (int i = 1; i < tnOVnC->header.adrLastBloc; i++) {

        liredir(tnOVnC, i, &buf);
        tailleBufavant = strlen(buf.bloc);              // on va teste la taille de bloc avant est apres pour savoir si on doit mettre a jour le bloc au pas

        for (numBloc = i + 1; numBloc <= tnOVnC->header.adrLastBloc; numBloc++) {               // prcoure le bl  oc

            debutEnregEtudiant = 0;
            liredir(tnOVnC, numBloc, &buf2);

            while (debutEnregEtudiant < TAILLE_DE_BLOC - 10) {                              // -10 car si la taille de donne est 1 on va avoir une taille 11 pour tout lenregesrement

                tailleBuf = strlen(buf.bloc);

                tailleBuf2 = strlen(buf2.bloc);
                lireEtudiant(buf2, debutEnregEtudiant, &etudiant);

                if ((etudiant.taille == 0)) { break; }                         // car on a initialise le bloc a \0 alors si ila lue 0 donc ila lu \0\0\0

                if (etudiant.taille <= (TAILLE_DE_BLOC - tailleBuf))
                {
                    ecrireEtudiant(&buf, etudiant);

                    //remplace le bloc de debut enreg que on va supp par le rest de bloc apres le enreg de debutEnregEtudt + etudiant.taille jusqua la fin )
                    memmove(&buf2.bloc[debutEnregEtudiant], &buf2.bloc[debutEnregEtudiant + etudiant.taille], tailleBuf2 - (debutEnregEtudiant + etudiant.taille));

                    // mettre le rest a \0
                    memset(&buf2.bloc[tailleBuf2 - etudiant.taille], '\0', etudiant.taille);

                    tailleBuf = strlen(buf.bloc);               // renvoi la taille de buffer

                }
                else // on va saute ou bien dire on va met le curseur au prochain enregistrement
                {
                    debutEnregEtudiant = debutEnregEtudiant + etudiant.taille;
                }

                memset(etudiant.etudiantData, '\0', sizeof(etudiant.etudiantData));                               // vide le tableau
            }
            ecriredir(tnOVnC, numBloc, buf2);

            sauvgardeFichier(tnOVnC);
            memset(buf2.bloc, '\0', sizeof(TBloc));
        }


        if (tailleBufavant < tailleBuf)             // si le cas donc le buf est modifier sinon il est pas necessaire de l'ecrire dans le disque pas de changement
            ecriredir(tnOVnC, i, buf);

        memset(buf.bloc, '\0', sizeof(TBloc));

    }
    int nbrBloc = GetHeader(tnOVnC, 1);
    for (int j = nbrBloc; j > 0; j--) {

        TBloc buf; memset(buf.bloc, '\0', sizeof(TBloc));
        liredir(tnOVnC, j, &buf);

        if (strlen(buf.bloc) == 0) SetHeader(tnOVnC, 1, (GetHeader(tnOVnC, 1) - 1));
    }
    return 0;

}


int ordonner(TnOVnC* tnOVnC, char nomFichier[], bool defaultName)
{
    char nomFichierOrdonne[40];

    if (defaultName) {// le nom est de nomAvant_Ordonne
        strcpy(nomFichierOrdonne, nomFichier);
        strcat(nomFichierOrdonne, "_Ordonne");
    }
    else {
        strcpy(nomFichierOrdonne, nomFichier);
    }

    // creer le nouveau fichier
    TnOVnC* tOVnC = openMainFile(nomFichierOrdonne, 'w');  // corrige : malloc inutile supprime , openMainFile alloue deja
    if (tOVnC == NULL) { printf(RED"\nERROR, impossible de creer le fichier ordonne !" WHITE); return -1; }  // corrige : verifie NULL

    Etudiant etudiant = { 0 };
    int TAILLE_TABLEAU = GetHeader(tnOVnC, 2) + GetHeader(tnOVnC, 3);  // envoye la taille de tableau
    int numBloc;
    TBloc buf;
    bool trouve, test, corb;
    int debutEnregEtudiant = 0;

    //variable - sized array(VLA) dans le C99 on peut creer un tableau de taille determine a l'execution
    Tmatricule tableau[TAILLE_TABLEAU];
    int j = 0; // indice des elements du tableau "tableau[j]"

    // lire les matricules a un tableau de matricule
    for (numBloc = 1; numBloc <= GetHeader(tnOVnC, 1); numBloc++) {

        debutEnregEtudiant = 0;
        liredir(tnOVnC, numBloc, &buf);

        // lire les enreg d'bloc
        while (debutEnregEtudiant < TAILLE_DE_BLOC - 10) {

            lireEtudiant(buf, debutEnregEtudiant, &etudiant);

            if ((etudiant.taille == 0)) {
                debutEnregEtudiant = 10000;
            }
            else {
                // eriture de la matriulle au tableau
                tableau[j].Tmatricule = etudiant.matricule;
                debutEnregEtudiant = debutEnregEtudiant + etudiant.taille;
                j++;
            }

        }
    }
    int temp;

    // tri a bulle de tableau
    for (int i = 0; i < TAILLE_TABLEAU; i++) {
        for (int j = 0; j < TAILLE_TABLEAU - 1; j++) {
            if (tableau[j].Tmatricule > tableau[j + 1].Tmatricule) {
                temp = tableau[j].Tmatricule;
                tableau[j].Tmatricule = tableau[j + 1].Tmatricule;
                tableau[j + 1].Tmatricule = temp;
            }
        }
    }

    numBloc = 1;
    debutEnregEtudiant = 0;

    memset(etudiant.etudiantData, '\0', sizeof(etudiant.etudiantData));
    // l'ecriture au fichier selon les matricule ordonne
    for (j = 0; j < TAILLE_TABLEAU; j++)
    {
        // rechrhe
        rechercheEtudiant(tnOVnC, tableau[j].Tmatricule, &trouve, &etudiant, &numBloc, &debutEnregEtudiant, &corb);

        //ereture
        insertionEtudiant(tOVnC, etudiant, &test);
        //initialisation des donne de l'etudiant pour la prochain ecriture
        memset(etudiant.etudiantData, '\0', sizeof(etudiant.etudiantData));
        sauvgardeFichier(tOVnC);

    }

    // fermeture de fichier ordonne
    closeMainFile(tOVnC);
    return 0;
}


int modifierEtudiant(TnOVnC* tnOVnC, Etudiant etudiant,int numBloc,int debutEnregDansBloc,Etudiant etudiant2) {

    TBloc buf;
    memset(buf.bloc, '\0', sizeof(TBloc));

    liredir(tnOVnC, numBloc, &buf);
    int tailleBuf = strlen(buf.bloc);               // renvoi la taille de buffer

    //remplace le bloc de debut enreg que on va supp par le rest de bloc apres le enreg de debutEnregEtudt + etudiant.taille jusqua la fin )
    memmove(&buf.bloc[debutEnregDansBloc], &buf.bloc[debutEnregDansBloc + etudiant2.taille], tailleBuf - (debutEnregDansBloc + etudiant2.taille));

    // mettre le rest a \0
    memset(&buf.bloc[tailleBuf - etudiant2.taille], '\0', etudiant2.taille);

    ecriredir(tnOVnC, numBloc, buf);
    sauvgardeFichier(tnOVnC);

    liredir(tnOVnC, GetHeader(tnOVnC, 1), &buf);  //lecture de dernier  bloc
    if (TAILLE_DE_BLOC - strlen(buf.bloc) >= etudiant.taille)               // test si il y a de l'espace
    {
        ecrireEtudiant(&buf, etudiant);
        ecriredir(tnOVnC, GetHeader(tnOVnC, 1), buf);
    }
    else                // sinon on crier un bloc et on ecrit l'etudiant
    {
        creerBloc(tnOVnC);
        memset(buf.bloc, '\0', sizeof(TBloc)); // initialise le buffer
        ecrireEtudiant(&buf, etudiant);
        ecriredir(tnOVnC, GetHeader(tnOVnC, 1), buf);
    }

    return 0;


}

void enterSound() {
    Beep(750, 150);     // la premier case pour Hz et la 2 case pour le temps
    // par example ici 750 Hz et 150 ms
}

void successSound() {    // le son augment la dure deminiue

    Beep(2100, 190);
    Beep(2600, 100);
}

void failSound() {      // le son deminiue la dure augment
    Beep(700, 300);
    Beep(400, 800);

}

void sFSD_ProjectAffichige() {

    system("title SFSD - Gestion des Etudiants  |  NAIT KACI Anis");
    system("color 0F");
    system("chcp 65001 > nul");
    // corrige : taille fixe supprimee , le terminal est maintenant redimensionnable et supporte le plein ecran
    system("cls");

    printf("\n\n\n\n\n\n\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x94\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x97\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" GRAY "                                                          " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" BLUE "      S F S D  \xE2\x80\x94  Gestion de Fichiers Dynamiques         " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" GRAY "                                                          " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" GRAY "          Structure TnOVnC  \xC2\xB7  Blocs 1000 octets           " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" GRAY "                                                          " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" CYAN "               Realise par : NAIT KACI Anis               " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" CYAN "         Fellag Aghiles  \xC2\xB7  Hamrani Mouhand Idir          " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x9A\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x9D\n");

    printf(WHITE "\n\n\n\t\t\t\t\t    Appuyez sur une touche pour continuer...\n" GRAY);
    _getch();
    enterSound();
}

int testRevenireMenu(bool* contin ) {

    char restart = ' ';
    printf("\n\nVoulez vous revenire ou menu y/n ? : " CYAN);
    scanf(" %c", &restart); printf(WHITE);
    enterSound();

    if(restart == 'y' || restart == 'Y') *contin = true;
     else*contin = false;
    return 0;
}

void affichageDebut(int optionSelectionne, bool selection, char* nomFichierSelectionne, int nbrEnreg, int nbrBlocs, int nbrSuprLogiq) {

    system("cls");

    // --- En-tete centree (59 chars interieur) ---
    printf("\n");
    printf(PURPLE "\t\t\t\xE2\x95\x94\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x97\n");
    if (selection) {
        printf(PURPLE "\t\t\t\xE2\x95\x91" GRAY "  Fichier : " GREEN "%-20s" GRAY "                           " PURPLE "\xE2\x95\x91\n", nomFichierSelectionne);
        printf(PURPLE "\t\t\t\xE2\x95\x91" YELLOW "  nbrEnreg : %3d" GRAY " | " YELLOW "corbeille : %3d" GRAY " | " YELLOW "blocs : %2d" GRAY "            " PURPLE "\xE2\x95\x91\n", nbrEnreg, nbrSuprLogiq, nbrBlocs);
    } else {
        printf(PURPLE "\t\t\t\xE2\x95\x91" ORANGE "  Aucun fichier selectionne                                " PURPLE "\xE2\x95\x91\n");
        printf(PURPLE "\t\t\t\xE2\x95\x91" GRAY "  Utilisez l option 0 pour selectionner un fichier         " PURPLE "\xE2\x95\x91\n");
    }
    printf(PURPLE "\t\t\t\xE2\x95\x9A\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x9D\n\n");

    // --- Logo ---
    printf(PURPLE
        "\t _____ _     _    _____     _        _____             \n"
        "\t|     |_|___|_|  |     |___|_|___   |     |___ ___ _ _ \n"
        "\t| | | | |   | |  | | | | .'| |   |  | | | | -_|   | | |\n"
        "\t|_|_|_|_|_|_|_|  |_|_|_|__,|_|_|_|  |_|_|_|___|_|_|___|\n\n");

    // --- Items du menu (normal) ---
    const char* menu[] = {
        GRAY"  0  " BLUE"Selectionner un fichier existant\n",
        GRAY"  1  " BLUE"Creer un nouveau fichier\n",
        GRAY"  2  " BLUE"Inserer un etudiant\n",
        PURPLE"+--[ LECTURE ]---------------------------------------------------------------------------+\n",
        GRAY"  3  " BLUE"Rechercher un etudiant par matricule\n",
        GRAY"  4  " BLUE"Lire tous les etudiants d'un bloc\n",
        GRAY"  5  " BLUE"Lire tous les etudiants du fichier\n",
        GRAY"  6  " BLUE"Afficher la corbeille (suppr. logiques)\n",
        PURPLE"+--[ STATISTIQUES ]----------------------------------------------------------------------+\n",
        GRAY"  7  " BLUE"Nombre total d'etudiants\n",
        GRAY"  8  " BLUE"Nombre d'etudiants dans la corbeille\n",
        GRAY"  9  " BLUE"Espace restant dans un bloc\n",
        PURPLE"+--[ SUPPRESSION ]-----------------------------------------------------------------------+\n",
        GRAY" 10  " BLUE"Suppression logique (vers corbeille)\n",
        GRAY" 11  " BLUE"Suppression physique (definitive)\n",
        GRAY" 12  " BLUE"Vider la corbeille\n",
        PURPLE"+--[ MODIFICATION / GESTION ]------------------------------------------------------------+\n",
        GRAY" 13  " BLUE"Modifier un etudiant existant\n",
        GRAY" 14  " BLUE"Restaurer un etudiant depuis la corbeille\n",
        GRAY" 15  " BLUE"Restaurer tous les etudiants de la corbeille\n",
        GRAY" 16  " BLUE"Ordonner le fichier\n",
        PURPLE"+--[ CONTROLE ]--------------------------------------------------------------------------+\n",
        GRAY" 17  " RED "Deselectionner le fichier en cours\n",
        GRAY" 18  " RED "Quitter le programme\n"
    };

    // --- Items du menu (selectionne) ---
    const char* menuSel[] = {
        GRAY"  0  " GREEN"Selectionner un fichier existant\n",
        GRAY"  1  " GREEN"Creer un nouveau fichier\n",
        GRAY"  2  " GREEN"Inserer un etudiant\n",
        PURPLE"+--[ LECTURE ]---------------------------------------------------------------------------+\n",
        GRAY"  3  " GREEN"Rechercher un etudiant par matricule\n",
        GRAY"  4  " GREEN"Lire tous les etudiants d'un bloc\n",
        GRAY"  5  " GREEN"Lire tous les etudiants du fichier\n",
        GRAY"  6  " GREEN"Afficher la corbeille (suppr. logiques)\n",
        PURPLE"+--[ STATISTIQUES ]----------------------------------------------------------------------+\n",
        GRAY"  7  " GREEN"Nombre total d'etudiants\n",
        GRAY"  8  " GREEN"Nombre d'etudiants dans la corbeille\n",
        GRAY"  9  " GREEN"Espace restant dans un bloc\n",
        PURPLE"+--[ SUPPRESSION ]-----------------------------------------------------------------------+\n",
        GRAY" 10  " GREEN"Suppression logique (vers corbeille)\n",
        GRAY" 11  " GREEN"Suppression physique (definitive)\n",
        GRAY" 12  " GREEN"Vider la corbeille\n",
        PURPLE"+--[ MODIFICATION / GESTION ]------------------------------------------------------------+\n",
        GRAY" 13  " GREEN"Modifier un etudiant existant\n",
        GRAY" 14  " GREEN"Restaurer un etudiant depuis la corbeille\n",
        GRAY" 15  " GREEN"Restaurer tous les etudiants de la corbeille\n",
        GRAY" 16  " GREEN"Ordonner le fichier\n",
        PURPLE"+--[ CONTROLE ]--------------------------------------------------------------------------+\n",
        GRAY" 17  " ORANGE"Deselectionner le fichier en cours\n",
        GRAY" 18  " ORANGE"Quitter le programme\n"
    };

    for (int i = 0; i < 24; ++i) {
        bool isSep = (i == 3 || i == 8 || i == 12 || i == 16 || i == 21);
        if (isSep) {
            printf(WHITE"%s", menu[i]);
        } else if (i == optionSelectionne) {
            printf(INVERSE GREEN" >> " RESET);
            printf("%s", menuSel[i]);
        } else {
            printf(WHITE"    ");
            printf("%s", menu[i]);
        }
    }

    printf(PURPLE"+--[ " GRAY"Haut/Bas : naviguer   |   Entree : executer" PURPLE" ]-------------------------------------------+\n" WHITE);
}



void asciiArtTextLine(const char* text, int animspeed)
{
    int i;
    for (i = 0; i < strlen(text); i++) {

        printf("%c", text[i]);
        Sleep(animspeed);
    }
}


void affichageFin() {

    system("cls");
    Sleep(300);
    printf(PURPLE);

    printf("\n\n\n\n\n\n\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x94\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x97\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" GRAY "                                                          " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" BLUE "            Projet Academique  2023 / 2024                " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" GRAY "                                                          " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" CYAN "     TnOVnC : Tableau Non Ordonne Variable                " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" CYAN "              Sans Chevauchement                         " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" GRAY "                                                          " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" GREEN "               Realise par : NAIT KACI Anis               " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x91" GREEN "         Fellag Aghiles  \xC2\xB7  Hamrani Mouhand Idir          " PURPLE "\xE2\x95\x91\n");
    printf(PURPLE "\t\t\t\t  \xE2\x95\x9A\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x90\xE2\x95\x9D\n");

    printf(WHITE "\n\n\n\t\t\t\t\t      Merci d'avoir utilise ce programme.\n\n" GRAY);
    Sleep(300);
    asciiArtTextLine(" \t\t\t\t\t ++  .........-----------------------------------------------------.-...-++##++\n"
        " \t\t\t\t\t  #+.......--------------------------------------------------------.--......-+##++ \n"
        " \t\t\t\t\t  +#-...--.-------------------......-------------------------------.....-..... .+##++ \n"
        " \t\t\t\t\t   +#+...-.-------------------.------...-........---....--...---------------....  .+ \n"
        " \t\t\t\t\t    +#+..-.-----------...-----.--..-.--.-.---.--...-.--....-...-----------...-...-+ \n"
        " \t\t\t\t\t    +##+--.----------....-.....--  +....-..-.-.-.-....-.-.----.-----------.-.. -##+\n"
        " \t\t\t\t\t  ##+. ....--------...-+.-.-...#   ----.-.-.-+ #..-..-..# -..-.-----------....##+ \n"
        " \t\t\t\t       ##+-.......---------..- #.-...-#. .. -.....-.#   #....-..# +-.-.------------.-##+ \n"
        " \t\t\t\t     +-.  ...--------------.-# #.-..--. ....--------. . .+.---.-  .+...----...-----.-##\n"
        " \t\t\t\t      #- ......----.........-  -...--- ......+....+ . .. --....# . .+-..-...-....-... -##\n"
        " \t\t\t\t        #+.....----.....-..-- ..+...+. ..... .-.-.+ ......---+-#.----++++++++--------...-## \n"
        " \t\t\t\t\t  ##........-----..# .. +-.-. ........+..#.....---.--        .     -. ...-+++++-..+#+ \n"
        " \t\t\t\t\t    ##--.--.--.--.-  ....-.-.--...... -+-..--..    .- #.-.   .- . # +          ---.+#+\n"
        " \t\t\t\t\t    #-..--..- .+..+..- -.+-. ..........-#.           .+  .-.- ..  #  -  ... ..      .++- \n"
        " \t\t\t\t\t   #-....----  ---...... .- .......--.                       .#+ -   +. ....    .#### \n"
        " \t\t\t\t\t #+...--.--.- ..-#. .....  ....---.                            . +    - .... -#### \n"
        " \t\t\t\t\t#-...---..--+ ..............---                                .#     + ...  ##\n"
        " \t\t\t\t       + ......-.-.-. ............--            ..                            ......  +#\n"
        " \t\t\t\t      +- ....-.-.-.+ ......    ___         .    .--           +####+_         . ....   +# \n"
        " \t\t\t\t\t##-......-.- .......###++###- .       ....-        -##       ##+       - .....  ##\n"
        " \t\t\t\t\t   ##-..--.-.......*           +     .+-..-+.     --                   . . ...   ## \n"
        " \t\t\t\t\t     #---.--......--.                                                  -....      ## \n"
        " \t\t\t\t\t    +#-.-. +.. ....                                           --.       . ..        +#- \n"
        " \t\t\t\t\t   #-..-.- +- ..-.-+.                 .-. -. .                     .   - .. .#######+\n"
        " \t\t\t\t\t  #- ..- .  #..-       .                                     --       +  +.  -#\n"
        " \t\t\t\t\t +-    . .. ...    +                                            ..       ..  +#+ \n"
        " \t\t\t\t\t  #####- ...-  ..     .                  ..........       .         +.       -#+ \n"
        " \t\t\t\t\t\t#  ...       -.         .   +##..           --##-.  ..       ..-  ##.  ++ \n"
        " \t\t\t\t\t\t#. ..-.    -         -#.                         #    --    -... .#####+++ \n"
        " \t\t\t\t\t\t #  .-.         ..-        .####################-#         ---.  ##+ \n"
        " \t\t\t\t\t\t  #. ...          .  +########+---------#####+###         #  .  ##+ \n"
        " \t\t\t\t\t\t   #.  ..         .####+#+.................-+###         +     ##+ \n"
        " \t\t\t\t\t\t    #-  -           -##+--.----.--------++++#           .    ###+\n"
        " \t\t\t\t\t\t     ###-.-              +#++++++++++#+#.              ..+####+ \n"
        " \t\t\t\t\t\t\t#..-+                                        .-. +#+\n"
        " \t\t\t\t\t\t\t#+. ..-.                                   -.. . ++ \n"
        " \t\t\t\t\t\t\t #. #.. -.                              -#. -#.  #+ \n"
        " \t\t\t\t\t\t\t #+ ##-.####-.                      .-#.#+# +##+ + \n"
        " \t\t\t\t\t\t\t   #-.+++++#-.----               .--..- +++#+.-###+ \n"
        " \t\t\t\t\t\t\t #+. +++---#.-....---.-----------...... #+--+++.-#+ \n"
        " \t\t\t\t\t\t     +##+....+++---+.  ........................ #+++++-...#+ \n"
        " \t\t\t\t\t\t  +##+.......-++---+. ...........----------...  +---+-.--..-##+\n"
        " \t\t\t\t\t\t++#-..........#+---++  ....-----.            . --.-++. ......-##+ \n"
        " \t\t\t\t\t    ++##+............ #+---+++  ..-                  .----+-- .......  .+##+ \n"
        " \t\t\t\t\t  +++#-................#+-++++++                    .+--+--- ........... .++ \n"
        " \t\t\t\t        +++#+..............-.-.-#++-----++.               .++--+--+............... .##+-\n"
        " \t\t\t\t       ++#+............------.. .#+---+--++++.        ..+++--++--+.................. -##+- \n"
        " \t\t\t\t      ++#+..........----........  +#+-------++++++#++++++-------+. ..................  +#++ \n"
        " \t\t\t\t     ++++.........---............. .++---------+++++----------++....................... -#++ \n"
        " \t\t\t\t    ++++.......---.................. .-++-----------------+++-. ........................ -#++ \n"
        " \t\t\t\t   ++++......--.......................  .+++---------+++++-.  ........................... -#++ \n"
        " \t\t\t\t  ++#+.....--............................  --++++++++-..   ................................#++ \n"
        " \t\t\t\t ++++.....--................................    +     .....................................-#++ \n"
        " \t\t\t\t+++#-...--..................................................................................+++ \n", 0);

    printf(GRAY);
}



void fAQ() {

    char faq[300] = { 0 };
    char testfAQ;

    printf(WHITE"Any FAQ  or advice about our tool ? y/n: " CYAN);
    scanf(" %c", &testfAQ);
    printf(GRAY);

    if (testfAQ == 'y' || testfAQ == 'Y') {

        printf(GRAY"\nFAQ : "); printf(CYAN);

        getchar();
        fgets(faq, sizeof(faq), stdin);
        printf(WHITE);

        FILE* faqf = fopen(FAQ_FILE, "wb+");
        if (faqf == NULL) { printf(RED"\nERROR, impossible d'ecrire le fichier FAQ !" WHITE); return; }  // corrige : verifie NULL

        fwrite(faq, sizeof(faq), 1, faqf);
        fclose(faqf);

        printf(WHITE"\nThanksss !!\n" GRAY);

    }


}