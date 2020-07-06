project(game)

set(SOURCES
        game.cpp)
set(HEADERS
        game.h
        )

include_directories(../Homework_4)

add_library(game ${SOURCES} ${HEADERS})
