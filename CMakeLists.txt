cmake_minimum_required(VERSION 3.6)
project(merge)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    board.cpp
    board.h
    game.cpp
    game.h
    gui.cpp
    gui.h
    image.cpp
    image.h
    location.cpp
    location.h
    main.cpp
    picture.h)

add_executable(merge ${SOURCE_FILES})