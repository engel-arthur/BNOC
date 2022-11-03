#ifndef _PLAYER_H
#define _PLAYER_H

#include <string>
#include "Territory.h"

class Player
{
public:
	Player();
	Player(std::string nick, int territoryH, int territoryW, std::string* shipNames, int nbShips, bool realPlayer=true);
	Player(std::string nick, Territory* terr ,bool realPlayer=true);

	~Player();
	bool isAlive();
	bool attack(Player *enemy, int x, int y, bool &sunk);
	//GETTERS
	Territory* getTerritory();
	int getScore() const;
	std::string getName();
	bool isHuman() const;
	void increaseScore(int n);
	std::string* getShipsName() const;
	//SETTERS
	void setName(std::string newName);
	void setShipsName(std::string* shipNames);
private:
	std::string name;
	std::string* shipNames;
	Territory*  territory;
	bool isReal;//isRael
	int score;
};

#endif
