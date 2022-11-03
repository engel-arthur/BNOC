#include "../Headers/Menu.h"
#include "../Headers/Message.h"
using namespace std;

Menu::Menu(){
  this->heightS = 40;
  this->widthS = 130;
  this->positionArrow = 0;  //Position de la selection dans le menu
  this->nbElements = 0;     //Nombre elements dans le menu
  this->elements = new string[0];
  this->arrow = ">>";

  //Ajout des elements au menu
  this->addElement("Player vs Computer");
  this->addElement("Partie personnalisee");
  this->addElement("Sauvegarde");
  this->addElement("Options");
  this->addElement("QUITTER");
}

Menu::Menu(int height, int width, std::string arrowDesign){
  this->heightS = height;
  this->widthS = width;
  this->positionArrow = 0;  //Position de la selection dans le menu
  this->nbElements = 0;     //Nombre elements dans le menu
  this->elements = new string[0];
  this->arrow = arrowDesign;
}

Menu::~Menu(){
  //nothing
}


//PERMET DE RECUPERER LE LOGO N'IMPORTE OU
int bnocLogoHauteur;
int bnocLogoLargeur;
int** bnocLogo = Picture::getPicture("Bnoc.txt", &bnocLogoHauteur, &bnocLogoLargeur);

int Menu::start(){
  //Création d'un window de couleur
  menuWindow = new Window(this->heightS,this->widthS,0,0);
  menuWindow->setCouleurFenetre(WBLACK);

  //Récupère l'image de transition
  int transitionHauteur;
  int transitionLargeur;
  int** transitionImage = Picture::getPicture("SplashScreen2.txt", &transitionHauteur, &transitionLargeur);

  //On remplis la fenêtre du fond bleu (couleur 10) avec le logo
  menuWindow->fillPixels(10, this->heightS, this->widthS/2,0,0);
  menuWindow->setPixels(bnocLogo, bnocLogoHauteur, bnocLogoLargeur,heightS/2 - bnocLogoHauteur/2 - 11,widthS/4 - bnocLogoLargeur/2);
  menuWindow->showPixels();

  //On affiche tous les elements de choix du menu
  this->showElement(menuWindow,this->widthS/2 - bnocLogoLargeur,20);
  //On affiche la flêche
  menuWindow->print(this->widthS/2 - 28,positionArrow + 20,arrow, COLOR_PAIR(10) | A_BOLD);

  //ON AFFICHE LES MEILLEURS SCORES
    Score score;
    int *nbScore;
    string *nameScore;
    int taille;
    score.get5BestScore(nbScore, nameScore, taille);

    menuWindow->print(this->widthS/2 - 6,this->heightS - 8,"HIGH SCORES", COLOR_PAIR(17) | A_BOLD);

    for (int i = 0; i < taille; i++) {
      usleep(5000);
      menuWindow->print(this->widthS/2 - 6,i + this->heightS - 6,to_string(nbScore[i]) + " : " +nameScore[i], COLOR_PAIR(17));
    }
  //FIN D'AFFICHAGE DES MEILLEURS SCORES

  //AFFICHE MESSAGE AIDE
  Message::Alert("Choisissez a l'aide/ndes touches directionnelles./nPuis appuyez sur/nla touche Entree.", "menuMessage.txt", 15, 44, 80, 16, 6, 18);


  int c=0;        //Touche appuyée

  //Tant qu'on a appuyé sur "Entrée" on sort pas
  while(c != '\n'){
    c = wgetch(menuWindow->getWindow()); //On prend la touche appuyée
    // cout << c << endl;
		switch(c){
    	case 'A': //Touche du bas
        moveArrow(menuWindow, -1);
				break;
			case 'B': //Touche du haut
        moveArrow(menuWindow, 1);
				break;
		}
  }

  //ANIMATION DE SORTIE
    int position=0;
    menuWindow->print(this->widthS/2 - 6,this->heightS - 8,"HIGH SCORES", COLOR_PAIR(17) | A_BOLD);

    int positionTemp = 0;
    while(position <= this->widthS + bnocLogoLargeur){
      // menuWindow->fillPixels(16, bnocLogoHauteur, bnocLogoLargeur,heightS/2 - bnocLogoHauteur/2 - 11,widthS/4 - bnocLogoLargeur/2 + positionTemp);
      position++;
      positionTemp = position/2;

      menuWindow->setPixels(bnocLogo, bnocLogoHauteur, bnocLogoLargeur,heightS/2 - bnocLogoHauteur/2 - 11,widthS/4 - bnocLogoLargeur/2 + positionTemp);
      for (int i = 0; i < this->heightS/bnocLogoHauteur + 5; i++) {
        menuWindow->setPixels(bnocLogo, bnocLogoHauteur, bnocLogoLargeur,bnocLogoHauteur*i, - (bnocLogoLargeur*(i+1))/2 + position);
      }
      menuWindow->setPixels(transitionImage, transitionHauteur, transitionLargeur, 0, -transitionLargeur*1.8 + position*1.1);
      menuWindow->showPixels();
      usleep(10000);
    }
    delete menuWindow; //Suppression de la fenêtre
    return positionArrow;
}

void Menu::addElement(string name){
  this->nbElements++;
  string* elementsTEMP = new string[this->nbElements];
  for (int i = 0; i < this->nbElements-1; i++) {
    elementsTEMP[i] = this->elements[i];
  }
  elementsTEMP[this->nbElements - 1] = name;

  this->elements = new string[this->nbElements];
  for (int i = 0; i < this->nbElements; i++) {
    this->elements[i] = elementsTEMP[i];
  }
  delete[] elementsTEMP;
}

void Menu::showElement(Window *window, int x, int y){
  int position = 0;
  while (position <= this->widthS/2) {
    menuWindow->fillPixels(16, 13, position,3,0);
    menuWindow->fillPixels(17, 9, 1,this->heightS -9,this->widthS/2 - position);
    menuWindow->setPixels(bnocLogo, bnocLogoHauteur, bnocLogoLargeur,heightS/2 - bnocLogoHauteur/2 - 11,widthS/4 - bnocLogoLargeur/2);
    menuWindow->showPixels();
    position++;
    usleep(10000);
  }

  for (int i = 0; i < this->nbElements; i++) {
    for (unsigned int j = 0; j < this->elements[i].size(); j++) {
      window->print(x + j,y + i,this->elements[i].substr(j,1), COLOR_PAIR(10) | A_BOLD);
      usleep(10000);
    }
  }
}

void Menu::moveArrow(Window *window,int move){
  window->print(this->widthS/2 - 28,positionArrow + 20,"  ", COLOR_PAIR(10) | A_BOLD);

  positionArrow += move;
  if(positionArrow < 0)
    positionArrow = nbElements -1;
  if(positionArrow >= nbElements)
    positionArrow = 0;

  window->print(this->widthS/2 - 28,positionArrow + 20,arrow, COLOR_PAIR(10) | A_BOLD);
}
