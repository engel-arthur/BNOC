#include "../Headers/Player.h"

using namespace std;

Player::Player(){
  name="Thibault et la SNCF";
  int nbShip = 5;
  shipNames = new string[nbShip];
  shipNames[0]="porte_avion";
  shipNames[1]="croiseur";
  shipNames[2]="contre-torpilleur";
  shipNames[3]="sous-marin";
  shipNames[4]="torpilleur";
  territory = new Territory(10,10,shipNames, nbShip);
  isReal=false;
  score=0;
}
Player::Player(std::string nick, int territoryH, int territoryW, std::string* shipNames, int nbShips, bool realPlayer) : name(nick), shipNames(shipNames), territory(new Territory(territoryH, territoryW, shipNames, nbShips)), isReal(realPlayer), score(0)
{}
Player::Player(std::string nick, Territory* terr ,bool realPlayer):name(nick) , shipNames(NULL), territory(terr), isReal(realPlayer), score(0)
{}

Player::~Player(){
  delete[] territory;
}

bool Player::isAlive(){
  return (territory->getNbShipAlive() > 0);
}
bool Player::attack(Player *enemy, int x, int y, bool &sunk){
  return enemy->getTerritory()->attackTerritory(x, y, sunk);
}
//GETTERS
Territory* Player::getTerritory(){
  return territory;
}
int Player::getScore() const{
  return score;
}
string Player::getName(){
  return name;
}
bool Player::isHuman() const{
  return isReal;
}
void Player::increaseScore(int n)
{
  score+=n;
}
std::string* Player::getShipsName() const{
  return shipNames;
}

void Player::setName(std::string newName){
  if(newName != ""){
    name = newName;
  }
}

void Player::setShipsName(std::string* shipNames){
  this->shipNames = shipNames;
}
