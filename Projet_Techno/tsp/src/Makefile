VPATH = C:\Users\Akuma\Documents\Programs\GitHub_Work\Projet_Technologique\tsp\include

INCLUDE = -I C:\Users\Akuma\Documents\Programs\GitHub_Work\Projet_Technologique\tsp\include

OBJECTS = memoire.o matrice2d.o tsp.o

make all : $(wildcard *.o) solve-TSP convert-2D-to-full

memoire.o : memoire.c memoire.h
	gcc -c $< $(INCLUDE)
matrice2d.o : matrice2d.c memoire.h
	gcc -c $< $(INCLUDE)
tsp.o : tsp.c tsp.h 
	gcc -c $< $(INCLUDE)
solve-TSP.o : solve-TSP.c $(OBJECTS)
	gcc -c $< $(INCLUDE)
solve-TSP : solve-TSP.o $(OBJECTS)
	gcc -o $@ $< $(OBJECTS) $(INCLUDE)
	
convert-2D-to-full.o : convert-2D-to-full.c $(OBJECTS)
	gcc -c $< $(INCLUDE) 
convert-2D-to-full : convert-2D-to-full.o $(OBJECTS)
	gcc -o $@ $< $(OBJECTS) $(INCLUDE)

clean_o :
	del *.o
clean_all : 
	del *.o *.exe

