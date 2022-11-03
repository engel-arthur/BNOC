#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include "Headers/window.h"
#include "Headers/Score.h"
#include "Headers/Ship.h"
#include "Headers/SplashScreen.h"
#include "Headers/Menu.h"
#include "Headers/Message.h"
#include "Headers/Game.h"
#include "Headers/Player.h"
#include "Headers/CustomizeGame.h"
#include "Headers/PreparationPlayer.h"
#include "Headers/ConfigManagement.h"

using namespace std;

//Changer la taille du terminal
void setScreenSize(int height = 40, int width = 130);

int main(int argc, char** argv){
  ConfigManagement::loadConfig();
  //Ajouter les couleurs
  setenv("TERM", "xterm-256color", true);
  setScreenSize(ConfigManagement::getHScreen(), ConfigManagement::getWScreen());

  if(argc > 1) {
    ConfigManagement::checkParam(argc, argv);
    cout << "Appuyer sur <entrée> pour continuer..." ;
    getchar();
  }

  startProgramX();
  ConfigManagement::loadColor("Color.txt");

  if(ConfigManagement::existSave()){
    Game* game = new Game(ConfigManagement::getNameSave(), false);
    game->start();
  }

  Menu *menu = new Menu();
  CustomizeGame *custo = new CustomizeGame();
  SplashScreen::start();

  int choice = 0;
  int choice2 = 0;

  int nbBot = 0;
  int nbPlayer = 0;
  string chemin;

  do {
    choice = menu->start();
    if(choice == 0){
      Game* game = new Game;
      game->start();
      delete game;
    }else if(choice == 1){
      do {
        choice2 = custo->start(nbBot, nbPlayer, chemin);
        if(choice2 == 2){
          if(nbBot == 0 && nbPlayer == 0){
            Game* game = new Game(chemin);
            game->start();
            delete game;
          }else{
            Game* game = new Game(nbPlayer, nbBot);
            game->start();
            delete game;
          }
        }else{
          cout << "Choix : " << choice2 << endl;
        }
      } while(choice2 != 3);
    }else
      cout << "Choix : " << choice << endl;

  } while(choice != 4);
  stopProgramX();
  //cout << choice << endl;
  //Enlever les couleurs
  unsetenv("TERM");
  // cout << "nbPlayer : " << nbPlayer << endl;
  // cout << "nbBot : " << nbBot << endl;
  // cout << "Chemin : " << chemin << endl;
  return 0;
}

void setScreenSize(int height, int width){
  cout << "\033[8;"<< height << ";" << width << "t";
}
