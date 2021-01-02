//
// Created by jan on 31. 12. 2020.
//

#include "../HeaderFiles/GameManager.h"





GameManager::GameManager( uint w,  uint h)
{
    this->height = h;
    this->width = w;
    playing = false;
    paused = false;
    windowFocus = true;
    imClient = false;
    imServer = false;
}

void GameManager::OpenWindow()
{

}



//void GameManager::scoreUp(Player* player)
//{
//    if (player == player1) {
//        scorePlayer1++;
//    }
//    else {
//        scorePlayer2++;
//    }
//
//    ball->Reset();
//    player1->Reset();
//    player2->Reset();
//}



void GameManager::setWindow(sf::RenderWindow* win ) {
    window = win;
}

void GameManager::update() {
    int n;
    if (playing) {

        if(!paused) {
            if(imServer || (imServer && imClient)) {
                switch (colisDetector->CheckForBallColision())  //checking for colisions
                {
                    case noColision:
                        break;
                    case ColisionPlayer1:

                        ball->changeDirection((ballDirection) ((rand() % 3) + 4));
                        break;
                    case ColisionPlayer2:

                        ball->changeDirection((ballDirection) ((rand() % 3) + 1));
                        break;
                    case ColisionUpperCorner:
                        ball->changeDirection((ballDirection) (ball->getBallDireciton() + 1));
                        break;
                    case ColisionDownCorner:
                        ball->changeDirection((ballDirection) (ball->getBallDireciton() - 1));
                        break;
                    case ColisionLeftSide:

                        // player 2(red) scores
                        std::cout << "{GAME} -> 'Player 2' scores !!! " << std::endl;
                        scorePlayer2++;
                        sleep(1.5);
                        this->resetPositions();
                        ball->changeDirection((ballDirection) 1);

                        break;
                    case ColisionRightSide:
                        // player 1(red) scores
                        std::cout << "{GAME} -> 'Player 1' scores !!! " << std::endl;
                        scorePlayer1++;
                        sleep(1.5);
                        this->resetPositions();
                        ball->changeDirection((ballDirection) 1);
                        break;
                    default:
                        break;
                }
            }

            // check for player controls
            if (windowFocus) {

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !imServer && !imClient) {
                    if (0 < player2->getY() - player2->object.getSize().y / 2) {
                        (*player2).moveUp();
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !imServer && !imClient) {

                    if (height > (player2->getY() + player2->object.getSize().y / 2)) {
                        (*player2).moveDown();
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                    if (0 < player1->getY() - player1->object.getSize().y / 2) {

                        if (imServer) {
                            (*player1).moveUp();
                            const char *msg = "w";
                            int n;
                            n = write(*newsockfd, msg, strlen(msg) + 1);
                            if (n < 0) {
                                perror("Error writing to socket");
                            }
                        }
                        if (imClient && 0 < player2->getY() - player2->object.getSize().y / 2) {
                            (*player2).moveUp();
                            const char *msg = "w";
                            int n;
                            n = write(*socketfd, msg, strlen(msg) + 1);
                            if (n < 0) {
                                perror("Error writing to socket");
                            }
                        }
                        if (!imClient && !imServer) {
                            player1->moveUp();
                        }
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                    if (height > (player1->getY() + player1->object.getSize().y / 2)) {

                        if (imServer) {
                            player1->moveDown();
                            const char *msg = "s" ;
                            n = write(*newsockfd, msg, strlen(msg) + 1);
                            if (n < 0) {
                                perror("Error writing to socket");
                            }
                        }
                        if (imClient && height > (player2->getY() + player2->object.getSize().y / 2)) {
                            (*player2).moveDown();
                            const char *msg = "s";
                            int n;
                            n = write(*socketfd, msg, strlen(msg) + 1);
                            if (n < 0) {
                                perror("Error writing to socket");
                            }
                        }
                        if (!imClient && !imServer) {
                            player2->moveDown();
                        }

                    }
                }
            }
            if (imServer || (!imServer && !imClient)) {

                ball->move();

                if (imServer){
                    char integer_stringx[32];
                    char integer_stringy[32];
                    int x = ball->getX();
                    int y = ball->getY();

                    sprintf(integer_stringx, "%d", x);
                    sprintf(integer_stringy, "%d", y);
                    char msg[128] = "BallPostition: "; // make sure you allocate enough space to append the other string

                    strcat(msg, integer_stringx);
                    strcat(msg, ":");
                    strcat(msg, integer_stringy);

                    n = write(*newsockfd, msg, strlen(msg) + 1);
                    if (n < 0) {
                        perror("Error writing to socket");
                    }

                }
            }


        } else {
            this->draw();
            inGameMenu->draw(window);

        }



    } else {

        mainMenu->draw(window);


    }

}

void GameManager::draw() {
    this->player1->draw(window);
    this->player2->draw(window);
    this->ball->draw(window);
}

void GameManager::initializeComponents(float playerSizeX, float playerSizeY, float ballRadius) {

    scorePlayer1 = 0;
    scorePlayer2 = 0;
    ball = new Ball((int)width/2, (int)height / 2);
    ball->setRadius(ballRadius);
    ball->setDefaultSpeed(1.0);

    player1 = new Player(playerSizeX / 2,width / 2);
    player1->setSize(playerSizeX, playerSizeY);

    player2 = new Player( width - playerSizeX / 2,width / 2);
    player2->setSize(playerSizeX, playerSizeY);
    player2->setColor(sf::Color::Red);

    colisDetector = new ColisionDetector(player1, player2, ball, width, height);

    this->mainMenu = new Menu(window->getSize().x, window->getSize().y,4);
    mainMenu->setOption(0, "Play");
    mainMenu->setOption(1, "Create SERVER");
    mainMenu->setOption(2, "Join SERVER");
    mainMenu->setOption(3, "EXIT");

    this->inGameMenu = new Menu(window->getSize().x, window->getSize().y,3);
    inGameMenu->setOption(0, "Resume");
    inGameMenu->setOption(1, "Back to main menu");
    inGameMenu->setOption(2, "Exit");

}



void GameManager::resetGame() {
    this->scorePlayer1 = 0;
    this->scorePlayer2 = 0;
    this->resetPositions();


}

void GameManager::resetPositions() {
    if (imServer) {
        const char *msg = "r" ;
        int n = write(*newsockfd, msg, strlen(msg) + 1);
        if (n < 0) {
            perror("Error writing to socket");
        }
    }
    this->ball->Reset();
    this->player1->Reset();
    this->player2->Reset();
}


void GameManager::startServer() {
  /*  gameServer = new server();//// here will some staff to create server
    gameServer->startServer(player1 , player2, ball);
    this->imServer = true;
    this->imClient = false;*/



}


