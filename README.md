#TETRIS

Comment lancer le jeux ? 
	1- lancer un         >> bash compilation.sh 
	2- lancer sur un terminal    >> ./tetris   remplir le nombre de joueurs
	3- lancer sur un autre terminal   >> ./tetris_client  donner nom / adresse IP du serveur ( localhost si en local)

La partie se lance : en local c'est immédiat /  en ligne la connexion peut etre longue et infructueuse (elle fonctionne mieux en local)




Grille.cpp

On créer une classe grid, celle-ci va permettre dans la suite de contenir le jeux de chaque joueur et de faire les calculs et opérations pour chaque joueur. Chaque grille possède un attribut grid_tab, un tableau de 10*22 qui contient le jeux du joueur, cette grille contient les pieces
Le principe est simple : A chaque joueur on associe une grille dans laquelle sont placée les pieces. On commentce par poser une piece dans la grille puis grace aux différentes méthode de la grille, on peut deplacer la piece vers la droite / vers la gauche / vers le bas / faire tourner.
Une fois la piece arrivé au bout, une nouvelle piece est ajoutée à la grille et ainsi de suite. Cette classe contient les méthode suivante :



piece.cpp

Cette methode permet de créer une piece que l’on inserera dans la grille puis que l’on manipulera selon la volonté du joueur. Une fois arriver au bas de la grille la piece sera « figée » tandis qu’une nouvelle piece sera créer et ainsi de suite jusqu'à que le jeux se termine

 

value_piece.cpp

Ce fichier contient une tableau de dimension 7*4*4*4 qui permet de stocker les pieces, leur forme et leur différentes orientations.

Il y a 7 pieces : 0 / I/ S / Z / L/ J / T  chaque piece est représenté par une grille de 4 par 4. A chaque grille est associé un numero afin de pouvoir conserver les couleurs. Pour chaque piece on a 4 grille de 4 par 4, chaque grille representant une rotation de la piece. 

