#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

using namespace std;

#define boardsize 12
#define left 36
#define top 3

extern bool _turn;
extern int _command;
extern int _X, _Y;

extern bool _sound;
extern int _volume;
extern bool _block2ends;
extern int _keyup, _keydown, _keyleft, _keyright, _keyenter;

struct _point {
    int x, y, c;
};

struct CellPos {
    int r, c;
};

extern _point _a[boardsize][boardsize];
extern vector<CellPos> winCells;

void fix_window();
void fix_screen();
void setcolor(int, int bg = 0);
void showcursor(bool);
void wineffect(int );
void settingscreen();
void playSound(int, int);
int askcontinue();
int finish(int);
void gotoXY(int, int);
void startgame();
void exitgame();
int checkboard(int, int);
void moveto(int);
int testboard();
void resetdata();
void drawboard(int);
void redrawchess();
void drawmainscreen();
void newgamescreen();
void mainscreen();
void loadgamescreen();
void PvP(bool = false);
void PvBscreen();
int pausescreen();
void exitconfirmscreen();
bool saveGameData(const string& filename);
bool loadGameData(const string& filename);
void redrawslots(int);