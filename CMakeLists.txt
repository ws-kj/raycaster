cmake_minimum_required(VERSION 3.5)

project(raycaster LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    *.h
    *.cpp)

add_executable(raycaster
    ${SOURCES}
)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
target_link_libraries(raycaster sfml-graphics sfml-audio)

target_include_directories(raycaster PUBLIC ${CMAKE_INCLUDE_CURRENT_DIR})
target_include_directories(raycaster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
