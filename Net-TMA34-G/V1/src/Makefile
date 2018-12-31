CC = gcc
VPATH = ../include
CFLAGS = -Wall -std=c99 -g
CPPFLAGS = -I ../include
LDFLAGS = -lgame -L.
OBJECTS = game.o game_io.o
FW_OBJECTS = testfw_thdupont.o testfw_erenel.o testfw_fpissot.o
LIBRARIES = -ltestfw_main -ltestfw -L../lib -lgame -L.
ERENEL = ./test_erenel
FPISSOT = ./test_fpissot
THDUPONT = ./test_thdupont

all : net_text test_erenel test_fpissot test_thdupont

################ LINKING ################

test_erenel : test_erenel.o libgame.a
	$(CC) $< $(LDFLAGS) -o $@

test_fpissot : test_fpissot.o libgame.a
	$(CC) $< $(LDFLAGS) -o $@

test_thdupont : test_thdupont.o libgame.a
	$(CC) $< $(LDFLAGS) -o $@

net_text : net_text.o libgame.a
	$(CC) $< $(LDFLAGS) -o $@

################ PRECOMPILING ################

test_erenel.o : test_erenel.c game.h tools.h

test_fpissot.o : test_fpissot.c game.h tools.h

test_thdupont.o : test_thdupont.c game.h tools.h

net_text.o : net_text.c game.h game_io.h

game.o : game.c game.h

game_io.o : game_io.c game.h game_io.h

################ FRAMEWORK TEST ################

testfw_all : testfw_thdupont.o testfw_erenel.o testfw_fpissot.o libgame.a
	$(CC) -rdynamic $(FW_OBJECTS) $(LIBRARIES) -ldl -o $@
	./testfw_all

testfw_thdupont.o : test_thdupont.c game.h tools.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -DTESTFW $< -o $@

testfw_erenel.o : test_erenel.c game.h tools.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -DTESTFW $< -o $@

testfw_fpissot.o : test_fpissot.c game.h tools.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -DTESTFW $< -o $@

################ LIBRARY AND OTHERS ################

libgame.a : $(OBJECTS)
	ar rcs $@ $(OBJECTS)

divers : divers.o libgame.a
	$(CC) $< $(LDFLAGS) -o $@

divers.o : divers.c game.h

################ TEST PROGRAMS ################

test : run_test_erenel run_test_fpissot run_test_thdupont

run_test_erenel : test_erenel
	$(ERENEL) new
	$(ERENEL) set_piece_current_dir
	$(ERENEL) shuffle
	$(ERENEL) edge
	$(ERENEL) edge_coordinate
	$(ERENEL) over


run_test_fpissot : test_fpissot
	$(FPISSOT) set_piece
	$(FPISSOT) game_width
	$(FPISSOT) rotate_one
	$(FPISSOT) rotate_piece
	$(FPISSOT) get_piece
	$(FPISSOT) restart_game

run_test_thdupont : test_thdupont
	$(THDUPONT) empty
	$(THDUPONT) opposite_dir
	$(THDUPONT) copy_game
	$(THDUPONT) game_height
	$(THDUPONT) get_current_dir
	$(THDUPONT) delete_game

################ CLEAN RULE ################

.PHONY: clean

clean :
	rm -f libgame.a divers divers.o net_text net_text.o test_erenel test_erenel.o test_fpissot test_fpissot.o test_thdupont test_thdupont.o divers divers.o testfw_thdupont.o testfw_fpissot.o testfw_erenel.o testfw_all game.o game_io.o
clean_w :
	del /F libgame.a net_text.exe net_text.o divers divers.o test_erenel.exe test_erenel.o test_fpissot.exe test_fpissot.o test_thdupont.exe test_thdupont.o divers.exe divers.o testfw_thdupont.o testfw_fpissot.o testfw_erenel.o testfw_all.exe game.o game_io.o
