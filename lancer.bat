@echo off
title SFSD - Gestion des Etudiants  |  UMMTO 
color 0F
cd /d "%~dp0"

:: Compiler le projet
"C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe" main.c fonction.c -o sfsd.exe -std=c99 2>erreurs.txt
if %errorlevel% neq 0 (
    color 0C
    echo.
    echo  ERREUR DE COMPILATION :
    echo.
    type erreurs.txt
    echo.
    pause
    exit
)
del erreurs.txt 2>nul

:: Lancer le programme
sfsd.exe
