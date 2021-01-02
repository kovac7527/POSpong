//
// Created by jan on 31. 12. 2020.
//

#ifndef PONG_MENU_H
#define PONG_MENU_H


#include "SFML/Graphics.hpp"


class Menu
{
private:
    float width, height;
    int selectedItemIndex, elementsCount;
    sf::Font font;


public:
    sf::Text* menuText;
    Menu(float w, float h, int elements);
    ~Menu();

    void draw(sf::RenderWindow* window);
    void MoveUp();
    void MoveDown();

    void setOption(int optionIndex, sf::String text);


    int GetPressedItem();


};


#endif //PONG_MENU_H
