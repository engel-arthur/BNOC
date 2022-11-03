#ifndef _PREPARATIONPLAYER_H
#define _PREPARATIONPLAYER_H

#include "window.h"
#include "Ship.h"
#include "Territory.h"
#include "Player.h"
#include "Picture.h"
#include "ConfigManagement.h"
#include "Message.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h> //Ne pas oublier d'inclure le fichier time.h

//#include <iostream>

class PreparationPlayer{
public:
	static bool PreparePlayer(Player *player);
private:
	PreparationPlayer();
	~PreparationPlayer();
	static void createMainWindow();
	static void createTerrWindow();
	static void	createShipWindow();
	static void initShip();
	static int selectShip();
	static bool shipToTerr(int iShip);
	static void hardReset();
	static std::string changeName(Player* player);
	// static void moveShip(Territory *terr, int iShip);
	// static void placeShip(Territory *terr);
	static bool* usedShips;
	static int nbShip;
	static int heightS;
	static int widthS;
	static Territory* terr;
 	static Window* mainWindow;
 	static Window* territoryWindow;
 	static Window* shipWindow;
};

#endif
