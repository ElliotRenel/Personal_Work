Welcome to groupe TMA-34G's Net project by DUPONT Theo, PISSOT Frederic, RENEL Elliot, and LAFF Valerian.


Requirement :
    - Linux OS
    - Cmake version 3.5 or higher
    - C/C++ compiler
    - A brain

Compiling the files :

    0 - create folder for cmake compilation (not mandatory but advised)-> "mkdir build && cd build"
    1 - execute cmake command -> "cmake .."
    2 - execute makefile -> "make"

Playing net :

    0 - follow compilation steps.
    1a- execute game file -> "./net_text <game>"
        └> if no argument <game> is passed, the default game will be used (default.txt within game_files).
        |
        └> you can pass any game's name <game> that are within the game_files folder (or that you put within your build folder).
           Note : <game> must be the full name of the file (with extension).
    1b- generating random game -> "./net_text <width> <height> [S|N] [3|4]"
        └> <width> <height> : int value for the size of the game
        └> S = is swap / N = no swap (default)
        └> 3 = CROSS not allowed / 4 CROSS allowed (default)
    2 - enter the coordinates in <x y> format then press enter.
    3 - the program will stop once you finish the game or press CRTL+C.
    
    !!! WARNING !!! : coordinates starts at 0 0 at the bottom left corner.

Testing game.h functions :

    0 - follow compilation steps (at list to Step 2).
    1 - run test rule of makefile -> "make test".
  
    Note : shuffle's test has a very small chance of failing due to the test being probabilistic, 
    if so, do "make test" once more and it should be good, if it still fails, then shuffle is not valid.

Using solver :

    0 - follow compilation steps.
    1 - execute solver file -> "./net_solve FIND_ONE|NB_SOL|FIND_ALL <game_file_name> <sol_file_radical>".
        └> Arguments : 1 - the task asked (see below for more info) / 2 - the game file to be solved / 3 - the radical of the upcoming saved file(s).
        |
        └>  FIND_ONE : finding one solution (if it exist) of the game stored in the file <game_file_name>. 
        |   If a solution exist, it will be stored in the file <sol_file_radical>.sol. If no solution exist,
        |   the file <sol_file_radical>.sol will only contain the message "NO SOLUTION". 
        |
        └>  NB_SOL : saving the number of solution in <sol_file_radical>.nbsol as "NB_SOL = <nb_sol>", 
        |   where <nb_sol> is the number of solution of the game passed as argument.
        |
        └>  FIND_ALL : saving in files <sol_file_radical>.sol1, <sol_file_radical>.sol2, ...<sol_file_radical>.solN 
            the list of solution of the game passed as argument.
    2 - the new file(s) is now saved inside the build file (or whatever file you created to execute "cmake ..").

    Note : <game_file_name> can contain the full path to a game (exemple : ../game_files/default.txt), but <sol_file_radical> cannot.
    !!! WARNING !!! : the path given in <game_file_name> will be seen from within the build folder, don't forget to add "../" if you want something from elsewhere.
    All games within the folder "game_files" are accessible within the build folder and don't require any adition to their path. The folder "game_test_files" is also available from within the build folder as long as you add "./game_test_file/" before the game name.


Random info :

    - Without any surprise the "src" folder contains the source code of the game.
    - The "game_test_files" folder is there to be left alone or the test command will be screwed over.
    - divers.c is only there to make personnal test, feel free to play with it if you want, cmake contains a rule to compile it so just use make to compile it.
