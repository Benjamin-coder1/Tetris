#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


// nom des couleurs
const char couleur[7][20] = {"red", "green", "blue", "grey", "brown", "purpose", "black"};

// noms des pieces
enum 
name {O = 1,I,S,Z,L,J,T };



class piece {

	private :
		// propriété de la piece
		name nom_piece;

		// 4 formes qui sont tournées
		int nombre_rotation;
		int color_piece;

		// position centre piece dans la grille de jeu
		int x; // numero ligne
		int y; // numero colone 


	public : 

		// récupérer des grandeurs courante
		int get_x() ;
		int get_y() ;
		int get_rotation() ;
		int get_name() ;

		// modifier_valeur courante
		void set_x( int x_d) ; 
		void set_y( int y_d) ; 
		void set_rotation( int nombre_rotation_d) ;

		// tourner 
		void turn() ; 

		// initialiser la piece 
 		piece(name nom_piece) ;

 		
};




// // affichage nom piece 
// inline std::ostream& piece::operator << ( std::ostream& stream, name nom_piece  );








 



