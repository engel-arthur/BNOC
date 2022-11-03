#include "../Headers/Option.h"

Option::Option(): id(-1), name(), shortcuts(), desc(), type(NONE){}

Option::Option(int id, const std::string &name, const std::string &shortcuts, const std::string desc, Type type):
id(id), name(name), shortcuts(shortcuts), desc(desc), type(type){}

//GETTER
int Option::getId() const{return id;}
std::string Option::getName ()const{return name;}
std::string Option::getShortcuts ()const{return shortcuts;}
std::string Option::getDesc ()const{return desc;}
Option::Type Option::getType() const{return type;}

//SETTER
void Option::setId (int id){this->id = id;}
void Option::setName (const std::string &name){this->name = name;}
void Option::setShortcuts (const std::string &shortcuts){this->shortcuts = shortcuts;}
void Option::setDesc (const std::string &desc){this->desc = desc;}
void Option::setType (Type t){this->type = t;}

//Affichage
void Option::print() const{
  std::cout << name << " (" << shortcuts << ") " << Type2String(type) << "\t--> " << desc << std::endl;
}

std::string Type2String(Option::Type t){
  std::string temp;
  switch (t) {
    case Option::NONE : temp=""; break;
    case Option::BOOL : temp="<bool>"; break;
    case Option::INT : temp="<int>"; break;
    case Option::FLOAT : temp="<float>"; break;
    case Option::CHAR : temp="<char>"; break;
    case Option::STRING : temp="<string>"; break;
  }
  return temp;
}
