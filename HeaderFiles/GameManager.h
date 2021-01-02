//
// Created by jan on 31. 12. 2020.
//

#ifndef PONG_GAMEMANAGER_H
#define PONG_GAMEMANAGER_H


#include "Player.h"
#include "Ball.h"
#include <iostream>
#include "ColisionDetector.h"
#include "Menu.h"

class GameManager
{
private:


public:
    bool windowFocus;
    bool playing;
    bool paused;
    uint width, height;
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

    [[noreturn]] void start();

    void initializeComponents(float playerSizeX, float playerSizeY, float ballRadius);
    void update();

    void draw();
    void setWindow(sf::RenderWindow* window);

    void resetGame();
    void resetPositions();




};

#endif //PONG_GAMEMANAGER_H
