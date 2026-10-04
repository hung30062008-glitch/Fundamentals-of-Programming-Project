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
	if (direction == _keyLeft) {
		moveleft();
	}
	else if (direction == _keyDown) {
		movedown();
	}
	else if (direction == _keyRight) {
		moveright();
	}
	else if (direction == _keyUp) {
		moveup();
	}
}

void PvP() {
	startgame();
	bool validEnter = true;
	while (1) {
		int command = toupper(_getch());
		if (command == 27) {
			int choice = pausescreen();
			if (choice == 1) {
				system("cls");
				drawboard(boardsize);
				redrawBoardMarks();
				gotoXY(_X, _Y);
			}
			else if (choice == 2) {
				exitConfirmScreen();
				return;
			}
		}
		else if (command == _keyEnter) {
			switch (checkboard(_X, _Y)) {
			case 1:
				setColor(12);
				cout << "X";
				gotoXY(_X, _Y);
				break;
			case -1:
				setColor(10);
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
	drawmainscreen();
	mainscreen();
	return 0;
}
