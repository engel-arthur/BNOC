#include "../Headers/CustomizeGame.h"
#include "../Headers/Message.h"
using namespace std;

CustomizeGame::CustomizeGame(){
  this->heightS = 40;
  this->widthS = 130;
  this->positionArrow = 0;  //Position de la selection dans le menu
  this->nbElements = 0;     //Nombre elements dans le menu
  this->elements = new string[0];
  this->arrow = ">>";

  //Ajout des elements au menu
  this->addElement("0");
  this->addElement("0");
  this->addElement("JOUER");
  this->addElement("RETOUR");
  this->addElement("Fichier Config: ");
}

CustomizeGame::CustomizeGame(int height, int width, std::string arrowDesign){
  this->heightS = height;
  this->widthS = width;
  this->positionArrow = 0;  //Position de la selection dans le menu
  this->nbElements = 0;     //Nombre elements dans le menu
  this->elements = new string[0];
  this->arrow = arrowDesign;
}

CustomizeGame::~CustomizeGame(){
  //nothing
}

//PERMET DE RECUPERER LE LOGO N'IMPORTE OU
int bnocLogoHauteur2;
int bnocLogoLargeur2;
int** bnocLogo2 = Picture::getPicture("Bnoc.txt", &bnocLogoHauteur2, &bnocLogoLargeur2);

int CustomizeGame::start(int &nbBot, int &nbPlayer, string &chemin){
  //Création d'un window de couleur
  menuWindow = new Window(this->heightS,this->widthS,0,0);
  menuWindow->setCouleurFenetre(WBLACK);

  //Récupère l'image de transition
  int transitionHauteur;
  int transitionLargeur;
  int** transitionImage = Picture::getPicture("SplashScreen2.txt", &transitionHauteur, &transitionLargeur);

  //On remplis la fenêtre du fond bleu (couleur 10) avec le logo
  menuWindow->fillPixels(10, this->heightS, this->widthS/2,0,0);
  menuWindow->setPixels(bnocLogo2, bnocLogoHauteur2, bnocLogoLargeur2,heightS/2 - bnocLogoHauteur2/2 - 11,widthS/4 - bnocLogoLargeur2/2);
  menuWindow->showPixels();

  //On affiche tous les elements de choix du menu
  this->showElement(menuWindow,this->widthS/2 - bnocLogoLargeur2,20);
  //On affiche la flêche
  positionArrow=0;
  menuWindow->print(this->widthS/2 - 28 + 20,positionArrow + 20,arrow, COLOR_PAIR(10) | A_BOLD);

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
  //messageAafficher urlImage hauteur largeur positionX positionY positionXMessage positionYMessage couleur
  Message::Alert("Touches par defaut:/n/nFleche HAUT: Monter/nFleche BAS: Descendre/nFleche DROITE: Aller a droite/nFleche GAUCHE: Aller a gauche", "menuMessage.txt", 15, 44, 80, 16, 3, 10);


  int c=0;        //Touche appuyée

  string writeChemin;
  string blank;
  //Tant qu'on a appuyé sur "Entrée" on sort pas
  while(c != '\n' || (positionArrow!=2 && positionArrow!=3)){
    c = wgetch(menuWindow->getWindow()); //On prend la touche appuyée
    // cout << c << endl;
    switch(c){
      case 'A': //Touche du bas
        moveArrow(menuWindow, -1);
        break;
      case 'B': //Touche du haut
        moveArrow(menuWindow, 1);
        break;
      case 'C': //Touche du droit
        if(atoi(elements[positionArrow].c_str())+1<=10){
          if(positionArrow==1 || positionArrow==0){
            elements[positionArrow] = to_string(atoi(elements[positionArrow].c_str())+1);
            nbPlayer=atoi(elements[0].c_str());
            nbBot=atoi(elements[1].c_str());
          }
        }
          break;
      case 'D': //Touche du gauche
        if(atoi(elements[positionArrow].c_str())-1>=0){
          if(positionArrow==1 || positionArrow==0){
            elements[positionArrow] = to_string(atoi(elements[positionArrow].c_str())-1);
            nbPlayer=atoi(elements[0].c_str());
            nbBot=atoi(elements[1].c_str());
          }
        }
        break;
    }
    if(positionArrow == 0 || positionArrow == 1){
      menuWindow->print(this->widthS/2 - bnocLogoLargeur2 + 20,20+positionArrow, "     ", COLOR_PAIR(10) | A_BOLD);
      menuWindow->print(this->widthS/2 - bnocLogoLargeur2 + 20,20+positionArrow, elements[positionArrow], COLOR_PAIR(10) | A_BOLD);
    }else if(positionArrow == 3){
      nbPlayer=atoi(elements[0].c_str());
      nbBot=atoi(elements[1].c_str());
    }else if(positionArrow == 4){ //si curseur position "selectionner un fichier"
      if((c == 127 || c == 8 || c == '\b') && writeChemin.size()>0){//BOUTON SUPPR
        writeChemin=writeChemin.erase(writeChemin.size()-1,1);
        for (unsigned int i = 0; i < writeChemin.size()+1; i++){
          menuWindow->print(this->widthS/2 - 28 + 20 +2+i,20 + 8," ", COLOR_PAIR(17) | A_BOLD);
        }
        menuWindow->print(this->widthS/2 - 28 + 20 +2,20 + 8,writeChemin, COLOR_PAIR(17) | A_BOLD);
      }else if(c == '\n'){
        ifstream fichier("Config/"+writeChemin, ios::in);  // on ouvre en lecture
        if(fichier){
          // int l = 1;
          // string ligne;
          // while(getline(fichier, ligne)){  // tant que l'on peut mettre la ligne dans "contenu"
          //   menuWindow->print(this->widthS/2 - 28 + 20 +2,20 + 9+l,ligne, COLOR_PAIR(17) | A_BOLD); //on affiche
          //   l++;
          // }
          menuWindow->print(this->widthS/2 - 28 + 20 +2,20 + 9,"Fichier ouvert avec succès!    ", COLOR_PAIR(10) | A_BOLD); //on affiche

          fichier.close();
          chemin = writeChemin;
          nbBot=0; nbPlayer=0;
          return 2; //équivalent de la toucher JOUER
        }else{
          chemin = "";
          menuWindow->print(this->widthS/2 - 28 + 20 +2,20 + 9,"Impossible d'ouvrir le fichier!", COLOR_PAIR(10) | A_BOLD); //on affiche
        }
      //éviter d'avoir les chaines de caractères représentants les flèches du clavier (peut-être moyen de raccourcir ce if)
      }else if(writeChemin.size()<25 && c != '^' && to_string(c) != "27" && c != '[' && c != 127 && c!= 72 && c != 75 && c != 77 && c != 80 && c != 8 && c != '\b' && c != 'A' && c != 'B' && c != 'C' && c != 'D'){
        writeChemin += c;
        menuWindow->print(this->widthS/2 - 28 + 20 +2,20 + 8,writeChemin, COLOR_PAIR(17) | A_BOLD);
      }
    }
  }

  //ANIMATION DE SORTIE
    int position=0;
    menuWindow->print(this->widthS/2 - 6,this->heightS - 8,"HIGH SCORES", COLOR_PAIR(17) | A_BOLD);

    int positionTemp = 0;
    while(position <= this->widthS + bnocLogoLargeur2){
      // menuWindow->fillPixels(16, bnocLogoHauteur2, bnocLogoLargeur2,heightS/2 - bnocLogoHauteur2/2 - 11,widthS/4 - bnocLogoLargeur2/2 + positionTemp);
      position++;
      positionTemp = position/2;

      menuWindow->setPixels(bnocLogo2, bnocLogoHauteur2, bnocLogoLargeur2,heightS/2 - bnocLogoHauteur2/2 - 11,widthS/4 - bnocLogoLargeur2/2 + positionTemp);
      for (int i = 0; i < this->heightS/bnocLogoHauteur2 + 5; i++) {
        menuWindow->setPixels(bnocLogo2, bnocLogoHauteur2, bnocLogoLargeur2,bnocLogoHauteur2*i, - (bnocLogoLargeur2*(i+1))/2 + position);
      }
      menuWindow->setPixels(transitionImage, transitionHauteur, transitionLargeur, 0, -transitionLargeur*1.8 + position*1.1);
      menuWindow->showPixels();
      usleep(10000);
    }
    delete menuWindow; //Suppression de la fenêtre
    return positionArrow;
}

void CustomizeGame::addElement(string name){
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

void CustomizeGame::showElement(Window *window, int x, int y){
  int position = 0;
  while (position <= this->widthS/2) {
    menuWindow->fillPixels(16, 13, position,3,0);
    menuWindow->fillPixels(17, 9, 1,this->heightS -9,this->widthS/2 - position);
    menuWindow->setPixels(bnocLogo2, bnocLogoHauteur2, bnocLogoLargeur2,heightS/2 - bnocLogoHauteur2/2 - 11,widthS/4 - bnocLogoLargeur2/2);
    menuWindow->showPixels();
    position++;
    usleep(10000);
  }

  for (int i = 0; i < this->nbElements; i++) {
  // this->addElement("0");
  // this->addElement("0");
  // this->addElement("JOUER");
  // this->addElement("XXX");
  // this->addElement("RETOUR");
    for (unsigned int j = 0; j < this->elements[i].size(); j++) {
      if(i==0){
        window->print(x+j,y+i, "PLAYER", COLOR_PAIR(17) | A_BOLD);
        window->print(x+20+j,y+i, this->elements[i].substr(j,1), COLOR_PAIR(10) | A_BOLD);
      }else if(i==1){
        window->print(x+j,y+i, "BOT", COLOR_PAIR(17) | A_BOLD);
        window->print(x+20+j,y+i, this->elements[i].substr(j,1), COLOR_PAIR(10) | A_BOLD);
      }else if(i==2){
        window->print(x+20+j,y+i+2, this->elements[i].substr(j,1), COLOR_PAIR(10) | A_BOLD);
      }else if(i==4){
        window->print(x+j,y+i+4, this->elements[i].substr(j,1), COLOR_PAIR(10) | A_BOLD);
        usleep(10000);
      }else if(i==3)
        window->print(5+j,y+i+2, this->elements[i].substr(j,1), COLOR_PAIR(10) | A_BOLD);
      usleep(10000);
      }
  }
}

void CustomizeGame::moveArrow(Window *window,int move){
  if(positionArrow==0 || positionArrow==1){
    window->print(this->widthS/2 - 28 + 20,positionArrow + 20,"  ", COLOR_PAIR(10) | A_BOLD);
  }else if(positionArrow==2){
    window->print(this->widthS/2 - 28 + 20,positionArrow + 20 + 2,"  ", COLOR_PAIR(10) | A_BOLD);
  }else if(positionArrow==4){
    window->print(this->widthS/2 - 28 + 20,positionArrow + 20 + 4,"  ", COLOR_PAIR(10) | A_BOLD);
  }else if(positionArrow==3)
    window->print(1,positionArrow + 20 + 2,"  ", COLOR_PAIR(10) | A_BOLD);

  positionArrow += move;
  if(positionArrow < 0)
    positionArrow = nbElements -1;
  if(positionArrow >= nbElements)
    positionArrow = 0;

  if(positionArrow==0 || positionArrow==1){
    window->print(this->widthS/2 - 28 + 20,positionArrow + 20,arrow, COLOR_PAIR(10) | A_BOLD);
  }else if(positionArrow==2){
    window->print(this->widthS/2 - 28 + 20,positionArrow + 20 + 2,arrow, COLOR_PAIR(10) | A_BOLD);
  }else if(positionArrow==4){
    window->print(this->widthS/2 - 28 + 20,positionArrow + 20 + 4,arrow, COLOR_PAIR(10) | A_BOLD);
  }else if(positionArrow==3)
    window->print(1,positionArrow + 20 + 2,arrow, COLOR_PAIR(10) | A_BOLD);
}
