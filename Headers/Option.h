#ifndef _OPTION_H
#define _OPTION_H

#include <string>
#include <iostream>

class Option{
  public:
    enum Type {
      NONE,
      BOOL,
      INT,
      FLOAT,
      CHAR,
      STRING
    };
  private:
    int id;
    std::string name, shortcuts, desc;
    Type type;
  public:
    Option();
    Option(int id, const std::string &name, const std::string &shortcuts, const std::string desc, Type type);

    //GETTER
    int getId() const;
    std::string getName ()const;
    std::string getShortcuts ()const;
    std::string getDesc ()const;
    Type getType() const;

    //SETTER
    void setId (int id);
    void setName (const std::string &name);
    void setShortcuts (const std::string &shortcuts);
    void setDesc (const std::string &desc);
    void setType (Type t);

    //Affichage
    void print() const;
};
//Permet de convertir le type en string
std::string Type2String(Option::Type t);

#endif
