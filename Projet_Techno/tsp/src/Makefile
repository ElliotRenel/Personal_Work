VPATH = ../include

LPFLAGS = -lm

CPPFLAGS = -I ../include

OBJECTS = memoire.o matrice2d.o tsp.o

all : solve-TSP convert-2D-to-full

solve-TSP : solve-TSP.o $(OBJECTS)
	$(CC) -o $@ $(LPFLAGS) $< $(OBJECTS)

convert-2D-to-full : convert-2D-to-full.o $(OBJECTS)
	$(CC) -o $@ $(LPFLAGS) $< $(OBJECTS)

solve-TSP.o : solve-TSP.c $(OBJECTS)
	$(CC) -c $< $(CPPFLAGS)

convert-2D-to-full.o : convert-2D-to-full.c $(OBJECTS)
	$(CC) -c $< $(CPPFLAGS)

memoire.o : memoire.c memoire.h
	$(CC) -c $< $(CPPFLAGS)
matrice2d.o : matrice2d.c memoire.h
	$(CC) -c $< $(CPPFLAGS)
tsp.o : tsp.c tsp.h
	$(CC) -c $< $(CPPFLAGS)


.PHONY : clean

clean :
	rm *.o solve-TSP convert-2D-to-full
