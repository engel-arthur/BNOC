#include "../Headers/SaveManagement.h"

using namespace std;

Player** SaveManagement::players = NULL;
int SaveManagement::nbPlayer=0;
string SaveManagement::logPath = "Log/moves.log";

void SaveManagement::setPlayers(Player** players, int nbP)
{
	players=players;
	nbPlayer=nbP;
}

Player** SaveManagement::getPlayers()
{
	return players;
}
bool SaveManagement::writeLog(int attacker, int attacked, int y, int x)//écrire le coup joué dans le log
{
	ofstream log(logPath.c_str(), ios::out | ios::app);
	if(log)
	{
		log << attacker << " " << attacked << " " << y << " " << x << endl;
	}
	else return false;
	log.close();
	return true;
}
bool SaveManagement::saveState(string target,Player** allPlayers, int nbP)//sauvegarder la partie!
{
	players=allPlayers;
	nbPlayer=nbP;
	int ships=0;
	string path = "Save/";
	string fullPath = path+target;
	ofstream save;
	save.open(fullPath.c_str(), ofstream::out | ofstream::trunc);
	if(save)//on écrit la première partie, les carac des joueurs et leurs bateaux
	{
		for(int i=0; i<nbPlayer; ++i)
		{
			save << (players[i]->isHuman() ? "hum;" : "bot;") << players[i]->getName() << ";" << players[i]->getTerritory()->getHeight() << ";" << players[i]->getTerritory()->getHeight() << ";";
			for(int j=0; j<players[i]->getTerritory()->getNbShip(); j++)
			{
				save 	<< players[i]->getShipsName()[j]
						<< ";" << players[i]->getTerritory()->getShips(ships)[j]->getY()//pos y du bateau
						<< ";" << players[i]->getTerritory()->getShips(ships)[j]->getX()//x
						<< ";" << players[i]->getTerritory()->getShips(ships)[j]->getRotation()//rotation en degrés du bateau
						<< ";";
			}
			save << endl;
		}
		save << endl;
		ifstream log(logPath.c_str(), ios::in);
		if(log)//on copie l'historique des mouvements dans la sauvegarde
		{
			save << log.rdbuf();
			log.close();
		}
		save.close();
	}
	else return false;
	return true;
}
bool SaveManagement::loadSave(string save, int& nbHumans, int& nbBots)//charger une sauvegarde
{
	string path = "Save/" + save;
	string line = "";
	string cvar = "";
	string space = ";";//parce que passage par référence haha
	string nick = "";
	string* nameShips;
	int territoryH = 0;
	int territoryW = 0;
	Territory* terr = NULL;
	Ship** shipList = NULL;
	bool **shipMatrix = NULL;
	int hShip=0;
	int wShip=0;
	int shipHealth=0;
	int nbShips = 0;
	int xShip = 0;
	int yShip = 0;
	int rotationShip = 0;
	int colorShip=20;
	string shipName="";
	bool realPlayer = false;
	int count=0;
	ifstream saveFile(path, ios::in);
	if(saveFile)
	{
		while(getline(saveFile,line) && line != "")
		{
			ConfigManagement::getFirstElement(line, cvar, space);//fonction qui récupère le premier élément d'un string, le supprime, et le met dans une autre (basé sur un séparateur)
			if(cvar == "hum"){//humain
				realPlayer = true;
				nbHumans++;
			}else{
				realPlayer = false;
				nbBots++;
			}
			ConfigManagement::getFirstElement(line, cvar, space);
			nick = cvar;
			ConfigManagement::getFirstElement(line, cvar, space);
			territoryH = stoi(cvar);
			ConfigManagement::getFirstElement(line, cvar, space);
			territoryW = stoi(cvar);
			while(line!="" && line != space)
			{
				ConfigManagement::getFirstElement(line, cvar, space);
				switch(count)
				{
					case 0:
						shipName = cvar;
						break;
					case 1:
						yShip = stoi(cvar);
						break;
					case 2:
						xShip = stoi(cvar);
						break;
					case 3:
						rotationShip = stoi(cvar);
						break;
				}
				count++;
				if(count==4)
				{
					nbShips++;
					Ship** shipListTEMP = new Ship*[nbShips];
					string* nameShipsTEMP = new string[nbShips];
					for (int i = 0; i < nbShips - 1; i++) {
						shipListTEMP[i] = shipList[i];
						nameShipsTEMP[i] = nameShips[i];
					}
					shipMatrix = Picture::getShip(shipName+".ship", &hShip, &wShip, &shipHealth);
					shipListTEMP[nbShips-1] = new Ship(shipMatrix, hShip, wShip, shipHealth, colorShip);
					shipListTEMP[nbShips-1]->setPosition(xShip, yShip);
					nameShipsTEMP[nbShips-1] = shipName;
					for(int i=0; i<rotationShip;i++)
						shipListTEMP[nbShips-1]->rotate(90);
					colorShip++;
					count = 0;
					shipList = new Ship*[nbShips];
					nameShips = new string[nbShips];
					for (int i = 0; i < nbShips; i++) {
						shipList[i] = shipListTEMP[i];
						nameShips[i] = nameShipsTEMP[i];
					}
				}
			}
			nbPlayer++;
			terr = new Territory(territoryH, territoryW, shipList, nbShips);
			Player** playersTEMP = new Player*[nbPlayer];
			for (int i = 0; i < nbPlayer-1; i++) {
				playersTEMP[i] = players[i];
			}
			playersTEMP[nbPlayer-1] = new Player(nick, terr, realPlayer);
			playersTEMP[nbPlayer-1]->setShipsName(nameShips);
			players = new Player*[nbPlayer];
			for (int i = 0; i < nbPlayer; i++) {
				players[i] = playersTEMP[i];
			}
			nbShips = 0;
			colorShip=20;
		}
		ofstream log(logPath, ios::out | ios::trunc);
		log << saveFile.rdbuf();
		log.close();
		saveFile.close();
	}
	else
	{
		return false;
	}
	for (int i = 0; i < nbPlayer; i++) {
		cout << players[i]->getName() << endl;
	}
	return true;
}
bool SaveManagement::readLog(Player** allPlayers, int nbP, int& lastplayer)
{
	players=allPlayers;
	nbPlayer=nbP;
	int heightTEMP, widthTEMP;
	lastplayer = rand() % nbP;

	for (int i = 0; i < nbPlayer; i++) {
		players[i]->getTerritory()->afficheTerrForEnn(heightTEMP, widthTEMP);
	}

	ifstream log(logPath.c_str(), ios::in);
	string readNumber;
	int count=0;
	int idPlayer=0;
	int attackingPlayer=0;
	int xAtt=0,yAtt=0;
	bool sunk = false;
	if(log)
	{
		while(log >> readNumber)
		{
			switch(count)
			{
				case 0:
					attackingPlayer=stoi(readNumber);
					count++;
					break;
				case 1:
					idPlayer=stoi(readNumber);
					count++;
					break;
				case 2:
					xAtt=stoi(readNumber);
					count++;
					break;
				case 3:
					yAtt=stoi(readNumber);
					count++;
					break;
			}
			if(count == 4){
				if(!players[attackingPlayer]->attack(players[idPlayer],xAtt,yAtt,sunk)){
					lastplayer = (attackingPlayer + 1) % nbP;
					players[attackingPlayer]->increaseScore(100);
					if(sunk){
						players[attackingPlayer]->increaseScore(100);
					}
				}else{
					lastplayer = attackingPlayer;
					players[attackingPlayer]->increaseScore(100);
				}
			}
			count=count%4;
		}
	}
	cout << "Tout fonctionne au revoir <3" << endl;
	return true;
}
void SaveManagement::emptyLog()
{
	ofstream log(logPath.c_str(), ofstream::out | ofstream::trunc);
	log.close();
}
bool SaveManagement::getLastMove(int& idAttacker, int& idAttacked, int& y, int& x)//horrible hacks + code moche
{
	string idAttackerStr="", idAttackedStr="", yStr="", xStr="", space=" ";
	string lastMove=" ";
	string lastLine=" ";
	ifstream file(logPath.c_str(), ios::in);
	ofstream temp("Log/logTemp.log", ios::out | ios::trunc);
	if(file && temp)
	{
		while(getline(file, lastLine))
		{
			if(lastMove!=" ")
				temp << lastMove << endl;
			if(lastLine=="")
				break;
			lastMove=lastLine;
		}
	}
	file.close();
	temp.close();
	if(lastMove==" ")
		return false;
	ifstream tempRead("Log/logTemp.log", ios::in);
	ofstream replaceLog(logPath.c_str(), ios::out | ios::trunc);
	replaceLog << tempRead.rdbuf();
	replaceLog.close();
	tempRead.close();
	ConfigManagement::getFirstElement(lastMove,idAttackerStr,space);
	ConfigManagement::getFirstElement(lastMove,idAttackedStr,space);
	ConfigManagement::getFirstElement(lastMove,xStr,space);
	ConfigManagement::getFirstElement(lastMove,yStr,space);
	idAttacker=stoi(idAttackerStr);
	idAttacked=stoi(idAttackedStr);
	x=stoi(xStr);
	y=stoi(yStr);
	return true;
}
