cmake_minimum_required(VERSION 3.0)

project(netput-sdl-exampe)

add_subdirectory(netput)
option(SDL_STATIC "" ON)
option(SDL_STATIC_PIC "" ON)
add_subdirectory(SDL)
add_subdirectory(sigfn)

add_executable(client client.cpp)
add_executable(server server.cpp)

target_link_libraries(client SDL3-static sigfn++_a netput)
target_link_libraries(server sigfn++_a netput)