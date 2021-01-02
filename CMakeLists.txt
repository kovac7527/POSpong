cmake_minimum_required(VERSION 3.17)
project(Pong)

set(CMAKE_CXX_STANDARD 11)

add_executable(Pong main.cpp SourceFiles/Ball.cpp HeaderFiles/Ball.h SourceFiles/ColisionDetector.cpp HeaderFiles/ColisionDetector.h SourceFiles/Player.cpp HeaderFiles/Player.h SourceFiles/Menu.cpp HeaderFiles/Menu.h SourceFiles/GameManager.cpp HeaderFiles/GameManager.h)

target_link_libraries(Pong sfml-system sfml-window sfml-graphics sfml-network sfml-audio pthread)