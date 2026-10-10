#include "config.h"

void fix_window() {
	HWND consolewindow = GetConsoleWindow();
	LONG_PTR style = GetWindowLongPtr(consolewindow, GWL_STYLE);
	style = style & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
	SetWindowLongPtr(consolewindow, GWL_STYLE, style);
}

void fix_screen() {
	HANDLE fixcreen = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size = { (SHORT)120, (SHORT)30 };
	SetConsoleScreenBufferSize(fixcreen, size);
	SMALL_RECT windowsize = { 0, 0, (SHORT)(size.X - 1), (SHORT)(size.Y - 1) };
	SetConsoleWindowInfo(fixcreen, TRUE, &windowsize);
}

void gotoXY(int x, int y) {
	COORD pos = { (SHORT)x,(SHORT)y };
	HANDLE currentscreen = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(currentscreen, pos);
}

void setcolor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

void showcursor(bool visible) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(handle, &cursorInfo);
}

void wineffect(int winner) {
	showcursor(false);
	for (int loop = 0; loop < 5; loop++) {
		setcolor(0, 14);
		for (auto cell : winCells) {
			gotoXY(_a[cell.r][cell.c].x, _a[cell.r][cell.c].y);
			cout << (winner == 1 ? "X" : "O");
		}
		Sleep(160);

		for (auto cell : winCells) {
			gotoXY(_a[cell.r][cell.c].x, _a[cell.r][cell.c].y);
			setcolor(winner == 1 ? 12 : 10, 0);
			cout << (winner == 1 ? "X" : "O");
		}
		Sleep(160);
	}
	showcursor(true);
}

void drawboard(int psize) {
	setcolor(7);
	for (int i = 0; i <= psize; i++) {
		gotoXY(left, top + 2 * i);
		for (int j = 0; j < psize; j++) {
			cout << "+---";
		}
		cout << "+";

		if (i < psize) {
			gotoXY(left, top + 2 * i + 1);
			for (int j = 0; j <= psize; j++) {
				cout << "|   ";
			}
		}
	}
	gotoXY(_X, _Y);
}

void redrawchess() {
	for (int i = 0; i < boardsize; i++) {
		for (int j = 0; j < boardsize; j++) {
			if (_a[i][j].c == 1) {
				gotoXY(_a[i][j].x, _a[i][j].y);
				setcolor(12);
				cout << "X";
			}
			else if (_a[i][j].c == -1) {
				gotoXY(_a[i][j].x, _a[i][j].y);
				setcolor(10);
				cout << "O";
			}
		}
	}
	setcolor(7);
}

void drawmainscreen() {
	system("cls");
	string title = "CARO GAME";
	string option[] = { "", "New Game", "Load Game", "Setting", "Credit", "Exit" };
	gotoXY(55, 10);
	cout << title;
	for (int i = 1; i <= 5; i++) {
		gotoXY(55, 10 + 2 * i);
		cout << option[i];
	}
}

void mainscreen() {
	showcursor(false);
	const int startX = 53;
	const int startY = 12;
	const int step = 2;

	int option = 1;
	int totaloption = 5;

	gotoXY(startX, startY);
	cout << ">";
	gotoXY(startX, startY);

	while (1) {
		int move = toupper(_getch());
		int preoption = option;

		if (move == 'W') {
			option = (option == 1) ? totaloption : option - 1;
		}
		else if (move == 'S') {
			option = (option == totaloption) ? 1 : option + 1;
		}
		else if (move == _keyenter) {
			switch (option) {
			case 1: 
				newgamescreen(); 
				break;
			case 2:
				loadgamescreen();
				break;
			case 3:
				settingscreen();
				break;
			case 4:
				break;
			case 5: exit(0); break;
			}
		}
		if (option != preoption) {
			gotoXY(startX, startY + (preoption - 1) * step);
			cout << " ";
			gotoXY(startX, startY + (option - 1) * step);
			cout << ">";
			gotoXY(startX, startY + (option - 1) * step);
		}
	}
}

void newgamescreen() {
	system("cls");
	const int startX = 50;
	const int startY = 13;
	const int step = 4;
	const int totaloption = 2;

	int option = 1;

	gotoXY(startX + 2, startY);
	cout << "Play With Friend";
	gotoXY(startX + 2, startY + step);
	cout << "Play With Bot";
	gotoXY(startX, startY);
	cout << ">";
	gotoXY(startX, startY);
	while (1) {
		int move = toupper(_getch());
		int preoption = option;

		if (move == 'W') {
			option = (option == 1) ? totaloption : option - 1;
		}
		else if (move == 'S') {
			option = (option == totaloption) ? 1 : option + 1;
		}
		else if (move == _keyenter) {
			if (option == 1) PvP();
			else PvBscreen();
		}
		else if (move == 27) {
			drawmainscreen();
			mainscreen();
		}
		if (option != preoption) {
			gotoXY(startX, startY + (preoption - 1) * step);
			cout << " ";
			gotoXY(startX, startY + (option - 1) * step);
			cout << ">";
			gotoXY(startX, startY + (option - 1) * step);
		}
	}
}

void drawloadbox() {
	const int boardX = 19;
	const int boardY = 4;
	const int boardwidth = 81;
	const int boardheight = 21;

	gotoXY(boardX, boardY);
	cout << "+================================================================================+";
	for (int i = 1;i < boardheight;i++) {
		gotoXY(boardX, boardY + i);
		cout << "|";
		gotoXY(boardX + boardwidth, boardY + i);
		cout << "|";
	}
	gotoXY(boardX, boardY + boardheight);
	cout << "+================================================================================+";
}

void loadgamescreen() {
	system("cls");
	showcursor(false);
	drawloadbox();

	const int totaloption = 3;
	int option = 0;

	redrawslots(0);

	while (1) {
		int move = toupper(_getch());
		int preoption = option;

		if (move == 'W') {
			option = (option == 0) ? totaloption - 1 : option - 1;
		}
		else if (move == 'S') {
			option = (option == totaloption - 1) ? 0 : option + 1;
		}
		else if (move == 27 || move == _keyenter) { 
			showcursor(true);
			drawmainscreen();
			mainscreen();
			return;
		}

		if (option != preoption) {
			redrawslots(option);
		}
	}
}

void drawslot (int slot, int startY, bool choosing) {
	const int startX = 22;
	if (choosing) {
		setcolor(14);
		gotoXY(startX, startY);
		cout << "+==========================================================================+";
		gotoXY(startX, startY + 1);
		cout << "| > Slot " << (slot + 1) << "                                                                 |";
		gotoXY(startX, startY + 2);
		cout << "|                                                                          |";
		gotoXY(startX, startY + 3);
		cout << "|                                                                          |";
		gotoXY(startX, startY + 4);
		cout << "+==========================================================================+";
	}
	else {
		gotoXY(startX, startY);
		cout << "+--------------------------------------------------------------------------+";
		gotoXY(startX, startY + 1);
		cout << "|   Slot " << (slot + 1) << "                                                                 |";
		gotoXY(startX, startY + 2);
		cout << "|                                                                          |";
		gotoXY(startX, startY + 3);
		cout << "|                                                                          |";
		gotoXY(startX, startY + 4);
		cout << "+--------------------------------------------------------------------------+";
	}
	setcolor(7);
}

void redrawslots(int chosing) {
	int startY[3] = { 6, 12, 18 };
	for (int i = 0; i < 3; i++) {
		drawslot(i, startY[i], (i==chosing));
	}
}

int pausescreen() {
	showcursor(false);
	const int boxX = 47;
	const int boxY = 11;

	setcolor(11);
	gotoXY(boxX, boxY);     cout << "+--------------------------+";
	gotoXY(boxX, boxY + 1); cout << "|        PAUSE GAME        |";
	gotoXY(boxX, boxY + 2); cout << "+--------------------------+";
	gotoXY(boxX, boxY + 3); cout << "|                          |";
	gotoXY(boxX, boxY + 4); cout << "|     Continue             |";
	gotoXY(boxX, boxY + 5); cout << "|                          |";
	gotoXY(boxX, boxY + 6); cout << "|     Exit                 |";
	gotoXY(boxX, boxY + 7); cout << "|                          |";
	gotoXY(boxX, boxY + 8); cout << "+--------------------------+";

	const int startX = boxX + 4;
	const int startY = boxY + 4;
	const int step = 2;
	const int totaloption = 2;

	int option = 1;
	gotoXY(startX, startY);
	setcolor(14);
	cout << ">";
	gotoXY(startX, startY);

	while (1) {
		int move = toupper(_getch());
		int preoption = option;
		if (move == 'S')
			option = (option == totaloption) ? 1 : option + 1;
		else if (move == 'W')
			option = (option == 1) ? totaloption : option - 1;
		else if (move == _keyenter) {
			setcolor(7);
			showcursor(true);
			return option;
		}

		if (option != preoption) {
			gotoXY(startX, startY + (preoption - 1) * step);
			cout << " ";
			gotoXY(startX, startY + (option - 1) * step);
			setcolor(14);
			cout << ">";
			gotoXY(startX, startY + (option - 1) * step);
		}
	}
}

void exitconfirmscreen() {
	showcursor(false);
	const int boxX = 47;
	const int boxY = 11;

	setcolor(11);
	gotoXY(boxX, boxY);     cout << "+--------------------------+";
	gotoXY(boxX, boxY + 1); cout << "|     BAN CO MUON LUU?     |";
	gotoXY(boxX, boxY + 2); cout << "+--------------------------+";
	gotoXY(boxX, boxY + 3); cout << "|                          |";
	gotoXY(boxX, boxY + 4); cout << "|     Save Game            |";
	gotoXY(boxX, boxY + 5); cout << "|                          |";
	gotoXY(boxX, boxY + 6); cout << "|     Don't Save           |";
	gotoXY(boxX, boxY + 7); cout << "|                          |";
	gotoXY(boxX, boxY + 8); cout << "+--------------------------+";

	const int startX = boxX + 4;
	const int startY = boxY + 4;
	const int step = 2;
	const int totaloption = 2;

	int option = 1;
	gotoXY(startX, startY);
	setcolor(14);
	cout << ">";
	gotoXY(startX, startY);

	while (1) {
		int move = toupper(_getch());
		int preoption = option;
		if (move == 'S')
			option = (option == totaloption) ? 1 : option + 1;
		else if (move == 'W')
			option = (option == 1) ? totaloption : option - 1;
		else if (move == _keyenter) {
			if (option == 1) { 
				gotoXY(boxX, boxY + 3); cout << "|  Nhap ten file de luu:   |";
				gotoXY(boxX, boxY + 4); cout << "|  [                      ]|";
				gotoXY(boxX, boxY + 5); cout << "|                          |";
				gotoXY(boxX, boxY + 6); cout << "|                          |";
				gotoXY(boxX + 4, boxY + 4);
				setcolor(14);
				showcursor(true);
				string filename;
				cin >> filename;

				if (saveGameData(filename)) {
					gotoXY(boxX, boxY + 6);
					setcolor(10);
					cout << "|  Luu file thanh cong!    |";
				} else {
					gotoXY(boxX, boxY + 6);
					setcolor(12);
					cout << "|  Loi khi luu file!       |";
				}
				Sleep(900);

				setcolor(7);
				showcursor(true);
				drawmainscreen();
				mainscreen();
				return;
			}
			else if (option == 2) {
				setcolor(7);
				showcursor(true);
				drawmainscreen();
				mainscreen();
				return;
			}
		}

		if (option != preoption) {
			gotoXY(startX, startY + (preoption - 1) * step);
			cout << " ";
			gotoXY(startX, startY + (option - 1) * step);
			setcolor(14);
			cout << ">";
			gotoXY(startX, startY + (option - 1) * step);
		}
	}
}

void PvBscreen() {
	system("cls");
	const int startX = 56;
	const int startY = 11;
	const int step = 4;
	const int totaloption = 3;

	int option = 1;
	string level[] = { "Easy", "Medium", "Hard" };

	for (int i = 0; i < 3; i++) {
		gotoXY(startX + 2, startY + step * i);
		cout << level[i];
	}

	gotoXY(startX, startY);
	cout << ">";
	gotoXY(startX, startY);

	while (1) {
		int move = toupper(_getch());
		int preoption = option;

		if (move == 'W') {
			option = (option == 1) ? totaloption : option - 1;
		}
		else if (move == 'S') {
			option = (option == totaloption) ? 1 : option + 1;
		}
		else if (move == _keyenter) {
			if (option == 1) PvP();
			else PvBscreen();
		}
		else if (move == 27)
			newgamescreen();
		if (option != preoption) {
			gotoXY(startX, startY + (preoption - 1) * step);
			cout << " ";
			gotoXY(startX, startY + (option - 1) * step);
			cout << ">";
			gotoXY(startX, startY + (option - 1) * step);
		}
	}
}

void drawsetting(int option, int startX, int startY) {
	gotoXY(startX, startY);
	switch (option) {
	case 1:
		cout << "Sound:        < " << (_sound ? "ON " : "OFF") << " >      ";
		break;
	case 2:
		cout << "Volume:       < " << (_volume < 100 ? " " : "") << _volume << "% >     ";
		break;
	case 3:
		cout << "Block 2 Ends: < " << (_block2ends ? "ON " : "OFF") << " >      ";
		break;
	case 4:
		cout << "Move Up:      [ " << (char)_keyup << " ]         ";
		break;
	case 5:
		cout << "Move Down:    [ " << (char)_keydown << " ]         ";
		break;
	case 6:
		cout << "Move Left:    [ " << (char)_keyleft << " ]         ";
		break;
	case 7:
		cout << "Move Right:   [ " << (char)_keyright << " ]         ";
		break;
	}
	gotoXY(startX - 2, startY);
}

void settingscreen() {
	system("cls");
	showcursor(false);

	const int startX = 46;
	const int startY = 9;
	const int step = 2;
	const int totaloption = 7;

	gotoXY(55, 7);
	cout << "SETTINGS";

	for (int i = 0; i < totaloption; i++) {
		drawsetting(i+1, startX + 2, startY + i * step);
	}

	int option = 1;
	gotoXY(startX, startY);
	cout << ">";
	gotoXY(startX, startY);

	while (1) {
		int move = toupper(_getch());
		int preoption = option;

		if (move == 'W') {
			option = (option == 1) ? totaloption : option - 1;
		}
		else if (move == 'S') {
			option = (option == totaloption) ? 1 : option + 1;
		}
		else if (move == _keyenter && option >= 1 && option <= 3) {
			while (1) {
				int select = toupper(_getch());
				if (select == 'A' || select == 'D') {
					if (option == 1) {
						_sound = !_sound;
						drawsetting(1, startX + 2, startY);
					}
					else if (option == 2) {
						if (select == 'A' && _volume > 0) _volume -= 10;
						if (select == 'D' && _volume < 100) _volume += 10;
						drawsetting(2, startX + 2, startY + step);
					}
					else if (option == 3) {
						_block2ends = !_block2ends;
						drawsetting(3, startX + 2, startY + 2 * step);
					}
				}
				if (select == 27 || select == _keyenter)
					break;
			}
		}
		if (move == _keyenter && option >= 4 && option <= 7) {
			gotoXY(startX + 18, startY + 2 * (option - 1));
			cout << " ";
			gotoXY(startX + 18, startY + 2 * (option - 1));
			showcursor(true);

			int newKey = toupper(_getch());
			if (option == 4) _keyup = newKey; 
			else if (option == 5) _keydown = newKey;
			else if (option == 6) _keyleft = newKey;
			else if (option == 7) _keyright = newKey;
			drawsetting(option, startX + 2, startY + (option - 1) * step);
			showcursor(false);
		}
		else if (move == 27) {
			drawmainscreen();
			mainscreen();
			return;
		}

		if (option != preoption) {
			gotoXY(startX, startY + (preoption - 1) * step);
			cout << " ";
			gotoXY(startX, startY + (option - 1) * step);
			cout << ">";
			gotoXY(startX, startY + (option - 1) * step);
		}
	}
}

int finish(int winner) {
	if (winner == 2) {
		_turn = !_turn;
		return winner;
	}
	gotoXY(0, _a[boardsize - 1][boardsize - 1].y + 2);
	switch (winner) {
	case 1:
		setcolor(12);
		cout << "Nguoi choi 1 da thang va nguoi choi 2 da thua";
		wineffect(winner);
		break;
	case -1:
		setcolor(10);
		cout << "Nguoi choi 2 da thang va nguoi choi 1 da thua";
		wineffect(winner);
		break;
	case 0:
		setcolor(14);
		cout << "Nguoi choi  2 da hoa nguoi choi 1";
		break;
	}
	setcolor(7);
	return winner;
}

int askcontinue() {
	gotoXY(0, _a[boardsize - 1][boardsize - 1].y + 3);
	cout << "Nhan Y/N de tiep tuc/dung: ";
	return toupper(_getch());
}
