//
// Created by jan on 31. 12. 2020.
//

#include "../HeaderFiles/Menu.h"

#include <iostream>

Menu::Menu(float w, float h, int elements)
{
    elementsCount = elements;
    menuText = new sf::Text[elementsCount];

    width = w;
    height = h;
    if (!font.loadFromFile("../Fonts/arial.ttf")) {
        std::cout << "{MENU} Error loading menu fonts" << std::endl;
    }


    for (int i = 0; i < elementsCount; i++)
    {
        menuText[i].setFont(font);
        menuText[i].setFillColor(sf::Color::White);
    }


    selectedItemIndex = 0;
    menuText[selectedItemIndex].setFillColor(sf::Color::Red);
}



void Menu::draw(sf::RenderWindow* window)
{
    for (int i = 0; i < elementsCount; i++)
    {
        window->draw(menuText[i]);
    }
}

void Menu::MoveUp()
{
    if (selectedItemIndex - 1 >= 0) {
        menuText[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuText[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::MoveDown()
{
    if (selectedItemIndex + 1 < elementsCount) {
        menuText[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuText[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::setOption(int optionIndex, sf::String text)
{
    menuText[optionIndex].setString((sf::String)text);
    sf::FloatRect textRect = menuText[optionIndex].getLocalBounds();
    menuText[optionIndex].setOrigin(textRect.left + textRect.width / 2.0f,
                                    textRect.top + textRect.height / 2.0f);
    menuText[optionIndex].setPosition(sf::Vector2f(width / 2, height / (elementsCount + 1) * (optionIndex+1) ));

}

int Menu::GetPressedItem()
{
    return selectedItemIndex;
}

Menu::~Menu()
{

   delete[] menuText;

}