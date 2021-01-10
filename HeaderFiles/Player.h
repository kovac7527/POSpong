//
// Created by jan on 31. 12. 2020.
//

#ifndef PONG_PLAYER_H
#define PONG_PLAYER_H


#include <SFML/Graphics.hpp>

class Player
{
private:

    int origX, origY;

public:
    sf::RectangleShape object;



    Player(float posX, float posY)  {

        origX = posX;
        origY = posY;


        object = sf::RectangleShape(sf::Vector2f(1.0f, 1.0f));
        object.setFillColor(sf::Color::Cyan);

        object.setPosition(sf::Vector2f(origX,origY));




    };

    void setColor(sf::Color color);
    void setSize(float x, float y);
    void Reset();
    int getX();
    int getY();
    void moveUp();
    void moveDown();
    void draw(sf::RenderWindow* window);


};

#endif //PONG_PLAYER_H
