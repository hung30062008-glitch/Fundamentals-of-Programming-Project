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

// ==========================================
// CÁC HẰNG SỐ CẤU HÌNH BÀN CỜ VÀ GIAO DIỆN
// ==========================================
#define boardsize 12   // Kích thước bàn cờ (12x12 ô)
#define left 36        // Tọa độ X (cột) bắt đầu vẽ bàn cờ trên console
#define top 3          // Tọa độ Y (dòng) bắt đầu vẽ bàn cờ trên console

// ==========================================
// CÁC BIẾN TOÀN CỤC TRẠNG THÁI TRÒ CHƠI
// ==========================================
extern bool _turn;       // Lượt chơi hiện tại: true = Lượt người chơi 1 (X), false = Lượt người chơi 2 (O)
extern int _command;     // Mã phím điều khiển cuối cùng người chơi vừa bấm
extern int _X, _Y;       // Tọa độ con trỏ hiện tại trên màn hình console

// ==========================================
// CÁC BIẾN CÀI ĐẶT (SETTINGS)
// ==========================================
extern bool _sound;      // Trạng thái bật/tắt âm thanh (true: bật, false: tắt)
extern int _volume;      // Mức âm lượng (từ 0 đến 100%)
extern bool _block2ends; // Luật chơi: true = áp dụng luật chặn 2 đầu (5 con bị chặn 2 đầu không thắng)
extern int _keyup, _keydown, _keyleft, _keyright, _keyenter; // Mã phím điều khiển di chuyển và đánh dấu cờ

// ==========================================
// CÁC CẤU TRÚC DỮ LIỆU (STRUCTS)
// ==========================================
// Cấu trúc biểu diễn một ô trên bàn cờ
struct _point {
    int x; // Tọa độ X của ô trên màn hình console
    int y; // Tọa độ Y của ô trên màn hình console
    int c; // Trạng thái ô cờ: 0 = ô trống, 1 = quân X, -1 = quân O
};

// Cấu trúc lưu vị trí ô cờ theo chỉ số ma trận bàn cờ (hàng r, cột c)
struct CellPos {
    int r; // Chỉ số hàng (row: 0 -> boardsize - 1)
    int c; // Chỉ số cột (column: 0 -> boardsize - 1)
};

extern _point _a[boardsize][boardsize]; // Ma trận lưu thông tin toàn bộ bàn cờ 12x12
extern vector<CellPos> winCells;        // Danh sách lưu tọa độ 5 ô cờ chiến thắng để tạo hiệu ứng nhấp nháy

// ===================================================
// KHAI BÁO CÁC HÀM GIAO DIỆN & HIỂN THỊ (VIEW)
// ===================================================
void fix_window();                      // Cố định kích cỡ cửa sổ console, vô hiệu hóa nút phóng to và kéo giãn viền
void fix_screen();                      // Đặt kích thước buffer và khung hình console về chuẩn 120 cột x 30 dòng
void setcolor(int text, int bg = 0);    // Thay đổi màu chữ (text) và màu nền (bg) trong console
void showcursor(bool visible);          // Ẩn (false) hoặc hiện (true) con trỏ nhấp nháy trên console
void wineffect(int winner);             // Tạo hiệu ứng nhấp nháy nổi bật 5 ô cờ chiến thắng
void drawboard(int psize);              // Vẽ khung lưới bàn cờ kích thước psize x psize lên màn hình
void redrawchess();                     // Vẽ lại toàn bộ quân cờ X và O đã đánh lên bàn cờ
void drawmainscreen();                  // Vẽ giao diện menu chính của trò chơi
void mainscreen();                      // Điều khiển vòng lặp tương tác trên menu chính
void newgamescreen();                   // Màn hình chọn chế độ chơi mới (Vs Bạn bè / Vs Máy)
void drawloadbox();                     // Vẽ khung viền ngoài cho màn hình tải ván cờ (Load Game)
void loadgamescreen();                  // Màn hình quản lý danh sách slot tải ván cờ đã lưu
void drawslot(int slot, int startY, bool choosing); // Vẽ chi tiết 1 slot dữ liệu tải game
void redrawslots(int chosing);          // Vẽ lại danh sách 3 slot dữ liệu tải game
int pausescreen();                      // Menu tạm dừng ván cờ khi người chơi bấm phím ESC (trả về 1: Tiếp tục, 2: Thoát)
void exitconfirmscreen();               // Menu xác nhận lưu ván cờ trước khi thoát
void PvBscreen();                       // Màn hình chọn độ khó khi chơi với Bot (Easy, Medium, Hard)
void drawsetting(int option, int startX, int startY); // Vẽ từng dòng cài đặt tùy chọn
void settingscreen();                   // Màn hình giao diện cài đặt (âm thanh, luật chơi, phím bấm)
int finish(int winner);                 // Hiển thị thông báo kết quả ván cờ (thắng/thua/hòa) và hiệu ứng
int askcontinue();                      // Hiển thị lời nhắc người chơi có muốn tiếp tục ván mới (Y/N)
void gotoXY(int x, int y);              // Di chuyển con trỏ console tới vị trí (x, y)
void playSound(int type, int volume);   // Khai báo phát âm thanh (chưa cài đặt chi tiết trong code)

// ===================================================
// KHAI BÁO CÁC HÀM XỬ LÝ DỮ LIỆU & LOGIC (MODEL)
// ===================================================
void resetdata();                       // Khởi tạo/đặt lại dữ liệu bàn cờ về trạng thái ban đầu
int testboard();                        // Thuật toán kiểm tra thắng, thua, hòa hoặc trận đấu đang tiếp tục
int checkboard(int pX, int pY);         // Ghi nhận nước cờ mới tại vị trí (pX, pY) vào ma trận dữ liệu
bool saveGameData(const string& filename); // Ghi trạng thái ván cờ hiện tại ra file văn bản
bool loadGameData(const string& filename); // Đọc và khôi phục trạng thái ván cờ từ file văn bản

// ===================================================
// KHAI BÁO CÁC HÀM ĐIỀU KHIỂN & LUỒNG CHƠI (CONTROL)
// ===================================================
void startgame();                       // Khởi tạo và bắt đầu một ván chơi mới
void exitgame();                        // Xóa màn hình và kết thúc ván chơi
void moveright();                       // Di chuyển con trỏ sang phải 1 ô cờ
void moveleft();                        // Di chuyển con trỏ sang trái 1 ô cờ
void moveup();                          // Di chuyển con trỏ lên trên 1 ô cờ
void movedown();                        // Di chuyển con trỏ xuống dưới 1 ô cờ
void moveto(int direction);             // Xử lý di chuyển theo phím điều hướng người chơi vừa bấm
void PvP(bool resume = false);          // Vòng lặp điều khiển chính chế độ chơi 2 người (Người vs Người)