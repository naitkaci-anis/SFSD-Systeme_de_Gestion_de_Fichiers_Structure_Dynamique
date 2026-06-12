# SFSD — Système de Gestion de Fichiers à Structure Dynamique

Projet universitaire — Structures de Fichiers et Systèmes de Données
**Département Informatique · UMMTO**

---

## Description

Application console Windows pour gérer des enregistrements d'étudiants dans un fichier binaire de type **TnOVnC** — un **Tableau Non Ordonné de taille Variable et sans Chevauchement**.

Concrètement, ce sigle décrit la façon dont les données sont organisées dans le fichier :

- **Non Ordonné** — les étudiants sont stockés dans l'ordre où ils sont insérés, pas triés par matricule. Une recherche doit donc parcourir le fichier (sauf après utilisation de l'option « Ordonner »).
- **taille Variable** — chaque enregistrement occupe exactement la place dont il a besoin, au lieu d'une taille fixe identique pour tous. Un étudiant avec beaucoup d'informations prend plus de place qu'un autre.
- **sans Chevauchement** — un même enregistrement ne peut pas être coupé en deux entre la fin d'un bloc et le début du suivant. Chaque enregistrement tient entièrement dans un seul bloc ; s'il ne rentre pas dans le bloc courant, il est écrit dans un nouveau bloc.

Le principe : au lieu de stocker les étudiants dans des cases de taille fixe (ce qui gaspille de l'espace), chaque enregistrement occupe exactement la place dont il a besoin. Le fichier est découpé en **blocs de 1000 octets**, et les enregistrements de taille variable sont écrits les uns à la suite des autres à l'intérieur de ces blocs. Quand un bloc est plein, un nouveau bloc est créé automatiquement.

Caractéristiques principales :

- **Blocs de 1000 octets** — unité de lecture/écriture sur le disque.
- **Enregistrements de taille variable** — chaque étudiant occupe juste la place nécessaire.
- **Suppression logique (corbeille)** — l'étudiant n'est pas effacé, juste marqué ; on peut le restaurer.
- **Suppression physique** — l'étudiant est définitivement retiré et l'espace est récupéré.
- **Navigation au clavier** — menu interactif avec les flèches ↑ / ↓ et Entrée.

---

## À quoi sert chaque partie

### La structure TnOVnC

TnOVnC (*Tableau Non Ordonné de taille Variable et sans Chevauchement*) est le cœur du projet. En mémoire, c'est la structure qui représente le fichier ouvert. Elle contient :

- un **pointeur vers le fichier** binaire ouvert sur le disque ;
- un **en-tête (Header)** chargé en mémoire qui résume l'état du fichier.

L'en-tête contient trois informations mises à jour à chaque opération :

| Champ | Rôle |
|-------|------|
| `adrLastBloc` | nombre de blocs actuellement utilisés dans le fichier |
| `nbrEnreg` | nombre d'étudiants actifs (non supprimés) |
| `nbrSuprLogiq` | nombre d'étudiants dans la corbeille (supprimés logiquement) |

Grâce à cet en-tête, le programme connaît l'état du fichier sans avoir à le parcourir entièrement à chaque fois.

### Comment un étudiant est stocké

Chaque enregistrement écrit dans un bloc est composé d'un petit en-tête de 10 octets suivi des données :

| Partie | Taille | Rôle |
|--------|--------|------|
| `taille` | 3 octets | taille totale de l'enregistrement (en-tête + données) |
| `matricule` | 6 octets | identifiant unique de l'étudiant |
| `efface` | 1 octet | bit d'effacement logique : `0` = actif, `1` = dans la corbeille |
| `données` | variable | informations de l'étudiant (nom, date de naissance, section, groupe…) |

Le champ `taille` permet, lors de la lecture, de savoir où commence l'enregistrement suivant : il suffit d'avancer de `taille` octets. C'est ce qui rend la structure dynamique possible sans séparateur entre les enregistrements.

### La suppression logique vs physique

La **suppression logique** se contente de mettre le bit `efface` à `1` : l'étudiant reste physiquement dans le bloc mais n'apparaît plus dans les listes normales. Il est consultable dans la corbeille et peut être restauré. C'est rapide et réversible.

La **suppression physique** retire réellement l'enregistrement du bloc et décale les enregistrements suivants pour combler le trou (compactage). L'espace est récupéré, mais l'opération est définitive.

### La réorganisation

Après des suppressions physiques, des blocs peuvent se retrouver à moitié vides. La fonction de réorganisation regroupe les enregistrements pour libérer les blocs devenus inutiles et réduire le nombre total de blocs, afin de gagner de l'espace disque.

---

## Comment l'exécuter

> Appuyez sur **F11** pour mettre le terminal en plein écran avant de lancer — nécessaire pour bien voir l'interface (logo, menu et bordures).

### Option 1 — Double-clic (recommandé)

Double-cliquer sur **`lancer.bat`**. Le script se place automatiquement dans le bon dossier, compile le projet, puis lance le programme. Si la compilation échoue, l'erreur est écrite dans la fenêtre (et le détail dans `erreurs.txt`) au lieu de planter silencieusement.

### Option 2 — VS Code

- **`Ctrl+Shift+B`** : compiler et lancer
- **`F5`** : lancer en mode debug

### Option 3 — Terminal

```bash
gcc main.c fonction.c -o sfsd.exe -std=c99
sfsd.exe
```

**Prérequis :** GCC MinGW installé (via Dev-C++ ou MSYS2).
Chemin attendu par le `.bat` : `C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe`

---

## Fonctionnalités

| Option | Fonction |
|--------|----------|
| 0 | Sélectionner un fichier existant |
| 1 | Créer un nouveau fichier |
| 2 | Insérer un étudiant |
| 3 | Rechercher par matricule |
| 4 | Lire un bloc |
| 5 | Lire tout le fichier |
| 6 | Afficher la corbeille |
| 7 | Nombre total d'étudiants |
| 8 | Nombre dans la corbeille |
| 9 | Espace restant dans un bloc |
| 10 | Suppression logique |
| 11 | Suppression physique |
| 12 | Vider la corbeille |
| 13 | Modifier un étudiant |
| 14 | Restaurer un étudiant |
| 15 | Restaurer tous |
| 16 | Ordonner le fichier |
| 17 | Désélectionner |
| 18 | Quitter |

### Navigation dans le menu

| Touche | Action |
|--------|--------|
| `↑` / `↓` | Naviguer entre les options |
| `Entrée` | Exécuter l'option sélectionnée |

---

## Structure du projet

```
sfsd/
├── main.c        — point d'entree, menu, navigation clavier
├── fonction.c    — toutes les fonctions (gestion fichier, blocs, etudiants, affichage)
├── header.h      — types, constantes, prototypes
├── lancer.bat    — compilation + lancement
└── .vscode/      — configuration VS Code (compilation, debug, IntelliSense)
```

Répartition du code :

- **`header.h`** — déclare les types (`TnOVnC`, `Header`, `TBloc`, `Etudiant`, `Container`), les constantes (taille de bloc, codes couleur ANSI) et les prototypes de toutes les fonctions.
- **`fonction.c`** — contient l'implémentation : ouverture/fermeture du fichier, lecture/écriture des blocs, insertion, recherche, suppression, restauration, tri, réorganisation, ainsi que tout l'affichage (menu, logo, sons).
- **`main.c`** — gère la boucle principale, la navigation au clavier dans le menu et l'appel des fonctions selon l'option choisie.

---

## Notes techniques

- **Standard C : C99** (notamment pour le tableau de taille variable utilisé lors du tri).
- **Plateforme : Windows uniquement** — utilise `windows.h` (sons, console), `conio.h` (lecture clavier `_getch`) et `io.h` (écriture bas niveau sur disque).
- **Taille d'un bloc : 1000 octets**, configurable via `TAILLE_DE_BLOC` dans `header.h`.
- **Matricule : 6 chiffres maximum.**
- Le fichier **`Default_File`** est recréé automatiquement à chaque lancement avec un jeu d'étudiants de démonstration.
- L'affichage utilise des **codes couleur ANSI** et l'**UTF-8** (`chcp 65001`) pour les bordures et le logo.

---

## Réalisé par :
| NAIT KACI Anis |
| Fellag Aghiles |
| Hamrani Mouhand-Idir |

*Projet académique — Département Informatique · UMMTO* 
