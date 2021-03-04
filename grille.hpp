#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "piece.hpp"
#include "value_piece.cpp"


class grid {	

	private:
		
		// statut partie 1 si en cours / 0 si finis
		int status_partie;

		// score du joueur
		int score;

		// grille de jeux     
		int grid_tab[22][10];

		// nom du joueur 
		std::string name; 


	public : 
		int v(int i,int j) {
			return grid_tab[i][j];
		}

		// constructeur de la grille, initialement grid_tab est remplie de 0 le score est nul
		// le status de la partie vaut 1
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

		// fonction d'affichage debeugage
		void print_grid();

};




