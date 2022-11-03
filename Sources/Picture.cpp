#include "../Headers/Picture.h"

using namespace std;

bool Picture::validFile(string folder, string filename, int* h, int* w)
{
  *w=0;//ini de la largeur
  *h=1;//ini de la hauteur (1 puisqu'on va parcourir une ligne)
  bool result=false;
  string convert="Ressources/"+ folder + "/" + filename;//on compose le chemin à partir du nom du fichier
  ifstream file(convert.c_str());//on ouvre le fichier en lecture
  if(file)//si il s'ouvre
  {
      result=true;
      string lecture;//on crée une chaine qui va lire le fichier
      getline(file, lecture);//on inscrit la ligne 1 dans lecture
      *w=nbSpaces(lecture, result)+1;//on prend la taille de cette chaine pour la mettre dans largeur
      while(getline(file,lecture))//tant qu'on peut lire des lignes
      {
	if(nbSpaces(lecture, result)+1!=*w)//si la taille d'une ligne est différente de ce qu'on a mesuré au début
        {
           result=false;
	         break;//on sort de la boucle
	      }
	    (*h)++;//sinon on augmente la hauteur
	   }
  file.close();//on ferme le fichier
  }
  return result;
}
int** Picture::getPicture(string filename, int* h, int* w)
{
  if(validFile("Image", filename, h, w))//si le fichier est valide
  {
      string convert="Ressources/Image/"+ filename;//on fait une chaine contenant le chemin pour ouvrir le fichier
      string aux;
      ifstream file(convert.c_str(), ios::in);//ouverture en lecture
      if(file)//si le fichier s'ouvre
      {
       int** resultat=new int*[*h];
       for(int i(0); i<*h; i++)
       {
         resultat[i]=new int[*w];
         for(int j(0); j<*w; j++)//+1 pour compter le saut de ligne (et oui il est compté ce sale petit polisson)
         {
	   file >> aux;//on fait un tableau de taille w*h tout en écrivant les caractères du fichier convertis en entier
	   resultat[i][j]=stoi(aux);
  	      }
        }
        return resultat;
      }
  }
  *w=0;
  *h=0;
  int** resultat= new int*[0];
  resultat[0]=new int[0];
  return resultat;
}
bool** Picture::getShip(string filename, int* h, int* w, int* vie)
{
    *vie=0;
    if(validFile("Ship", filename, h, w))
    {
      string convert="Ressources/Ship/"+ filename;
      string aux;
      ifstream file(convert.c_str(), ios::in);
      if(file)
      {
       bool** resultat=new bool*[*h];
       for(int i(0); i<*h; i++)
       {
          resultat[i]=new bool[*w];
          for(int j(0); j<*w; j++)
          {
            file >> aux;
		          if(aux=="0")//si la chaîne est 0, on écrit false
                resultat[i][j]=false;
			  else //si la chaîne est 1 ou autre chose, on écrit true
			    {
			      resultat[i][j]=true;
			      (*vie)++;
			    }
          }
       }
        return resultat;
     }
    }
    *h=0;
    *w=0;
bool** resultat= new bool*[0];
resultat[0]=new bool[0];
return resultat;
}
int Picture::nbSpaces(string str, bool& doubleSpace)
{
  int result=0;
  for(size_t i(0); i<str.size();i++)
    {
      if (str.at(i) == ' ')
	{
	result++;
	if(i!=0 && str.at(i-1)== ' ')
	  {
	  doubleSpace=false;
	  return 0;
	  }
	}
    }
  return result;
}
int** Picture::rotationR(int** tab, int& h, int& w)
{
  int** result = new int*[w];
  for(int i(0); i<w; i++)
  {
    result[i]= new int[h];
    for(int j(0); j<h; j++)
    {
      result[i][j]=tab[h-1-j][i];
    }
  }
  int aux=h;
  h=w;
  w=aux;
  return result;
}
bool** Picture::rotationR(bool** tab, int& h, int& w)//algorithme évident
{
  bool** result = new bool*[w];
  for(int i(0); i<w; i++)
  {
    result[i]= new bool[h];
    for(int j(0); j<h; j++)
    {
      result[i][j]=tab[h-1-j][i];
    }
  }
  int aux=h;//on inverse h et w pour facilité d'utilisation
  h=w;
  w=aux;
  return result;
}
int** Picture::rotationL(int** tab, int& h, int& w)
{
  int** result = new int*[w];
  for(int i(0); i<w; i++)
  {
    result[i]= new int[h];
    for(int j(0); j<h; j++)
    {
      result[i][j]=tab[j][w-1-i];
    }
  }
  int aux=h;
  h=w;
  w=aux;
  return result;
}
bool** Picture::rotationL(bool** tab, int& h, int& w)
{
  bool** result = new bool*[w];
  for(int i(0); i<w; i++)
  {
    result[i]= new bool[h];
    for(int j(0); j<h; j++)
    {
      result[i][j]=tab[j][w-1-i];
    }
  }
  int aux=h;
  h=w;
  w=aux;
  return result;
}
