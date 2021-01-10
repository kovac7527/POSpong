//
// Created by jan on 31. 12. 2020.
//

#ifndef PONG_BALL_H
#define PONG_BALL_H


#include <SFML/Graphics.hpp>
enum ballDirection
{
    STOP = 0,
    LEFT = 1,
    UPLEFT = 2,
    DOWNLEFT = 3,
    RIGHT = 4,
    UPRIGHT = 5,
    DOWNRIGHT = 6
};
class Ball
{
private :
    int origX, origY;
    float BallSpeed = 0.2;


public :
    float defaultBallSpeed = 0.2;
    ballDirection direction;
    sf::CircleShape object;
    Ball(int posX, int posY);
    void Reset();
    void changeDirection(ballDirection dir);
    void move();


    void draw(sf::RenderWindow* window) ;


    void setDefaultSpeed(float speed);
    void setRadius(float radius);

    int getBallDireciton() const;
    int getX();
    int getY();
    ~Ball();

};


#endif //PONG_BALL_H
