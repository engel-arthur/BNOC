#include <sstream>
#include <fstream> //pour lire fichier texte
#include <string>
#include <stdio.h> //printf, fgets
#include <stdlib.h> //atoi
#include "../Headers/Score.h"
using namespace std; //permet d'enlever "std::"

Score::Score(){
	this->_nbScore=0;
	this->_nameScore="NoName";
	this->_lienScore="Score/BestScore.txt";
}

Score::Score(int nbScore, string nameScore){
	this->_nbScore=nbScore;
	this->_nameScore=nameScore;
	this->_lienScore="Score/BestScore.txt";
}

Score::~Score(){
} //destructeur


//fonction test pour lire fichier texte
void Score::lireFichierScore(){
	ifstream fichier(this->_lienScore.c_str());
	int i=0;
	if(fichier){
		//L'ouverture s'est bien passée, on peut donc lire
		string ligne; //Une variable pour stocker les lignes lues

		while(getline(fichier, ligne)){ //Tant qu'on n'est pas à la fin, on lit
      		i++;
			int posPoint1 = ligne.find(';',0);
			int posPoint2 = ligne.find(';',posPoint1+1);
			string text1 = ligne.substr(0,posPoint1);
			string text2 = ligne.substr(posPoint1+1,posPoint2-(posPoint1+1));
    	    cout << i << ". " << text1 << " " << text2 << endl; //Et on l'affiche dans la console
   		}
  	}else{
    	cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
 	}
}


//remplir les tableaux et la taille
void Score::get5BestScore(int *&nbScore, string *&nameScore, int &taille){

	ifstream fichier(this->_lienScore.c_str());
	string ligne; //Une variable pour stocker les lignes lues
	taille = 0;

	if(fichier){
		//L'ouverture s'est bien passée, on peut donc lire
		while(getline(fichier, ligne)){ //Tant qu'on n'est pas à la fin, on lit
      		taille++;
    	}
	 	nbScore = new int[taille];
		nameScore = new string[taille];

		fichier.close();
  	}else{
    	cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
 	}

	ifstream fichier2(this->_lienScore.c_str());
	if(fichier2){
		int i = 0;
		while(getline(fichier2, ligne)){ //Tant qu'on n'est pas à la fin, on lit
			int posPoint1 = ligne.find(';',0);
			int posPoint2 = ligne.find(';',posPoint1+1);
			string text1 = ligne.substr(0,posPoint1);
			string text2 = ligne.substr(posPoint1+1,posPoint2-(posPoint1+1));
			nbScore[i]=atoi(text1.c_str());
			nameScore[i]=text2;
			i++;
	    }
		fichier2.close();
  	}else{
    	cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
 	}

 	this->trierScore(nbScore, nameScore, taille);

}

string* Score::getBestScore(int nbScore){

	string* allScore = new string[nbScore];

	for (int i = 0; i < nbScore; ++i)
	{
		allScore[i] = ":C";
	}

	ifstream fichier(this->_lienScore.c_str());
	string ligne; //Une variable pour stocker les lignes lues
	int taille = 0;

	if(fichier){
		//L'ouverture s'est bien passée, on peut donc lire
		while(getline(fichier, ligne)){ //Tant qu'on n'est pas à la fin, on lit
      		taille++;
    	}

		fichier.close();
  	}else{
    	cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
 	}

	ifstream fichier2(this->_lienScore.c_str());
	if(fichier2){
		int i = 0;
		while(getline(fichier2, ligne)){ //Tant qu'on n'est pas à la fin, on lit
			int posPoint1 = ligne.find(';',0);
			int posPoint2 = ligne.find(';',posPoint1+1);
			string text1 = ligne.substr(0,posPoint1);
			string text2 = ligne.substr(posPoint1+1,posPoint2-(posPoint1+1));

			allScore[i] = text1.c_str() + string(" : ") + text2.c_str();
			i++;
	    }
		fichier2.close();
  	}else{
    	cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
 	}

 	return allScore;
 }



void Score::getBestScoreTOP5(int *&nbScore, string *&nameScore, int &taille){
	string ligne; //Une variable pour stocker les lignes lues
	taille = 0;

    	taille=5;
	 	nbScore = new int[taille];
		nameScore = new string[taille];

	ifstream fichier2(this->_lienScore.c_str());
	if(fichier2){
		int i = 0;
		while(getline(fichier2, ligne) && i<5){ //Tant qu'on n'est pas à la fin, on lit
			int posPoint1 = ligne.find(';',0);
			int posPoint2 = ligne.find(';',posPoint1+1);
			string text1 = ligne.substr(0,posPoint1);
			string text2 = ligne.substr(posPoint1+1,posPoint2-(posPoint1+1));
			nbScore[i]=atoi(text1.c_str());
			nameScore[i]=text2;
			i++;
	    }
		fichier2.close();
  	}else{
    	cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
 	}

}


int Score::getnbScore(){
	return this->_nbScore;
}
string Score::getNameScore(){
	return this->_nameScore;
}

bool Score::checkScoreSup(Score *score){
	return(score->getnbScore() > this->_nbScore);
}

void Score::setNewScore(int nbScore, string nameScore){
	ofstream fichier(this->_lienScore.c_str(), ios::app);  // ouverture en écriture avec effacement du fichier ouvert

	if(fichier){
		fichier << nbScore << ";" << nameScore << endl;

		fichier.close();
	}else
		cerr << "Impossible d'ouvrir le fichier !" << endl;
}


void Score::trierScore(int *&nbScore, string *&nameScore, int &taille){
	int i=0;
	int tmp=0;
	string tmp2;
	int j=0;

	//trier tableau passé en paramètre
	for(i = 0; i < taille; i++){ //On veut remplir la case i du tableau
		for(j = i+1; j < taille; j++){ //On vérifie s'il n'y a pas de nombre inférieur
			//Dans les cases suivantes
			if(nbScore[j] > nbScore[i]){
				tmp = nbScore[i]; //Si c'est le cas on intervertit les cases
				nbScore[i] = nbScore[j];
				nbScore[j] = tmp;

				tmp2 = nameScore[i]; //Si c'est le cas on intervertit les cases
				nameScore[i] = nameScore[j];
				nameScore[j] = tmp2;
			}
		}
	}

	//clean et trier fichier txt
	ofstream fichier(this->_lienScore.c_str(), ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier
	if(fichier){  // si l'ouverture a réussi
		for(i = 0; i < taille; i++){
			fichier << nbScore[i] << ";" << nameScore[i] << endl;
		}
		fichier.close();  // on referme le fichier
	}else  // sinon
		cerr << "Erreur à l'ouverture !" << endl;

	this->getBestScoreTOP5(nbScore, nameScore, taille);

}
