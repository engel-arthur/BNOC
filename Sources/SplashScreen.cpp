#include "../Headers/SplashScreen.h"
#include "../Headers/Message.h"

using namespace std;

SplashScreen SplashScreen::m_instance = SplashScreen();

SplashScreen::SplashScreen(){
  //nothing
}
SplashScreen::~SplashScreen(){
  //nothing
}

SplashScreen& SplashScreen::Instance(){
  return m_instance;
}

int SplashScreen::start(){
  int heightS = 40, widthS = 130;
  Window *menu = new Window(heightS,widthS,0,0);
  menu->setCouleurFenetre(WBLACK);

  int** logo;
  int logoHauteur;
  int logoLargeur;
  logo = Picture::getPicture("logoBorder.txt", &logoHauteur, &logoLargeur);

  int** bnocLogo;
  int bnocLogoHauteur;
  int bnocLogoLargeur;
  bnocLogo = Picture::getPicture("Bnoc.txt", &bnocLogoHauteur, &bnocLogoLargeur);

  int** Splash;
  int SplashHauteur;
  int SplashLargeur;
  Splash = Picture::getPicture("SplashScreen.txt", &SplashHauteur, &SplashLargeur);

  menu->setPixels(Splash, SplashHauteur, SplashLargeur,0,0);
  //menu->setPixels(logo, logoHauteur, logoLargeur,4,widthS/4 - logoLargeur/2);
  menu->setPixels(bnocLogo, bnocLogoHauteur, bnocLogoLargeur,heightS/2 - bnocLogoHauteur/2 - 7,widthS/4 - bnocLogoLargeur/2);

  menu->showPixels();

  string text = "BATAILLE NAVALE OF CLANS";
  menu->print(widthS/2 - bnocLogoLargeur -1,heightS/2 + bnocLogoHauteur/2 -6,text, COLOR_PAIR(10) | A_BOLD);
  Message::Alert("APPUYEZ SUR ENTREE", "homeMessage.txt", 3, 24, 51, 1, 1, 18);

  // string text = "Press Enter to continue...";
  // menu->print(widthS/2 - text.size()/2,heightS - 3,text);
  //
  // text = "BATAILLE NAVALE OF CLANS";
  // menu->print(widthS/2 - text.size()/2,2,text, COLOR_PAIR(5) | A_BOLD);
  //
  // text = "DEVELOPPED BY : ";
  // menu->print(widthS/2 - text.size()/2,heightS/2,text,A_BOLD);
  //
  // text = "- Arthur Engel";
  // menu->print(widthS/2 - 10,heightS/2 + 2,text);
  //
  // text = "- Gaël Mullier";
  // menu->print(widthS/2 - 10,heightS/2 + 3,text);
  //
  // text = "- Romain Saclier";
  // menu->print(widthS/2 - 10,heightS/2 + 4,text);
  //
  // text = "- Thibaut Siniecki";
  // menu->print(widthS/2 - 10,heightS/2 + 5,text);
  //
  // text = "@2017";
  // menu->print(8,heightS - 1,text,A_BOLD);

  //Tant qu'on a appuyé sur "Entrée" on sort pas
  int c = 0;
  while(c != '\n'){
    c = wgetch(menu->getWindow());
		switch(c){
			case 'a':
        Splash = Picture::getPicture("SplashScreen2.txt", &SplashHauteur, &SplashLargeur);
        // menu->clearPixels(logo, logoHauteur, logoLargeur,widthS/2 - logoLargeur,4);
        logo = Picture::rotationR(logo, logoHauteur, logoLargeur);
        menu->setPixels(Splash, SplashHauteur, SplashLargeur,0,0);
        menu->setPixels(logo, logoHauteur, logoLargeur,4,widthS/4 - logoLargeur/2);
				break;
      case 'z':
        Splash = Picture::getPicture("SplashScreen.txt", &SplashHauteur, &SplashLargeur);
        // menu->clearPixels(logo, logoHauteur, logoLargeur,widthS/2 - logoLargeur,4);
        logo = Picture::rotationL(logo, logoHauteur, logoLargeur);
        menu->setPixels(Splash, SplashHauteur, SplashLargeur,0,0);
        menu->setPixels(logo, logoHauteur, logoLargeur,4,widthS/4 - logoLargeur/2);
				break;
		}
    menu->showPixels();
  }

  menu->showPixelsHARD();
  int position = 0;
  int positionTemp = 0;
  while(position < 25){
    position++;
    if(positionTemp < 10)
      positionTemp = position/5;

    menu->fillPixels(10, SplashHauteur, SplashLargeur);
    menu->setPixels(Splash, SplashHauteur, SplashLargeur,position,0);
    menu->setPixels(bnocLogo, bnocLogoHauteur, bnocLogoLargeur,heightS/2 - bnocLogoHauteur/2 - (7 + positionTemp),widthS/4 - bnocLogoLargeur/2);
    menu->showPixels();
    usleep(20000);
  }

  delete[] logo;
  delete[] Splash;
  return 1;
}
