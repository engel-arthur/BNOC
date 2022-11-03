#include "../Headers/Game.h"

using namespace std;

Game::Game(){
	loadDefault();
}

Game::Game(int nbPlayer, int nbBot) : nbHumans(nbPlayer),nbBots(nbBot), nbShips(5),isUndo(false), PreparePlayer(true){
	shipNames = new string[nbShips];
	shipNames[0]="porte_avion";
	shipNames[1]="croiseur";
	shipNames[2]="contre-torpilleur";
	shipNames[3]="sous-marin";
	shipNames[4]="torpilleur";
	loadedSave=false;

	players = new Player*[nbHumans + nbBots];

	for(int i=0;i<nbHumans; i++)
	{
		players[i] = new Player("Joueur " + to_string(i+1), 10, 10, shipNames, nbShips, true);
	}
	for(int i=0;i<nbBots; i++)
	{
		players[nbHumans + i] = new Player("Joueur " + to_string(nbHumans + i + 1), 10, 10, shipNames, nbShips, false);
	}
}

Game::Game(string src, bool isConfig):PreparePlayer(true){
	nbBots=0;
	nbHumans=0;
	bool loaded = false;
	if(isConfig){
		loadedSave=false;
		players = new Player*[1];
		players[0] = new Player;//initialisation
		isUndo = false;
		if(!readConfig(src)){
			loadDefault();
		}
	}else{
		loaded = SaveManagement::loadSave(src, nbHumans, nbBots);
		if(loaded)
		{
			players = SaveManagement::getPlayers();
		SaveManagement::readLog(players, nbHumans+nbBots,choixJoueur);
		PreparePlayer = false;
		loadedSave=true;
		}
		else
		{
			loadDefault();
		}
	}
}
Game::~Game(){

}

void Game::loadDefault(){
	loadedSave=false;
	PreparePlayer = true;
	nbHumans = 1;
	nbBots = 1;
	nbShips = 5;
	shipNames = new string[nbShips];
	shipNames[0]="porte_avion";
	shipNames[1]="croiseur";
	shipNames[2]="contre-torpilleur";
	shipNames[3]="sous-marin";
	shipNames[4]="torpilleur";

	players = new Player*[nbHumans + nbBots];
	players[0] = new Player("Joueur 1", 10, 10, shipNames, nbShips, true);
	players[1] = new Player("Joueur 2", 10, 10, shipNames, nbShips, false);
	// players[2] = new Player("Joueur 3", 10, 10, shipNames, nbShips, false);
	// players[3] = new Player("Joueur 4", 10, 10, shipNames, nbShips, false);
}

void Game::start(){
		SaveManagement::setPlayers(players,nbHumans+nbBots);
		this->heightS = 40;
		this->widthS = 130;
		int x = 0;
		int y = 0;
		bool hit = false;

		GameWindow = new Window(this->heightS,this->widthS,0,0);
		GameWindow->setCouleurFenetre(10);
		srand(time(NULL)); // initialisation de rand

		if(PreparePlayer){
			for (int i = 0; i < nbHumans + nbBots; i++) {
				PreparationPlayer::PreparePlayer(players[i]);
			}
		}
		// GameWindow->showPixels();
		// animationOUT();
		//CHOIX DU PREMIER JOUEUR ALEATOIREMENT
		if(PreparePlayer){
			animationRandomPlayer(2);
		}
		if(!loadedSave)
			SaveManagement::emptyLog();
		//DEBUT DU JEU
			//Clear
			GameWindow->fillPixels(10,this->heightS,this->widthS);
			GameWindow->showPixels();

			while (getAlivePlayers() > 1) {
				do{
					int choicePosition = 0;
					do{
						isUndo = false;
						choicePosition = ennemyChoice();
					}while(isUndo || !choiceEnnemyPosition(choicePosition,x,y));
					bool sunk;
					SaveManagement::writeLog(choixJoueur,choicePosition,y,x);
					hit=players[choixJoueur]->attack(players[choicePosition], y, x, sunk);
					if(hit){
						players[choixJoueur]->increaseScore(100);
						if(sunk)
						{
							players[choixJoueur]->increaseScore(100);
							Message::AlertAnims(players[choixJoueur]->getName() + "/na coule un bateau de/n" + players[choicePosition]->getName(), "inGame1.txt", "inGame2.txt", 8, 50, 40, 29, 3, 18);
						}
						else
							Message::AlertAnims(players[choixJoueur]->getName() + "/na touche/n" + players[choicePosition]->getName() + "/n", "inGame1.txt", "inGame2.txt", 8, 50, 40, 29, 2, 18);
					}else{
						players[choixJoueur]->increaseScore(-30);
						Message::AlertAnims(players[choixJoueur]->getName() + "/na rate", "inGame1.txt", "inGame2.txt", 8, 50, 40, 29, 3, 18);
					}
					//usleep(500000
				}while(hit && getAlivePlayers() > 1);
				do{
					GameWindow->fillPixels(10,this->heightS,this->widthS);
					GameWindow->showPixels();
					choixJoueur++;
					if(choixJoueur==nbBots+nbHumans)
						choixJoueur=0;
				}while(!players[choixJoueur]->isAlive());
			}
			Score score;//HELP ?
			for(int i=0; i<nbBots+nbHumans; i++)
			{
				score.setNewScore(players[i]->getScore(), players[i]->getName());
			}
		Message::AlertAnims("! BRAVO !/n"+players[choixJoueur]->getName()+"/nVOUS AVEZ GAGNE", "Victory.txt", "Victory2.txt", 32, 100, 15, 5, 22, 18);
		usleep(500000);

	/* DEUXIEME PARTIE ~ DEBUT DE LA PARTIE */

		//	Choix aléatoire de l'ordre des joueurs

		//	Quand c'est au tour d'un joueur celui ci doit :
		//		-> Choisir le joueur à attaquer
		//		-> Choisir quelle case attaquer de son adversaire visé
		//		-> Vérifier si la case choisie est déjà prise 'l'éviter'
		//		-> Quand le joueur a choisi la case à attaquer, attaquer le joueur en question
		//
		//		-> à tout moment il est possible de revenir en arrière dans le Choix
		//		-> Un joueur peut attaquer ou être attaquer, seulement si il est en vie

		//	Tant qu'il y a plus de deux joueurs en vie la partie continue
		//	Si un joueur touche un adversaire il peut rejouer
		//	Chaques actions doit être enregistré dans un fichier log
		//	Il est possible à n'importe quel moment d'enregistrer la partie en question

	/* TROISIEME PARTIE ~ FIN DE LA PARTIE */

		//	Quand la partie est finie il faut checker si il y a eu un nouveau meilleur Score
		//	Si oui affichers les nouveaux meilleurs scores
		//	Possibilité de pouvoir enregistrer sa partie
}

int Game::ennemyChoice(){
	string ARROW = ">>";
	int choicePosition = 0;
	string message = "a toi de jouer, " + players[choixJoueur]->getName() +" !";

	//ON AFFICHE L'ARROW
		while (!players[choicePosition]->isAlive() || choicePosition == choixJoueur) {
			choicePosition++;
		}
	//On affiche le territory préseulement
		int hterr, wterr;
		int** showterr = players[choicePosition]->getTerritory()->afficheTerrForEnn(hterr, wterr);
		GameWindow->setPixels(showterr, hterr, wterr, 0,this->widthS/2 - wterr);

	//On affiche le territory de la personne qui joue
		int hterrP, wterrP;
		int** showterrP = players[choixJoueur]->getTerritory()->afficheTerrForEnn(hterrP, wterrP);
		GameWindow->setPixels(showterrP, hterr, wterr, this->heightS - hterrP,this->widthS/2 - wterrP);

		GameWindow->showPixelsHARD();

		GameWindow->print(0,0, message , COLOR_PAIR(52)); //Message informatif
		GameWindow->print(1,choicePosition +1, ARROW, COLOR_PAIR(51)); //Arrow
		Message::Alert("Choisissez le joueur a attaquer/npuis appuyez sur Entree","inGame1.txt", 8, 50, 40, 17, 3, 18);
		Message::Alert("b -> retour en arriere/ns -> sauvegarder la partie","inGame1.txt", 8, 50, 40, 27, 3, 18);
	//ON AFFICHE LES JOUEURS SUR LE CÔTE
		for (int i = 0; i < nbHumans + nbBots; i++) {
			string scoreString = "";
			stringstream ss;
      ss << players[i]->getScore();
			scoreString = "\t -> " + ss.str() + " pts";
			if(i == choixJoueur)
				GameWindow->print(5,1+i, players[i]->getName() + " " + scoreString ,COLOR_PAIR(52));
			else if(!players[i]->isAlive())
				GameWindow->print(5,1+i, players[i]->getName() + " " + scoreString ,COLOR_PAIR(50));
			else{
				GameWindow->print(5,1+i, players[i]->getName() + " " + scoreString ,COLOR_PAIR(51));
			}
		}

	int c=0;//caractère
	bool printInfo = false;

	if(players[choixJoueur]->isHuman()){//si c'est un humain
		while(c != '\n' && !isUndo){
			c = wgetch(GameWindow->getWindow()); //On prend la touche appuyée
			if (c == 'A') {//haut
				GameWindow->print(1,choicePosition +1, "  ", COLOR_PAIR(51));
				choicePosition--;
				if(choicePosition < 0) choicePosition = nbBots + nbHumans -1;
				while (!players[choicePosition]->isAlive() || choicePosition == choixJoueur) {
					choicePosition--;
					if(choicePosition < 0) choicePosition = nbBots + nbHumans -1;
				}
				printInfo = true;
			}
			if (c == 'B') {//bas
				GameWindow->print(1,choicePosition +1, "  ", COLOR_PAIR(51));
				choicePosition++;
				choicePosition = choicePosition % (nbBots + nbHumans);
				while (!players[choicePosition]->isAlive() || choicePosition == choixJoueur) {
					choicePosition++;
					choicePosition = choicePosition % (nbBots + nbHumans);
				}
				printInfo = true;
			}
			if(c == 's')//sauvegarder
			{
				SaveManagement::saveState(ConfigManagement::getNameSave(),players, nbHumans + nbBots);//on sauvegarde en générant un nom auto
				Message::Alert("Sauvegarde reussie/n"+ConfigManagement::getNameSave(),"inGame1.txt", 8, 50, 40, 27, 3, 18);
			}
			if(c == 'b')//back
			{
				int idLastAttacker=0, idLastAttacked=0, lastY=0, lastX=0;//id du dernier attaquant, dernier attaqué, position d'attaque (syntaxe du log)
				bool gotLastMove=false;//savoir si on a réussi à obtenir le dernier coup
				do
				{
					gotLastMove=SaveManagement::getLastMove(idLastAttacker,idLastAttacked,lastY,lastX);//on chope le dernier coup et on renvoie si oui ou non on a réussi à l'avoir (cas ou on remonte toute la partie)
					if(gotLastMove)//si on a eu le premier coup
					{
						players[idLastAttacked]->getTerritory()->undoAttack(lastX,lastY);//on défait l'attaque
						choixJoueur = idLastAttacker;//on change le joueur actif
						printInfo = true;//on indique qu'il faut actualiser l'affichage
						isUndo = true;//on indique qu'on a défait un coup
						Message::Alert("Retour en arrière","inGame1.txt", 8, 50, 40, 27, 3, 18);
					}
				}while(!players[idLastAttacker]->isHuman() && gotLastMove);//si c'est un bot et qu'on est pas au début de la partie, on continue à remonter

			}
			if(printInfo){
				showterr = players[choicePosition]->getTerritory()->afficheTerrForEnn(hterr, wterr);
				GameWindow->setPixels(showterr, hterr, wterr, 0,this->widthS/2 - wterr);
				GameWindow->showPixels();
				GameWindow->print(1,choicePosition +1, ARROW, COLOR_PAIR(51));//actu affichage
			}
		}
	} else{
		do
		{
			choicePosition = rand() % (nbHumans + nbBots);//si c'est un bot il choisit au hasard
		} while (choicePosition == choixJoueur || !players[choicePosition]->isAlive());

		// for (int i = 0; i < choicePosition; ++i)
		// {
		// 	GameWindow->print(1,i +1, "  ", COLOR_PAIR(51));
		// 	GameWindow->print(1,i +2, ARROW, COLOR_PAIR(51));
		// 	//usleep(200000);
		// }
		//usleep(500000);
	}



	GameWindow->print(1,choicePosition +1, "  ", COLOR_PAIR(51));//pour clear les messages
	string clear = "";
	for (unsigned int i = 0; i < message.size(); i++)
	{
		clear += " ";
	}
	//GameWindow->print(0,0, clear , COLOR_PAIR(52));
	GameWindow->clear();
	return choicePosition;
}

bool Game::choiceEnnemyPosition(int ennemy, int &x, int &y){//choisir la case à attaquer
	x = 0, y = 0;//position du curseur
	string SELECT = "><";//icone de sélection de case
	int hTerr, wTerr;
	int direction = 1; //permet de savoir de quelle direction on vient pour le saut de case
	Territory *terr = players[ennemy]->getTerritory();//on prend le territoire de l'ennemi attaqué

	Window* TERRWindow = new Window(terr->getHeight(),terr->getWidth() * 2,this->widthS/2 - terr->getWidth(),this->heightS/2 - terr->getHeight()/2);//fenetre du territoire
	Window* border = new Window(terr->getHeight()+2,(terr->getWidth()+2) * 2,this->widthS/2 - terr->getWidth()-2,this->heightS/2 - terr->getHeight()/2-1);//fenetre de bordure (parce que els bordures buggent)
	TERRWindow->setCouleurFenetre(10);
	border->setCouleurFenetre(10);
	border->fillPixels(1,border->getHauteur(), border->getLargeur());
	border->showPixels();

	int** pixelTerr = terr->afficheTerrForEnn(hTerr, wTerr);
	TERRWindow->setPixels(pixelTerr, hTerr, wTerr);//on afficher la matrice correspondant aux cases attaquées de l'ennemi
	TERRWindow->showPixels();
	TERRWindow->print(x*2,y,SELECT, COLOR_PAIR(10) | A_BOLD);

	GameWindow->print(this->widthS/2 - players[ennemy]->getName().size()/2,this->heightS/2 + terr->getHeight()/2 + 2, players[ennemy]->getName(), COLOR_PAIR(51) | A_BOLD);
	if(players[choixJoueur]->isHuman()){
		Message::Alert("Deplacez vous a l'aide du pave directionnel,/npuis appuyez sur Entree pour attaquer/n/nTouche a : retour arriere","inGame1.txt", 8, 50, 40, 29, 2, 18);
	}
	else{
		Message::Alert("CPU/nATTACK!","inGame1.txt", 8, 50, 40, 29, 3, 18);
	}
	if(players[choixJoueur]->isHuman()){
		int c = 0;
		while (c != '\n') {//tant qu'on n'appuie pas sur entrée
			do
			{
				if(x>=0 && y>=0 && x<=terr->getWidth()-1 && y<=terr->getHeight()-1)
				{
					if(terr->alreadyAttacked(y,x))
					{
						TERRWindow->print(x*2,y,"  ", COLOR_PAIR(pixelTerr[y][x]) | A_BOLD);
						switch(direction)
						{
							case 0://gauche
								x--;
								break;
							case 1://droite
								x++;
								break;
							case 2://haut
								y--;
								break;
							case 3://bas
								y++;
								break;

						}
					}
				}
				if(x<0)//fonctions de saut d'écran
				{
					x=terr->getWidth()-1;
					y--;
				}
				if(x>=terr->getWidth())
				{
					x=0;
					y++;
				}
				if(y<0)
				{
					y=terr->getHeight()-1;
				}
				if(y>=terr->getHeight())
				{
					y=0;
				}
			}while(terr->alreadyAttacked(y,x));//on bouge le curseur tant que la case est attaquée (ça inclut la position 0,0 par défaut)
			TERRWindow->print(x*2,y,SELECT, COLOR_PAIR(10) | A_BOLD);
			c = wgetch(GameWindow->getWindow());
			TERRWindow->print(x*2,y,"  ", COLOR_PAIR(pixelTerr[y][x]) | A_BOLD);
			if(c == 'A'){ //TOUCHE HAUT, on indique la direction d'où vient le curseur pour savoir comment sauter si on rencontre une case attaquée
				y--;
				direction = 2;
			}
			if(c == 'B'){ //BAS
				y++;
				direction = 3;
			}
			if(c == 'C'){ //DROITE
				x++;
				direction = 1;
			}
			if(c == 'D'){ //GAUCHE
				x--;
				direction = 0;
			}
			if(c == 'a'){ //annuler, pour choisir un autre ennemi
				string clearText = "";
				for (unsigned int i = 0; i < players[ennemy]->getName().size(); i++)
				{
					clearText += " ";
				}
				GameWindow->print(this->widthS/2 - players[ennemy]->getName().size()/2,this->heightS/2 + terr->getHeight()/2 + 2, clearText, COLOR_PAIR(51) | A_BOLD);
				delete TERRWindow;
				delete border;
				return false;
			}
		}
	}else{//si c'est un bot, random
		do{
			x = rand() % terr->getWidth();
			y = rand() % terr->getHeight();
		}while(terr->alreadyAttacked(y,x));
		for(int i=0;i<x;i++)///animation
		{
			if(i>0)
				TERRWindow->print((i-1)*2,0,"  ", COLOR_PAIR(pixelTerr[0][i-1]) | A_BOLD);
			TERRWindow->print(i*2,0,SELECT, COLOR_PAIR(10) | A_BOLD);
			usleep(50000);
		}
		TERRWindow->print((x*2)-2,0,"  ", COLOR_PAIR(pixelTerr[0][x-1]) | A_BOLD);
		for(int j=0;j<y;j++)
		{
			if(j>0)
				TERRWindow->print(x*2,j-1,"  ", COLOR_PAIR(pixelTerr[j-1][x]) | A_BOLD);
			TERRWindow->print(x*2,j,SELECT, COLOR_PAIR(10) | A_BOLD);
			usleep(50000);
		}
		//usleep(100000);
	}

	string clearText = "";
	for (unsigned int i = 0; i < players[ennemy]->getName().size(); ++i)
	{
		clearText += " ";
	}
	GameWindow->print(this->widthS/2 - players[ennemy]->getName().size()/2,this->heightS/2 + terr->getHeight()/2 + 2, clearText, COLOR_PAIR(51) | A_BOLD);
	delete TERRWindow;
	delete border;

	return true;
}

bool Game::readConfig(string cfg){
	string filename = "Config/" + cfg;
	//cout << filename << endl;
	ifstream file(filename.c_str(), ios::in);
	if(file){
		string cvar;//variable de la config lue
		string line;//ligne lue
		string shipFullPath;
		string shipPath = "Ressources/Ship/";
		int invalidShips=0;
		int lCount=0;//compteur de lignes (et donc de joueurs)
		int cvarCount=0;//compteur de variables (et donc indirectement de bateaux)
		while(getline(file, line))//tant qu'on peut lire des lignes de fichier, en les mettant dans line
		{
			lCount++;//on augmente le nombre de lignes (=joueurs)
			istringstream iss(line);//on crée un stream pour lire mot par mot
			string* AllShip;
			string name;
			bool isHuman = false;
			int tHeight = 10;
			int tWidth = 10;
			int nbShips = 0;
			while(iss >> cvar)//tant qu'on peut lire des mots dans la ligne, en les mettant dans cvar
			{
				//cout << cvar << endl;
				cvarCount++;//on augmente le compteur de cvar de la ligne, pour savoir quelle variable on lit
				switch(cvarCount){ //cas par cas, on assigne ce qu'on récupère de la config dans les variables correspondantes, avec des checks si les variables sont valides
					case 1:
						if(cvar == "bot"){
							isHuman = false;
							nbBots++;
						}else if(cvar == "hum"){
							isHuman = true;
							nbHumans++;
						}else
							return false;
						break;
					case 2:
						name = cvar;
						break;
					case 3:
						for(unsigned int i(0); i< cvar.size(); i++)
						{
							if(cvar.at(i)!='0' && cvar.at(i)!='1' && cvar.at(i)!='2' && cvar.at(i)!='3' && cvar.at(i)!='4' && cvar.at(i)!='5' && cvar.at(i)!='6' && cvar.at(i)!='7' && cvar.at(i)!='8' && cvar.at(i)!='9')
							{
								return false;
							}
							else
								tHeight=stoi(cvar);
						}
						//DO
						break;
					case 4:
						for(unsigned int i(0); i< cvar.size(); i++)
						{
							if(cvar.at(i)!='0' && cvar.at(i)!='1' && cvar.at(i)!='2' && cvar.at(i)!='3' && cvar.at(i)!='4' && cvar.at(i)!='5' && cvar.at(i)!='6' && cvar.at(i)!='7' && cvar.at(i)!='8' && cvar.at(i)!='9')
							{
								return false;
							}
							else
								tWidth=stoi(cvar);
						}
						//DO
						break;
					default:
						shipFullPath = shipPath + cvar + ".ship";
						ifstream ship(shipFullPath.c_str(), ios::in);
						if(ship)//Merci les vect.. ah non
						{
							string* shipListTEMP = new string[cvarCount-invalidShips-4];
							for (int i = 0; i < cvarCount-invalidShips-5; ++i){
								shipListTEMP[i] = AllShip[i];
							}
							shipListTEMP[cvarCount-invalidShips-5]=cvar;
							AllShip = new string[cvarCount-invalidShips-4];
							for (int i = 0; i < cvarCount-invalidShips-4; ++i){
								AllShip[i] = shipListTEMP[i];
							}
							nbShips++;
							delete[] shipListTEMP;
						}
						else
							invalidShips++;
						break;
				}
			}
			if(cvarCount-invalidShips-4==0)//si on ne trouve aucun bateau valide ou aucun tout court, on retourne false
				return false;
			//cout << "lcount : " << lCount << endl;
			Player** playersTEMP = new Player*[nbBots+nbHumans];
			for(int i = 0; i < nbBots+nbHumans; i++){
				playersTEMP[i]=players[i];
			}
			playersTEMP[nbBots+nbHumans-1] = new Player(name, tHeight, tWidth, AllShip, nbShips, isHuman);
			//cout << endl << "playersTEMP : " << playersTEMP[0]->getName() << endl;
			players = new Player*[nbBots+nbHumans];
			for(int i = 0; i < nbBots+nbHumans; i++){
				players[i]=playersTEMP[i];
				//cout << endl << "players : " << players[i]->getName() << endl;
			}
			cvarCount=0;//on réinitialise le nombre de variables lues, pour passer au prochain joueur
			delete[] playersTEMP;
		}
		file.close();
		if(nbBots+nbHumans<2) return false;
	}
	else {return false;}
	return true;
}

void Game::animationOUT(unsigned int speed){
	if(speed == 0) speed = 1;
	//ANIMATION DE SORTIE
		int bnocLogoHauteur;
		int bnocLogoLargeur;
		int** bnocLogo = Picture::getPicture("Bnoc.txt", &bnocLogoHauteur, &bnocLogoLargeur);

		//Récupère l'image de transition
		int transitionHauteur;
		int transitionLargeur;
		int** transitionImage = Picture::getPicture("SplashScreen2.txt", &transitionHauteur, &transitionLargeur);

		int position=0;
		while(position <= this->widthS + bnocLogoLargeur){
			// GameWindow->fillPixels(16, bnocLogoHauteur, bnocLogoLargeur,heightS/2 - bnocLogoHauteur/2 - 11,widthS/4 - bnocLogoLargeur/2 + positionTemp);
			position++;

			// GameWindow->setPixels(bnocLogo, bnocLogoHauteur, bnocLogoLargeur,heightS/2 - bnocLogoHauteur/2 - 11,widthS/4 - bnocLogoLargeur/2 + positionTemp);
			for (int i = 0; i < this->heightS/bnocLogoHauteur + 5; i++) {
				GameWindow->setPixels(bnocLogo, bnocLogoHauteur, bnocLogoLargeur,bnocLogoHauteur*i, - (bnocLogoLargeur*(i+1))/2 + position);
			}
			GameWindow->setPixels(transitionImage, transitionHauteur, transitionLargeur, 0, -transitionLargeur*1.8 + position*1.1);
			GameWindow->showPixels();
			//usleep(10000/speed);
		}
}

void Game::animationRandomPlayer(unsigned int speed){
	if(speed == 0) speed = 1;
	//Récupère l'image de choix
		int choixH;
		int choixL;
		int** choixImage = Picture::getPicture("ChoixJoueur.txt", &choixH, &choixL);

		int splash4H;
		int splash4L;
		int** splash4Image = Picture::getPicture("SplashScreen4.txt", &splash4H, &splash4L);

		GameWindow->fillPixels(10,this->heightS,this->widthS);
		GameWindow->setPixels(splash4Image, splash4H, splash4L, this->heightS - splash4H + 5, 0);
		GameWindow->setPixels(choixImage, choixH, choixL, 5, this->widthS/4 - choixL/2);
		GameWindow->showPixels();

		int hauteurText = 12;
		string text = "Est le premier à jouer";
		GameWindow->print(this->widthS/2 - text.size()/2, hauteurText + 2 + choixH ,text , COLOR_PAIR(51) | A_BOLD);

		for (int i = 0; i < 100; i++) {
			choixJoueur = rand() % (nbHumans + nbBots);
			string pseudo = "                                                        ";
			GameWindow->print(this->widthS/2 - pseudo.size()/2, hauteurText + choixH ,pseudo , COLOR_PAIR(51) | A_BOLD);
			pseudo = players[choixJoueur]->getName();
			GameWindow->print(this->widthS/2 - pseudo.size()/2, hauteurText + choixH ,pseudo , COLOR_PAIR(51) | A_BOLD);
			// COLOR_PAIR(19) | A_BOLD
			usleep(30000 / speed);
		}

		for (int i = 0; i < 4; i++) {
			string pseudo = "                                                        ";
			GameWindow->print(this->widthS/2 - pseudo.size()/2, hauteurText + choixH ,pseudo , COLOR_PAIR(51) | A_BOLD);
			usleep(100000 / speed);
			pseudo = players[choixJoueur]->getName();
			GameWindow->print(this->widthS/2 - pseudo.size()/2, hauteurText + choixH ,pseudo , COLOR_PAIR(10) | A_BOLD);
			usleep(300000 / speed);
		}
		usleep(500000 / speed);
		for (int i = 0; i < this->heightS; i++) {
			GameWindow->setPixels(splash4Image, splash4H, splash4L, this->heightS - splash4H + 5 -i, 0);
			GameWindow->showPixels();
			usleep(30000 / speed);
		}
}

int Game::getNbBots() const{
	return nbBots;
}

int Game::getNbHumans() const{
	return nbHumans;
}

int Game::getAlivePlayers() const{
	int nbPlayerAlive = 0;
	for (int i = 0; i < (nbBots + nbHumans); i++) {
		if(players[i]->isAlive()){
			nbPlayerAlive++;
		}
	}
	return nbPlayerAlive;
}

Player** Game::getPlayers(){
  return players;
}
