add_library(game game.c game_io.c game_rand.c game_sdl.c)

file(COPY Arial.ttf DESTINATION ..)

add_subdirectory(assets)
