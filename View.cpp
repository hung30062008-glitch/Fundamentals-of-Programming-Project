#include "config.h"

/**
 * @brief Cố định kích thước cửa sổ Console.
 * - Sử dụng Windows API GetConsoleWindow() và GetWindowLongPtr/SetWindowLongPtr.
 * - Loại bỏ các thuộc tính WS_MAXIMIZEBOX (nút phóng to cực đại) và WS_THICKFRAME (viền kéo giãn thay đổi kích thước).
 * - Mục đích: Giữ nguyên bố cục giao diện game không bị vỡ do người dùng vô tình kéo giãn cửa sổ.
 */
void fix_window() {
	HWND consolewindow = GetConsoleWindow();
	LONG_PTR style = GetWindowLongPtr(consolewindow, GWL_STYLE);
	style = style & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
	SetWindowLongPtr(consolewindow, GWL_STYLE, style);
}

/**
 * @brief Cố định độ phân giải vùng đệm màn hình và cửa sổ hiển thị Console.
 * - Đặt kích thước Buffer của Console thành 120 cột và 30 dòng (SetConsoleScreenBufferSize).
 * - Cố định kích thước khung nhìn cửa sổ khớp chính xác với Buffer (SetConsoleWindowInfo).
 * - Đảm bảo toàn bộ bàn cờ và các bảng điều khiển hiển thị vừa vặn, không xuất hiện thanh cuộn (scroll bar).
 */
void fix_screen() {
	HANDLE fixcreen = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size = { (SHORT)120, (SHORT)30 };
	SetConsoleScreenBufferSize(fixcreen, size);
	SMALL_RECT windowsize = { 0, 0, (SHORT)(size.X - 1), (SHORT)(size.Y - 1) };
	SetConsoleWindowInfo(fixcreen, TRUE, &windowsize);
}

/**
 * @brief Di chuyển con trỏ hiển thị Console đến tọa độ xác định (x, y).
 * - Sử dụng Windows API SetConsoleCursorPosition trên luồng STD_OUTPUT_HANDLE.
 * 
 * @param x Tọa độ cột (0-indexed, từ trái sang phải)
 * @param y Tọa độ dòng (0-indexed, từ trên xuống dưới)
 */
void gotoXY(int x, int y) {
	COORD pos = { (SHORT)x,(SHORT)y };
	HANDLE currentscreen = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(currentscreen, pos);
}

/**
 * @brief Thiết lập màu chữ (foreground) và màu nền (background) cho Console.
 * - Dùng Windows API SetConsoleTextAttribute kết hợp bit: (bg << 4) | text.
 * 
 * @param text Mã màu chữ (0 - 15: 7 = trắng xám, 10 = xanh lá, 12 = đỏ, 14 = vàng, v.v.)
 * @param bg Mã màu nền (mặc định 0 = đen)
 */
void setcolor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

/**
 * @brief Ẩn hoặc hiện con trỏ nhấp nháy của Console.
 * - Sử dụng Windows API SetConsoleCursorInfo để điều khiển thuộc tính bVisible.
 * 
 * @param visible true: hiển thị con trỏ; false: ẩn con trỏ (giúp giao diện menu mượt mà hơn)
 */
void showcursor(bool visible) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(handle, &cursorInfo);
}

/**
 * @brief Tạo hiệu ứng nhấp nháy nổi bật 5 quân cờ chiến thắng khi ván cờ kết thúc.
 * - Tạm ẩn con trỏ console.
 * - Lặp lại 5 lần chu kỳ đổi màu:
 *   + Bước 1: Đổi 5 ô thắng sang màu vàng nền sáng (chữ đen, nền vàng mã 14), dừng 160ms.
 *   + Bước 2: Đổi lại màu nguyên bản (X: đỏ mã 12, O: xanh mã 10), dừng 160ms.
 * - Hiện lại con trỏ console sau khi hoàn tất hiệu ứng.
 * 
 * @param winner Người chiến thắng (1: X, -1: O)
 */
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

/**
 * @brief Vẽ khung lưới bàn cờ kích thước psize x psize lên màn hình Console.
 * - Tọa độ bắt đầu vẽ tại (left, top).
 * - Sử dụng các ký tự ASCII "+---" và "|   " để tạo ô cờ rộng 4 ký tự, cao 2 dòng.
 * - Đưa con trỏ console về vị trí con trỏ bàn cờ hiện tại (_X, _Y).
 * 
 * @param psize Số ô của một cạnh bàn cờ (mặc định boardsize = 12)
 */
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

/**
 * @brief Vẽ lại toàn bộ quân cờ X và O trên bàn cờ.
 * - Thường dùng khi nạp lại game đã lưu (load game) hoặc quay lại từ màn hình Pause.
 * - Duyệt qua ma trận _a[boardsize][boardsize]:
 *   + Ô có giá trị 1: vẽ quân "X" màu đỏ (mã màu 12).
 *   + Ô có giá trị -1: vẽ quân "O" màu xanh (mã màu 10).
 * - Khôi phục màu console về mặc định (trắng 7).
 */
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

/**
 * @brief Vẽ giao diện tĩnh của Menu chính (Main Menu).
 * - Xóa màn hình console.
 * - In tiêu đề trò chơi "CARO GAME" ở giữa màn hình.
 * - In danh sách 5 tùy chọn: New Game, Load Game, Setting, Credit, Exit.
 */
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

/**
 * @brief Vòng lặp điều khiển tương tác trên Menu chính.
 * - Ẩn con trỏ console để menu nhìn sạch sẽ.
 * - Hiển thị ký hiệu con trỏ lựa chọn ">" tại mục đang được chọn.
 * - Xử lý phím:
 *   + 'W': Di chuyển lên mục trước.
 *   + 'S': Di chuyển xuống mục kế tiếp.
 *   + Enter: Kích hoạt chức năng tương ứng:
 *     1. newgamescreen(): Vào màn hình chọn ván mới.
 *     2. loadgamescreen(): Vào màn hình nạp ván cờ đã lưu.
 *     3. settingscreen(): Vào màn hình cấu hình cài đặt.
 *     4. Credit: (tạm thời chưa xử lý).
 *     5. Exit: Thoát chương trình (exit(0)).
 */
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

/**
 * @brief Màn hình lựa chọn chế độ chơi mới (New Game).
 * - Cung cấp 2 lựa chọn:
 *   1. "Play With Friend": Chế độ 2 người chơi đấu với nhau (PvP).
 *   2. "Play With Bot": Chế độ người chơi đấu với máy (PvB).
 * - Xử lý điều khiển bằng phím W/S để di chuyển, Enter để xác nhận chọn, ESC để quay lại Menu chính.
 */
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

/**
 * @brief Vẽ khung viền hộp tải game (Load Game Box) kích thước 81x21.
 * - Tọa độ góc trên bên trái tại (boardX = 19, boardY = 4).
 * - Dùng ký tự viền đôi "+===" và "|" tạo khung chữ nhật lớn bao quanh danh sách các slot game.
 */
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

/**
 * @brief Vòng lặp điều khiển giao diện nạp ván cờ đã lưu (Load Game).
 * - Xóa màn hình, ẩn con trỏ và vẽ khung viền (drawloadbox).
 * - Hiển thị 3 ô lưu trữ (Slot 1, 2, 3) thông qua redrawslots.
 * - Điều hướng bằng phím 'W'/'S' để chọn giữa 3 slot.
 * - Nhấn ESC hoặc Enter để thoát quay về Menu chính.
 */
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

/**
 * @brief Vẽ khung hiển thị cho một Slot lưu trữ ván cờ.
 * - Tọa độ bắt đầu tại (startX = 22, startY).
 * - Nếu choosing == true: Vẽ viền đôi màu vàng nổi bật kèm dấu con trỏ "> Slot (slot + 1)".
 * - Nếu choosing == false: Vẽ viền đơn nét đứt mảnh màu trắng xám.
 * 
 * @param slot Chỉ số slot (0, 1, 2 tương ứng Slot 1, 2, 3)
 * @param startY Tọa độ dòng Y bắt đầu vẽ slot
 * @param choosing true nếu slot này đang được người dùng trỏ tới
 */
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

/**
 * @brief Vẽ lại danh sách cả 3 slot lưu ván cờ trên màn hình Load Game.
 * - Xác định tọa độ Y cho 3 slot lần lượt là {6, 12, 18}.
 * - Gọi drawslot cho từng slot, đánh dấu slot chosing đang được chọn.
 * 
 * @param chosing Chỉ số slot đang được chọn (0, 1 hoặc 2)
 */
void redrawslots(int chosing) {
	int startY[3] = { 6, 12, 18 };
	for (int i = 0; i < 3; i++) {
		drawslot(i, startY[i], (i==chosing));
	}
}

/**
 * @brief Hiển thị hộp thoại tạm dừng (Pause Game) giữa trận đấu.
 * - Xuất hiện khi người chơi nhấn phím ESC trong trận PvP.
 * - Vẽ bảng thông báo màu xanh lơ (11) tại tọa độ (47, 11) với 2 tùy chọn:
 *   1. "Continue": Tiếp tục trận đấu đang diễn ra.
 *   2. "Exit": Thoát trận đấu (chuyển sang bước hỏi lưu ván cờ).
 * - Người chơi dùng phím W/S để chuyển mục và nhấn Enter để chọn.
 * 
 * @return int: 1 nếu chọn Continue, 2 nếu chọn Exit
 */
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

/**
 * @brief Hiển thị hộp thoại hỏi lưu ván cờ trước khi thoát (Exit Confirmation).
 * - Cung cấp 2 lựa chọn:
 *   1. "Save Game": Cho phép người chơi nhập tên tệp để lưu ván cờ hiện tại qua saveGameData.
 *   2. "Don't Save": Bỏ qua không lưu và trở về màn hình Menu chính.
 * - Sau khi xử lý xong, đưa người chơi về màn hình Menu chính (drawmainscreen() -> mainscreen()).
 */
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

/**
 * @brief Màn hình lựa chọn độ khó khi chơi với Bot (Người vs Máy - PvB).
 * - Cung cấp 3 mức độ: "Easy", "Medium", "Hard".
 * - Điều hướng bằng phím 'W'/'S', chọn bằng phím Enter, nhấn ESC để quay lại màn hình newgamescreen.
 * - Lưu ý: Logic trí tuệ nhân tạo (Bot AI) thực tế hiện đang trong giai đoạn phát triển.
 */
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

/**
 * @brief Vẽ một dòng tùy chọn cấu hình trong màn hình Cài đặt (Settings).
 * - Option 1: Bật/tắt âm thanh (Sound ON/OFF).
 * - Option 2: Điều chỉnh âm lượng (Volume 0% - 100%).
 * - Option 3: Bật/tắt luật chặn 2 đầu (Block 2 Ends ON/OFF).
 * - Option 4 - 7: Hiển thị phím điều khiển tương ứng (Move Up, Down, Left, Right).
 * 
 * @param option Mã số tùy chọn (từ 1 đến 7)
 * @param startX Tọa độ cột hiển thị trên console
 * @param startY Tọa độ dòng hiển thị trên console
 */
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

/**
 * @brief Màn hình quản lý Cài đặt (Settings Screen).
 * - Hiển thị 7 mục cài đặt:
 *   1. Sound (Âm thanh: ON/OFF)
 *   2. Volume (Âm lượng: 0% - 100%, tăng/giảm từng bước 10%)
 *   3. Block 2 Ends (Luật chặn 2 đầu: ON/OFF)
 *   4 - 7. Đổi phím di chuyển (Lên, Xuống, Trái, Phải)
 * - Điều khiển:
 *   + Dùng W/S để di chuyển con trỏ giữa các mục.
 *   + Đối với mục 1-3: Nhấn Enter, sau đó dùng A/D để thay đổi giá trị, Enter/ESC để lưu/xác nhận.
 *   + Đối với mục 4-7: Nhấn Enter, sau đó nhấn một phím bất kỳ để gán phím mới.
 *   + Nhấn ESC để lưu cấu hình và quay lại Menu chính.
 */
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

/**
 * @brief Xử lý và hiển thị thông báo kết quả ván cờ khi kết thúc.
 * - Nếu winner == 2: Trận đấu chưa kết thúc, tự động đổi lượt chơi (_turn = !_turn) và tiếp tục.
 * - Nếu ván cờ kết thúc (winner != 2):
 *   + In thông báo kết quả ở dòng bên dưới bàn cờ:
 *     * winner == 1: "Nguoi choi 1 da thang va nguoi choi 2 da thua" (màu đỏ) kèm hiệu ứng nhấp nháy wineffect.
 *     * winner == -1: "Nguoi choi 2 da thang va nguoi choi 1 da thua" (màu xanh lá) kèm hiệu ứng nhấp nháy wineffect.
 *     * winner == 0: "Nguoi choi  2 da hoa nguoi choi 1" (màu vàng).
 * 
 * @param winner Trạng thái kết quả nhận từ testboard() (1: X thắng, -1: O thắng, 0: hòa, 2: tiếp tục)
 * @return int: Trả về chính giá trị winner
 */
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

/**
 * @brief Hiển thị lời nhắc hỏi người chơi có muốn tiếp tục ván mới hay dừng lại.
 * - In dòng thông báo: "Nhan Y/N de tiep tuc/dung: " bên dưới kết quả ván cờ.
 * - Đợi người chơi nhấn một phím từ bàn phím và trả về ký tự in hoa.
 * 
 * @return int: Ký tự in hoa tương ứng ('Y' để chơi ván mới, phím khác/ 'N' để dừng lại)
 */
int askcontinue() {
	gotoXY(0, _a[boardsize - 1][boardsize - 1].y + 3);
	cout << "Nhan Y/N de tiep tuc/dung: ";
	return toupper(_getch());
}
