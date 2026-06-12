
#include "header.h"

int main()
{
    // ouvrir on "full screen"
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

    TnOVnC* tnOVnC = (TnOVnC*)malloc(sizeof(TnOVnC));
    char restart = 'n';                 // indique quand on va sortir de programe
    bool selection = false;
    int optionSelectionne = 0;
    int key=0;
    bool contin=true;

    // donne des fonction
    char nomFichierSelectionne[30] = { 0 };
    int matricule;
    int nbrBloc;
    int numBloc;
    int debutOEnreg;
    bool trouv;
    bool corbeille;
    int debutEnregEtudiant = 0;

    int countAffich;                                    // compt le nombre d'affichage pour determine si le fichier est vide ou pas
    bool testSuppresionPhysique = false;                // pour la reorganisation

    sFSD_ProjectAffichige();
    _getch();

    crierFichierParDefaut();    // son nome est "Deafult_File"

Start:       // une etiquette pour relance le programe

   // do {
    while (contin){

        affichageDebut(optionSelectionne, selection, nomFichierSelectionne,
                       selection ? GetHeader(tnOVnC, 2) : 0,
                       selection ? GetHeader(tnOVnC, 1) : 0,
                       selection ? GetHeader(tnOVnC, 3) : 0);               // affiche le menu

        key = _getch();  // avoir le button dans le clavier

        if (key == 224) {  // test si un upper ou down button de clavier (on a fait 2 test car il envoye un chaine de code ascii )
            key = _getch();
            switch (key) {
            case 72:  // le cas est upper button clavier donc on decremante
                if (optionSelectionne > 0) {
                    optionSelectionne--;
                    // test si on doit sauter la ligne ou pas "+--------------------------------------------------------------------------------+"
                    if (optionSelectionne == 3 || optionSelectionne == 8 || optionSelectionne == 12 || optionSelectionne == 16 || optionSelectionne == 21) {
                        optionSelectionne--;
                    }
                }
                else {
                    if (optionSelectionne == 0) optionSelectionne = 23;
                }

                break;
            case 80:  // le cas est down button clavier donc on augmante
                if (optionSelectionne < 23) {
                    optionSelectionne++;
                    // test si on doit sauter la ligne ou pas "+--------------------------------------------------------------------------------+"
                    if (optionSelectionne == 3 || optionSelectionne == 8 || optionSelectionne == 12 || optionSelectionne == 16 || optionSelectionne == 21) {
                        optionSelectionne++;
                    }
                }
                else {// pour retourne au debut si il depasse
                    if (optionSelectionne == 23) optionSelectionne = 0;
                }
                break;

            }
        }
        else if
            (key == 13) {

            switch (optionSelectionne) {

                // selectionne un fichier deja creer
            case 0: {
                system("cls");

                // ferme le fichier deja selectionne
                if (selection) {
                    if (testSuppresionPhysique) {reorganisation(tnOVnC); testSuppresionPhysique=false; }   // si il y a un suppression physique on fait une reorganisation
                    closeMainFile(tnOVnC);
                }

            Reselectionne:

                // lire le nom de fichier
                system("cls");
                printf("Entrez le nom de fichier a selectionne : ");  printf(CYAN);
                scanf(" %29s", nomFichierSelectionne);
                printf(WHITE);  // tourne la couleur blanc

                // test si le fichier existe
                if (_access(nomFichierSelectionne, 0) == 0) {

                    // si oui on l'ouvre et on fait les changement necessaire " selection a vrai "
                    successSound();                         // le son de succes
                    tnOVnC = openMainFile(nomFichierSelectionne, 'a');
                    if (tnOVnC == NULL) { printf(RED"\nERROR, ouverture du fichier echoue !" WHITE); failSound(); break; }  // corrige : verifie NULL
                    selection = true;
                    printf(GRAY"\nvous avez selectionne le fichier " BLUE"%s " WHITE, nomFichierSelectionne);
                    Sleep(1000);

                }
                else
                {
                    // sinon on affiche erreur et on fait les changement necessaire " selection a faux "
                               // le son d'erreur
                    selection = false;
                    printf(ORANGE"\nle fichier n'existe pas !" WHITE",voulez vous selecionne un autre fichier y/n ?" CYAN);
                    failSound();
                    memset(nomFichierSelectionne, ' ', sizeof(nomFichierSelectionne));                                          // initialise car il peut le selectionne just apres

                    // test si l'utilisateur veut reutilisr cette fonction
                    char reselectionne;
                    scanf(" %c", &reselectionne);  printf(WHITE);
                    enterSound();

                    if (reselectionne == 'y' || reselectionne == 'Y') goto  Reselectionne;

                }

                break;
            }

            case 1: {  // creation d'un fichier

                char nomFichier[30] = { 0 };

            Recreer:      // une etiquette pour relance la fonction
                   system("cls");

                // lire nom de fichier
                printf("Entrez le nom de fichier a creer : ");  printf(CYAN);
                scanf(" %29s", nomFichier);  printf(WHITE);

                // compare si le fichier et deja ouvert ou bien selectionne
                if (strcmp(nomFichier, nomFichierSelectionne)) {

                    if (_access(nomFichier, 0) != 0) {
                    Creer:
                        // si il existe pas on le crier
                        successSound();
                        // corrige : verifie NULL avant closeMainFile sinon crash
                        { TnOVnC* tmp = openMainFile(nomFichier, 'w'); if (tmp != NULL) closeMainFile(tmp); }
                        printf(GREEN"\nLa creation a ete bien effectue !");
                        printf(WHITE"\nVoulez vous selectionne ce fichier y/n ?" CYAN);
                        char testselectionne;
                        scanf(" %c", &testselectionne); printf(WHITE);

                        // on test si l'utilisateur veut le selectionne
                        if (testselectionne == 'y' || testselectionne == 'Y') {

                            // si on a pas selectionne un fichier ,on ouvre le fichier et on fait les changement necessaire
                            //"  copie le nom de fichier a fichier selectionne" et  " selectionne a vrai "
                            if (selection == false) {

                                tnOVnC = openMainFile(nomFichier, 'w');
                                if (tnOVnC == NULL) { printf(RED"\nERROR, ouverture du fichier echoue !" WHITE); failSound(); break; }  // corrige : verifie NULL
                                selection = true;
                                strcpy(nomFichierSelectionne, nomFichier);

                            }
                            // sinon ferme le fichier precedent et copie le nom de fichier a fichier selectionne
                            else
                            {
                                closeMainFile(tnOVnC);
                                tnOVnC = openMainFile(nomFichier, 'w');
                                if (tnOVnC == NULL) { printf(RED"\nERROR, ouverture du fichier echoue !" WHITE); failSound(); selection = false; break; }  // corrige : verifie NULL
                                strcpy(nomFichierSelectionne, nomFichier);

                            }

                        }

                    }
                    else {// si le fichier exist deja

                        printf(RED"\nLe fichier existe deja " WHITE"voulez vous l'ecrase y/n ?! : " CYAN);
                        char testEcrase;
                        scanf(" %c", &testEcrase); printf(WHITE);

                        // si oui on fait l'algorithm de cration derect
                        if (testEcrase == 'y' || testEcrase == 'Y') goto Creer;

                    }


                }
                else  //  donc l'utilisateur veut crier le meme fichier qu'il utilise
                {
                    failSound();
                    printf(ORANGE);
                    printf("\nLe fichier est actuellement utilise par le programme\nSi vous souhaitez le recreer, vous devez le deselectionner d'abord !");
                    printf(WHITE);
                }

                // pour recration test si l'utilisateur veut reutilise cette fonction
                char recreer;
                printf("\nvoulez vous recreer y/n ?");     printf(CYAN);
                scanf(" %c", &recreer);  printf(WHITE);
                enterSound();
                if (recreer == 'y' || recreer == 'Y') goto  Recreer;
                testRevenireMenu(&contin);

                break;
            }

            case 2: { // pour l'insertion d'un etudiant dans un fichier deja selectionne

                Etudiant etudiant = { 0 };                  // initialise
                bool testInsertion;
                int nbrBlocAvant = GetHeader(tnOVnC, 1);        // pour affiche si on va ajout un bloc
                 system("cls");
                // si l'utilisateru a selectionne un fichier
                if (selection) {


                insertion:
                    // lire matricule
                    printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                    printf("\nEntrez la matricule de l'etudiant (max 6 chifres): ");  printf(CYAN);
                    scanf(" %d", &etudiant.matricule);  printf(WHITE);

                    // test matricule si elle est valide
                    if (etudiant.matricule < 1000000 && etudiant.matricule >= 0) {
                        getchar();                                              // pour recevoire le \n dans le buffer  car (scanf envoye un \n a la fin execution et fgets termine la lecture...
                        printf("\nEntrez les informations de l'etudiant : " CYAN);           //... dit que elle trouve un \n , si pour sa que j ai utilise getchar pour qu'elle recevoi ce \n

                        // pour pouvoir ecrire avec l'espace vide blanc dans le tableau (avec scanf on peut pas)
                        fgets(etudiant.etudiantData, sizeof(etudiant.etudiantData), stdin);
                        printf(WHITE);

                        // fait l'insertion
                        remplireRestEtudiantDefault(&etudiant);   // remplire le rest des donne pour l'insere " la taille , bit supression,.. "
                        insertionEtudiant(tnOVnC, etudiant, &testInsertion);   // insertion de l'etudiant

                        if (GetHeader(tnOVnC, 1) > nbrBlocAvant) printf(GRAY"\nLe Bloc %d " GRAY"a ete crier" WHITE, GetHeader(tnOVnC, 1));
                        nbrBlocAvant = GetHeader(tnOVnC, 1);

                        // test l'execution de l'insertion
                        if (testInsertion) {
                            printf(GREEN"\nl'insertion est effectue" WHITE);
                            sauvgardeFichier(tnOVnC);                                          // fonction pour sauvgarde les mise ajour sans ferme le fichier
                            successSound();  // le son de succes
                        }
                        else
                        {
                            printf(ORANGE"\nL'etudiant exist deja !" WHITE);
                            failSound();  // le son de l'erreur
                        }
                    }
                    // cas la mtricule est grand
                    else { printf(ORANGE"\nERROR , la taille de la matricule est inacceptable" WHITE);    failSound(); }


                    // Testez si l'utilisateur souhaite reutiliser cette fonction
                    printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                    char reinsere;
                    printf("\nvoulez vous reinserer y/n ? : " CYAN);
                    scanf(" %c", &reinsere); printf(WHITE);
                    enterSound();
                    if (reinsere == 'y' || reinsere == 'Y') goto  insertion;



                }
                // erreur l'utilisateur n'a pas selectionne un ficheir
                else { printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }

            case 4: {
                system("cls");

                Etudiant etudiant1 = { 0 };
                memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));  // mettre to les charahtaire  tableau a \0

                // si lutilisateur a selectionne un fichier
                if (selection) {


                Recherche:
                    // lire la matricule
                    printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                    printf("\nEntrez la matricule de l'etudiant a affiche : " CYAN);
                    scanf(" %d", &matricule); printf(WHITE);

                    // test lexecution de la fonction
                    rechercheEtudiant(tnOVnC, matricule, &trouv, &etudiant1, &numBloc, &debutOEnreg, &corbeille);
                    // si on le trouve on l'affiche
                    if (trouv && !corbeille) {
                        printf(GRAY"\nLa matricule est :" YELLOW" %06d \n" GRAY"les information de l'etudiant sont : " YELLOW"%s " WHITE, etudiant1.matricule, etudiant1.etudiantData);
                        successSound();
                    }
                    // sinon on affiche l'erreur  de " exception de la non existance! "
                    else {
                        printf(ORANGE"\nL'etdudiant avec la matricule " YELLOW"%06d " ORANGE"n existe pas !" WHITE, matricule);
                        failSound();
                    }

                    // test si lutilisateur veut reutilise cette fonction
                    char recherche;
                    printf("\nvoulez vous relire y/n ? : " CYAN);
                    scanf(" %c", &recherche); printf(WHITE);
                    enterSound();
                    if (recherche == 'y' || recherche == 'Y') goto Recherche;   // rexecute la fonction
                }
                // erreur l'utilisateur n'a pas selectionne un ficheir
                else {
                    printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }

            case 5: {
                system("cls");

                Etudiant etudiant1 = { 0 };
                TBloc buf;
                memset(buf.bloc, '\0', sizeof(TBloc));

                if (selection) {

                    // affcihe le nombre de bloc existe deja
                    printf(GRAY"le nombre de bloc existe dans le fichier est : " YELLOW" %d\n" WHITE, GetHeader(tnOVnC, 1));
                LireBloc:

                    // lire nombre de bloc
                    printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                    printf("\nEntrez le num de bloc a lire : " CYAN);
                    scanf(" %d", &numBloc); printf(WHITE);

                    // test si le nombre de bloc est correct
                    if (numBloc <= GetHeader(tnOVnC, 1) && numBloc >= 1) {

                        // lire le bloc
                        liredir(tnOVnC, numBloc, &buf);
                        debutEnregEtudiant = 0;

                        // parcoure les enreg
                        while (debutEnregEtudiant < TAILLE_DE_BLOC - 10) {

                            lireEtudiant(buf, debutEnregEtudiant, &etudiant1);
                            if ((etudiant1.taille == 0)) { break; }  // dans ce cas il a lut espace vide donc la fin

                            // l'adr pour lire le prochain enreg
                            debutEnregEtudiant = debutEnregEtudiant + etudiant1.taille;

                            // si il est pas efface logiqement
                            if (!etudiant1.efface)
                                printf(GRAY"\nLa matricule " YELLOW"%06d " GRAY"les donne de letudiant " YELLOW"%s " WHITE, etudiant1.matricule, etudiant1.etudiantData);

                            memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData)); // initialise
                        }
                        successSound();
                    }
                    // si le num de bloc est faux
                    else {
                        printf(ORANGE"\nERROR, numero de bloc n'existe pas" WHITE);
                        failSound();
                    }

                    // pour rexecute la fonction
                    printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                    char recherche;
                    printf("\nvoulez vous relire un bloc y/n ? " CYAN);
                    scanf(" %c", &recherche); printf(WHITE);
                    enterSound();
                    if (recherche == 'y' || recherche == 'Y') goto LireBloc;

                }
                else { printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }

            case 6: {
                system("cls");

                Etudiant etudiant1 = { 0 };
                TBloc tbloc;
                countAffich = 0;

                if (selection) {


                    // parcour les bloc
                    for (numBloc = 1; numBloc <= tnOVnC->header.adrLastBloc; numBloc++) {

                        int debutEnregEtudiant = 0;
                        liredir(tnOVnC, numBloc, &tbloc);

                        // parcour les enreg dans un bloc
                        while (debutEnregEtudiant < TAILLE_DE_BLOC - 10) {

                            lireEtudiant(tbloc, debutEnregEtudiant, &etudiant1);

                            if ((etudiant1.taille == 0)) { break; }

                            // l'adr pour lire le prochain enreg
                            debutEnregEtudiant = debutEnregEtudiant + etudiant1.taille;
                            if (!etudiant1.efface) { // si il est pas efface

                                printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                                printf(GRAY"\nLa matricule " YELLOW"%06d " GRAY"les information de l'etudiant " YELLOW"%s " WHITE, etudiant1.matricule, etudiant1.etudiantData);
                                countAffich++;
                            }

                            memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));                               // vide le tableau
                        }

                    }
                    if (countAffich == 0) { printf(YELLOW"\nLe fichier est vide !" WHITE); }
                    successSound();

                }
                else { printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }

            case 7: {

                 system("cls");
                Etudiant etudiant1 = { 0 };
                TBloc tbloc;
                int numBloc = 1;
                countAffich = 0;                                                           // conteur de nombre matricule affiche

                if (selection) {


                    for (numBloc = 1; numBloc <= tnOVnC->header.adrLastBloc; numBloc++) {               // prcoure le bloc

                        int debutEnregEtudiant = 0;
                        liredir(tnOVnC, numBloc, &tbloc);

                        while (debutEnregEtudiant < TAILLE_DE_BLOC - 10) {      // parcour les enreg

                            lireEtudiant(tbloc, debutEnregEtudiant, &etudiant1);

                            if ((etudiant1.taille == 0)) { break; }             // si on a lut un null resp \0 on sort

                            debutEnregEtudiant = debutEnregEtudiant + etudiant1.taille;     // pass resp adr procain enreg
                            if (etudiant1.efface) {     // letudiant est dans la corbeille donc on affiche sa matricules

                                printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                                printf(GRAY"\n la matricule : " YELLOW"%06d  " WHITE, etudiant1.matricule);
                                countAffich++;
                            }

                            memset(etudiant1.etudiantData, '\0', sizeof(etudiant1.etudiantData));                               // vide le tableau
                        }


                    }

                    if (countAffich == 0) printf(YELLOW"\n la corbeille est vide !" WHITE);     // la corbeille est donc vide
                    successSound();
                }
                else { printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }

            case 9: {
                system("cls");

                // affiche le nombre des etudiant a partire de la tete
                if (selection) {

                      printf(GRAY"\nle nombre des etudiant est :" YELLOW" %d" WHITE, tnOVnC->header.nbrEnreg);
                      successSound(); }
                else {
                    printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }

            case 10: {
                system("cls");

                if (selection) {

                    // affiche le nombre des etudiant dans la corbeille de la tete
                    printf(GRAY"\nLe nombre des etudiant dans la corbeille  est :" YELLOW" %d" WHITE, tnOVnC->header.nbrSuprLogiq);
                    successSound();
                }
                else {
                    printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);
                    failSound();

                }

                testRevenireMenu(&contin);

                break;
            }
            case 11: {
                system("cls");

                if (selection) {


                    printf("Le nombre de bloc existe dans le fichier est :" YELLOW" %d\n" WHITE, GetHeader(tnOVnC, 1));
                RestDeBloc:

                    printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                    printf("\nEntrez le numero du bloc pour afficher l'espace restant : \n" CYAN);
                    scanf(" %d", &nbrBloc); printf(WHITE);

                    //Affiche l'espace
                    if (nbrBloc >= 0 && nbrBloc <= GetHeader(tnOVnC, 1)) {
                        printf(GRAY"L'espace restant est  : " YELLOW"%d " GRAY"caractere" WHITE, RestDeBloc(tnOVnC, nbrBloc));
                        successSound();
                    }
                    else
                    {
                        printf(ORANGE"\nNumero de ce bloc n'existe pas dans ce fichier" WHITE);
                        failSound();

                    }

                    // un test pour relance la fonction
                    char restBlock;
                    printf("\nvoulez vous savoire le rest d'un autre bloc y/n ?" CYAN);
                    scanf(" %c", &restBlock); printf(WHITE);
                    enterSound();
                    if (restBlock == 'y' || restBlock == 'Y') goto RestDeBloc;

                }
                else { printf(RED"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }
            case 13: {
                system("cls");

                if (selection) {

                SuprimLogique:
                    // lire la matricule
                    printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                    printf("\nEntrez la matricule de letudiant a supprime (logiquement)  : " CYAN);
                    scanf(" %d", &matricule); printf(WHITE);

                    // test de la fonction supprimeLogique()
                    int testsupp = supprimeLogique(tnOVnC, matricule, &trouv, &corbeille);

                    if (trouv && !corbeille && testsupp == 0) { printf(GREEN"\nLa suppression a ete bien effectue !" WHITE);   sauvgardeFichier(tnOVnC);  successSound(); }
                    else {
                        if (trouv && corbeille && testsupp == -1) {

                            printf(YELLOW"\nL'etudiant est deja supprime " WHITE", voulez vous le restorez y/n?" CYAN);

                            // test si l'utilisateur veut restore l'etudinat car il existe deja
                            char restore;
                            scanf(" %c", &restore); printf(WHITE);
                            if (restore == 'y' || restore == 'Y') restoreUnEtudiant(tnOVnC, matricule);
                            sauvgardeFichier(tnOVnC);       // sauvgarde dans le disque dure
                            successSound();
                        }
                        else {
                            printf(ORANGE"\nL'etudiant n'existe pas !" WHITE);
                            failSound();
                        }
                    }

                    // test si l'utilisateur veut supprime un autre etudiant
                    char suppLogique;
                    printf("\nVoulez vous supprimez un autre etudiant y/n ?" CYAN);
                    scanf(" %c", &suppLogique);  printf(WHITE);
                    enterSound();
                    if (suppLogique == 'y' || suppLogique == 'Y') goto SuprimLogique;

                }
                else { printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }

            case 14: {
                system("cls");

                int testSuppPhysique = -1;
                if (selection) {

                SuprimPhysique:

                    // lire la matricule
                    printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                    printf("\nEntrez la matricule de l'etudiant a supprime completement (physiquement) : " CYAN);
                    scanf(" %d", &matricule);

                    // test si il veur continue
                    printf(RED"\"VOUS POUVEZ PAS LE RESTORE APRES CETTE OPERATION !!" WHITE " , vous etes sure de continue y/n ?\" " CYAN);
                    char suppPhisique;
                    scanf(" %c", &suppPhisique); printf(WHITE);
                    if (suppPhisique == 'y' || suppPhisique == 'Y') { testSuppPhysique = supprimePhysique(tnOVnC, matricule); }

                    // test l'execution de la fonction
                    if (testSuppPhysique == 0) { printf(GREEN"\n\nLa suppression a ete bien effectue" WHITE); testSuppresionPhysique = true;  sauvgardeFichier(tnOVnC);  successSound(); }

                    else { printf(ORANGE"ERROR,La matricule n'existe pas !  " WHITE);    failSound(); }

                    // test si il veut supprime un autre etudiant
                    printf("\nVoulez vous supprimez un autre etudiant y/n ?" CYAN);
                    scanf(" %c", &suppPhisique);  printf(WHITE);
                    enterSound();
                    if (suppPhisique == 'y' || suppPhisique == 'Y') goto SuprimPhysique;

                }
                else { printf(ORANGE "\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }


            case 15: {
                system("cls");

                int testvideCorbeille = -1;
                if (selection) {


                    // test si l'utilisateur veut vraiment vide la corbeille
                    printf(RED"\n\" VOUS NE POUVEZ PAS LES RESTORES APRES CETTE OPERATION \" " WHITE", vous etes sur de continue y/n ? " CYAN);
                    char videCorbeille;
                    scanf(" %c", &videCorbeille); printf(WHITE);
                    if (videCorbeille == 'y' || videCorbeille == 'Y') testvideCorbeille = videLaCorbeille(tnOVnC);

                    // test l'execution de la corbeille
                    if (testvideCorbeille == 0) { printf(GREEN"\nLa corbeille a ete bien vide " WHITE); testSuppresionPhysique = true;  successSound(); }
                    else { printf(ORANGE"ERROR , dans la fonction 12 \" videLaCorbeille \"" WHITE);    failSound(); }

                    sauvgardeFichier(tnOVnC);
                }
                else { printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);     failSound();
                }

                testRevenireMenu(&contin);

                break; // Fin
            }

            case 17: {
                system("cls");

                Etudiant etudiant = { 0 };          // pour ecrire dasn le fichier l'etudiant apres modification
                Etudiant etudiant2 = { 0 };   // pour laffichage de letudiant a modifier

                if (selection)
                {

                    // lire matricule
                    printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                    printf("\nEntrez la matricule de l'etudiant a modifier (max 6 chifres): ");  printf(CYAN);
                    scanf(" %d", &etudiant.matricule);  printf(WHITE);

                    // test matricule si elle est valide
                    if (etudiant.matricule < 1000000 && etudiant.matricule >= 0) {


                        rechercheEtudiant(tnOVnC, etudiant.matricule, &trouv, &etudiant2, &numBloc, &debutEnregEtudiant, &corbeille);
                        if (trouv && !corbeille) {


                            // pour affiche letudiant avant la modification
                            printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                            printf(GRAY"\nLa matricule " YELLOW"%06d " GRAY"les information de l'etudiant a modifier : " YELLOW"%s " WHITE, etudiant2.matricule, etudiant2.etudiantData);

                            getchar();                                              // pour recevoire le \n dans le buffer  car (scanf envoye un \n a la fin execution et fgets termine la lecture...
                            printf("\n\nEntrez les informations de l'etudiant a modifier : " CYAN);           //... dit que elle trouve un \n , si pour sa que j ai utilise getchar pour qu'elle recevoi ce \n

                            // pour pouvoir ecrire avec l'espace vide blanc dans le tableau (avec scanf on peut pas)
                            fgets(etudiant.etudiantData, sizeof(etudiant.etudiantData), stdin);
                            printf(WHITE);

                            // fait l'inserion
                            remplireRestEtudiantDefault(&etudiant);   // remplire le rest des donne pour l'insere " la taille , bit supression,.. "
                            int test = modifierEtudiant(tnOVnC, etudiant, numBloc, debutEnregEtudiant, etudiant2);
                            if (test == 0) {
                                sauvgardeFichier(tnOVnC);
                                successSound();
                                printf(GRAY"\n+-------------------------------------------------------------------------------------------+" WHITE);
                                printf(GREEN"\nLa suppression a ete bien effectue !" WHITE);
                            }
                            else
                            {
                                printf(ORANGE"\nERROR, dans la fonction modifierEtudiant()" WHITE);
                            }


                        }
                        else
                        {

                                failSound();
                                printf(ORANGE"\nL'etudiant est dans la corbeille vous devez le restorer dabord" WHITE);

                        }


                    }
                    else {
                        printf(ORANGE"\nERROR , la taille de la matricule est inacceptable" WHITE);    failSound();
                    }
                }
                else {
                    printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);  failSound();
                }

                testRevenireMenu(&contin);

                break; }

            case 18: {
                system("cls");

                int testRestoere = -1;
                if (selection) {

                    // lire la matricule
                    printf("\nEntrez la matricule de l'etudiant a restore : " CYAN);
                    scanf(" %d", &matricule); printf(WHITE);

                    // test l'execution de la fonction
                    testRestoere = restoreUnEtudiant(tnOVnC, matricule);
                    if (testRestoere == 0) { printf(GREEN"\nLa restoration de letudiant avec la matriculle " YELLOW"%06d " GREEN"a ete bien effectue " WHITE, matricule); successSound(); }
                    else { printf(ORANGE"\nERROR, La matriculle n'existe pas" WHITE);    failSound(); }

                    sauvgardeFichier(tnOVnC);
                }
                else { printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);    failSound();
                }

                testRevenireMenu(&contin);

                break;
            }  // Fin


            case 19: {
                system("cls");

                if (selection) {

                    // test l'exicution de la fonction
                    int test = restoreToutLesEtudiantDansLaCorbeille(tnOVnC);
                    if (test == 0) { printf(GREEN"\nLa restoration a ete effectuer " WHITE); successSound(); }
                    else { printf(ORANGE"\nERROR, dans la fonction  restoreToutLesEtudiantDansLaCorbeille() " WHITE);  failSound(); }

                    sauvgardeFichier(tnOVnC);
                }
                else { printf(ORANGE"\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);  failSound();
                }

                testRevenireMenu(&contin);

                break;
            }

            case 20: {
                system("cls");

                char nomFichierOrdonne[30];
                bool defaultName = true;
                int testOrdonne = -1;

                if (selection) {

                    // affichage, lire si il veut change le nom par defaut
                    printf(GRAY"\n+-----------------------------------------------------------------------------------------------------------------------------+\n" WHITE);
                    printf(GRAY"Le fichier a creer va avoir le nom " BLUE"%s_Ordonne " GRAY"par defaut,"  WHITE" voulez vous le changez y/n ? " CYAN, nomFichierSelectionne);
                    char changeNom;
                    scanf(" %c", &changeNom); printf(WHITE);

                    // test le choix
                    if (changeNom == 'y' || changeNom == 'Y') {
                        printf("\nEntrer le nom de fichier : " CYAN);
                        scanf(" %29s", nomFichierOrdonne); printf(WHITE);
                        defaultName = false;
                        testOrdonne = ordonner(tnOVnC, nomFichierOrdonne, defaultName);  //on execute la fonction avec  defaultName = non
                    }
                    else
                    {
                        testOrdonne = ordonner(tnOVnC, nomFichierSelectionne, defaultName); // on execute la fonction par defaut
                    }

                    // affichage l'etat d'execution
                    if (testOrdonne == 0) { printf(GREEN"\nLa creation de fichier ordonne a ete bien effectue !" WHITE); successSound(); }
                    else {
                        printf(ORANGE"\nERROR, la fonction n'est pas execute !!" WHITE);
                        failSound();
                    }

                }
                else { printf(ORANGE"\n\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);  failSound();  }

                testRevenireMenu(&contin);

                break;
            }


            case 22: {

                if (selection)
                {
                    // si il ya une suppression physique (resp vide la corbeille) on reoganise le fichier
                    if (testSuppresionPhysique) reorganisation(tnOVnC);
                    closeMainFile(tnOVnC);
                    testSuppresionPhysique= true;
                    successSound();
                }
                else {
                    printf(ORANGE"\n\nERROR , choizez ou bien selectionne un fichier dans la fonction 0" WHITE);  failSound();  Sleep(500);

                }

                // on deselictionne et revient au debut
                selection = false;
                goto Start;
                break; }

            case 23: {

                // alle a la fin
                goto End;
                break;
            }

            default: printf(ORANGE"\nERROR, la fonction n'existe pas !" WHITE" Press any key to restart"); failSound();
                Sleep(500);

                goto Start;
                break;
            }
        }


    }

End:

    if (selection) {
        if (testSuppresionPhysique) reorganisation(tnOVnC);  // si il ya une suppression physique (resp vide la corbeille) on reoganise le fichier
        // ferme le fichier selectionne
        closeMainFile(tnOVnC);
    }

    system("cls");
    affichageFin();

    fAQ();

}

