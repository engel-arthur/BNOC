#ifndef _GAME_H
#define _GAME_H

#include "window.h"
#include "Player.h"
#include "PreparationPlayer.h"
#include "Picture.h"
#include "Message.h"
#include "SaveManagement.h"
#include "ConfigManagement.h"
#include "Score.h"
#include <string>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h> //Ne pas oublier d'inclure le fichier time.h

class Game
{
public:
	Game();//partie par défaut
	Game(std::string src, bool isConfig=true);//partie avec une config ou une sauvegarde (défaut config)
	Game(int nbPlayer, int nbBot);//partie avec un nombre variable de bots et de joueurs humains
	~Game();
	void loadDefault();//charger le défaut (failsafe)

	void start();//lancer la partie
	int ennemyChoice();//choisir qui attaquer
	bool choiceEnnemyPosition(int ennemy, int &x, int &y);//choisir quelle case attaquer
	//GETTERS
	Player** getPlayers();
	int getNbBots() const;
	int getNbHumans() const;

	int getAlivePlayers() const;
	//ANIMATIONS
	void animationOUT(unsigned int speed = 1);
	void animationRandomPlayer(unsigned int speed = 1);

private:
	bool readConfig(std::string cfg);//lire config (privé, car utile seulement à game)
	int nbHumans;
	int nbBots;
	int nbShips;
	std::string* shipNames;
	Player** players;
	std::string configLink;
	int heightS;//taille de fenetre
  	int widthS;
	Window* GameWindow;

	int choixJoueur;
	bool isUndo;//si on revient en arrière d'un coup
	bool PreparePlayer;
	bool loadedSave;
};

#endif
