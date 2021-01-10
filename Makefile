CC=g++
all : Server Klient

Server: main.o ball.o ColisionDetector.o GameManager.o Menu.o Player.o
        $(CC) main.o ball.o ColisionDetector.o GameManager.o Menu.o Player.o -o Server

Klient: main.o ball.o ColisionDetector.o GameManager.o Menu.o Player.o
        $(CC)  main.o ball.o ColisionDetector.o GameManager.o Menu.o Player.o -o Server

main.o : main.cpp
         $(CC) -c main.cpp

Ball.o : ball.cpp
         $(CC) -c Ball.cpp

ColisionDetector.o : ColisionDetector.cpp
                     $(CC) -c ColisionDetector.cpp

GameManager.o : GameManager.cpp
                $(CC) -c GameManager.cpp

Menu.o : Menu.cpp
          $(CC) -c Menu.cpp

Player.o : Player.cpp
            $(CC) -c Player.cpp