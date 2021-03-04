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
	int size_msg_to_send = 2*sizeof(int) + 22*10*sizeof(int);
	int* message_to_send = (int *) malloc( size_msg_to_send);

	while( ma_grille.get_status_partie() ){

		// on supprime ligne entiere 
		ma_grille.clean();

		// on lance nouvelle piece
		piece p( choix_piece[ std::rand()%7] );	
		ma_grille.verif_end(p);	

		if (ma_grille.get_status_partie() == 0) {
			// Protocole d'envoit  			
 			message_to_send[0] = ma_grille.get_status_partie();
 			message_to_send[1] = ma_grille.get_score();
  			bzero( &message_to_send[2], 22*10*sizeof(int) );

 			// envoit 
 			write(socket_descriptor, message_to_send , size_msg_to_send ); 	
		}

		ma_grille.write_piece_grille(p);

		int end = 1;
 		while( end ) {

 			// Protocole d'envoit  			
 			message_to_send[0] = ma_grille.get_status_partie();
 			message_to_send[1] = ma_grille.get_score();
 			memcpy( &message_to_send[2], ma_grille.data(), 22*10*sizeof(int) );

 			// envoit 
 			write(socket_descriptor, message_to_send , size_msg_to_send ); 			

 		
	    	// On récupère la commande  
	    	int touch = 0;
	    	read(socket_descriptor, &touch, sizeof(int) );
 
			// On execute la commande
	    	if ( (char) touch == 'a') {
	    		ma_grille.piece_translation_G(p);
	    		//end = ma_grille.piece_translation_B(p);
	    	}

	    	else if ( (char) touch == 'z') {
	    		ma_grille.piece_translation_D(p);
	    		//end = ma_grille.piece_translation_B(p);
	    	}

	    	else if ( (char) touch == 'r') {
	    		ma_grille.piece_rotation(p);
	    		//end = ma_grille.piece_translation_B(p);
	    	}  

	    	else {
	    		end = ma_grille.piece_translation_B(p);
	    	}
 
	    }			

	}

	std::cout << ma_grille.get_name() << " a terminé sa partie" << std::endl;

	return 1;	 

}


void affichage_score( grid* ma_grille, int nombre_joueurs, int* status_partie){
	// affiche les score sur le serveur
	while( *status_partie ){
		for( int i = 0; i < nombre_joueurs - 1; i++){
			std::cout <<  ma_grille[i].get_name() << " : " << ma_grille[i].get_score() << " | ";
		}

		std::cout << ma_grille[nombre_joueurs - 1].get_name() << " : " << ma_grille[nombre_joueurs - 1].get_score() << "\r" << std::flush;
		std::this_thread::sleep_for( std::chrono::seconds(2));
	}
	
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
	int status_partie = 1;
	

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



	// ------------  LANCEMENT PARTIE ------------------ //
	

	// --- DEBUT --- on lance une partie par thread
	std::thread liste_joueur[nombre_joueurs];
	for (int i = 0; i < nombre_joueurs; i++){
		liste_joueur[i] = std::thread(partie, std::ref(ma_grille[i]), i, client_descriptor[i]);
	}

	std::thread score( affichage_score, (grid* ) ma_grille, nombre_joueurs, &status_partie);

	// --- FIN --- On joint tous les joueurs 
	for (int i = 0; i < nombre_joueurs; i++){
		liste_joueur[i].join();
	}

	// ------------  FIN DE LA PARTIE ------------------ //

 	status_partie = 0;
	score.join();

	// on affiche le nom du winner
	int ind_max = 0;
	for (int i = 1; i < nombre_joueurs ; i ++){
		if( ma_grille[i].get_score() > ma_grille[ind_max].get_score() ){
			ind_max = i;
		}
	}

	// fermeture socket 
	for (int i = 0; i < nombre_joueurs; i++){
		close(client_descriptor[i]);
	}

	std::cout << std::endl << "LE WINNER EST : " << ma_grille[ind_max].get_name() << std::endl;


	return 1;


}










