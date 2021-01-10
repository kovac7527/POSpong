//
// Created by jan on 31. 12. 2020.
//

#include "../HeaderFiles/Player.h"





void Player::setColor(sf::Color color)
{
    object.setFillColor(color);
}

void Player::setSize(float x,  float y)
{
    object.setSize(sf::Vector2f(x, y));
    object.setOrigin((object.getSize()).x / 2, (object.getSize()).y / 2);
}

void Player::Reset()
{
    object.setPosition(sf::Vector2f( origX,origY));
}

int Player::getX()
{
    return object.getPosition().x;
}

int Player::getY()
{
    return object.getPosition().y;
}

void Player::moveUp()
{
    object.move(0.0f, -1.0f);
}

void Player::moveDown()
{
    object.move(0.0f, +1.0f);
}

void Player::draw(sf::RenderWindow* window) {
    window->draw(object);
}
