file(GLOB game_pic
    "*.jpg"
)

foreach(loop_game ${game_pic})

    file(COPY ${loop_game} DESTINATION ../..)
    
endforeach(loop_game)

file(COPY bgr_sound.mp3 DESTINATION ../..)