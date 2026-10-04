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
	SMALL_RECT windowsize = { 0, 0, size.X - 1, size.Y - 1 };
	SetConsoleScreenBufferSize(fixcreen, size);
	SetConsoleWindowInfo(fixcreen, TRUE, &windowsize);
}

void gotoXY(int x, int y) {
	COORD pos = { (SHORT)x,(SHORT)y };
	HANDLE currentscreen = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(currentscreen, pos);
}

void setColor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

void showCursor(bool visible) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(handle, &cursorInfo);
}

void winEffect(int winner) {
	showCursor(false);
	for (int loop = 0; loop < 5; loop++) {
		setColor(0, 14);
		for (auto cell : winCells) {
			gotoXY(_a[cell.r][cell.c].x, _a[cell.r][cell.c].y);
			cout << (winner == 1 ? "X" : "O");
		}
		Sleep(160);

		for (auto cell : winCells) {
			gotoXY(_a[cell.r][cell.c].x, _a[cell.r][cell.c].y);
			setColor(winner == 1 ? 12 : 10, 0);
			cout << (winner == 1 ? "X" : "O");
		}
		Sleep(160);
	}
	showCursor(true);
}

void drawboard(int psize) {
	setColor(7);
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

void redrawBoardMarks() {
	for (int i = 0; i < boardsize; i++) {
		for (int j = 0; j < boardsize; j++) {
			if (_a[i][j].c == 1) {
				gotoXY(_a[i][j].x, _a[i][j].y);
				setColor(12);
				cout << "X";
			}
			else if (_a[i][j].c == -1) {
				gotoXY(_a[i][j].x, _a[i][j].y);
				setColor(10);
				cout << "O";
			}
		}
	}
	setColor(7);
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
		else if (move == 13) {
			switch (option) {
			case 1: newgamescreen(); break;
			case 2:
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
		else if (move == 13) {
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

int pausescreen() {
	showCursor(false);
	const int boxX = 47;
	const int boxY = 11;

	setColor(11);
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
	setColor(14);
	cout << ">";
	gotoXY(startX, startY);

	while (1) {
		int move = toupper(_getch());
		int preoption = option;
		if (move == 'S')
			option = (option == totaloption) ? 1 : option + 1;
		else if (move == 'W')
			option = (option == 1) ? totaloption : option - 1;
		else if (move == 13) {
			setColor(7);
			showCursor(true);
			return option;
		}

		if (option != preoption) {
			gotoXY(startX, startY + (preoption - 1) * step);
			cout << " ";
			gotoXY(startX, startY + (option - 1) * step);
			setColor(14);
			cout << ">";
			gotoXY(startX, startY + (option - 1) * step);
		}
	}
}

void exitConfirmScreen() {
	showCursor(false);
	const int boxX = 47;
	const int boxY = 11;

	setColor(11);
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
	setColor(14);
	cout << ">";
	gotoXY(startX, startY);

	while (1) {
		int move = toupper(_getch());
		int preoption = option;
		if (move == 'S')
			option = (option == totaloption) ? 1 : option + 1;
		else if (move == 'W')
			option = (option == 1) ? totaloption : option - 1;
		else if (move == 13) {
			if (option == 1) { 
				gotoXY(boxX, boxY + 3); cout << "|  Nhap ten file de luu:   |";
				gotoXY(boxX, boxY + 4); cout << "|  [                      ]|";
				gotoXY(boxX, boxY + 5); cout << "|                          |";
				gotoXY(boxX, boxY + 6); cout << "|                          |";
				gotoXY(boxX + 4, boxY + 4);
				setColor(14);
				showCursor(true);
				string filename;
				cin >> filename;

				if (saveGameData(filename)) {
					gotoXY(boxX, boxY + 6);
					setColor(10);
					cout << "|  Luu file thanh cong!    |";
				} else {
					gotoXY(boxX, boxY + 6);
					setColor(12);
					cout << "|  Loi khi luu file!       |";
				}
				Sleep(900);

				setColor(7);
				showCursor(true);
				drawmainscreen();
				mainscreen();
				return;
			}
			else if (option == 2) {
				setColor(7);
				showCursor(true);
				drawmainscreen();
				mainscreen();
				return;
			}
		}

		if (option != preoption) {
			gotoXY(startX, startY + (preoption - 1) * step);
			cout << " ";
			gotoXY(startX, startY + (option - 1) * step);
			setColor(14);
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
		else if (move == 13) {
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

void drawsetting(int option, int startX, int y) {
	gotoXY(startX, y);
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
		cout << "Move Up:      [ " << (char)_keyUp << " ]         ";
		break;
	case 5:
		cout << "Move Down:    [ " << (char)_keyDown << " ]         ";
		break;
	case 6:
		cout << "Move Left:    [ " << (char)_keyLeft << " ]         ";
		break;
	case 7:
		cout << "Move Right:   [ " << (char)_keyRight << " ]         ";
		break;
	}
	gotoXY(startX - 2, y);
}

void settingscreen() {
	system("cls");

	const int startX = 46;
	const int startY = 9;
	const int step = 2;
	const int totaloption = 7;

	gotoXY(55, 7);
	cout << "SETTINGS";

	for (int i = 1; i <= totaloption; i++) {
		drawsetting(i, startX + 2, startY + (i - 1) * step);
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
		else if (move == 13 && option >= 1 && option <= 3) {
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
				if (select == 27 || select == 13)
					break;
			}
		}
		if (move == 13 && option >= 4 && option <= 7) {

			int newKey = toupper(_getch());
			if (option == 4) _keyUp = newKey;
			else if (option == 5) _keyDown = newKey;
			else if (option == 6) _keyLeft = newKey;
			else if (option == 7) _keyRight = newKey;
			drawsetting(option, startX + 2, startY + (option - 1) * step);
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
	if (winner == 1 || winner == -1) {
		winEffect(winner);
	}
	gotoXY(0, _a[boardsize - 1][boardsize - 1].y + 2);
	switch (winner) {
	case 1:
		setColor(12);
		cout << "Nguoi choi 1 da thang va nguoi choi 2 da thua";
		break;
	case -1:
		setColor(10);
		cout << "Nguoi choi 2 da thang va nguoi choi 1 da thua";
		break;
	case 0:
		setColor(14);
		cout << "Nguoi choi  2 da hoa nguoi choi 1";
		break;
	}
	setColor(7);
	return winner;
}

int askcontinue() {
	gotoXY(0, _a[boardsize - 1][boardsize - 1].y + 3);
	cout << "Nhan Y/N de tiep tuc/dung: ";
	return toupper(_getch());
}
