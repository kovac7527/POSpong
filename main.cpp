#include <SFML/Graphics.hpp>
#include <iostream>
#include "HeaderFiles/GameManager.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>

#define SERVER_PORT 9999
#define HOSTNAME_PORT 9999


typedef struct dataCitac{
    int * newsockfd;
    Player* player2Klient;
    GameManager* manager;

} DATA_CIT;

typedef struct dataCitacServeru{
    int * sockfd;
    Player* player1Server;
    GameManager* manager;

} DATA_CIT_SERVER;

void * readFromClient (void* param) {
    char * pch;
    int i = 0;
    char buffer[256];
    int n;
    DATA_CIT * data = (DATA_CIT*) param;

    while (true) {

        bzero(buffer, 255);
        n = read(*data->newsockfd, buffer, 255);
        //sleep(2);
        if (n < 0) {
            perror("Error reading from socket");
            return NULL;
        }
        if (buffer[0] == 'm') {
            printf("{Thread (readFromClient)} -> Client player2 is moving \n");
            pch = strtok (buffer,":");
            i = 0;
            sf::Vector2f position;
            while (pch != NULL)
            {
                printf (" delimited %s\n",pch);

                if (i==1){
                    position.x = atoi(pch);
                } else if(i==2) {
                    position.y = atoi(pch);
                } pch = strtok (NULL, ":");
                i++;
            }
            data->manager->player2->object.setPosition(position);

        }
        printf("Here is the message: %s\n", buffer);

        if (!data->manager->imServer) {
            break;
        }


    }
    close(*data->newsockfd);
}


void * readFromServer (void* param) {

    char buffer[256];
    int n;
    DATA_CIT_SERVER * data = (DATA_CIT_SERVER *) param;
    printf("{Vlakno} : Reading from server started\n");
    char * pch;
    int i = 0;
    while (true) {

        bzero(buffer, 255);
        n = read(*data->sockfd, buffer, 255);
        //sleep(2);
        if (n < 0) {
            perror("Error reading from socket");
            return NULL;
        }
       if (buffer[0] == 'm') {
           printf("{Thread (readFromServer)} -> SERVER player1 is moving to position\n");
           pch = strtok (buffer,":");
           i = 0;
           sf::Vector2f position;
           while (pch != NULL)
           {
               printf (" delimited %s\n",pch);

               if (i==1){
                   position.x = atoi(pch);
               } else if(i==2) {
                   position.y = atoi(pch);
               } pch = strtok (NULL, ":");
               i++;
           }
           data->manager->player1->object.setPosition(position);

        }  else {

            printf ("Splitting string \"%s\" into tokens:\n",buffer);
            pch = strtok (buffer,":");
            i = 0;
            sf::Vector2f position;
            while (pch != NULL)
            {
                printf (" delimited %s\n",pch);

                if (i==1){
                    position.x = atoi(pch);
                } else if(i==2) {
                    position.y = atoi(pch);
                } pch = strtok (NULL, ":");
                i++;
            }
            data->manager->ball->object.setPosition(position);
        }
        printf("Here is the message: %s\n", buffer);


        if (!data->manager->imClient) {
            break;
        }

    }
    close(*data->sockfd);
}



int main() {


    uint width, height;
    width = 400;
    height = 400;
    sf::RenderWindow window(sf::VideoMode(width, height), "PONG GAME", sf::Style::Close);
    GameManager manager(width, height);
    manager.setWindow(&window);
    manager.initializeComponents(20,100,15);

    pthread_t read_vlakno;
    DATA_CIT dataCitac;

    pthread_t readFromServer_vlakno;
    DATA_CIT_SERVER dataCitacServer;
    //pthread_attr_t attr_t1;
    //pthread_attr_init(&attr_t1);
    //pthread_attr_setdetachstate(&attr_t1,PTHREAD_CREATE_DETACHED);
    dataCitac.player2Klient = manager.player2;
    dataCitacServer.player1Server = manager.player1;

    int sockfd;


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


                                        int newsockfd;
                                        socklen_t cli_len;
                                        struct sockaddr_in serv_addr, cli_addr;


                                        bzero((char *) &serv_addr, sizeof(serv_addr));
                                        serv_addr.sin_family = AF_INET;
                                        serv_addr.sin_addr.s_addr = INADDR_ANY;
                                        serv_addr.sin_port = htons(SERVER_PORT);
                                        sockfd = socket(AF_INET, SOCK_STREAM, 0);

                                        //printf("{SERVER} -> Waiting for other player to join server\n");

                                        if (sockfd < 0) {
                                            perror("Error creating socket");
                                            return 1;
                                        }

                                        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
                                            perror("Error binding socket address");
                                            return 2;
                                        }


                                        listen(sockfd, 5);
                                        cli_len = sizeof(cli_addr);



                                        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
                                        if (newsockfd < 0) {
                                            perror("ERROR on accept");
                                            return 3;
                                        } else {
                                            printf("{SERVER} -> Player joined server\n") ;
                                            manager.playing = true;
                                            manager.paused = false;
                                            manager.imServer = true;
                                            manager.imClient = false;
                                            manager.newsockfd = &newsockfd;
                                            manager.resetGame();
                                            manager.ball->direction=ballDirection::LEFT;

                                        }


                                        dataCitac.newsockfd = &newsockfd;
                                        dataCitac.manager = &manager;


                                        pthread_create(&read_vlakno,NULL,&readFromClient,&dataCitac);
                                        //pthread_detach(read_vlakno);
                                        break;
                                    case 2:
                                        std::cout << "{MENU} -> 'Join SERVER' Buton pressed" << std::endl;


                                        struct sockaddr_in serv_addres;
                                        struct hostent* server;



                                        server = gethostbyname("localhost");
                                        if (server == NULL)
                                        {
                                            fprintf(stderr, "Error, no such host\n");
                                            return 2;
                                        }

                                        bzero((char *) &serv_addr, sizeof(serv_addr));
                                        serv_addr.sin_family = AF_INET;
                                        bcopy(
                                                (char *) server->h_addr,
                                                (char *) &serv_addr.sin_addr.s_addr,
                                                server->h_length
                                        );
                                        serv_addr.sin_port = htons(SERVER_PORT);

                                        sockfd = socket(AF_INET, SOCK_STREAM, 0);
                                        if (sockfd < 0) {
                                            perror("Error creating socket");
                                            return 3;
                                        }

                                        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
                                            perror("Error connecting to socket");
                                            return 4;
                                        } else {
                                            dataCitacServer.sockfd = &sockfd;
                                            dataCitacServer.player1Server = manager.player1;
                                            dataCitacServer.manager = &manager;

                                            manager.playing = true;
                                            manager.paused = false;
                                            manager.imServer = false;
                                            manager.imClient = true;
                                            manager.socketfd = &sockfd;
                                            manager.resetGame();

                                            pthread_create(&readFromServer_vlakno,NULL,&readFromServer,&dataCitacServer);

                                        }





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
                                        if(manager.imServer) {
                                            pthread_cancel(read_vlakno);
                                            close(*manager.newsockfd);
                                            close(sockfd);


                                        } else if (manager.imClient) {
                                            pthread_cancel(readFromServer_vlakno);
                                            close(sockfd);
                                        }
                                        manager.imServer = false;
                                        manager.imClient = false;
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
    //pthread_join(read_vlakno,NULL);
    return 0;
}