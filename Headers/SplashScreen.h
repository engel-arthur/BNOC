#ifndef _SPLASHSCREEN_H
#define _SPLASHSCREEN_H

#include "../Headers/window.h"
#include "../Headers/Picture.h"
#include <string>

class SplashScreen{
  public:
    static SplashScreen& Instance();
    static int start();

  private:
    SplashScreen(const SplashScreen&){}
    static SplashScreen m_instance;
    SplashScreen();
    ~SplashScreen();
};

#endif
