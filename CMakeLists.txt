cmake_minimum_required(VERSION 3.12)

project(console_game)

set(CMAKE_CXX_FLAGS "")
set(CMAKE_EXPORT_COMPILE_COMMANDS "1")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB SOURCES (
    ./*.cpp
))

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME}
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
)
