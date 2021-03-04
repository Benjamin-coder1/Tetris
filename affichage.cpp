#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "color.hpp"
#include "grille.hpp"


int main() 
{

	grid grille;
	int indicateur = 1;

	piece p(S);
	p.set_x(2);
	p.set_y(2);


	grille.write_piece_grille(p);

	float timeElapsed = 0.0;

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "TETRIS");

    sf::RectangleShape fond(sf::Vector2f(1000, 1000));
    fond.setFillColor(sf::Color(4,20,40));

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

	sf::Text score;
	score.setFont(font);
	score.setString("SCORE");
	score.setCharacterSize(40);
	score.setFillColor(sf::Color(255, 0, 204));
	score.setPosition(800,400);

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
    sf::Time t = sf::seconds(2.0f);

	window.draw(fond);
	window.draw(titre);
	window.draw(pause);
	window.draw(score);
	window.display();

    while (window.isOpen())
    {
        sf::Event event;
	    sf::Time timeElapsed = clock.getElapsedTime();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
            	window.close();
            	return 0;
            }

            if (event.type == sf::Event::KeyPressed) {

	            if (event.key.code == sf::Keyboard::Left) {
					grille.piece_translation_G(p);
					indicateur = 1;
	            }

	            if (event.key.code == sf::Keyboard::Right) {
	        		grille.piece_translation_D(p);
	        		indicateur = 1;
	            }

	            if (event.key.code == sf::Keyboard::Down) {
					grille.piece_translation_B(p);
					indicateur = 1;
	            }

	            if (event.key.code == sf::Keyboard::Up) {
					grille.piece_rotation(p);
					indicateur = 1;
	            }

	         }
	    }

	   	if (timeElapsed > t) {
	    	clock.restart();
	       	grille.piece_translation_B(p);
	       	indicateur = 1;
	    }

	    if (indicateur == 1) {

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
		        	if (grille.v(i,j) > 0) {
		        		int indice = p.get_name();

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
	   	 	indicateur = 0;
	   	 }

    }
	return 0;
}
