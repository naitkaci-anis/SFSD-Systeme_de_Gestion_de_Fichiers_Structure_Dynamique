# SFSD — Système de Gestion de Fichiers à Structure Dynamique

Projet universitaire — Structures de Fichiers et Systèmes de Données
**Département Informatique · UMMTO**

## Réalisé par

| # | Nom |
|---|-----|
| 1 | NAIT KACI Anis |
| 2 | Fellag Aghiles |
| 3 | Hamrani Mouhand Idir |

---

## Description

Application console Windows pour gérer des enregistrements d'étudiants dans un fichier binaire à structure dynamique non ordonnée (NVO-NC).

- Blocs de **1000 octets**
- Enregistrements de **taille variable**
- Suppression logique (corbeille) et physique
- Navigation au clavier (↑ / ↓ / Entrée)

---

## Comment l'exécuter

> **Appuyez sur F11 pour mettre le terminal en plein écran avant de lancer — nécessaire pour bien voir l'interface.**

### Option 1 — Double-clic (recommandé)

Double-cliquer sur `lancer.bat`.
Le script compile automatiquement et lance le programme.
Si la compilation échoue, l'erreur s'affiche dans la fenêtre.

### Option 2 — VS Code

- **`Ctrl+Shift+B`** — compiler et lancer
- **`F5`** — lancer en mode debug

### Option 3 — Terminal

```bash
gcc main.c fonction.c -o sfsd.exe -std=c99
sfsd.exe
```

> **Prérequis :** GCC MinGW installé (Dev-C++ ou MSYS2).
> Chemin attendu par le `.bat` : `C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe`

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

---

## Structure du projet

```
sfsd/
├── main.c        — point d'entrée, menu, navigation clavier
├── fonction.c    — toutes les fonctions
├── header.h      — types, constantes, prototypes
├── lancer.bat    — compilation + lancement
└── .vscode/      — configuration VS Code
```

---

## Notes

- Standard C : **C99** — Windows uniquement (`windows.h`, `conio.h`, `io.h`)
- Taille d'un bloc : **1000 octets** (`TAILLE_DE_BLOC` dans `header.h`)
- Matricule : 6 chiffres maximum
- Le fichier `Default_File` est recréé automatiquement à chaque lancement
