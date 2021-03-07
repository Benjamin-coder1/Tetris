#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <thread>
#include <cstdlib>
#include <random>
#include <map>

#include "grille.hpp"
 

// tirage piece
name choix_piece[7] = {O, I, S, Z, L, J, T};


int partie(grid& ma_grille, int i , int socket_descriptor){

	// on lance une partie par joueur avec des thread
	int size_msg_to_send = 3*sizeof(int) + 22*10*sizeof(int);
	int* message_to_send = (int *) malloc( size_msg_to_send);
	int piece_suivante = std::rand()%7;

	while( ma_grille.get_status_partie() ){

		// on supprime les lignes entieres 
		ma_grille.clean();

		// on lance nouvelle piece
		piece p( choix_piece[piece_suivante] );	
		ma_grille.verif_end(p);
		piece_suivante = std::rand()%7;	

		// Partie terminée
		if (ma_grille.get_status_partie() == 0) {
			// Protocole d'envoi fin de partie 	
 			message_to_send[0] = ma_grille.get_status_partie();
 			message_to_send[1] = ma_grille.get_score();
 			message_to_send[2] = piece_suivante;
  			bzero( &message_to_send[3], 22*10*sizeof(int) );

 			// envoit 
 			write(socket_descriptor, message_to_send , size_msg_to_send ); 	
 			break;
		}


		// Partie non terminée
		ma_grille.write_piece_grille(p);
		int end = 1;

 		while( end ) {

 			// Protocole d'envoit  			
 			message_to_send[0] = ma_grille.get_status_partie();
 			message_to_send[1] = ma_grille.get_score();
 			message_to_send[2] = piece_suivante;
 			memcpy( &message_to_send[3], ma_grille.data(), 22*10*sizeof(int) );

 			// envoit 
 			write(socket_descriptor, message_to_send , size_msg_to_send ); 			

 		
	    	// On récupère la commande  
	    	int touch = 0;
	    	read(socket_descriptor, &touch, sizeof(int) );
 
			// On execute la commande
	    	if ( (char) touch == 'a') {
	    		ma_grille.piece_translation_G(p);
	    	}

	    	else if ( (char) touch == 'z') {
	    		ma_grille.piece_translation_D(p);
	    	}

	    	else if ( (char) touch == 'r') {
	    		ma_grille.piece_rotation(p);
	    	}  

	    	else {
	    		end = ma_grille.piece_translation_B(p);
	    	}
 
	    }			

	}

	std::cout << ma_grille.get_name() << " a terminé sa partie" << std::endl;
	

	return 1;	 

}


int main( int argc, char *argv[]  ){

	// ------------  CREATION SERVEUR ------------------ //
	int nombre_joueurs;
	std::cout << "Nombre de joueurs : ";
	scanf("%d", &nombre_joueurs);
		
	// creation d'un socket 
	int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_descriptor < 0){
		std::cout << "Echec de connexion " << std::endl;
		return 0;
	}

	// Initialisation du serveur 
	struct sockaddr_in serv_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	int port_number = 11003;

	serv_addr.sin_family = AF_INET;	
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);		
	serv_addr.sin_port = htons(port_number);

	if (bind(socket_descriptor, (struct sockaddr *) &serv_addr, sizeof(serv_addr) ) < 0) {
		std::cout << "Echec de la connexion du socket au réseau" << std::endl;
		return 0;
	}     

	listen(socket_descriptor,5);
	std::cout << "Serveur pret ! " << std::endl;
	std::cout << "Numéro de port : " << port_number << std::endl << std::endl;

	// ------------  MISE EN PLACE PARTIE ------------------ //

	// Creation du jeu 
	grid ma_grille[nombre_joueurs];

	// Connexion avec les autres joueurs
	int client_descriptor[nombre_joueurs];
	struct sockaddr_in client_adresse[nombre_joueurs];
	socklen_t client = sizeof(client_adresse[0]);

	

	for ( int i = 0; i < nombre_joueurs; i++){
		// connexion client 
		client_descriptor[i] = accept(socket_descriptor, (struct sockaddr *) &client_adresse[i], &client);

		// lecture du nom 
		char nom_joueur[25];
		read(client_descriptor[i] , nom_joueur, 25 ); 

		// sauvegarde du nom
		ma_grille[i].set_name( nom_joueur ) ;
		std::cout << nom_joueur << " connecté " << std::endl;
	}


	std::cout << std::endl <<  "Lancement de la partie " << std::endl << std::endl;
	int lancement = 1;
	for ( int i = 0; i < nombre_joueurs; i++){		
		write( client_descriptor[i], &lancement, sizeof(int) ); // notification lancement partie
	}




	// ------------  LANCEMENT PARTIE ------------------ //
	

	// --- DEBUT --- on lance une partie par thread
	std::thread liste_joueur[nombre_joueurs];
	for (int i = 0; i < nombre_joueurs; i++){
		liste_joueur[i] = std::thread(partie, std::ref(ma_grille[i]), i, client_descriptor[i]);
	}

	// --- FIN --- On joint tous les joueurs 
	for (int i = 0; i < nombre_joueurs; i++){
		liste_joueur[i].join();
	}

	// ------------  FIN DE LA PARTIE ------------------ //

	// on envoie le nom du winner
	int ind_max = 0;
	for (int i = 1; i < nombre_joueurs ; i ++){
		if( ma_grille[i].get_score() > ma_grille[ind_max].get_score() ){
			ind_max = i;
		}
	}


	// fermeture socket 
	for (int i = 0; i < nombre_joueurs; i++){
		// on envoit nom winner 
		char nom_winner[25]; memcpy(nom_winner, ma_grille[ind_max].get_name().c_str() , 25);
		write(client_descriptor[i], nom_winner, 25 );		
		// fermeture connexion
		close(client_descriptor[i]);
	}

 
	return 1;


}










