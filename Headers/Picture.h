#ifndef _PICTURE_H
#define _PICTURE_H
#include <string>
#include <fstream>//pour lire les fichiers

class Picture{
public:
  	static bool validFile(std::string folder, std::string filename, int* h, int* w);//savoir si un fichier est bien structuré et en même temps renvoyer sa hauteur et largeur
	static int** getPicture(std::string filename, int* h, int* w);//on passe des pointeurs qui seront initialisés dans la fonction (donc juste à créer une variable dans le programme sans l'initialiser)
	static bool** getShip(std::string filename, int* h, int* w, int* vie);//on rajoute la vie qui est calculée en fonction du nombre de cases
	static int nbSpaces(std::string str, bool& doubleSpace);
	static int** rotationR(int** tab, int& h, int& w);//tourne un tableau à 90° vers la droite, inverse aussi h et w
    static bool** rotationR(bool** tab, int& h, int& w);
	static int** rotationL(int** tab, int& h, int& w);//tourne un tableau à 90° vers la gauche, inverse aussi h et w
    static bool** rotationL(bool** tab, int& h, int& w);
private:
	Picture();//inutile
	~Picture();//inutile
};

#endif