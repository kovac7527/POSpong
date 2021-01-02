#include <SFML/Graphics.hpp>
#include <iostream>
#include "HeaderFiles/GameManager.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>




int main() {

    uint width, height;
    width = 400;
    height = 400;
    sf::RenderWindow window(sf::VideoMode(width, height), "PONG GAME", sf::Style::Close);
    GameManager manager(width, height);
    manager.setWindow(&window);
    manager.initializeComponents(20,100,15);


    //manager->start();
   // manager->ball->setDefaultSpeed(10.0);
   // manager->ball->setSpeed(10.0);






    //sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(height, height));  // is this ok ?s


    // (*player1).setPosition(-view.getSize().x / 2, -view.getSize().y / 2); // works fine only with default origin
    //(*player2).setPosition(view.getSize().x / 2 - ((*player2).object.getSize().x), view.getSize().y / 2 - ((*player2).object.getSize().y));   // works fine only with default origin
    while (window.isOpen()) {
        sf::Event evnt;


        while (window.pollEvent(evnt)) {
            switch (evnt.type) {
                case sf::Event::LostFocus:
                    manager.windowFocus = false;

                    break;
                case sf::Event::GainedFocus:
                    manager.windowFocus = true;

                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyReleased:

                    if (!manager.playing) {
                        switch (evnt.key.code) {
                            case sf::Keyboard::Up:
                                manager.mainMenu->MoveUp();
                                break;
                            case sf::Keyboard::Down:
                                manager.mainMenu->MoveDown();
                                break;
                            case sf::Keyboard::Return:

                                switch (manager.mainMenu->GetPressedItem()) {
                                    case 0:
                                        manager.playing = true;
                                        manager.paused = false;
                                        manager.resetGame();
                                        manager.ball->direction=ballDirection::LEFT;
                                        std::cout << "{MENU} -> 'Play' Buton pressed" << std::endl;
                                        break;
                                    case 1:
                                        std::cout << "{MENU} -> 'Create SERVER' Buton pressed" << std::endl;
                                        break;
                                    case 2:
                                        std::cout << "{MENU} -> 'Join SERVER' Buton pressed" << std::endl;
                                        break;
                                    case 3:
                                        std::cout << "{MENU} -> 'Exit' Buton pressed" << std::endl;
                                        window.close();
                                        break;
                                    default:
                                        break;
                                }
                            default:
                                break;

                        }
                    } else if (manager.playing && !manager.paused) {
                        switch (evnt.key.code) {
                            case sf::Keyboard::Escape:
                                manager.paused = true;
                                break;
                            default:
                                break;
                        }

                    } else if (manager.playing && manager.paused) {

                        switch (evnt.key.code) {
                            case sf::Keyboard::Up:
                                manager.inGameMenu->MoveUp();
                                break;
                            case sf::Keyboard::Down:
                                manager.inGameMenu->MoveDown();
                                break;
                            case sf::Keyboard::Return:

                                switch (manager.inGameMenu->GetPressedItem()) {
                                    case 0:
                                        manager.playing = true;
                                        manager.paused = false;
                                        std::cout << "{InGameMemu} -> 'Resume' Buton pressed" << std::endl;
                                        break;
                                    case 1:
                                        std::cout << "{InGameMemu} -> 'Back to main menu" << std::endl;
                                        manager.paused = false;
                                        manager.playing = false;
                                        break;
                                    case 2:
                                        std::cout << "{InGameMemu} -> 'Exit' Buton pressed" << std::endl;
                                        window.close();
                                        break;

                                    default:
                                        break;
                                }
                            default:
                                break;

                        }

                    }

                    break;
                default:
                    break;
            }

        }

        window.clear();
        manager.update();
        if (manager.playing){
            manager.draw();
        }

        window.display();



    }

    return 0;
}