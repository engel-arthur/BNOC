#include "../Headers/Territory.h"
using namespace std;

Territory::Territory() : heightT(10), widthT(10), nbShipT(5)
{
  positionAttacked = new int*[heightT];
  grilleTerritory = new int*[heightT];
  grilleForEnn = 0;

  for (int i = 0; i < heightT; i++) {
    positionAttacked[i] = new int[widthT];
    grilleTerritory[i] = new int[widthT];
    for (int j = 0; j < widthT; j++) {
      positionAttacked[i][j] = 0;
      grilleTerritory[i][j] = 11; //11 correspond au background
    }
  }

  string *shipNames = new string[nbShipT];
  shipNames[0] = "porte_avion";
  shipNames[1] = "croiseur";
  shipNames[2] = "contre-torpilleur";
  shipNames[3] = "sous-marin";
  shipNames[4] = "torpilleur";
  allShip = new Ship*[nbShipT];
  for (int i = 0; i < nbShipT; i++) {
    int h, w, vie;
    bool** temp = Picture::getShip(shipNames[i] + ".ship", &h, &w, &vie);
    allShip[i] = new Ship(temp, h, w, vie, 20 + (i % 10));
  }
}

Territory::Territory(int height, int width, string* shipNames, int nbShip) : heightT(height), widthT(width), nbShipT(nbShip)
{
  positionAttacked = new int*[heightT];
  grilleTerritory = new int*[heightT];
  grilleForEnn = 0;

  for (int i = 0; i < heightT; i++) {
    positionAttacked[i] = new int[widthT];
    grilleTerritory[i] = new int[widthT];
    for (int j = 0; j < widthT; j++) {
      positionAttacked[i][j] = 0;
      grilleTerritory[i][j] = 11; //11 correspond au background
    }
  }
  allShip = new Ship*[nbShipT];
  for (int i = 0; i < nbShipT; i++) {
    int h, w, vie;
    bool** temp = Picture::getShip(shipNames[i] + ".ship", &h, &w, &vie);
    allShip[i] = new Ship(temp, h, w, vie, 20 + (i % 10));
  }
}

Territory::Territory(int height, int width, Ship** shipAll, int nbShips) : heightT(height), widthT(width), allShip(shipAll), nbShipT(nbShips)
{
  positionAttacked = new int*[heightT];
  grilleTerritory = new int*[heightT];
  grilleForEnn = 0;

  for (int i = 0; i < heightT; i++) {
    positionAttacked[i] = new int[widthT];
    grilleTerritory[i] = new int[widthT];
    for (int j = 0; j < widthT; j++) {
      positionAttacked[i][j] = 0;
      grilleTerritory[i][j] = 11; //11 correspond au background
    }
  }
  for (int i = 0; i < nbShips; i++) {
    setShipPos(i, allShip[i]->getX(), allShip[i]->getY());
  }
}

void Territory::load(int height, int width, std::string* shipNames, int nbShips){
  positionAttacked = new int*[heightT];
  grilleTerritory = new int*[heightT];
  grilleForEnn = 0;

  for (int i = 0; i < heightT; i++) {
    positionAttacked[i] = new int[widthT];
    grilleTerritory[i] = new int[widthT];
    for (int j = 0; j < widthT; j++) {
      positionAttacked[i][j] = 0;
      grilleTerritory[i][j] = 11; //11 correspond au background
    }
  }
  allShip = new Ship*[nbShipT];
  for (int i = 0; i < nbShipT; i++) {
    int h, w, vie;
    bool** temp = Picture::getShip(shipNames[i] + ".ship", &h, &w, &vie);
    allShip[i] = new Ship(temp, h, w, vie, 20 + i % 10);
  }
}

Territory::~Territory(){
  for (int i = 0; i < heightT; i++) {
    delete[] positionAttacked[i];
    delete[] grilleTerritory[i];
    if(grilleForEnn != 0) delete[] grilleForEnn[i];
  }
  for (int i = 0; i < nbShipT; i++) {
    delete[] allShip[i];
  }
  delete[] positionAttacked;
  delete[] grilleTerritory;
  delete[] allShip;
  if(grilleForEnn != 0) delete[] grilleForEnn;
}

int Territory::getNbShip(){
  return nbShipT;
}

int Territory::getNbShipAlive(){
  int nbShipAlive = 0;
  for (int i = 0; i < nbShipT; i++) {
    if(allShip[i]->isAlive()) nbShipAlive++;
  }
  return nbShipAlive;
}

int** Territory::afficheTerritory(int& height, int& width, bool attacked){
  height = heightT;
  width = widthT;
  if(attacked){
    int** grilleTemp = new int*[heightT];

    for (int i = 0; i < heightT; i++) {
      grilleTemp[i] = new int[widthT];
      for (int j = 0; j < widthT; j++) {
        if(positionAttacked[i][j] == 2){
          grilleTemp[i][j] = 13;
        }else if(positionAttacked[i][j] == 1){
          grilleTemp[i][j] = 12;
        }else{
          grilleTemp[i][j] = grilleTerritory[i][j]; //11 correspond au background
        }
      }
    }
    return grilleTemp;
  }else{
    return grilleTerritory;
  }
}

int** Territory::afficheTerrForEnn(int& height, int& width){
  height = heightT;
  width = widthT;

  if(grilleForEnn == 0){
    grilleForEnn = new int*[heightT];

    for (int i = 0; i < heightT; i++) {
      grilleForEnn[i] = new int[widthT];
      for (int j = 0; j < widthT; j++) {
        if(positionAttacked[i][j] == 2){
          grilleForEnn[i][j] = 13;
        }else if(positionAttacked[i][j] == 1){
          grilleForEnn[i][j] = 12;
        }else{
          grilleForEnn[i][j] = 11; //11 correspond au background
        }
      }
    }
  }
  return grilleForEnn;
}

bool Territory::alreadyAttacked(int x, int y){
  return !(positionAttacked[x][y] == 0);
}

void Territory::resetTerr(){
  for (int i = 0; i < heightT; i++){
    for (int j = 0; j < widthT; j++){
      grilleTerritory[i][j] = 11;
    }
  }
}

bool Territory::attackTerritory(int x, int y, bool &sunk){
  if(x < 0 || y < 0 || x > widthT || y>heightT) return false;
  sunk = false;
  if(grilleTerritory[x][y] != 11 && grilleTerritory[x][y] != 12 && grilleTerritory[x][y] != 13){
    grilleForEnn[x][y] = 13;    //On remplace la grille avec du noir car touch??
    positionAttacked[x][y] = 2; //Attaqu?? et touch??
    touchBefore = true;         //On transmet l'information que le ship est touch??

    for (int i = 0; i < getNbShip(); i++) {
      //TEST SI LE POINT APPARTIENT A LA MATRICE DU SHIP
        //On r??cup??re le ship
          int hSM, wSM;
          bool** shipMatrice = allShip[i]->getShipMatrice(hSM, wSM);
      //Si pour chaques bateau on trouve un dont la position correspond ?? celui attaqu??
      //On v??rifie si la position x, y est dans un ship
      if(x - allShip[i]->getX() < hSM && x - allShip[i]->getX() >=0 && y - allShip[i]->getY() < wSM && y - allShip[i]->getY() >= 0){
        if(shipMatrice[x - allShip[i]->getX()][y - allShip[i]->getY()]){
          allShip[i]->removeLife(); //On enl??ve de la vie au Ship
          if(!allShip[i]->isAlive()){
            sunk = true; //Le bateau est coul??
            for (int posx = allShip[i]->getX(); posx < allShip[i]->getX() + hSM; posx++) {
              for (int posy = allShip[i]->getY(); posy < allShip[i]->getY() + wSM; posy++) {
                if(grilleTerritory[posx][posy] == allShip[i]->getColor()){
                  grilleForEnn[posx][posy] = allShip[i]->getColor();
                }
              }
            }
          }
        break; //On a trouv?? le ship
        }
      }
    }
  }else{
    positionAttacked[x][y] = 1; //Attaqu?? mais pas touch??
    grilleForEnn[x][y] = 12; //On remplace la grille avec du gris car pas touch??
    touchBefore =false; //On transmet l'information que le ship est pas touch??
  }
  return touchBefore;
}

bool Territory::undoAttack(int x, int y){
  //Si la position undo est en dehors du Territory on retourne faux
    if(x < 0 || y < 0 || x > widthT || y>heightT) return false;
  //Si l'attaque undo n'est ni le background, ni touch??, ni rat?? alors c'est qu'on est sur un bateau
  //Il faut donc le trouver et lui redonner de la vie
  //Si il avait ??t?? coul?? auparavant il faut enlever la couleur sur grilleForEnn
  if(grilleTerritory[x][y] != 11 && grilleTerritory[x][y] != 12 && grilleTerritory[x][y] != 13){
    for (int i = 0; i < getNbShip(); i++) {
      //TEST SI LE POINT APPARTIENT A LA MATRICE DU SHIP
        //On r??cup??re le ship
          int hSM, wSM;
          bool** shipMatrice = allShip[i]->getShipMatrice(hSM, wSM);
      //Si pour chaques bateau on trouve un dont la position correspond ?? celui attaqu??
      //On v??rifie si la position x, y est dans un ship
      if(x - allShip[i]->getX() < hSM && x - allShip[i]->getX() >=0 && y - allShip[i]->getY() < wSM && y - allShip[i]->getY() >= 0){
        if(shipMatrice[x - allShip[i]->getX()][y - allShip[i]->getY()]){
          if(!allShip[i]->isAlive()){
            for (int posx = allShip[i]->getX(); posx < allShip[i]->getX() + hSM; posx++) {
              for (int posy = allShip[i]->getY(); posy < allShip[i]->getY() + wSM; posy++) {
                if(grilleTerritory[posx][posy] == allShip[i]->getColor()){
                  grilleForEnn[posx][posy] = 13; //On enl??ve la couleur du bateau touch?? en rempla??ant par du noir
                }
              }
            }
          }
          allShip[i]->removeLife(-1); //On ajoute de la vie au Ship
          break; //On a trouv?? le ship
        }
      }
    }
  }
  grilleForEnn[x][y] = 11; //On remet le background
  positionAttacked[x][y] = 0; //On reset l'attaque des positions
  return false;
}

bool Territory::setShipPos(int i, int x, int y){
  //Si le Ship n'existe pas on retourne 0
    if(i >= nbShipT || i < 0) return false;

  //On r??cup??re les infos du bateau en question
    int heightShip, widthShip;
    bool** matriceShip = allShip[i]->getShipMatrice(heightShip, widthShip);

  //Si le bateau sort du territory on retourne faux
    if(heightShip + x > heightT || widthShip + y > widthT || x < 0 || y < 0)
      return false;

  //On recherche si une position du bateau pleine est sur une couleur non par d??faut de grilleTerritory
  //Soit son background
    for (int posI = 0; posI < heightShip; posI++) {
      for (int posY = 0; posY < widthShip; posY++) {
        if(matriceShip[posI][posY] && grilleTerritory[x + posI][y + posY] != 11)
          //On a trouv?? une position commune ?? Ship et un autre Ship
          return false;
      }
    }
  //La position du bateau est correcteur on push le bateau sur la grille
  //C'est ?? dire on applique la couleur du bateau ?? grilleTerritory
    for (int posI = 0; posI < heightShip; posI++) {
      for (int posY = 0; posY < widthShip; posY++) {
        if(matriceShip[posI][posY])
          grilleTerritory[x + posI][y + posY] = allShip[i]->getColor();
      }
    }

  //On applique le changement de position au ship
  allShip[i]->setPosition(x,y);

  return true; //Tout s'est bien pass??

}

bool Territory::getTouchBefore() const{
  return touchBefore;
}

Ship** Territory::getShips(int& nbShips) const{
  nbShips = nbShipT;
  return allShip;
}

int Territory::getHeight() const{
  return heightT;
}

int Territory::getWidth() const{
  return widthT;
}
