cmake_minimum_required(VERSION 3.0)

project(netput-sdl-exampe)

add_subdirectory(netput)
option(SDL_STATIC "" ON)
option(SDL_STATIC_PIC "" ON)
add_subdirectory(SDL)

