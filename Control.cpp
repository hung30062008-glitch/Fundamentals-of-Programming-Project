#include "config.h"

void startgame() {
	system("cls");
	resetdata();
	drawboard(boardsize);
}

void exitgame() {
	system("cls");
}

void moveright() {
	if (_X < _a[boardsize - 1][boardsize - 1].x) {
		_X += 4;
		gotoXY(_X, _Y);
	}
}

void moveleft() {
	if (_X > _a[0][0].x) {
		_X -= 4;
		gotoXY(_X, _Y);
	}
}

void moveup() {
	if (_Y > _a[0][0].y) {
		_Y -= 2;
		gotoXY(_X, _Y);
	}
}

void movedown() {
	if (_Y < _a[boardsize - 1][boardsize - 1].y) {
		_Y += 2;
		gotoXY(_X, _Y);
	}
}

void moveto(int direction) {
	if (direction == _keyleft) {
		moveleft();
	}
	else if (direction == _keydown) {
		movedown();
	}
	else if (direction == _keyright) {
		moveright();
	}
	else if (direction == _keyup) {
		moveup();
	}
}

void PvP(bool resume) {
	if (!resume) {
		startgame();
	}
	else {
		system("cls");
		drawboard(boardsize);
		redrawchess();
		gotoXY(_X, _Y);
	}
	bool validEnter = true;
	while (1) {
		int command = toupper(_getch());
		if (command == 27) {
			int choice = pausescreen();
			if (choice == 1) {
				system("cls");
				drawboard(boardsize);
				redrawchess();
				gotoXY(_X, _Y);
			}
			else if (choice == 2) {
				exitconfirmscreen();
				return;
			}
		}
		else if (command == _keyenter) {
			switch (checkboard(_X, _Y)) {
			case 1:
				setcolor(12);
				cout << "X";
				gotoXY(_X, _Y);
				break;
			case -1:
				setcolor(10);
				cout << "O";
				gotoXY(_X, _Y);
				break;
			case 0:
				validEnter = 0;
				gotoXY(_X, _Y);
				break;
			};
			if (validEnter) {
				if (finish(testboard()) != 2) {
					if (askcontinue() != 'Y') {
						exitgame();
						return;
					}
					else
						startgame();
				}
			}
		}
		else
			moveto(command);
		validEnter = true;
	}
}
int main() {
	fix_window();
	fix_screen();
	drawmainscreen();
	mainscreen();
	return 0;
}
