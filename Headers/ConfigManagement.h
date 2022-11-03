#ifndef _CONFIGMANAGEMENT_H
#define _CONFIGMANAGEMENT_H
#include <string>
#include <iostream>
#include <fstream>//pour lire les fichiers
#include <sstream>
#include <ncurses.h>
#include "Option.h"

#define NMAX_OPTS 100

class ConfigManagement{
  public:
    static void loadConfig();
    static void loadColor(std::string filename = "Color.txt");
    static void checkParam(int argc, char **argv);
    //cette fonction modifie le parent et l'enfant tel que, l'enfant prenne le premier element du parent quand
    //Il rencontre le separateur, le parent deviens donc lui même sans son premier element
    static void getFirstElement(std::string &parent, std::string &enfant, std::string &separator);

    static int getHScreen();
    static int getWScreen();

    static std::string getNameSave(); //Obtenir le nom de la sauvegarde
    static bool existSave(); //Savoir si une sauvegarde a été demandé

    static void showAllOptions();
  private:
  	ConfigManagement();//inutile
  	~ConfigManagement();//inutile

    static int heightScreen;
    static int widthScreen;
    static Option opts[NMAX_OPTS];
    static size_t nbOptions;
    static void addOption(Option opt);
    static std::string saveName;
};

#endif
