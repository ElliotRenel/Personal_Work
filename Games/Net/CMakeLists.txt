cmake_minimum_required(VERSION 3.5)
project(Net_V2)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

set(tests_args 
    empty
    empty_ext
    new
    new_ext
    copy_game
    delete_game
    restart_game
    set_piece
    rotate_one
    rotate_piece
    set_piece_current_dir
    shuffle
    game_height
    game_width
    get_piece
    get_current_dir
    opposite_dir
    wrapping
    edge
    edge_coordinate
    over
    load
    save
)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

add_subdirectory(game_files)

file(COPY game_test_files DESTINATION .)

add_executable(net_text net_text.c)

target_link_libraries(net_text game)

add_executable(divers divers.c)

target_link_libraries(divers game)

add_executable(net_solve net_solve.c)

target_link_libraries(net_solve game)

# SDL

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMG REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} ${SDL2_IMG_LIBRARIES} ${SDL2_TTF_LIBRARIES})
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS} ${SDL2_IMG_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

include_directories(${SDL2_ALL_INC})
add_executable(net_sdl net_sdl.c)
target_link_libraries(net_sdl ${SDL2_ALL_LIBS} m game)
target_link_libraries(net_sdl SDL2_mixer)

# TEST
add_executable(tests tests.c)

target_link_libraries(tests game)

enable_testing()

add_test(Tests_executable_build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target tests)

foreach(loop_var ${tests_args})
    add_test(
        NAME ${loop_var}
        COMMAND tests ${loop_var}
    )
endforeach(loop_var)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS tests)