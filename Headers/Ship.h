#ifndef _SHIP_H
#define _SHIP_H

#include "../Headers/window.h"

class Ship{
  public:
    Ship();
    Ship(bool** matriceShip, int h, int w, int life, int color);
    ~Ship();

    /* Getter */
      int getHeight() const;  //Récupère la hauteur
      int getWidth() const;   //Récupère la largeur
      int getLife() const;    //Récupère la vie
      int getColor() const;   //Récupère la couleur du bateau
      int getX() const;       //Récupère la position X du bateau
      int getY() const;       //Récupère la position X du bateau
      int getRotation() const;
      bool** getShipMatrice(int& height, int& width) const; //Récupère la matrice du bateau
      int** getPixelShip(int& height, int& width);

    /* Setter */
      void setColor(int color); //Change couleur du bateau
      void setPosition(int x, int y);

    /* Methods */
      int** show();                   //Afficher la grille de pixel dont le nombre correspond à une couleur d'après le fichier config
      bool isAlive();                 //Retourne vrai si le bateau est en vie
      void rotate(int degree);        //Tourne le bateau d'un certain degree
      void removeLife(int vie = 1);   //Enlève de la vie au bateau

      void load(bool** matriceShip, int h, int w, int life);

  private:
    //--------------------------------------
    //    Matrice du bateau
    //--------------------------------------
    //    False -> représente du vide
    //    True  -> représente case pleine
    //--------------------------------------
    bool** _matriceShip;
    int** _pixelShip;
    int _height, _width;
    int _life;
    int _color;
    int _posX;
    int _posY;
    int _nbRotation;
};

#endif
