#ifndef __WINDOW_H
#define __WINDOW_H

#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <iostream>

// Ensemble de couleurs possibles (fond+texte)
enum Color {
  WBLACK,  // couleur fond = noir ,   couleur texte = blanc
  WCYAN,   // couleur fond = cyan,    couleur texte = blanc
  WBLUE,   // couleur fond = bleu,    couleur texte = blanc
  WYELLOW, // couleur fond = jaune,   couleur texte = blanc
  WGREEN,  // couleur fond = vert,    couleur texte = blanc
  WMAGENTA,// couleur fond = magenta, couleur texte = blanc
  WRED,	   // couleur fond = rouge,   couleur texte = blanc
  BWHITE,  // couleur fond = blanc,   couleur texte = blanc
  BCYAN,   // couleur fond = cyan,    couleur texte = noir
  BBLUE,   // couleur fond = bleu,    couleur texte = noir
  BYELLOW, // couleur fond = jaune,   couleur texte = noir
  BGREEN,  // couleur fond = vert,    couleur texte = noir
  BMAGENTA,// couleur fond = magenta, couleur texte = noir
  BRED,    // couleur fond = rouge,   couleur texte = noir
  TEMPCOLOR
};

// fonction pour demarrer le mode console graphique
void startProgramX();
// fonction pour arreter le mode console graphique
void stopProgramX();

class Window {
 private:
  int height,width,startx,starty;
  WINDOW* win, *frame, *parent;
  Color colorwin, colorframe;
  int colorwinINT;
  char bord;
  void update() const;
  int** pixels;
  int** pixelsBEFORE;

  static int newColorINIT;
  static int newPairINIT;

 public:

  // constructeur d'un fenetre de hauteur=h, largeur=w dont le coin superieur gauche
  // a pour coordonnée (x,y), le caractère c est utilisé pour définir la bordure
  Window(int h,int w, int x, int y,bool border = false, char c=' ');
  //Constructeur pour créer une fenetre dans un parent
  Window(int h,int w, int x, int y,WINDOW *winParent);
  //Par default
  Window();

  ~Window();

  static int COLOR(int r, int g, int b, int rb = 0, int gb = 0, int bb = 0);

  // fonction permettant d'afficher une variable s de type (string ou char)
  // à la position (x,y) dans la fenetre.
  // si un couleur est spécifié l'affichage utilise cette couleur, sinon la couleur de la fenêtre est utilisée
  void print(int x, int y, std::string s, Color c) const;
  void print(int x, int y, std::string s, int mask, bool isColor = false, bool updateTHIS = true) const;
  void print(int x, int y, char s, int mask, bool isColor = false, bool updateTHIS = true) const;
  void print(int x, int y, char s, Color c) const;
  void print(int x, int y, std::string s) const;
  void print(int x, int y, char s) const;

  //Va modifier la matrice de pixel du window
  void setPixels(int** table,int heightTab, int widthTab, int x = 0, int y = 0);
  void fillPixels(int color, int heightTab, int widthTab, int x = 0, int y = 0);
  void showPixels() const;
  void showPixelsHARD() const;


  // accesseurs
  int getX() const;        // récupère l'abscisse du coin supérieur gauche de la fenêtre
  int getY() const;        // récupère l'ordonnée du coin supérieur gauche de la fenêtre
  int getHauteur() const ; // récupère la hauteur de la fenêtre
  int getLargeur() const ; // récupère la largeur de la fenêtre
  WINDOW* getWindow() const;

  Color getCouleurBordure() const; // récupère la couleur de la bordure
  Color getCouleurFenetre() const; // récupère la couleur de la fenêtre
  void setCouleurBordure(Color);   // modifie la couleur de la bordure
  void setCouleurFenetre(Color);   // modifie la couleur de la fenêtre (ATTENTION, tout le contenu de la fenêtre prend la couleur)
  void setCouleurFenetre(int mask);   // modifie la couleur de la fenêtre (ATTENTION, tout le contenu de la fenêtre prend la couleur)

  void clear() const; // enleve tout le contenu de la fenêtre

};

#endif
