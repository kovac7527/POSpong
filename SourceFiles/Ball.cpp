//
// Created by jan on 31. 12. 2020.
//

#include "../HeaderFiles/Ball.h"

#include <cstdlib>

Ball::Ball(int posX, int posY)
{
    origX = posX;
    origY = posY;
    direction = STOP;
    object = sf::CircleShape(40);
    object.setFillColor(sf::Color::Green);
    object.setOrigin((object.getRadius()), (object.getRadius()));
    object.setPosition(sf::Vector2f(origY, origX));
}

void Ball::Reset()
{
    object.setPosition(sf::Vector2f(origY, origX));
    direction = STOP;
    BallSpeed = defaultBallSpeed;

}

void Ball::changeDirection(ballDirection dir)
{
    direction = dir;
}

void Ball::setRandomDireciton()
{
    direction = (ballDirection) ((rand() % 6)+1);
}

void Ball::move()
{
    switch (direction)
    {
        case STOP:
            break;
        case LEFT:
            object.move(sf::Vector2f(-1.0f * BallSpeed , 0.0f * BallSpeed));
            break;
        case UPLEFT:
            object.move(sf::Vector2f(-1.0f * BallSpeed , -1.0f * BallSpeed));
            break;
        case DOWNLEFT:
            object.move(sf::Vector2f(-1.0f * BallSpeed, 1.0f * BallSpeed));
            break;
        case RIGHT:
            object.move(sf::Vector2f(0.0f * BallSpeed, 1.0f * BallSpeed));
            break;
        case UPRIGHT:
            object.move(sf::Vector2f(1.0f * BallSpeed, -1.0f * BallSpeed));
            break;
        case DOWNRIGHT:
            object.move(sf::Vector2f(1.0f * BallSpeed, 1.0f * BallSpeed));
            break;

    }
}

void Ball::setSpeed(float speed)
{
    BallSpeed = speed;
}

void Ball::setDefaultSpeed(float speed)
{
    defaultBallSpeed = speed;
}

int Ball::getBallDireciton() const
{
    return (int)direction;
}

int Ball::getX()
{
    return object.getPosition().x;
}

int Ball::getY()
{
    return object.getPosition().y;
}

Ball::~Ball() {

}

void Ball::draw(sf::RenderWindow* window)  {
    window->draw(object);
}

void Ball::setRadius(float radius) {
    this->object.setRadius(radius);
    object.setOrigin((object.getRadius()), (object.getRadius()));
}
