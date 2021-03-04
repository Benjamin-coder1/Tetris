#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


#include "piece.hpp"


// initialiser la piece 
piece::piece(name nom_piece) : color_piece( (int) nom_piece), nom_piece(nom_piece), nombre_rotation(0), x(1), y(5) {};


// recuperer valeur
int piece::get_x() { return x; }
int piece::get_y() { return y; }
int piece::get_rotation() { return nombre_rotation; }
int piece::get_name() { return (int) nom_piece; }

// modifier_valeur courante
void piece::set_x( int x_d) {x = x_d ;} 
void piece::set_y( int y_d) {y = y_d ;} 
void piece::set_rotation( int nombre_rotation_d) {nombre_rotation = nombre_rotation_d ;}

// tourner 
void piece::turn() { nombre_rotation++%4; } 

// affichage nom piece
// inline std::ostream& piece::operator << ( std::ostream& stream, name nom_piece  )
// {
// 	if ( nom_piece == O) stream << "O";
// 	if ( nom_piece == I) stream << "I";
// 	if ( nom_piece == S) stream << "S";
// 	if ( nom_piece == L) stream << "L";
// 	if ( nom_piece == J) stream << "J";
// 	if ( nom_piece == T) stream << "T";
//     return stream;
// }



