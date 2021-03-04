#compilation des programmes 
g++ tetris_serveur.cpp grille.cpp piece.cpp -lpthread -o tetris
g++ tetris_client.cpp -o tetris_client -lsfml-graphics -lsfml-window -lsfml-system
