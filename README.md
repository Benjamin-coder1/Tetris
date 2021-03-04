# TETRIS

Comment lancer le jeux ? 

	>> bash compilation.sh 
	>> ./tetris      sur un terminal
	>> ./tetris_client  pour chaque jouer ( "localhost" si jeux en local)

La partie se lance : en local c'est immédiat /  en ligne la connexion peut etre longue et infructueuse (elle fonctionne bien en local)


Le jeux fonctionne sous forme client / serveur : 
<ul>
	<li> <i> <b> Client </i> </b> : Recoit  un tableau d'entier de 22*10 ainsi que diverses informations sur le joueur et les affiche. </li>
 	<li> <i> <b> Serveur </i> </b>  : Pour chaque joueur le serveur recoit les touches, fais les calcul de mise a jour de la grille puis l'envoit au client afin de l'afficher </li>
</ul>

## Grille.cpp

On créer une classe grid, chaque joueur aura la sienne. Celle-ci va permettre dans la suite de contenir le jeux de chaque joueur et de faire les calculs et opérations pour chaque joueur. Chaque grille possède un attribut principal grid_tab : un tableau de 10*22 qui contient les pieces. Le principe de fonctionement de la grille est le suivant, on créer une piece que l'on insère dans la grille puis que l'on deplace grace aux méthodes de la grille.

Pour chaque joueur on procède ainsi : 
<ol>
	<li>  On commence par créer aléatoirement une piece </li>
	<li>  On pose cette piece dans la grille </li>
	<li>  On recoit les touches du joueur et on actualise la grille jusqu'a ce que la piece soit posée. </li>
	<li>  La piece est alors "figée" dans la grille et on en crée une nouvelle afin de recommencer </li>

</ol>


## piece.cpp

Cette methode permet de créer une piece que l’on inserera dans la grille puis que l’on manipulera selon la volonté du joueur. Une fois arriver au bas de la grille la piece sera « figée » tandis qu’une nouvelle piece sera créer et ainsi de suite jusqu'à que le jeux se termine

 

## value_piece.cpp

Ce fichier contient une tableau de dimension 7*4*4*4 qui permet de stocker les pieces, leur forme et leur différentes orientations.

Il y a 7 pieces : <i><b> O  I  S  Z  L  J  T </i></b> chaque piece est représenté par une grille de 4 par 4. A chaque grille est associé un numero afin de pouvoir conserver les couleurs. Pour chaque piece on a 4 grille de 4 par 4, chaque grille representant une rotation de la piece. 


## tetris_client.cpp 

Ce fichier se trouve coté client, elle est lancé par chaque joueur afin de se connecter au serveur et de pouvoir jouer. L'algorithme fonction ainsi 

<ol>
	<li> Etablissement d'une connexion avec le serveur et attente du lancement de la partie  </li>
	<li> Reception des données : grille du jeux / score / status de la partie / nom joueur </li>
	<li> Affichage de la grille et reception de la touche du choix du joueur </li>
	<li> Envoit de cette touche au serveur afin de mettre a jour la grille </li>
	<li> Retour a 3 tant que le joueur n'a pas perdu </li>
</ol>


## tetris_serveur.cpp 

Ce fichier est situé coté serveur, un seul joueur le lance puis envoie aux autres joueurs sont addresse IP. Le principe est le suivant : 

<ol>
	<li> Attente de connexion de tous les joueurs </li>
	<li> Creation d'un thread par joueur qui s'occupe de la partie dans lequel </li>
	<ol> 
		<li> Creation d'un objet grid </li>
		<li> Reception d'une touche</li> 
		<li> MAJ de la grille </li>
		<li> Envoit de cette nouvelle grille puis retour ii </li>
	</ol>
</ol>
