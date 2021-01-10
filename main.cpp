#include <SFML/Graphics.hpp>
#include <iostream>
#include "HeaderFiles/GameManager.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>

#define SERVER_PORT 9999
#define HOSTNAME_PORT 9999
#define IP_ADDRESS "localhost"


typedef struct dataStartServer{
    int * sockfd;
    int * newsockfd;
    GameManager* manager;
    pthread_cond_t * cond_player_joined;
    pthread_mutex_t * mutex;

} DATA_START_SERVER;

typedef struct dataCitac{
    bool * running;
    pthread_cond_t * cond_player_joined;
    pthread_mutex_t * mutex;
    int * newsockfd;
    Player* player2Klient;
    GameManager* manager;

} DATA_CIT;

typedef struct dataCitacServeru{
    bool * running;
    int * sockfd;
    Player* player1Server;
    GameManager* manager;

} DATA_CIT_SERVER;


void * startServer(void * param){
    DATA_START_SERVER * data = (DATA_START_SERVER * ) param;
    printf("{Thread (startServer} -> started\n");
    struct sockaddr_in  cli_addr;
    socklen_t cli_len;

    pthread_mutex_lock(data->mutex);
    printf("{Thread (startServer} -> locked starting to listed\n");
    listen(*data->sockfd, 5);
    printf("{Thread (startServer} -> after starting to listed\n");
    cli_len = sizeof(cli_addr);
    printf("{Thread (startServer} -> Before accept\n");
    *data->newsockfd = accept(*data->sockfd, (struct sockaddr *) &cli_addr, &cli_len);
    printf("{Thread (startServer} -> afteraccept\n");
    if (*data->newsockfd < 0) {
        perror("ERROR on accept");
    } else {
        printf("{SERVER} -> Player joined server\n") ;
        data->manager->playing = true;
        data->manager->paused = false;
        data->manager->imServer = true;
        data->manager->imClient = false;
        data->manager->newsockfd = data->newsockfd;
        data->manager->resetGame();
        data->manager->ball->direction=ballDirection::LEFT;

    }
    printf("{Thread (startServer} -> end sending signal unlocking mutex\n");

    pthread_cond_broadcast(data->cond_player_joined);
    pthread_mutex_unlock(data->mutex);
    return NULL;
}



void * readFromClient (void* param) {

    char * pch;
    int i = 0;
    char buffer[256];
    int n;
    DATA_CIT * data = (DATA_CIT*) param;

    printf("Vlakno read from client started\n");

    printf("Waiting for signal\n");
    pthread_cond_wait(data->cond_player_joined,data->mutex);
    printf("reading from client started\n");


    while (*data->running) {

        printf("reading from client  before read\n");
        bzero(buffer, 255);
        n = read(*data->newsockfd, buffer, 255);
        printf("reading from client  after read\n");
        if (buffer[0] == 'c') {
            printf("Cancel from client\n");
            data->manager->playing = false;
            data->manager->paused = false;

            char msg[128] = "close"; // make sure you allocate enough space to append the other string
            printf("Close msg send back \n");
            int n = write(*data->newsockfd, msg, strlen(msg) + 1);
            if (n < 0) {
                perror("Cancel");
            }
            break;
        }

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

    printf("{Vlakno} : Reading from CLIENT ended\n");
    return NULL;
}


void * readFromServer (void* param) {

    char buffer[256];
    int n;
    DATA_CIT_SERVER * data = (DATA_CIT_SERVER *) param;
    printf("{Vlakno} : Reading from server started\n");
    char * pch;
    int i = 0;
    while (*data->running) {

        bzero(buffer, 255);
        n = read(*data->sockfd, buffer, 255);
        if (buffer[0] == 'c') {
            printf("Cancel from server\n");

            data->manager->playing = false;
            data->manager->paused = false;

            char msg[128] = "close"; // make sure you allocate enough space to append the other string
            printf("Sending server info about disconected klient \n");
            int n = write(*data->sockfd, msg, strlen(msg) + 1);
            if (n < 0) {
                perror("Error writing to socket");
            }
            break;
        }
        //sleep(2);
        if (n < 0) {
            perror("Error reading from socket");
            return NULL;
        }
       if (buffer[0] == 'm') {
           //printf("{Thread (readFromServer)} -> SERVER player1 is moving to position\n");
           pch = strtok (buffer,":");
           i = 0;
           sf::Vector2f position;
           while (pch != NULL)
           {
               //printf (" delimited %s\n",pch);

               if (i==1){
                   position.x = atoi(pch);
               } else if(i==2) {
                   position.y = atoi(pch);
               } pch = strtok (NULL, ":");
               i++;
           }
           data->manager->player1->object.setPosition(position);

        }  else {

           // printf ("Splitting string \"%s\" into tokens:\n",buffer);
            pch = strtok (buffer,":");
            i = 0;
            sf::Vector2f position;
            while (pch != NULL)
            {
               // printf (" delimited %s\n",pch);

                if (i==1){
                    position.x = atoi(pch);
                } else if(i==2) {
                    position.y = atoi(pch);
                } pch = strtok (NULL, ":");
                i++;
            }
            data->manager->ball->object.setPosition(position);
        }
        if (buffer[0] == 'r') {
            data->manager->resetPositions();
        }
        //printf("Here is the message: %s\n", buffer);


        if (!data->manager->imClient) {
            break;
        }

    }
    printf("{Vlakno} : Reading from server ENDED\n");
    return NULL;

}



int main() {

    bool running = true;
    uint width, height;
    width = 400;
    height = 400;
    sf::RenderWindow window(sf::VideoMode(width, height), "PONG GAME", sf::Style::Close);
    GameManager manager(width, height);
    manager.setWindow(&window);
    manager.initializeComponents(20,100,15);

    pthread_t read_vlakno;
    DATA_CIT dataCitac;

    int newsockfd;
    pthread_t readFromServer_vlakno;
    DATA_CIT_SERVER dataCitacServer;

    pthread_t startServer_vlakno;
    DATA_START_SERVER dataStartServer;

    //pthread_attr_t attr_t1;
    //pthread_attr_init(&attr_t1);
    //pthread_attr_setdetachstate(&attr_t1,PTHREAD_CREATE_DETACHED);
    dataCitac.player2Klient = manager.player2;
    dataCitacServer.player1Server = manager.player1;
    dataCitac.running= &running;
    dataCitacServer.running = &running;

    int sockfd;


    //manager->start();
   // manager->ball->setDefaultSpeed(10.0);
   // manager->ball->setSpeed(10.0);






    //sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(height, height));  // is this ok ?s


    // (*player1).setPosition(-view.getSize().x / 2, -view.getSize().y / 2); // works fine only with default origin
    //(*player2).setPosition(view.getSize().x / 2 - ((*player2).object.getSize().x), view.getSize().y / 2 - ((*player2).object.getSize().y));   // works fine only with default origin
    pthread_cond_t cond_player_joined;
    pthread_cond_init(&cond_player_joined,NULL);
    pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

    dataCitac.newsockfd = &newsockfd;
    dataCitac.manager = &manager;
    dataCitac.cond_player_joined = &cond_player_joined;
    dataCitac.mutex = &mutex;



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

                    if (manager.imClient) {
                        char msg[128] = "close"; // make sure you allocate enough space to append the other string
                        printf("Sending server info about disconected klient \n");
                         int n = write(sockfd, msg, strlen(msg) + 1);
                        if (n < 0) {
                            perror("Error writing to socket");
                        }
                    }


                    if (manager.imServer) {
                        char msg[128] = "close"; // make sure you allocate enough space to append the other string
                        int n = write(newsockfd, msg, strlen(msg) + 1);
                        if (n < 0) {
                            perror("Error writing to socket");
                        }
                    }


                    running = false;
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
                                        pthread_create(&read_vlakno,NULL,&readFromClient,&dataCitac);



                                        socklen_t cli_len;
                                        struct sockaddr_in serv_addr;


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


                                        dataStartServer.sockfd = &sockfd;
                                        dataStartServer.newsockfd = &newsockfd;
                                        dataStartServer.manager = &manager;
                                        dataStartServer.cond_player_joined = &cond_player_joined;
                                        dataStartServer.mutex = &mutex;
                                        pthread_create(&startServer_vlakno,NULL,&startServer,&dataStartServer);
                                     /*   listen(sockfd, 5);
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

                                        }*/


                                        /*dataCitac.newsockfd = &newsockfd;
                                        dataCitac.manager = &manager;


                                        pthread_create(&read_vlakno,NULL,&readFromClient,&dataCitac);*/
                                        //pthread_detach(read_vlakno);
                                        break;
                                    case 2:
                                        std::cout << "{MENU} -> 'Join SERVER' Buton pressed" << std::endl;


                                        struct sockaddr_in serv_addres;
                                        struct hostent* server;



                                        server = gethostbyname(IP_ADDRESS);
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
                                        if (manager.imClient) {
                                            char msg[128] = "close"; // make sure you allocate enough space to append the other string
                                            printf("Sending server info about disconected klient \n");
                                            int n = write(sockfd, msg, strlen(msg) + 1);
                                            if (n < 0) {
                                                perror("Error writing to socket");
                                            }
                                        }


                                        if (manager.imServer) {
                                            char msg[128] = "close"; // make sure you allocate enough space to append the other string
                                            int n = write(newsockfd, msg, strlen(msg) + 1);
                                            if (n < 0) {
                                                perror("Error writing to socket");
                                            }
                                        }
                                        running = false;
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
                                            close(newsockfd);
                                            close(sockfd);


                                        } else if (manager.imClient) {
                                            pthread_cancel(readFromServer_vlakno);

                                        }
                                        manager.imServer = false;
                                        manager.imClient = false;
                                        manager.paused = false;
                                        manager.playing = false;
                                        break;
                                    case 2:
                                        std::cout << "{InGameMemu} -> 'Exit' Buton pressed" << std::endl;
                                        if (manager.imClient) {
                                            char msg[128] = "close"; // make sure you allocate enough space to append the other string
                                            printf("Sending server info about disconected klient \n");
                                            int n = write(sockfd, msg, strlen(msg) + 1);
                                            if (n < 0) {
                                                perror("Error writing to socket");
                                            }
                                        }


                                        if (manager.imServer) {
                                            char msg[128] = "close"; // make sure you allocate enough space to append the other string
                                            int n = write(newsockfd, msg, strlen(msg) + 1);
                                            if (n < 0) {
                                                perror("Error writing to socket");
                                            }
                                        }
                                        running = false;
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

    printf("{window} : is now closed\n");

    if (manager.imServer) {
        printf("{exit} : server closing threads\n");
        close(newsockfd);
        pthread_join(read_vlakno,NULL);
        pthread_join(startServer_vlakno, NULL);
        printf("{exit} : server closed threads\n");
    }

    if (manager.imClient) {
        close(sockfd);
        printf("{exit} : client closing threads\n");
        pthread_join(readFromServer_vlakno,NULL);
        printf("{exit} : client closed threads\n");
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_player_joined);

    return 0;
}