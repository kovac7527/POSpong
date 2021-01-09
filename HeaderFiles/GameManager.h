//
// Created by jan on 31. 12. 2020.
//

#ifndef PONG_GAMEMANAGER_H
#define PONG_GAMEMANAGER_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "Player.h"
#include "Ball.h"
#include <iostream>
#include "ColisionDetector.h"
#include "Menu.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class GameManager
{
private:


public:
    int* socketfd;
    int* newsockfd;
    bool imServer;
    bool imClient;
    bool windowFocus;
    bool playing;
    bool paused;
    uint width, height;
    pthread_mutex_t mutex;
    bool updatedPosPlayer1;
    bool updatedPosPlayer2;
    bool updatedPosBall;
    int scorePlayer1, scorePlayer2;
    Ball* ball;
    Player* player1 ;
    Player* player2 ;
    ColisionDetector* colisDetector;
    Menu* mainMenu;
    Menu* inGameMenu;


    sf::RenderWindow* window;

    GameManager( uint w,  uint h);
    void OpenWindow();


    //void scoreUp(Player * player);



    void initializeComponents(float playerSizeX, float playerSizeY, float ballRadius);
    void update();



    void draw();
    void setWindow(sf::RenderWindow* window);

    void resetGame();
    void resetPositions();

    char* createPositionMsg(char* msg, int x, int y);

    void startServer();


    void readFromCLient();
};

#endif //PONG_GAMEMANAGER_H
