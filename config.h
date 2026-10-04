#pragma once

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
extern int _keyUp, _keyDown, _keyLeft, _keyRight, _keyEnter;

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
void setColor(int text, int bg = 0);
void showCursor(bool visible);
void winEffect(int winner);
void settingscreen();
void playSound(int freq, int duration);
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
void redrawBoardMarks();
void drawmainscreen();
void newgamescreen();
void mainscreen();
void PvP();
void PvBscreen();
int pausescreen();
void exitConfirmScreen();
bool saveGameData(const string& filename);
bool loadGameData(const string& filename);