# TETRIS

Comment lancer le jeux ? 

	>> bash compilation.sh 
	>> ./tetris      sur un terminal
	>> ./tetris_client  pour chaque jouer ( "localhost" si jeux en local)

La partie se lance : en local c'est immédiat /  en ligne la connexion peut etre longue et infructueuse (elle fonctionne bien en local)


Le jeux fonctionne sous forme client / serveur : 
<ul>
	<li> <i> <b> Client </li> </b> : Recoit  un tableau d'entier de 22*10 ainsi que diverses informations sur le joueur et les affiche. </li>
 	<li> <i> <b> Serveur </li> </b>  : Pour chaque joueur le serveur recoit les touches, fais les calcul de mise a jour de la grille puis l'envoit au client afin de l'afficher </li>
</ul>
	





## Grille.cpp

On créer une classe grid, chaque joueur aura la sienne. Celle-ci va permettre dans la suite de contenir le jeux de chaque joueur et de faire les calculs et opérations pour chaque joueur. Chaque grille possède un attribut principal grid_tab : un tableau de 10*22 qui contient les pieces

Pour chaque joueur on procède ainsi : 
	1 -  On commence par créer aléatoirement une piece 
	2 -  On pose cette piece dans la grille 
	3 -  On recoit les touches du joueur et on actualise la grille jusqu'a ce que la piece soit posée. 
	4 - La piece est alors "figée" dans la grille et on en crée une nouvelle afin de recommencer 




## piece.cpp

Cette methode permet de créer une piece que l’on inserera dans la grille puis que l’on manipulera selon la volonté du joueur. Une fois arriver au bas de la grille la piece sera « figée » tandis qu’une nouvelle piece sera créer et ainsi de suite jusqu'à que le jeux se termine

 

## value_piece.cpp

Ce fichier contient une tableau de dimension 7*4*4*4 qui permet de stocker les pieces, leur forme et leur différentes orientations.

Il y a 7 pieces : 0 / I/ S / Z / L/ J / T  chaque piece est représenté par une grille de 4 par 4. A chaque grille est associé un numero afin de pouvoir conserver les couleurs. Pour chaque piece on a 4 grille de 4 par 4, chaque grille representant une rotation de la piece. 


## tetris_client.cpp 

Ce fichier se trouve coté client, elle est lancé par chaque joueur afin de se connecter au serveur et de pouvoir jouer. L'algorithme fonction ainsi 

	1 - Etablissement d'une connexion avec le serveur et attente du lancement de la partie 
	2 - Reception des données : grille du jeux / score / status de la partie / nom joueur 
	3 - Affichage de la grille et reception de la touche du choix du joueur 
	4 - Envoit de cette touche au serveur afin de mettre a jour la grille 
	5 - Retour a 3 tant que le joueur n'a pas perdu 

## tetris_serveur.cpp 

Ce fichier est situé coté serveur, un seul joueur le lance puis envoie aux autres joueurs sont addresse IP. Le principe est le suivant : 

	1 - Attente de connexion de tous les joueurs
	2 - Creation d'un thread par joueur qui s'occupe de la partie dans lequel 
		a - Creation d'un objet grid
		b - Reception d'une touche
		c - MAJ de la grille 
		d - Envoit de cette nouvelle grille puis retour b 
