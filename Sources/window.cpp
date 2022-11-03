#include "../Headers/window.h"
using namespace std;

void init_colors(void){
  start_color();

  init_pair(TEMPCOLOR,COLOR_BLACK, COLOR_BLACK);
  init_pair(WBLACK,   COLOR_WHITE, COLOR_YELLOW);
  init_pair(WCYAN,    COLOR_WHITE, COLOR_CYAN);
  init_pair(WBLUE,    COLOR_WHITE, COLOR_BLUE);
  init_pair(WYELLOW,  COLOR_WHITE, COLOR_YELLOW);
  init_pair(WGREEN,   COLOR_WHITE, COLOR_GREEN);
  init_pair(WMAGENTA, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(WRED,     COLOR_WHITE, COLOR_RED);
  init_pair(BWHITE,   COLOR_BLACK, COLOR_WHITE);
  init_pair(BCYAN,    COLOR_BLACK, COLOR_CYAN);
  init_pair(BBLUE,    COLOR_BLACK, COLOR_BLUE);
  init_pair(BYELLOW,  COLOR_BLACK, COLOR_YELLOW);
  init_pair(BGREEN,   COLOR_BLACK, COLOR_GREEN);
  init_pair(BMAGENTA, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(BRED,     COLOR_BLACK, COLOR_RED);
}

int Window::newColorINIT = 0;
int Window::newPairINIT = 9;

int Window::COLOR(int r, int g, int b, int rb, int gb, int bb){
  r = r * (1000 / 255);
  g = g * (1000 / 255);
  b = b * (1000 / 255);

  rb = rb * (1000 / 255);
  gb = gb * (1000 / 255);
  bb = bb * (1000 / 255);

  init_color(newColorINIT, r,g,b);
  init_color(newColorINIT + 1, rb,gb,bb);
  init_pair(newPairINIT, newColorINIT, newColorINIT+1);

  newColorINIT+=2;
  newPairINIT++;

  return newPairINIT;
}

void startProgramX() {
  initscr();             // initialize curses
  cbreak();              // pass key presses to program, but not signals
  noecho();              // don't echo key presses to screen
  keypad(stdscr, TRUE);  // allow arrow keys
  timeout(0);            // no blocking on getch()
  curs_set(0);           // set the cursor to invisible
  init_colors();
}

void stopProgramX() {
  refresh();
  getch();
  endwin();
}

void Window::update() const{
  wrefresh(win);
  wrefresh(frame);
  refresh();
}

Window::Window(int h,int w, int x, int y, bool border, char c)
  : height(h), width(w), startx(x), starty(y), bord(c)
{
  colorwin=WCYAN;
  colorwinINT = 0;
  colorframe=WBLACK;
  if(border){
    frame=newwin(h+2,w+4,y,x);
    win=subwin(frame,h,w,y+1,x+2);
    wbkgd(frame,COLOR_PAIR(colorwin));
    wbkgd(win,COLOR_PAIR(colorframe));
    wborder(frame, c,c,c,c,c,c,c,c);
    wattron(win,COLOR_PAIR(colorwin));
    wattron(frame,COLOR_PAIR(colorframe));
  }else{
    frame=newwin(h,w,y,x);
    win=subwin(frame,h,w,y,x);
  }

  pixels = new int*[height];
  for (int i = 0; i < height; i++) {
    pixels[i] = new int[width/2];
    for (int j = 0; j < width/2; j++) {
      pixels[i][j] = 0;
    }
  }
  pixelsBEFORE = new int*[height];
  for (int i = 0; i < height; i++) {
    pixelsBEFORE[i] = new int[width/2];
    for (int j = 0; j < width/2; j++) {
      pixelsBEFORE[i][j] = 0;
    }
  }

  update();
}

Window::Window(int h,int w, int x, int y,WINDOW *winParent)
  : height(h), width(w), startx(x), starty(y),parent(winParent)
{
  frame=subwin(parent,h,w,y,x);
  win=subwin(frame,h,w,y,x);
  colorwinINT = 0;

  pixels = new int*[height];
  for (int i = 0; i < height; i++) {
    pixels[i] = new int[width/2];
    for (int j = 0; j < width/2; j++) {
      pixels[i][j] = 0;
    }
  }
  pixelsBEFORE = new int*[height];
  for (int i = 0; i < height; i++) {
    pixelsBEFORE[i] = new int[width/2];
    for (int j = 0; j < width/2; j++) {
      pixelsBEFORE[i][j] = 0;
    }
  }
  update();
}

Window::Window()
  : height(20), width(20), startx(0), starty(0),parent(NULL)
{
  frame=subwin(parent,height,width,starty,startx);
  win=subwin(frame,height,width,starty,startx);
  colorwinINT = 0;

  pixels = new int*[height];
  for (int i = 0; i < height; i++) {
    pixels[i] = new int[width/2];
    for (int j = 0; j < width/2; j++) {
      pixels[i][j] = 0;
    }
  }
  pixelsBEFORE = new int*[height];
  for (int i = 0; i < height; i++) {
    pixelsBEFORE[i] = new int[width/2];
    for (int j = 0; j < width/2; j++) {
      pixelsBEFORE[i][j] = 0;
    }
  }
  update();
}

Window::~Window(){
  //Delete chaques parties des matrices pixels
  for(int i = 0; i < height; ++i) {
      delete[] pixelsBEFORE[i];
      delete[] pixels[i];
  }
  //Delete tableaux
  delete[] pixelsBEFORE;
  delete[] pixels;

  wborder(frame, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wattroff(win,COLOR_PAIR(colorwin));
  wattroff(win,COLOR_PAIR(colorframe));
  werase(win);
  update();
  delwin(win);
}

void Window::print(int x, int y, std::string s, Color c) const {
  wattron(win,COLOR_PAIR(c));
  mvwprintw(win,y,x,s.c_str());
  wattroff(win,COLOR_PAIR(c));
  update();
}
void Window::print(int x, int y, std::string s, int mask, bool isColor, bool updateTHIS) const{
  if(!isColor){
    wattron(win,mask);
  }else{
  wattron(win,COLOR_PAIR(mask));
  }

  mvwprintw(win,y,x,s.c_str());

  if(!isColor){
    wattroff(win,mask);
  }else{
    wattroff(win,COLOR_PAIR(mask));
  }
  if(updateTHIS)
    update();
}

void Window::print(int x, int y, char s, int mask, bool isColor, bool updateTHIS) const{
  if(!isColor){
    wattron(win,mask);
  }else{
  wattron(win,COLOR_PAIR(mask));
  }

  mvwaddch(win,y,x,s);

  if(!isColor){
    wattroff(win,mask);
  }else{
    wattroff(win,COLOR_PAIR(mask));
  }
  if(updateTHIS)
    update();
}
void Window::print(int x, int y, char s, Color c) const{
  wattron(win,COLOR_PAIR(c));
  mvwaddch(win,y,x,s);
  wattroff(win,COLOR_PAIR(c));
  update();
}
void Window::print(int x, int y, std::string s) const{
  mvwprintw(win,y,x,s.c_str());
  update();
}
void Window::print(int x, int y, char s) const{
  mvwaddch(win,y,x,s);
  update();
}

void Window::setPixels(int** table,int heightTab, int widthTab, int x, int y){
  //int c = 0;
  for (int i = x; i < heightTab + x; i++) {
    for (int j = y; j < widthTab + y; j++) {
      if(table[i -x][j-y] != 0 && i < this->height && j < this->width/2 && i >= 0 && j>=0){
        pixels[i][j] = table[i -x][j-y];
        //print(j*2 + x,i + y,"  ", table[i][j], true);

        // DEBUG ******************************
        // stringstream ss;
        // ss << table[i][j];
        // string str = ss.str() + " ";
        //
        // print(j*2 + x,i + y,str, table[i][j], true);
        // usleep(10000);
      }
    }
  }
}

void Window::fillPixels(int color, int heightTab, int widthTab, int x, int y){
  int** fill = new int*[heightTab];
  for (int i = 0; i < heightTab; i++) {
    fill[i] = new int[widthTab];
    for (int j = 0; j < widthTab; j++) {
      fill[i][j] = color;
    }
  }
  setPixels(fill, heightTab, widthTab,x,y);

  //On delete le tableau
  for (int i = 0; i < heightTab; i++) {
    delete[] fill[i];
  }
  delete[] fill;
}

void Window::showPixels() const{
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width/2; j++) {
      if(pixels[i][j] != 0 && pixelsBEFORE[i][j] != pixels[i][j])
        print(j*2 ,i,"  ", pixels[i][j], true, false);
      pixelsBEFORE[i][j] = pixels[i][j];
    }
  }
  update();
}

void Window::showPixelsHARD() const{
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width/2; j++) {
      if(pixels[i][j] != 0){
        print(j*2 ,i,"  ", pixels[i][j], true, false);
      }else{
        print(j*2 ,i,"  ", colorwinINT, true, false);
      }
      pixelsBEFORE[i][j] = pixels[i][j];
    }
  }
  update();
}

int Window::getX() const { return startx;}
int Window::getY() const { return starty;}
int Window::getHauteur() const { return height;}
int Window::getLargeur() const { return width;}
WINDOW* Window::getWindow() const {return win;}
Color Window::getCouleurBordure() const{ return colorframe;}
Color Window::getCouleurFenetre() const{ return colorwin;}
void Window::setCouleurBordure(Color c){
  colorframe=c;
  wattron(frame,COLOR_PAIR(colorframe));
  wborder(frame, bord,bord,bord,bord,bord,bord,bord,bord);

  update();
}
void Window::setCouleurFenetre(Color c){
  colorwin=c;
  wattron(win,COLOR_PAIR(colorwin));
  wbkgd(win,COLOR_PAIR(colorwin));
  update();
}

void Window::setCouleurFenetre(int mask){
  colorwinINT = mask;
  wattron(win,COLOR_PAIR(mask));
  wbkgd(win,COLOR_PAIR(mask));
  update();
}

void Window::clear() const{  werase(win); update(); }
