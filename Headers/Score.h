#ifndef _SCORE_H
#define _SCORE_H

#include <iostream>
#include <string>

class Score{

public:

	/* CONSTRUCTEURS */
	Score();
	Score(int nbScore, std::string nameScore);
	/* DESTRUCTEUR */
	~Score();


	/* GETTER */
	int getnbScore(); //Récupérer le score
	std::string getNameScore(); //Récupérer le nom

	//permet de sortir tous les scores du fichier text score.txt, de le trier, et de le remplir en sortie
	void get5BestScore(int *&nbScore, std::string *&nameScore, int &taille);
	

	std::string* getBestScore(int nbScore);

	//permet de voir si le score en cours et plus grand que celui passé en paramètre
	bool checkScoreSup(Score *score);

	//enregistrer un nouveau score dans fichier .txt
	void setNewScore(int nbScore, std::string nameScore);

	//fonction test pour lire fichier texte et sortir dans terminal
	void lireFichierScore();



private:

	//fonction permettant de trier tableau passé en paramètre & fichier txt par taille score
	void trierScore(int *&nbScore, std::string *&nameScore, int &taille);

	//get5BestScore passe par getBestScoreTOP5 pour retirer le top 5
	void getBestScoreTOP5(int *&nbScore, std::string *&nameScore, int &taille);

	std::string _lienScore; //lien du fichier txt score
	int _nbScore; //Score
	std::string _nameScore; //Nom score
};

#endif
