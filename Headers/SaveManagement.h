#ifndef _SAVEMANAGEMENT_H
#define _SAVEMANAGEMENT_H

#include <fstream>
#include <string>
#include <iostream>
#include "../Headers/Player.h"
#include "../Headers/ConfigManagement.h"
#include "../Headers/Ship.h"
#include "../Headers/Territory.h"
#include "../Headers/Picture.h"

class SaveManagement
{
public:
	static void setPlayers(Player** playerList, int nbP);
	static Player** getPlayers();
	static bool writeLog(int attacker, int attacked, int y, int x);
	static bool saveState(std::string target,Player** allPlayers, int nbP);
	static bool loadSave(std::string save, int& nbHumans, int& nbBots);
	static bool readLog(Player** allPlayers, int nbP, int& lastplayer);
	static void emptyLog();
	static bool getLastMove(int& idAttacker, int& idAttacked, int& y, int& x);
private:
	SaveManagement();
	~SaveManagement();
	static int nbPlayer;
	static Player** players;
	static std::string logPath;
};

#endif
