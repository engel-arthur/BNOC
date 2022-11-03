#include "../Headers/PreparationPlayer.h"

using namespace std;

Window* PreparationPlayer::mainWindow = NULL;
Window* PreparationPlayer::territoryWindow = NULL;
Window* PreparationPlayer::shipWindow = NULL;

bool* PreparationPlayer::usedShips = NULL;
int PreparationPlayer::nbShip = 0;
Territory* PreparationPlayer::terr = NULL;

int PreparationPlayer::heightS = ConfigManagement::getHScreen();
int PreparationPlayer::widthS = ConfigManagement::getWScreen();

bool PreparationPlayer::PreparePlayer(Player *player)
{
	terr = player->getTerritory();
	nbShip = terr->getNbShip();

	/* ON TRIE LES BATEAUX POUR LES JOUEURS */
	if(player->isHuman()){
		int iShip = 0;
		bool hardreset;
		usedShips = new bool[nbShip];
		for(int i=0;i<nbShip;i++)
		{
			usedShips[i]=false;
		}
		createMainWindow();
		createTerrWindow();
		createShipWindow();
		hardreset=false;
		initShip();
		player->setName(changeName(player));
		while(iShip!=-1){
			iShip = selectShip();
			if(iShip!=-1){
				hardreset=shipToTerr(iShip);
			}
			if(hardreset)
			{
				hardReset();
				hardreset=false;
			}
		}
	}
	/* SINON ON TRIE LES BATEAUX POUR LES BOTS */
	else{
		int x = 0,y =0, rotation = 0;
		Ship** ships=terr->getShips(nbShip);
		for (int i = 0; i < nbShip; i++) {
			do {
				y = rand() % (terr->getHeight() - ships[i]->getHeight() + 1);
				x = rand() % (terr->getWidth() - ships[i]->getWidth() + 1);
				rotation = rand() % 3;
				for (int j = 0; j < rotation; j++) {
					ships[i]->rotate(90);
				}
			} while(!terr->setShipPos(i, y, x));
		}
	}
	//TOUS LES BATEAUX SONT INIT
	return true;

}
//Créer la fenêtre principale
void PreparationPlayer::createMainWindow()
{
	// int heightS = 40, widthS = 130;
	mainWindow = new Window(heightS, widthS, 0, 0);
	int h,w;
	int** logo = Picture::getPicture("Bnoc.txt",&h,&w);
	Message::Alert("Touches directionnelles :/n1) Changer la selection/n2) Deplacer les bateaux/n/nTouche Entree :/n1) Prends un bateau selectionne/n2) Valide la position/n du bateau et passe au suivant/n/nTouche e,r : rotation gauche, droite/nTouche o : RESET", "menuMessage.txt", 15, 44, 82, 16, 2, 10);
	mainWindow->setPixels(logo,h,w,1,1);
	mainWindow->showPixels();
	// mainWindow->setCouleurFenetre(BWHITE);
}
//Affiche la fenêtre du territory
void PreparationPlayer::createTerrWindow()
{
	// int heightS = 40, widthS = 130;
	int heightT, widthT;
	int h = terr->getHeight(), w = terr->getWidth()*2;
	territoryWindow = new Window(h,w,widthS/2 - w/2,heightS/2 - h,mainWindow);
	int** territoryPixels = terr->afficheTerritory(heightT, widthT);
	territoryWindow->setPixels(territoryPixels, heightT, widthT, 0,0);
	territoryWindow->showPixels();
}
//Créer la fenêtre de tous les ships (avec rotation auto, le coté le plus long en hauteur)
void PreparationPlayer::createShipWindow()
{
	// int heightS = 40, widthS = 130;
	int h = terr->getHeight();

	Ship** ships=terr->getShips(nbShip);
	int wTot = 0, hMax = 0;
	for(int i=0; i < nbShip; i++)
	{
		if(ships[i]->getWidth() > ships[i]->getHeight()){
			ships[i]->rotate(90);
		}
		wTot+=ships[i]->getWidth()*2;
		if(ships[i]->getHeight() > hMax)
		{
			hMax = ships[i]->getHeight();
		}
	}
	shipWindow = new Window(hMax,wTot,widthS/2-wTot/2,heightS/2+h/2+3,mainWindow);
}
//affiche tous les ships dans la shipWindow
void PreparationPlayer::initShip()
{

	Ship** ships=terr->getShips(nbShip);
	//cout  << nbShip;
	int x=0;
	int h = 0;
	int w = 0;
	// cout << endl << "h :" << h << "w :" << w << endl;
	// cout << "COIN : " << coin[0][0]<< endl;
	for(int i=0; i < nbShip;i++)
	{
		int** shipPixels = ships[i]->getPixelShip(h,w);
		// cout << "BATEAU " << i << " h : " << h <<  " w : " << w << endl;
		shipWindow->setPixels(shipPixels, h, w, 0, x);
		//cout << x <<endl;
		x+=w;
	}
	shipWindow->showPixels();
}

int PreparationPlayer::selectShip()
{

	Ship** shipsTEMP = terr->getShips(nbShip);//on stocke les bateaux dans une matrice
	Ship** ships = new Ship*[nbShip];
	for (int i = 0; i < nbShip; i++){
		ships[i] = new Ship(*shipsTEMP[i]);
	}
	int iShip=0;//indice bateau
	int bShip=0;//coord début bateau
	int iShipBegin = -1;
	int iShipEnd = -1;
	for (int i = 0; i < nbShip; i++)
	{
		if(ships[i]->getWidth() > ships[i]->getHeight())
			ships[i]->rotate(90);

		if(!usedShips[i]){
			iShipEnd = i;
		}

		if(!usedShips[i] && iShipBegin == -1){
			iShipBegin = i;
		}else if(usedShips[i] && iShipBegin == -1){
			bShip+=(ships[i]->getWidth())*2;
		}
	}
	if(iShipBegin==-1)
		return -1;
	iShip = iShipBegin;
	int c=0;//caractère
	string arrow="><";//flèche
	int h,w;
	shipWindow->print(bShip,0,arrow, COLOR_PAIR(10) | A_BOLD);//affichage de base
	while(c != '\n'){
		c = wgetch(shipWindow->getWindow()); //On prend la touche appuyée
		// cout << c << endl;
		switch(c){
			case 'D': //Touche gauche
				if(iShip>iShipBegin)
				{
					if(ships[iShip]->getShipMatrice(h,w)[0][0])
					{
						shipWindow->print(bShip,0,"  ", COLOR_PAIR(ships[iShip]->getColor()));
					}
					else
					{
						shipWindow->print(bShip,0,"  ", COLOR_PAIR(16));
					}
					do{
					bShip-=(ships[iShip-1]->getWidth())*2;
					iShip--;
					}while(usedShips[iShip]);
					shipWindow->print(bShip,0,arrow, COLOR_PAIR(10) | A_BOLD);
				}
				break;
			case 'C': //Touche droite
				if(iShip<iShipEnd)
				{
					if(ships[iShip]->getShipMatrice(h,w)[0][0])
					{
						shipWindow->print(bShip,0,"  ", COLOR_PAIR(ships[iShip]->getColor()));
					}
					else
					{
						shipWindow->print(bShip,0,"  ", COLOR_PAIR(16));
					}
					do{
					bShip+=(ships[iShip]->getWidth())*2;
					iShip++;
					}while(usedShips[iShip]);
					shipWindow->print(bShip,0,arrow, COLOR_PAIR(10) | A_BOLD);
				}
				break;
			/*
			on modifie les coords bateaux en conséquence(pour la flêche)
			on modifie l'indice
			*/
		}
	}
	shipWindow->fillPixels(16, ships[iShip]->getHeight(), ships[iShip]->getWidth(), 0, bShip/2);
	shipWindow->showPixels();

	return iShip;
}

bool PreparationPlayer::shipToTerr(int iShip)
{

	Ship** ships=terr->getShips(nbShip);
	int hShip = 0;
	int wShip = 0;
	int heightT, widthT;
	int** territoryPixels = terr->afficheTerritory(heightT, widthT);


	int** shipPixels = ships[iShip]->getPixelShip(hShip,wShip);

	territoryWindow->setPixels(shipPixels,hShip,wShip,0,0);
	territoryWindow->showPixels();

	int x = 0, y =0;

	bool firstLoop = true;

	do
	{
		if(!firstLoop){
			//Afficher erreur
		}
		int c=0;//caractère

		while(c != '\n'){
			c = wgetch(shipWindow->getWindow()); //On prend la touche appuyée
			if (c == 'A'){ //HAUT
				y--;
			}if (c == 'B'){ //BAS
				y++;
			}if (c == 'C'){ //DROITE
				x++;
			}if (c == 'D'){ //GAUCHE
				x--;
			}if(c == 'r'){
				ships[iShip]->rotate(90);
				shipPixels = ships[iShip]->getPixelShip(hShip,wShip);
			}if(c == 'e'){
				ships[iShip]->rotate(-90);
				shipPixels = ships[iShip]->getPixelShip(hShip,wShip);
			}if(c == 'o'){
				return true;
			}

			if(x < 0) x = 0;
			if(y < 0) y = 0;
			if(x > terr->getWidth() - wShip) x = terr->getWidth() - wShip;
			if(y > terr->getHeight() - hShip) y = terr->getHeight() - hShip;

			territoryWindow->setPixels(territoryPixels, heightT, widthT, 0,0);
			territoryWindow->setPixels(shipPixels,hShip,wShip,y,x);
			territoryWindow->showPixels();
		}
		firstLoop = false;
	}
	while(!(terr->setShipPos(iShip, y, x)));
	usedShips[iShip]=true;
	return false;
}

void PreparationPlayer::hardReset()
{
	Ship** ships=terr->getShips(nbShip);//on stocke les bateaux dans une matrice
	int heightT, widthT;
	nbShip = terr->getNbShip();
	usedShips = new bool[nbShip];
	for(int i=0;i<nbShip;i++)
	{
		usedShips[i]=false;
		if(ships[i]->getWidth() > ships[i]->getHeight()){
			ships[i]->rotate(90);
		}
	}
	terr->resetTerr();
	createShipWindow();
	initShip();
	int** territoryPixels = terr->afficheTerritory(heightT, widthT);
	territoryWindow->setPixels(territoryPixels, heightT, widthT, 0,0);
	territoryWindow->showPixels();
}

std::string PreparationPlayer::changeName(Player* player){
	int c = 0;
	string TEMPNAME = player->getName();
	bool defaultName=true;
	mainWindow->print(widthS/2 - TEMPNAME.size()/2 - 8,(heightS/2)+3,"Enter name : "+player->getName(),COLOR_PAIR(19));

	while(c != '\n'){
		c = wgetch(shipWindow->getWindow()); //On prend la touche appuyée

		if(c != '\n'){
			if(defaultName)
			{
				TEMPNAME = "";
				defaultName=false;
			}
			if((c == 127 || c == 8 || c == '\b')){//BOUTON SUPPR
				if(TEMPNAME.size()>0)
					TEMPNAME = TEMPNAME.erase(TEMPNAME.size()-1,1);
			}else if(TEMPNAME.size() < 100){
				TEMPNAME += c;
			}
		}

		mainWindow->print(0,(heightS/2)+3,"                                                                                                                                                             ",COLOR_PAIR(19) | A_BOLD);
		mainWindow->print(0,(heightS/2)+4,"                                                                                                                                                             ",COLOR_PAIR(19) | A_BOLD);
		mainWindow->print(widthS/2 - TEMPNAME.size()/2,(heightS/2)+3,TEMPNAME,COLOR_PAIR(19));
		mainWindow->print(widthS/2 - TEMPNAME.size()/2 + TEMPNAME.size(),(heightS/2)+4,"^",COLOR_PAIR(19));
	}
	mainWindow->print(widthS/2 - TEMPNAME.size()/2,(heightS/2)+3,TEMPNAME,COLOR_PAIR(19) | A_BOLD);
	mainWindow->print(widthS/2 - TEMPNAME.size()/2 + TEMPNAME.size(),(heightS/2)+4," ",COLOR_PAIR(19) | A_BOLD);
	// Remis ici car bande noir sur message si non réactualisé
	Message::Alert("Touches directionnelles :/n1) Changer la selection/n2) Deplacer les bateaux/n/nTouche Entree :/n1) Prends un bateau selectionne/n2) Valide la position/n du bateau et passe au suivant/n/nTouche e,r : rotation gauche, droite/nTouche o : RESET", "menuMessage.txt", 15, 44, 82, 16, 2, 10);

	return TEMPNAME;
}
// void PreparationPlayer::moveShip(Territory *terr, int iShip){
// 	int xCursor = 0, yCursor = 0;
// }
// void PreparationPlayer::placeShip(Territory *terr){}
