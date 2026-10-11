#include "config.h"

/**
 * @brief Khởi tạo và thiết lập bàn cờ cho một ván chơi mới.
 * - Xóa sạch màn hình console (system("cls")).
 * - Đặt lại toàn bộ dữ liệu bàn cờ về mặc định (resetdata()).
 * - Vẽ khung lưới bàn cờ kích thước 12x12 (drawboard(boardsize)).
 */
void startgame() {
	system("cls");
	resetdata();
	drawboard(boardsize);
}

/**
 * @brief Dọn dẹp và kết thúc ván chơi.
 * - Xóa trắng màn hình console để chuẩn bị thoát hoặc chuyển màn hình.
 */
void exitgame() {
	system("cls");
}

/**
 * @brief Di chuyển con trỏ người chơi sang phải 1 ô cờ.
 * - Kiểm tra nếu chưa vượt quá giới hạn tọa độ X của ô góc dưới cùng bên phải.
 * - Tăng tọa độ _X thêm 4 đơn vị console (khoảng cách giữa 2 cột cờ).
 * - Cập nhật vị trí con trỏ chuột console đến tọa độ (_X, _Y) mới.
 */
void moveright() {
	if (_X < _a[boardsize - 1][boardsize - 1].x) {
		_X += 4;
		gotoXY(_X, _Y);
	}
}

/**
 * @brief Di chuyển con trỏ người chơi sang trái 1 ô cờ.
 * - Kiểm tra nếu chưa vượt quá giới hạn tọa độ X của ô góc trên cùng bên trái.
 * - Giảm tọa độ _X đi 4 đơn vị console.
 * - Cập nhật vị trí con trỏ chuột console đến tọa độ (_X, _Y) mới.
 */
void moveleft() {
	if (_X > _a[0][0].x) {
		_X -= 4;
		gotoXY(_X, _Y);
	}
}

/**
 * @brief Di chuyển con trỏ người chơi lên trên 1 ô cờ.
 * - Kiểm tra nếu chưa vượt quá giới hạn tọa độ Y của ô góc trên cùng bên trái.
 * - Giảm tọa độ _Y đi 2 đơn vị console (khoảng cách giữa 2 hàng cờ).
 * - Cập nhật vị trí con trỏ chuột console đến tọa độ (_X, _Y) mới.
 */
void moveup() {
	if (_Y > _a[0][0].y) {
		_Y -= 2;
		gotoXY(_X, _Y);
	}
}

/**
 * @brief Di chuyển con trỏ người chơi xuống dưới 1 ô cờ.
 * - Kiểm tra nếu chưa vượt quá giới hạn tọa độ Y của ô góc dưới cùng bên phải.
 * - Tăng tọa độ _Y thêm 2 đơn vị console.
 * - Cập nhật vị trí con trỏ chuột console đến tọa độ (_X, _Y) mới.
 */
void movedown() {
	if (_Y < _a[boardsize - 1][boardsize - 1].y) {
		_Y += 2;
		gotoXY(_X, _Y);
	}
}

/**
 * @brief Điều hướng con trỏ bàn cờ theo phím bấm người chơi.
 * - So sánh mã phím nhận vào với các phím tùy chỉnh trong Settings (_keyleft, _keydown, _keyright, _keyup).
 * - Gọi tương ứng hàm moveleft(), movedown(), moveright(), moveup().
 * 
 * @param direction Mã phím (ASCII) mà người chơi vừa bấm
 */
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

/**
 * @brief Vòng lặp điều khiển chính của chế độ chơi 2 người (Người vs Người - PvP).
 * - Nếu resume == false: Bắt đầu ván mới hoàn toàn bằng startgame().
 * - Nếu resume == true: Khôi phục bàn cờ từ file save (vẽ bàn cờ và vẽ lại quân cờ đã đánh).
 * - Vòng lặp vô hạn xử lý phím:
 *   + Phím ESC (27): Mở menu tạm dừng (pausescreen). Nếu chọn Continue (1) thì vẽ lại ván cờ;
 *     nếu chọn Exit (2) thì mở menu xác nhận lưu và thoát (exitconfirmscreen).
 *   + Phím Enter (_keyenter): Đánh cờ vào ô hiện tại thông qua checkboard(_X, _Y).
 *     * Đánh thành công X (1): in ký tự "X" màu đỏ (mã màu 12).
 *     * Đánh thành công O (-1): in ký tự "O" màu xanh (mã màu 10).
 *     * Ô đã có quân (0): không hợp lệ, giữ nguyên lượt.
 *     * Sau mỗi nước đi hợp lệ, gọi finish(testboard()) để kiểm tra kết quả thắng/thua/hòa.
 *     * Nếu ván cờ kết thúc, hỏi người chơi có muốn tiếp tục (askcontinue()) hay thoát ra.
 *   + Các phím khác: Gọi moveto(command) để di chuyển con trỏ.
 * 
 * @param resume true nếu tiếp tục ván cờ đã lưu, false nếu bắt đầu mới (mặc định false)
 */
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

/**
 * @brief Điểm khởi nhập (Entry Point) của toàn bộ chương trình trò chơi Caro.
 * - fix_window(): Vô hiệu hóa nút phóng to và viền kéo giãn console.
 * - fix_screen(): Cố định độ phân giải màn hình console 120x30.
 * - drawmainscreen(): Vẽ giao diện Menu chính.
 * - mainscreen(): Khởi động vòng lặp tương tác người dùng trên Menu chính.
 * 
 * @return int: Mã trả về của chương trình (0: kết thúc bình thường)
 */
int main() {
	fix_window();
	fix_screen();
	drawmainscreen();
	mainscreen();
	return 0;
}
