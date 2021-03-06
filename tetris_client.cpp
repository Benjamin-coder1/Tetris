#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <chrono>
#include <poll.h>
#include <SFML/Graphics.hpp>


int main( int argc, char *argv[]  ){

	// ------------  CONNECTION SERVEUR ------------------ //

	// recuperation informations partie
	char nom_joueur[25];
	std::cout << "Nom du joueur : ";
	scanf("%s", nom_joueur);

	char nom_hote[25];
	std::cout << "Adresse IP : " ;
	scanf(" %s", nom_hote);


	// connexion avec le serveur 
    struct sockaddr_in serv_addr;   
    struct hostent *server;

    int port_number = 11003;
 	int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
 	if (socket_descriptor < 0){
		std::cout << "Le lancement de la connexion à échoué impossible de créer un socket" << std::endl;
		return 0;
	}

 
	server = gethostbyname(nom_hote);
	if (server == NULL) {
		std::cout << "La connexion à échouée " << std::endl;
	    return 0;
	}
 
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy( (char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port_number);

    if (connect(socket_descriptor,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		std::cout << "La connexion à échoué " << std::endl;
        return 0;    	
    }
 
    // on envoit le nom du joueur
    if ( write(socket_descriptor, nom_joueur, 25) < 0) {
		std::cout << "La connexion à échoué " << std::endl;
        return 0;    
    }

    std::cout << std::endl << "Vous etes connecté à la partie, celle-ci débutera automatiquement des que tous les joueurs seront connectés " << std::endl << std::endl;
    int lancement = 0;
    read(socket_descriptor, &lancement, sizeof(int) );

    // ------------  AFFICHAGE ET COMMANDE ------------------ //
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> chrono;
    int* grid_tab = (int *) malloc( 22*10*sizeof(int));

    int status_partie = 1;
    int score = 0;

    // ------------------- INTERFACE GRAPHIQUE INITIALISATION -------------- //

    float timeElapsed = 0.0;

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "TETRIS");

    sf::RectangleShape fond(sf::Vector2f(1000, 1000));
    fond.setFillColor(sf::Color(4,20,40));

    sf::RectangleShape fond2(sf::Vector2f(1000, 1000));
    fond2.setFillColor(sf::Color(0,0,0,128));

    sf::RectangleShape rectangle(sf::Vector2f(400, 880));
    rectangle.setPosition(300,110);
    rectangle.setFillColor(sf::Color(4, 47, 80));
    rectangle.setOutlineThickness(5.f);
    rectangle.setOutlineColor(sf::Color(255, 0, 204));

    sf::Font font;
    font.loadFromFile("Franchise.ttf");
    sf::Text titre;
    titre.setFont(font);
    titre.setString("TETRIS");
    titre.setCharacterSize(80);
    titre.setFillColor(sf::Color(255, 0, 204));
    titre.setPosition(420,5);

    sf::Text score2;
    score2.setFont(font);
    score2.setString("SCORE");
    score2.setCharacterSize(40);
    score2.setFillColor(sf::Color(255, 0, 204));
    score2.setPosition(800,400);

    sf::Text score3;
    score3.setFont(font);
    score3.setString("0");
    score3.setCharacterSize(50);
    score3.setFillColor(sf::Color(255, 0, 204));
    score3.setPosition(810,480);

    sf::RectangleShape rectangleScore(sf::Vector2f(180, 110));
    rectangleScore.setPosition(750,470);
    rectangleScore.setFillColor(sf::Color(2,18,25));
    rectangleScore.setOutlineThickness(2.f);
    rectangleScore.setOutlineColor(sf::Color(255, 0, 204));

    sf::RectangleShape rectanglePause(sf::Vector2f(400, 200));
    rectanglePause.setPosition(300,400);
    rectanglePause.setFillColor(sf::Color(2,18,25));
    rectanglePause.setOutlineThickness(2.f);
    rectanglePause.setOutlineColor(sf::Color(255, 0, 204));

    sf::Text reprendre;
    reprendre.setFont(font);
    reprendre.setString("REPRENDRE");
    reprendre.setCharacterSize(60);
    reprendre.setFillColor(sf::Color(255, 0, 204));
    reprendre.setPosition(400,450);

    sf::Text pause;
    pause.setFont(font);
    pause.setString("PAUSE");
    pause.setCharacterSize(40);
    pause.setFillColor(sf::Color(255, 0, 204));
    pause.setPosition(800,700);

    sf::RectangleShape lineVertical(sf::Vector2f(880,2));
    lineVertical.rotate(90);
    lineVertical.setFillColor(sf::Color(129, 23, 142));

    sf::RectangleShape lineHorizontal(sf::Vector2f(400,2));
    lineHorizontal.setFillColor(sf::Color(129, 23, 142));

    sf::RectangleShape formeBase(sf::Vector2f(38,38));

    sf::Clock clock;
    sf::Time t = sf::seconds(1.0f);



    int indicateur = 1;
    int indicInit = 0;
    bool out;
    
    
    while ( window.isOpen() ){

        out = false;

        // recoit 
        int size_msg_to_rcv = 2*sizeof(int) + 22*10*sizeof(int);
        int* message_to_rcv = (int *) malloc( size_msg_to_rcv);
        read(socket_descriptor, message_to_rcv , size_msg_to_rcv );

        // Protocole reception 
        status_partie = message_to_rcv[0];
        if ( !status_partie ) break; 
        score = message_to_rcv[1];
        memcpy( grid_tab, &message_to_rcv[2], 22*10*sizeof(int) );   
    
        //std::cout << "Le score est de : " << score << "\r" << std::flush << std::endl << std::endl ;
        std::string sco = std::to_string(score);
        score3.setString(sco);

        if (indicInit == 0) {
            window.draw(fond);
            window.draw(titre);
            window.draw(pause);
            window.draw(score2);
            window.draw(rectangleScore);
            indicInit = 1;
        }

        window.draw(rectangleScore);
        window.draw(score3);
        window.draw(rectangle);
        for (int i=0;i<9;i++) {
            lineVertical.setPosition(300+40*(i+1)-1,110);
            window.draw(lineVertical);
        }
        for (int i=0;i<21;i++) {
            lineHorizontal.setPosition(300,110+40*(i+1)-1);
            window.draw(lineHorizontal);
        }
        for (int i =0;i<22;i++) {
            for(int j=0;j<10;j++) {
                if (grid_tab[i*10+j] > 0) {

                    int indice = grid_tab[i*10+j];
                    if (indice == 1)
                        formeBase.setFillColor(sf::Color(254,248,76));
                    if (indice == 2)
                        formeBase.setFillColor(sf::Color(81,225,252));
                    if (indice == 3)
                        formeBase.setFillColor(sf::Color(233,61,30));
                    if (indice == 4)
                        formeBase.setFillColor(sf::Color(121,174,61));
                    if (indice == 5)
                        formeBase.setFillColor(sf::Color(246,146,48));
                    if (indice == 6)
                        formeBase.setFillColor(sf::Color(241,110,185));
                    if (indice == 7)
                        formeBase.setFillColor(sf::Color(148,54,146));

                    formeBase.setPosition(300+40*j-1,110+40*i+1);
                    formeBase.setOutlineThickness(2.f);
                    formeBase.setOutlineColor(sf::Color(0,0,0));
                    window.draw(formeBase);
                }
            }
        }
        window.display();

        // ----- RECEPTION COMMANDE ------- //
        sf::Event event;
        char touch = 'x'; 

        sf::Time timeElapsed = clock.getElapsedTime();

        while (timeElapsed < t && !out) {

            timeElapsed = clock.getElapsedTime();
            
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return 0;
                }

                if (event.type == sf::Event::KeyPressed) {

                    if (event.key.code == sf::Keyboard::Left) {
                        touch = 'a';
                        indicateur = 1;
                    }

                    if (event.key.code == sf::Keyboard::Right) {
                        touch = 'z';
                        indicateur = 1;
                    }

                    if (event.key.code == sf::Keyboard::Down) {
                        touch = 'x';
                        indicateur = 1;
                    }

                    if (event.key.code == sf::Keyboard::Up) {
                        touch = 'r';
                        indicateur = 1;
                    }
                    out = true;
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2i globalPosition = sf::Mouse::getPosition(window);
                    if ( 800 < globalPosition.x && globalPosition.x < 900 && 700 < globalPosition.y && globalPosition.y < 800) {
                        window.draw(fond2);
                        window.draw(rectanglePause);
                        window.draw(reprendre);
                        window.display();
                        int indic = 0;
                        while (indic == 0) {
                            window.pollEvent(event);
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                globalPosition = sf::Mouse::getPosition(window);
                                if ( 200 < globalPosition.x && globalPosition.x < 600 && 200 < globalPosition.y && globalPosition.y < 600) {
                                    indic = 1;
                                    indicInit = 0;
                                }
                            }
                            if (event.type == sf::Event::Closed) {
                                window.close();
                                return 0;
                            }
                        }
                    }
                }
            }
        }

        if (timeElapsed>=t)
            clock.restart();

        // on envoit la touche 
        write(socket_descriptor, &touch, sizeof(char));     

    }

    // on ferme la connexion
    close(socket_descriptor);
    std::cout << std::endl << "Partie Terminée" << std::endl;


    char nom_winner[25];
    read(socket_descriptor, nom_winner, 25);
    std::cout << std::endl << "LE WINNER EST : " << nom_winner << std::endl;



    return 1;






}
