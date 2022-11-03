#include "../Headers/Ship.h"
#include "../Headers/Picture.h"

#include <iostream>

Ship::Ship() : _height(5), _width(1), _life(5), _color(13), _posX(-1), _posY(-1), _nbRotation(0)
{
  _pixelShip = 0;
  _matriceShip = new bool*[_height];
  for (int i = 0; i < _height; i++) {
    _matriceShip[i] = new bool[_width];
    for (int j = 0; j < _width; j++) {
      _matriceShip[i][j] = 1;
    }
  }
}

Ship::Ship(bool** matriceShip, int h, int w, int life, int color) : _matriceShip(matriceShip), _height(h), _width(w), _life(life), _color(color), _posX(-1), _posY(-1), _nbRotation(0)
{
  _pixelShip = 0;
}

Ship::~Ship(){
  for (int i = 0; i < _height; i++) {
    delete[] _matriceShip[i];
    delete[] _pixelShip[i];
  }
  delete[] _matriceShip;
  delete[] _pixelShip;
}

/* Getter */
  int Ship::getHeight() const   {return this->_height;}
  int Ship::getWidth() const    {return this->_width;}
  int Ship::getLife() const     {return this->_life;}
  int Ship::getColor() const    {return this->_color;}
  int Ship::getX() const        {return this->_posX;}
  int Ship::getY() const        {return this->_posY;}
  int Ship::getRotation() const       {return this->_nbRotation;}
  bool** Ship::getShipMatrice(int& height, int& width) const{
    height = getHeight();
    width = getWidth();
    return _matriceShip;
  }

  int** Ship::getPixelShip(int& height, int& width){
    height = this->_height;
    width = this->_width;
    if(_pixelShip == 0){
      this->_pixelShip = new int*[height];
      for (int i = 0; i < height; i++) {
        this->_pixelShip[i] = new int[width];
        for (int j = 0; j < width; j++) {
          if(this->_matriceShip[i][j]){
            this->_pixelShip[i][j] = this->_color;
          }else{
            this->_pixelShip[i][j] = 0;
          }
        }
      }
    }
    return this->_pixelShip;
  }

/* Setter */
  void Ship::setColor(int color){
    this->_color = color;
  }

  void Ship::setPosition(int x, int y){
    this->_posX = x;
    this->_posY = y;
  }

/* Methods */
  int** Ship::show(){
    int** matrice = new int*[this->_height];
    for (int i = 0; i < this->_height; i++) {
      for (int j = 0; j < this->_width; j++) {
        if(this->_matriceShip[i][j] == false){
          matrice[i][j] = this->_color;
        }else{
          matrice[i][j] = this->_color;
        }
      }
    }
    return matrice;
  }
  bool Ship::isAlive(){
    return this->_life > 0;
  }
  void Ship::rotate(int degree){
    if(degree == 90){
      this->_matriceShip = Picture::rotationR(this->_matriceShip, this->_height, this->_width);
      _nbRotation++;
    }else if(degree == -90){
      this->_matriceShip = Picture::rotationL(this->_matriceShip, this->_height, this->_width);
      _nbRotation+=3;
    }else{
      this->_matriceShip = Picture::rotationR(this->_matriceShip, this->_height, this->_width);
      this->_matriceShip = Picture::rotationR(this->_matriceShip, this->_height, this->_width);
      _nbRotation+=2;
    }
    _pixelShip = 0;
    _nbRotation = _nbRotation % 4;
  }

  void Ship::removeLife(int vie){
    this->_life -= vie;
  }
