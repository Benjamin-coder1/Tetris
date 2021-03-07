#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "grille.hpp"


grid::grid() {
	score = 0;
	for (int i = 0; i < 22; i++){
		for(int j = 0; j < 10; j++){
			grid_tab[i][j] = 0;
		}
	}
	status_partie = 1;	 
}

std::string grid::get_name(){
	return name;
}

void grid::set_name(std::string given_name){
	name = given_name;
}

int*
grid::data() { return (int *) grid_tab; }

int 
grid::get_score() { return score; }


int 
grid::get_status_partie() { return status_partie; }



void 
grid::write_piece_grille(piece la_piece){
	// cette fonction ecrit la piece dans la grille 
	int x = la_piece.get_x();
	int y = la_piece.get_y();
	 
	for( int i = 0; i < 4; i++){
		for( int j = 0; j < 4; j++){
			if ( piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i][j] != 0 )
				grid_tab[i + x - 1][j + y - 2] = piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i][j]; 
		}
	}
}


void 
grid::suppress_piece_grille(piece la_piece){
	// cette fonction supprime une piece de la grille
	int x = la_piece.get_x();
	int y = la_piece.get_y();
	 
	for( int i = 0; i < 4; i++){
		for( int j = 0; j < 4; j++){
			if  (piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i][j] != 0) {
				grid_tab[i + x - 1][j + y - 2] = 0;
			}
		}
	}
}


int 
grid::piece_translation_D(piece& la_piece){
	// cette fonction translatte verifiant tout d'abord si c'est possible 

	// 1 - BORD PLATEAU
	if( la_piece.get_y() == 9){
		return 0;
	}

	// 2 - possibilité
	for( int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i][j] != 0 && (j == 3 || piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i][j + 1] == 0)){
				if( (la_piece.get_y() - 2 + j) == 9 || grid_tab[la_piece.get_x() - 1 + i][la_piece.get_y() + j - 1] != 0) {
					return 0;
				} 
			}
		}
	}

		(*this).suppress_piece_grille(la_piece);
		la_piece.set_y( la_piece.get_y() + 1);
		(*this).write_piece_grille(la_piece);   

	return 1;

}


int 
grid::piece_translation_B(piece& la_piece){
	// cette fonction translatte verifiant tout d'abord si c'est possible 

	// 1 - BORD PLATEAU
	if( la_piece.get_x() == 21){
		score += 10;
		return 0;
	}

	// 2 - possibilité
	for( int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i][j] != 0 && ( i == 3 || piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i + 1][j] == 0 ) ){
				if( (la_piece.get_x() - 1 + i) == 21 || grid_tab[la_piece.get_x() + i][la_piece.get_y() + j - 2] != 0) {
					score += 10;
					return 0;
				} 
			}
		}
	}

	(*this).suppress_piece_grille(la_piece);
	la_piece.set_x( la_piece.get_x() + 1);
	(*this).write_piece_grille(la_piece);   

	return 1;

}


int 
grid::piece_translation_G(piece& la_piece){
	// cette fonction translatte verifiant tout d'abord si c'est possible 

	// 1 - BORD PLATEAU
	if( la_piece.get_y() == 0){
		return 0;
	}

	// 2 - possibilité
	for( int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i][j] != 0 && (j == 0 || piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i][j - 1] == 0)){
				if( (la_piece.get_y() - 2 + j) == 0 || grid_tab[la_piece.get_x() - 1 + i][la_piece.get_y() + j - 3] != 0) {
					return 0;
				} 
			}
		}
	}

	(*this).suppress_piece_grille(la_piece);
	la_piece.set_y( la_piece.get_y() - 1);
	(*this).write_piece_grille(la_piece);   

	return 1;

}


int 
grid::piece_rotation(piece& la_piece){
	// cette fonction fait tourner la piece en verifiant que cela est possible 
	int new_rotation = (la_piece.get_rotation() + 1)%4 ;


	// 1 - BORD PLATEAU
	/*if( la_piece.get_y() == 0) || la_piece.get_y() == 19 ){
		return 0;
	}
	*/
	
	

	if (la_piece.get_name() == 2) {
		if( la_piece.get_y() == 0 || la_piece.get_y() == 1) {
			for (int i = 0; i < 4; i++) {
				if (grid_tab[la_piece.get_x()+i][la_piece.get_y()+1] != 0)
					return 0;
			}
			this->piece_translation_D(la_piece);
			this->piece_rotation(la_piece);
			(*this).write_piece_grille(la_piece);
			return 1;
		}
		if(la_piece.get_y() == 9 || la_piece.get_y() == 8) {

			for (int i = 0; i < 4; i++) {
				if (grid_tab[la_piece.get_x()+i][la_piece.get_y()-1] != 0)
					return 0;
			}
			this->piece_translation_G(la_piece);
			this->piece_rotation(la_piece);
			(*this).write_piece_grille(la_piece); 
			return 1;
		}
	}
	(*this).suppress_piece_grille(la_piece);
	// Si la piece est dans un bord on la decale pour pouvoir la faire pivoter sans etre gener par la limite
	if( la_piece.get_y() == 0) {
		this->piece_translation_D(la_piece);
		this->piece_rotation(la_piece);
	}
	
	if(la_piece.get_y() == 9 ) {
		this->piece_translation_G(la_piece);
		this->piece_rotation(la_piece);
	}


	// 2 - possibilité



	for( int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			// on test si c'est possible
			if (piece_value[la_piece.get_name()-1][new_rotation][i][j] != 0 ){
				if ( grid_tab[la_piece.get_x() - 1 + i][la_piece.get_y() + j - 2] != 0 ){
					(*this).write_piece_grille(la_piece); 
					return 0;
				}
			}
		}
	}

	la_piece.set_rotation( new_rotation );
	(*this).write_piece_grille(la_piece); 

	return 1;


}


void 
grid::print_grid(){
	// pour debeugage 
	for( int i = 0; i < 22; i++){
		for( int j = 0; j < 10; j++){
			std::cout << grid_tab[i][j] << " ";
		}
		std::cout << std::endl;
	}
}



void 
grid::verif_end(piece& la_piece) { 
	// teste si on peux rajouter la piece p a la grille 
	for( int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			// on test si c'est possible
			if (piece_value[la_piece.get_name()-1][la_piece.get_rotation()][i][j] != 0 ){
				if ( grid_tab[la_piece.get_x() - 1 + i][la_piece.get_y() + j - 2] != 0 ){
					status_partie = 0;
 				}
			}
		}
	}
}


int 
line_full(int grid_tab[22][10], int line){
	for( int j = 0; j < 10; j++){
		if (grid_tab[line][j] == 0 ){
			return 0;
		}
	}

	return 1;
}

void 
decallage_line(int grid_tab[22][10], int line){
	// on supprime ligne line et on decale tout vers le bas 
	for (int k = line  ; k != 1 ; k--){
		for (int j = 0; j < 10; j++){
			grid_tab[k][j] = grid_tab[k - 1][j];

		}

	}

	for (int j = 0; j < 10; j++){
		grid_tab[0][j] = 0;

	}
}


void
grid::clean(){
	// fonction supprime toutes les lignes entières
	int nb_full_line = 0;
	for (int i = 0; i < 22; i++){
		if ( line_full(grid_tab, i) == 1){
			// ligne pleine 
			decallage_line(grid_tab, i);
			nb_full_line++;
		}
	}

	// on ajoute score 
	switch(nb_full_line){
		case 1 : 
			score += 40;
			break;
		case 2 : 
			score += 100;
			break;
		case 3 : 
			score += 300;
			break;
		case 4 : 
			score += 1200;
			break;
	}
}











