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
		// nom de la piece en fonction de sa forme
		name nom_piece;

		// la piece peut tourner 4 fois 
		int nombre_rotation;

		// sa couleur 
		int color_piece;

		// position centre piece dans la grille de jeu
		int x; // numero ligne
		int y; // numero colone 


	public : 

		// getter 
		int get_x() ;
		int get_y() ;
		int get_rotation() ;
		int get_name() ;

		// setter
		void set_x( int x_d) ; 
		void set_y( int y_d) ; 
		void set_rotation( int nombre_rotation_d) ;

		// faire tourner la piece une fois 
		void turn() ; 

		// constructeur d'une piece 
 		piece(name nom_piece) ;

 		
};







 



