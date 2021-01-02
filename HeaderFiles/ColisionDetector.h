//
// Created by jan on 31. 12. 2020.
//

#ifndef PONG_COLISIONDETECTOR_H
#define PONG_COLISIONDETECTOR_H


#pragma once
#include "Player.h"
#include "Ball.h"

enum ballColison
{
    noColision = 0,
    ColisionPlayer1 = 1,
    ColisionPlayer2 = 2,
    ColisionUpperCorner = 3,
    ColisionDownCorner = 4,
    ColisionLeftSide = 5,
    ColisionRightSide = 6,
};

class ColisionDetector
{
private :
    uint width;
    uint height;
    Player* player1;
    Player* player2;
    Ball* ball;
public:
    ColisionDetector(Player* player1, Player* player2, Ball* ball, uint w, uint  h);
    ballColison CheckForBallColision();
    ~ColisionDetector();


};



#endif //PONG_COLISIONDETECTOR_H
