#include "../Headers/Message.h"

using namespace std;

Window* Message::popup = NULL;

//message d'alerte avec par ex le texte centrée même pour les pseudos entrés par l'user
void Message::Alert(string message, string image, int h, int w, int x, int y, int yNew, int textColor){

  int bnocMessageHeight, bnocMessageWidth, xString = 0, cmpt=0, lineLength=0;
  size_t j=0;

  Message::popup = new Window(h, w, x, y);
  int ** bnocMessage = Picture::getPicture(image, &bnocMessageHeight, &bnocMessageWidth);
  Message::popup->setPixels(bnocMessage, bnocMessageHeight, bnocMessageWidth);
  Message::popup->showPixels();

  for(size_t i = 0; i < message.size(); i++) {
    // centre le texte
    if(xString==0){
      j = i;
      while((message.at(j)!='/' || message.at(j+1)!='n') && j < message.size()-2){
        if(j == message.size()-3){
          lineLength++;
        }
        lineLength++;
        j++;
    }
  }
    // passage a la ligne sans mordre une bordure
    if(message.at(i)=='/' && message.at(i+1)=='n') {
      cmpt++;
      xString = 0;
      lineLength = 0;
      i++;
    }
    else { //print 1 a 1 les caractères
      Message::popup->print(((w-lineLength)/2)+xString, yNew+cmpt, message[i], COLOR_PAIR(textColor) | A_BOLD);
      xString++;

    }
  }
}

void Message::AlertAnims(string message, string image, string image2, int h, int w, int x, int y, int yNew, int textColor){
// l'alerte est passé par la touche entrée et passe a la suite aprés
  int c=0, d=0,t=0;

  while(c != '\n' && t != 5){ // switch entre les 2 images pour créer une animation.
    if(d%2 == 0){
    Message::Alert(message, image, h, w, x, y, yNew, textColor);
    usleep(150000);

    }
    else if(d%2 == 1){
    Message::Alert(message, image2, h, w, x, y, yNew, textColor);
    usleep(150000);
    if (image2 != "Victory2.txt"){
      t++; // incrémentation pour passage automatique si on appuie pas sur entrée
      }
    }
    c = getch();
    d++;
  }
}
