@echo off

gcc -c memoire.c
gcc -c matrice2d.c 
gcc -c tsp.c 
gcc -c solve-TSP.c 
gcc -o solve-TSP.exe solve-TSP.o memoire.o matrice2d.o tsp.o 
gcc -c convert-2D-to-full.c 
gcc -o convert-2D-to-full.exe convert-2D-to-full.o memoire.o matrice2d.o tsp.o 

.\solve-TSP.exe 
.\convert-2D-to-full.exe

@pause
