file(GLOB game_list
    "*.txt"
)

foreach(loop_game ${game_list})

    file(COPY ${loop_game} DESTINATION ..)
    
endforeach(loop_game)