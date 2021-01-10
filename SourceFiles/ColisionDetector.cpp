//
// Created by jan on 31. 12. 2020.
//


#include "../HeaderFiles/ColisionDetector.h"



ColisionDetector::ColisionDetector(Player* p1, Player* p2, Ball* b, uint w, uint h)
{
    height = h;
    player1 = p1;
    player2 = p2;
    ball = b;
}

ballColison ColisionDetector::CheckForBallColision()
{

    if (ball->object.getGlobalBounds().intersects( player1->object.getGlobalBounds())) {
        return ballColison::ColisionPlayer1;
    }


    if (ball->object.getGlobalBounds().intersects( player2->object.getGlobalBounds())) {
        return ballColison::ColisionPlayer2;
    }



    if (ball->getY() - ball->object.getRadius() <= 0) {
        return (ballColison)3;
    }

    if (ball->getY() >= (height))  {
        return (ballColison)4;
    }

    if (ball->getX() < player1->getX() + player1->object.getSize().x/2) {

        return (ballColison)5;

    }

    if (ball->getX() > player2->getX()-player2->object.getSize().x/2) {

       return (ballColison)6;

    }







    return (ballColison)0;

}

ColisionDetector::~ColisionDetector()
{

}
