#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include "window.h"
#include "Picture.h"
#include <iostream>

class Message{
public:
  static void Alert(std::string message, std::string image, int h, int w, int x, int y, int yNew, int textColor);
  static void AlertAnims(std::string message, std::string image, std::string image2, int h, int w, int x, int y, int yNew, int textColor);

private:
	Message();//inutile
	~Message();//inutile

  static Window *popup;
};

#endif