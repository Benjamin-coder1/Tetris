#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "piece.hpp"
#include "value_piece.cpp"


class grid {	

	private:
		
		// statut partie
		int status_partie;
		// donne score		
		int score;
		// pour la grille        
		int grid_tab[22][10];
		// information sur le propriétaire de la grille 
		std::string name; 


	public : 
		int v(int i,int j) {
			return grid_tab[i][j];
		}

		// constructeur de la grille
		grid() ;	

		// recupérer le score
		int get_score() ;

		// gere le nom 
		std::string get_name();
		void set_name(std::string given_name);

		// recupérer la status
		int get_status_partie() ;

		//recupere grille
		int* data();

		// fonction pour ecrire et supprimer piece
		void write_piece_grille(piece la_piece);
		void suppress_piece_grille(piece la_piece);

		// fonction pour translater piece
		int piece_translation_D(piece& la_piece);
		int piece_translation_B(piece& la_piece);
		int piece_translation_G(piece& la_piece);

		// fonction pour faire tourner piece
		int piece_rotation(piece& la_piece);

		// fonction pour verifier que l'on a pas terminé la partie 
		void verif_end(piece& la_piece);	

		// fonction pour supprimer ligne entière
		void clean();	

		// fonction d'affichage 
		void print_grid();

};




