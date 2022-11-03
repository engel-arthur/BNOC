#ifndef _TERRITORY_H
#define _TERRITORY_H

#include <string>
#include "Ship.h"
#include "Picture.h"

class Territory{
  public:
    /* CONSTRUCTOR */
      Territory();
      Territory(int height, int width, std::string* shipNames, int nbShips);
      Territory(int height, int width, Ship** shipAll, int nbShips);
    /* DESCTRUCTOR */
      ~Territory();

    /* GETTER */
      //Retourne le nombre total de Ship
      int getNbShip();
      //Retourne le nombre total de Ship qui sont en vie
      int getNbShipAlive();
      int getHeight() const; //Retourne hauteur du Territory
      int getWidth() const; //Retourne la largeur du Territory
      //Retourn Vrai si le Territory a un bateau touché lors de la dernière attaque
      bool getTouchBefore() const;
      //Renvois tous les ships
      Ship** getShips(int& nbShips) const;


    /* SETTER */
      //Set toutes les informations du Territory (Hauteur, Largeur, String contenant les noms des Ships, nombre de Ship)
      void load(int height, int width, std::string* shipNames, int nbShips);
      //Set la position du Ship (i) en x et y
      bool setShipPos(int i, int x=0, int y=0);

    /* METHODES TERRITORY */
      //Permet d'attaquer le territory à la position x, y, renvois vrai si un ship a été touché
      bool attackTerritory(int x, int y, bool &sunk);
      //Permet de reset une attaque sur Territoru
      bool undoAttack(int x, int y);
      //Renvois la grille de pixel du territory, si attacked est à true, on renvois toutes les positions touché
      int** afficheTerritory(int& height, int& width, bool attacked = false);
      //Renvois la grille de pixel pour les ennemis (positions des bateaux cachés)
      int** afficheTerrForEnn(int& height, int& width);
      //Renvois une grille contenant les positions attaqués 0 -> pas attaqué, 1 -> attaqué mais pas touché, 2 -> attaqué et touché
      bool alreadyAttacked(int x, int y);
      //Reset le territory
      void resetTerr();

  private:
    //Grille des potision attaqué 0 -> pas attaqué, 1 -> attaqué mais pas touché, 2 -> attaqué et touché
    int** positionAttacked;
    //Grille du Territory contenant les ships
    int** grilleTerritory;
    //Grille du Territory contenant les pixels
    int** grilleForEnn;

    //Hauteur du territory
    int heightT;
    //Largeur du territory
    int widthT;
    //Tous les Ships
    Ship** allShip;
    //Nombre de Ships
    int nbShipT;
    //Booléen permettant de savoir si le Territory a un ship touché après la dernière attaque
    bool touchBefore;

};

#endif
