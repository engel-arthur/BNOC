#include "../Headers/ConfigManagement.h"
using namespace std;

#define HELP_ID 1
#define VERSION_ID 2
#define AUTHOR_ID 3
#define BNOC_ID 4
#define LOAD_ID 5

#define VERSION_BNOC "0.1.0"

int ConfigManagement::heightScreen = 40;
int ConfigManagement::widthScreen = 130;
size_t ConfigManagement::nbOptions = 0;
string ConfigManagement::saveName = "";

Option ConfigManagement::opts[NMAX_OPTS];

void ConfigManagement::loadConfig(){
  addOption(Option(HELP_ID,"--HELP", "-h", "connaître toutes les options", Option::NONE));
  addOption(Option(VERSION_ID,"--VERSION", "-v", "connaître la version du programme", Option::NONE));
  addOption(Option(AUTHOR_ID,"--AUTHOR", "-a", "connaître toutes les auteurs du programme", Option::NONE));
  addOption(Option(BNOC_ID,"--BNOC", "-b", "???", Option::NONE));
  addOption(Option(LOAD_ID,"--LOAD", "-l", "load une save et jouer avec", Option::STRING));
}

void ConfigManagement::loadColor(string filename){
  string convert="Config/" + filename;//on compose le chemin à partir du nom du fichier
  ifstream fichier(convert.c_str(), ios::in);  // on ouvre le fichier en lecture

  if(fichier){
    string ligne;
    while(getline(fichier, ligne)){
      //SUPPRESSION DES COMMENTAIRES
      for (unsigned int i = 0; i < ligne.size(); i++) {
        if(ligne.at(i) == '#'){
          ligne = ligne.substr(0,i);
        }
      }
      if(ligne != ""){
        string separator = " ";
        unsigned int pos = ligne.find(separator);
        string token = ligne.substr(0, pos);
        string suite = ligne.substr(pos,ligne.size()-1);

        if(token == "COLOR"){
          string id = "";
          string R = "";
          string G = "";
          string B = "";
          getFirstElement(suite, id, separator);
          getFirstElement(suite, R, separator);
          getFirstElement(suite, G, separator);
          getFirstElement(suite, B, separator);
          int r = stoi(R)* (1000 / 255);
          int g = stoi(G)* (1000 / 255);
          int b = stoi(B)* (1000 / 255);
          init_color(stoi(id),r ,g ,b);

        }else if(token == "PAIR"){
          string id = "";
          string idColor1 = "";
          string idColor2 = "";
          getFirstElement(suite, id, separator);
          getFirstElement(suite, idColor1, separator);
          getFirstElement(suite, idColor2, separator);
          init_pair(stoi(id),stoi(idColor1),stoi(idColor2));
        }

        cout << endl;
      }
    }
    fichier.close();  // on ferme le fichier
  }
  else  // sinon
    cerr << "Impossible d'ouvrir le fichier !" << endl;
}

void ConfigManagement::checkParam(int argc, char **argv){
  bool opt_error = false;
  int i=0;
  while (i < argc && !opt_error) {
    int pos=-1;
    for (size_t j = 0; j < nbOptions; j++) {
      if(opts[j].getName() == argv[i] || opts[j].getShortcuts() == argv[i]){
        pos = opts[j].getId();
        break;
      }
    }
    switch (pos) {
      case HELP_ID: showAllOptions(); break;
      case VERSION_ID: cout << "Version : " << VERSION_BNOC << endl; break;
      case AUTHOR_ID:   cout << "Romain Saclier" << endl << "Arthur Engel"
                             << endl << "Gael Mullier" << endl << "Thibaut "
                             << endl;
                        break;
      case BNOC_ID: cout << "coucou" << endl; break;
      case LOAD_ID:
        i++;
        if(i >= argc){ cout << "Aucun fichier entrée en paramètre" << endl; break;}
        saveName = argv[i];
        cout << "Chargement de la sauvegarde : " << argv[i] << endl;
        break;
    }
    i++;
  }
}

void ConfigManagement::getFirstElement(string &parent, string &enfant, string &separator){
  enfant = "";
  int pos = 0;
  unsigned comparaison = 0;
  while (enfant == "") {
    pos = parent.find(separator);
    enfant = parent.substr(0, pos); // token is "scott"
    if(pos > 0){
      comparaison = pos;
    }else{
      comparaison = 0;
    }
    if(enfant == "" && comparaison+1 <= parent.size()-1){
      parent = parent.substr(comparaison+1,parent.size()-1);
    }
  }
  if(parent.size()-1 >= 0 && pos >= 0)
    parent = parent.substr(comparaison,parent.size()-1);
}

int ConfigManagement::getHScreen(){
  return heightScreen;
}

int ConfigManagement::getWScreen(){
  return widthScreen;
}

string ConfigManagement::getNameSave(){
  if(saveName == ""){
    int i = 1;
    bool suite = true;
    do{
      stringstream ss;
      ss << i;
      saveName = "Save" + ss.str() + ".txt";
      string saveNameTEMP = "Save/"+saveName;
      ifstream fp(saveNameTEMP.c_str(), ios::in);
      if(fp){
        i++;
      }else{
        suite = false; //On a trouvé le fichier save non utilisé
      }
    }while(suite);
  }
  return saveName;
}

bool ConfigManagement::existSave(){
  return saveName != "";
}

void ConfigManagement::showAllOptions(){
  cout << "Affichages de toutes les options : " << endl;
  for (size_t i = 0; i < NMAX_OPTS; i++) {
    if(opts[i].getId() != -1){
      opts[i].print();
    }
  }
}

void ConfigManagement::addOption(Option opt){
  opts[nbOptions] = opt;
  nbOptions++;
}
